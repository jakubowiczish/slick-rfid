#include <common/TouchGFXInit.hpp>
#include <touchgfx/hal/BoardConfiguration.hpp>
#include <touchgfx/hal/GPIO.hpp>
#include <platform/driver/lcd/LCD24bpp.hpp> 

#include <STM32F4DMA.hpp> /* generated for F4 DMA2D acceleration */
#include <STM32F4HAL_DSI.hpp> /* generated when a DSI display is selected on F4 */
#include <STM32F4Instrumentation.hpp>

#include <OTM8009TouchController.hpp>
#include <otm8009a.h>

/* USER CODE BEGIN user includes */

/* USER CODE END user includes */

/**
 * In order to use double buffering, simply enable the USE_DOUBLE_BUFFERING #define below
 * Enabling double buffering will yield a performance increase (potentionally higher frame rate),
 * at the expense of consuming an additional frame buffer's worth of external RAM - e.g. 800 * 480 * 2 bytes (16 bpp)
 */
//#define USE_DOUBLE_BUFFERING 1

/***********************************************************
 ******         24 Bits Per Pixel Support            *******
 ***********************************************************
 *
 * The default bit depth of the framebuffer is 16bpp. If you want 24bpp support, define the symbol "USE_BPP" with a value
 * of "24", e.g. "USE_BPP=24". This symbol affects the following:
 *
 * 1. Type of TouchGFX LCD (16bpp vs 24bpp)
 * 2. Bit depth of the framebuffer(s)
 * 3. TFT controller configuration.
 *
 * WARNING: Remember to modify your image formats accordingly in app/config/. Please see the following knowledgebase article
 * for further details on how to choose and configure the appropriate image formats for your application:
 *
 * https://touchgfx.zendesk.com/hc/en-us/articles/206725849
 */
#include "HW_Init.hpp"
extern "C"
{
#include "stm32f4xx_hal.h" 
#include "stm32f4xx_hal_dsi.h"

extern DSI_HandleTypeDef    hdsi;

uint32_t LCD_GetXSize()
{
    return OTM8009A_800X480_WIDTH;
}

uint32_t LCD_GetYSize()
{
    return OTM8009A_800X480_HEIGHT;
}

#define LAYER0_ADDRESS  (0xc0000000)

}

uint8_t pCols[4][4] =
{
    {0x00, 0x00, 0x00, 0xC7}, /*   0 -> 199 */
    {0x00, 0xC8, 0x01, 0x8F}, /* 200 -> 399 */
    {0x01, 0x90, 0x02, 0x57}, /* 400 -> 599 */
    {0x02, 0x58, 0x03, 0x1F}, /* 600 -> 799 */
};

uint8_t pColLeft[]    = {0x00, 0x00, 0x01, 0x8F}; /*   0 -> 399 */
uint8_t pColRight[]   = {0x01, 0x90, 0x03, 0x1F}; /* 400 -> 799 */

static uint32_t frameBuf0 = (uint32_t)(LAYER0_ADDRESS);

/**
  * @brief  DCS or Generic short/long write command
  * @param  NbParams: Number of parameters. It indicates the write command mode:
  *                 If inferior to 2, a long write command is performed else short.
  * @param  pParams: Pointer to parameter values table.
  * @retval HAL status
  */
extern "C" void DSI_IO_WriteCmd(uint32_t NbrParams, uint8_t *pParams)
{
    if (NbrParams <= 1)
        HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, pParams[0], pParams[1]);
    else
        HAL_DSI_LongWrite(&hdsi, 0, DSI_DCS_LONG_PKT_WRITE, NbrParams, pParams[NbrParams], pParams); 
}

extern "C"
{
    /**
     * Request TE at scanline.
     */
    void LCD_ReqTear(void)
    {
        uint8_t ScanLineParams[2];
        uint16_t scanline = 533;

        ScanLineParams[0] = scanline >> 8;
        ScanLineParams[1] = scanline & 0x00FF;

        HAL_DSI_LongWrite(&hdsi, 0, DSI_DCS_LONG_PKT_WRITE, 2, OTM8009A_CMD_WRTESCN, ScanLineParams);
        HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, OTM8009A_CMD_TEEON, OTM8009A_TEEON_TELOM_VBLANKING_INFO_ONLY);
    }

    void LCD_SetUpdateRegion(int idx)
    {
        HAL_DSI_LongWrite(&hdsi, 0, DSI_DCS_LONG_PKT_WRITE, 4, OTM8009A_CMD_CASET, pCols[idx]);
    }

    void LCD_SetUpdateRegionLeft()
    {
        HAL_DSI_LongWrite(&hdsi, 0, DSI_DCS_LONG_PKT_WRITE, 4, OTM8009A_CMD_CASET, pColLeft);
    }

    void LCD_SetUpdateRegionRight()
    {
        HAL_DSI_LongWrite(&hdsi, 0, DSI_DCS_LONG_PKT_WRITE, 4, OTM8009A_CMD_CASET, pColRight);
    }
}

/**
  * @brief  BSP LCD Reset
  *         Hw reset the LCD DSI activating its XRES signal (active low for some time)
  *         and desactivating it later.
  *         This signal is only cabled on disco Rev B and beyond.
  */
static void LCD_LL_Reset(void)
{
   /* Activate XRES active low */
  HAL_GPIO_WritePin(GPIOH, GPIO_PIN_7, GPIO_PIN_RESET);

  HAL_Delay(20); /* wait 20 ms */

  /* Desactivate XRES */
  HAL_GPIO_WritePin(GPIOH,GPIO_PIN_7, GPIO_PIN_SET);

  /* Wait for 10ms after releasing XRES before sending commands */
  HAL_Delay(10);
}

using namespace touchgfx;
void GRAPHICS_HW_Init()
{
     
    
    MX_FMC_Init();
    MX_SDRAM_InitEx();
    LCD_LL_Reset();
    MX_LCD_Init();
    MX_DSI_Init();

    GPIO::init();

}

__weak void OTM8009A_IO_Delay(uint32_t Delay)
{
  HAL_Delay(Delay);
}

namespace touchgfx
{
STM32F4DMA dma; /* generated DMA2D acceleration module declaration for F4 devices */
OTM8009TouchController tc;
STM32F4Instrumentation mcuInstr;

static LCD24bpp display;
static uint16_t bitdepth = 24;

void touchgfx_init()
{ 
  uint16_t dispWidth = 800;
  uint16_t dispHeight = 480;

  HAL& hal = touchgfx_generic_init<STM32F4HAL_DSI>(dma, display, tc, dispWidth, dispHeight, (uint16_t*) 0, 
                                               0, 0);                                                     

    hal.setFrameBufferStartAddress((uint16_t*)frameBuf0, bitdepth, false, false);

    // By default frame rate compensation is off.
    // Enable frame rate compensation to smooth out animations in case there is periodic slow frame rates.
    hal.setFrameRateCompensation(false);

    hal.setTouchSampleRate(2);
    hal.setFingerSize(1);

    // This platform can handle simultaneous DMA and TFT accesses to SDRAM, so disable lock to increase performance.
    hal.lockDMAToFrontPorch(false);

    mcuInstr.init();

    //Set MCU instrumentation and Load calculation
    hal.setMCUInstrumentation(&mcuInstr);
    hal.enableMCULoadCalculation(true);  
}
}
using namespace touchgfx;

void GRAPHICS_Init()
{
   touchgfx::touchgfx_init();
}

void GRAPHICS_MainTask(void)
{
    touchgfx::HAL::getInstance()->taskEntry();
}
