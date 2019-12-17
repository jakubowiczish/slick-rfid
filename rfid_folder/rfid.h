#ifndef __RFID_H__
#define __RFID_H__

#ifdef __cplusplus
 extern "C" {
#endif


#include "main.h"
#include <stdbool.h>

/* REGISTERS ADDRESSES */
#define REG_VERSION                0x37
#define REG_COMMAND                0x01
#define REG_FIFO_DATA              0x09   // input and output of 64 byte FIFO buffer
#define REG_AUTO_TEST              0x36    // controls the digital self-test
#define REG_FIFO_LEVEL             0x0A   // number of bytes stored in the FIFO buffer
#define REG_COLL                   0x0E   // bit position of the first bit-collision detected on the RF interface
#define REG_BIT_FRAMING            0x0D
#define REG_COMM_IRQ               0x04
#define REG_ERROR                  0x06
#define REG_CONTROL                0x0C
#define REG_DIV_IRQ                0x05
#define REG_CRC_RESULT_L           0x22
#define REG_CRC_RESULT_M           0x21
#define REG_TX_MODE                0x12
#define REG_RX_MODE                0x13
#define REG_MOD_WIDTH              0x24
#define REG_STATUS_2               0x08
#define REG_RECEIVER_GAIN          0x26
#define REG_T_PRESCALER            0x2B    // the lower 8 bits of the TPrescaler value. The 4 high bits are in TModeReg.
#define REG_T_RELOAD_H             0x2C    // defines the 16-bit timer reload value
#define REG_T_RELOAD_L             0x2D
#define REG_TX_ASK                 0x15    // controls the setting of the transmission modulation
#define REG_T_MODE                 0x2A    // defines settings for the internal timer
#define REG_MODE                   0x11   // defines general modes for transmitting and receiving
#define REG_TX_CONTROL             0x14   // controls the logical behavior of the antenna driver pins TX1 and TX2


/* COMMANDS */
#define COM_SOFT_RESET                    0x0F    //Reset
#define COM_MEM                        0x01    //Transfer FIFO to Internal MEM
#define CMD_CALCCRC                        0x03    //CRC Calculate
#define CMD_IDLE                        0x00    // Cancel current command
#define CMD_TRANSCEIVE                    0x0C
#define CMD_AUTHENT                        0x0E


/* Mifare One commands */
#define MIF_REQA                        0x26   // Request
#define MIF_AUTHENTA                    0x60   // authentication key A
#define MIF_AUTHENTB                    0x61
#define MIF_SEL_CL1                        0x93
#define MIF_CT                            0x88        // Cascade Tag. Not really a command, but used during anti collision.
#define MIF_HALT                        0x50
#define MIF_READ                        0x30
#define MIF_WRITE                        0xA0
#define MIF_ACK                        0x0A


typedef enum {
    MI_OK = 0,
    MI_ERR = 1,
    MI_TIMEOUT = 2,
    MI_NO_ROOM = 3,
    MI_COLLISION = 4,
    MI_CRC_WRONG = 5,
    MI_NACK = 6,
    MI_INTERNAL_ERROR = 7
} rfid_status_t;


void rfid_configure(
        SPI_HandleTypeDef *s,
        GPIO_TypeDef *cs_po,
        uint16_t cs_pi,
        GPIO_TypeDef *reset_po,
        uint16_t reset_pi
);

//void rfid_configure(SPI_HandleTypeDef* spi, GPIO_TypeDef *cs_port, uint16_t cs_pin);
void rfid_init();

void rfid_reset();

void rfid_antenna_on();

void rfid_self_test();

void rfid_set_bit_mask(uint8_t reg, uint8_t mask);

void rfid_clear_bit_mask(uint8_t reg, uint8_t mask);

uint8_t rfid_read_register(uint8_t addr);

void rfid_write_register(uint8_t addr, uint8_t val);

void rfid_cs_write(uint8_t val);

void rfid_set_gain(uint8_t mask);

uint8_t rfid_get_gain();

uint8_t rfid_read_version();

rfid_status_t rfid_card_transceive(
        uint8_t *send_data,        ///< Pointer to the data to transfer to the FIFO. Do NOT include the CRC_A.
        uint8_t send_len,        ///< Number of bytes in sendData.
        bool accept_timeout    ///< True => A timeout is also success
);

rfid_status_t rfid_card_write(
        uint8_t block_addres,
        const uint8_t *buffer,
        uint8_t buffer_size
);

rfid_status_t rfid_card_read(
        uint8_t block_addres,
        uint8_t *buffer,
        uint8_t *buffer_size
);

rfid_status_t rfid_calc_crc(
        uint8_t *tab,
        uint8_t len,
        uint8_t *out
);

void rfid_read_register_many(
        uint8_t addr,
        uint8_t count,
        uint8_t *tab,
        uint8_t rx_align
);

rfid_status_t rfid_to_card(
        uint8_t command,        ///< The command to execute. One of the PCD_Command enums.
        uint8_t waitIRq,        ///< The bits in the ComIrqReg register that signals successful completion of the command.
        uint8_t *send_data,        ///< Pointer to the data to transfer to the FIFO.
        uint8_t send_len,        ///< Number of uint8_ts to transfer to the FIFO.
        uint8_t *back_data,        ///< nullptr or pointer to buffer if data should be read back after executing the command.
        uint8_t *back_len,        ///< In: Max number of uint8_ts to write to *backData. Out: The number of uint8_ts returned.
        uint8_t *valid_bits,    ///< In/Out: The number of valid bits in the last uint8_t. 0 for 8 valid bits.
        uint8_t rx_align,        ///< In: Defines the bit position in backData[0] for the first bit received. Default 0.
        bool check_CRC        ///< In: True => The last two uint8_ts of the response is assumed to be a CRC_A that must be validated.
);

rfid_status_t rfid_transcive_data(
        uint8_t *send_data,        ///< Pointer to the data to transfer to the FIFO.
        uint8_t send_len,        ///< Number of uint8_ts to transfer to the FIFO.
        uint8_t *back_data,        ///< nullptr or pointer to buffer if data should be read back after executing the command.
        uint8_t *back_len,        ///< In: Max number of uint8_ts to write to *backData. Out: The number of uint8_ts returned.
        uint8_t *valid_bits,    ///< In/Out: The number of valid bits in the last uint8_t. 0 for 8 valid bits.
        uint8_t rx_align,        ///< In: Defines the bit position in backData[0] for the first bit received. Default 0.
        bool check_CRC        ///< In: True => The last two uint8_ts of the response is assumed to be a CRC_A that must be validated.
);

rfid_status_t rfid_reqa_or_wupa(
        uint8_t command,                // MIF_REQA or MIF_WUPA
        uint8_t *response,
        uint8_t *response_size
);

rfid_status_t rfid_reqa(
        uint8_t *response,
        uint8_t *response_size
);

rfid_status_t rfid_select_tag(
        uint8_t *uid_tab,
        uint8_t *size,
        uint8_t *sak
);

rfid_status_t rfid_authenticate(
        uint8_t command,        ///< PICC_CMD_MF_AUTH_KEY_A or PICC_CMD_MF_AUTH_KEY_B
        uint8_t blockAddr,    ///< The block number. See numbering in the comments in the .h file.
        const uint8_t *key,    ///< Pointer to the Crypteo1 key to use (6 bytes)
        const uint8_t *uid            ///< Pointer to Uid struct. The first 4 bytes of the UID is used.
);

bool rfid_is_new_card();

rfid_status_t rfid_halt();

void rfid_stop_crypto();


#ifdef __cplusplus
 }
#endif

#endif

