#include "rfid.h"
#include "term_io.h"
#include "string.h"
#include "main.h"

SPI_HandleTypeDef *spi = NULL;
GPIO_TypeDef *cs_port = NULL;
uint16_t cs_pin = 0;

GPIO_TypeDef *reset_port = NULL;
uint16_t reset_pin = 0;

void rfid_configure(SPI_HandleTypeDef *s, GPIO_TypeDef *cs_po, uint16_t cs_pi, GPIO_TypeDef *reset_po, uint16_t reset_pi) {
	spi = s;
	cs_port = cs_po;
	cs_pin = cs_pi;
	reset_port = reset_po;
	reset_pin = reset_pi;
}

void rfid_init() {
	rfid_cs_write(GPIO_PIN_SET);

	//HAL_GPIO_WritePin(RFID_RESET_GPIO_Port, RFID_RESET_Pin, GPIO_PIN_RESET);
	//HAL_Delay(5);
	//HAL_GPIO_WritePin(RFID_RESET_GPIO_Port, RFID_RESET_Pin, GPIO_PIN_SET);
	//HAL_Delay(50);
	//xprintf("reading %d",HAL_GPIO_ReadPin(RFID_RESET_GPIO_Port,RFID_RESET_Pin));

	rfid_reset();

	rfid_write_register(REG_T_MODE, 0x8D);

	// 48dB gain
	rfid_set_gain(0xff);

	rfid_write_register(REG_RX_MODE, 0x00);
	rfid_write_register(REG_MOD_WIDTH, 0x26);

	// When communicating with a PICC we need a timeout if something goes wrong.
	// f_timer = 13.56 MHz / (2*TPreScaler+1) where TPreScaler = [TPrescaler_Hi:TPrescaler_Lo].
	// TPrescaler_Hi are the four low bits in TModeReg. TPrescaler_Lo is REG_T_PRESCALER.
	rfid_write_register(REG_TX_MODE, 0x80); // TAuto=1; timer starts automatically at the end of the transmission in all communication modes at all speeds
	rfid_write_register(REG_T_PRESCALER, 0xA9); // TPreScaler = TModeReg[3..0]:REG_T_PRESCALER, ie 0x0A9 = 169 => f_timer=40kHz, ie a timer period of 25μs.
	rfid_write_register(REG_T_RELOAD_H, 0x03); // Reload timer with 0x3E8 = 1000, ie 25ms before timeout.
	rfid_write_register(REG_T_RELOAD_L, 0xE8);

	rfid_write_register(REG_TX_ASK, 0x40); // Default 0x00. Force a 100 % ASK modulation independent of the ModGsPReg register setting
	rfid_write_register(REG_MODE, 0x3D); // Default 0x3F. Set the preset value for the CRC coprocessor for the CalcCRC command to 0x6363 (ISO 14443-3 part 6.2.4)

	rfid_antenna_on();        //Open the antenna
}

void rfid_self_test() {

	rfid_reset();

	rfid_set_bit_mask(REG_FIFO_LEVEL, 0x80);
	for (int i = 0; i < 25; i++) {
		rfid_write_register(REG_FIFO_DATA, 0x00);
	}

	rfid_write_register(REG_COMMAND, COM_MEM);

	rfid_set_bit_mask(REG_FIFO_LEVEL, 0x80);
	rfid_write_register(REG_AUTO_TEST, 0x09);

	rfid_write_register(REG_FIFO_DATA, 0x00);

	rfid_write_register(REG_COMMAND, CMD_CALCCRC);
	HAL_Delay(100);

	int n = rfid_read_register(REG_FIFO_LEVEL);
	xprintf("Test result (%d uint8_ts):\r\n", n);

	for (int i = 0; i < n; ++i) {
		xprintf("%02x ", rfid_read_register(REG_FIFO_DATA));
	}

	xprintf("\n\r");
}

void rfid_set_bit_mask(uint8_t reg, uint8_t mask) {
	rfid_write_register(reg, rfid_read_register(reg) | mask);
}

void rfid_clear_bit_mask(uint8_t reg, uint8_t mask) {
	rfid_write_register(reg, rfid_read_register(reg) & (~mask));
}

void rfid_antenna_on(void) {
	uint8_t temp;

	temp = rfid_read_register(REG_TX_CONTROL);
	if (!(temp & 0x03)) {
		rfid_set_bit_mask(REG_TX_CONTROL, temp | 0x03);
	}
}

uint8_t rfid_read_version() {
	return rfid_read_register(REG_VERSION);
}

void rfid_cs_write(uint8_t val) {
	HAL_GPIO_WritePin(cs_port, cs_pin, val);
}

uint8_t rfid_read_register(uint8_t addr) {
	uint8_t val = 0x00;

	//CS low
	rfid_cs_write(GPIO_PIN_RESET);

	HAL_StatusTypeDef transmitStatus;
	addr = (addr << 1) | 0x80;

	transmitStatus = HAL_SPI_Transmit(spi, &addr, 1, 1000);

	if (transmitStatus != HAL_SPI_ERROR_NONE) {
		xprintf("read register error \r\n");
	}

	uint8_t dummy = 0x00;
	transmitStatus = HAL_SPI_TransmitReceive(spi, &dummy, &val, 1, 1000);
	if (transmitStatus != HAL_SPI_ERROR_NONE) {
		xprintf("read register error \r\n");
	}

	//CS high
	rfid_cs_write(GPIO_PIN_SET);

	return val;
}

void rfid_read_register_many(uint8_t addr, uint8_t count, uint8_t *tab, uint8_t rx_align) {
	if (count == 0)
		return;

	uint8_t index = 0;

	if (rx_align != 0) {
		uint8_t mask = (0xFF << rx_align) & 0xFF;
		uint8_t value = rfid_read_register(addr);
		tab[0] = (tab[0] & ~mask) | (value & mask);
		index++;
	}

	for (; index < count; ++index) {
		tab[index] = rfid_read_register(addr);
	}
}

void rfid_write_register(uint8_t addr, uint8_t val) {
	//CS low
	rfid_cs_write(GPIO_PIN_RESET);

	HAL_StatusTypeDef transmitStatus;

	//Send address ## HAL_MAX_DELAY --> infinite poll until process is successful
	addr = (addr << 1) & 0x7E;

	transmitStatus = HAL_SPI_Transmit(spi, &addr, 1, HAL_MAX_DELAY);
	if (transmitStatus != HAL_SPI_ERROR_NONE) {
		xprintf("write register error \r\n");

	}
	//Send data
	transmitStatus = HAL_SPI_Transmit(spi, &val, 1, HAL_MAX_DELAY);
	if (transmitStatus != HAL_SPI_ERROR_NONE) {
		xprintf("write register error \r\n");
	}

	//CS high
	rfid_cs_write(GPIO_PIN_SET);
}

void rfid_reset() {
	rfid_write_register(REG_COMMAND, COM_SOFT_RESET);
	uint8_t count = 0;
	do {
		// Wait for the PowerDown bit in CommandReg to be cleared (max 3x50ms)
		HAL_Delay(50);
	} while ((rfid_read_register(REG_COMMAND) & (1 << 4)) && (++count) < 3);
}

void rfid_set_gain(uint8_t mask) {
	rfid_clear_bit_mask(REG_RECEIVER_GAIN, (0x07 << 4)); // clear needed to allow 000 pattern
	rfid_set_bit_mask(REG_RECEIVER_GAIN, mask & (0x07 << 4));
}

uint8_t rfid_get_gain() {
	return rfid_read_register(REG_RECEIVER_GAIN) & (0x07 << 4);
}

rfid_status_t rfid_halt() {
	rfid_status_t status;
	uint8_t buffer[4];

	// Build command buffer
	buffer[0] = MIF_HALT;
	buffer[1] = 0;
	// Calculate CRC_A
	status = rfid_calc_crc(buffer, 2, &buffer[2]);
	if (status != MI_OK) {
		return status;
	}

	// Send the command.
	// The standard says:
	//		If the PICC responds with any modulation during a period of 1 ms after the end of the frame containing the
	//		HLTA command, this response shall be interpreted as 'not acknowledge'.
	// We interpret that this way: Only STATUS_TIMEOUT is a success.
	status = rfid_transcive_data(buffer, sizeof(buffer), 0, 0, 0, 0, false);
	if (status == MI_TIMEOUT) {
		return MI_OK;
	}

	if (status == MI_OK) { // That is ironically NOT ok in this case ;-)
		return MI_ERR;
	}

	return status;
}

rfid_status_t rfid_card_write(uint8_t block_addres, const uint8_t *buffer, uint8_t buffer_size) {
	rfid_status_t status;

	// Sanity check
	if (buffer == NULL || buffer_size < 16) {
		return MI_NO_ROOM;
	}

	// Mifare Classic protocol requires two communications to perform a write.
	// Step 1: Tell the PICC we want to write to block blockAddr.
	uint8_t cmdBuffer[2];
	cmdBuffer[0] = MIF_WRITE;
	cmdBuffer[1] = block_addres;
	status = rfid_card_transceive(cmdBuffer, 2, false); // Adds CRC_A and checks that the response is MF_ACK.

	if (status != MI_OK) {
		return status;
	}

	// Step 2: Transfer the data
	status = rfid_card_transceive(buffer, buffer_size, false); // Adds CRC_A and checks that the response is MF_ACK.
	if (status != MI_OK) {
		return status;
	}

	return MI_OK;
}

rfid_status_t rfid_card_transceive(uint8_t *send_data, 	///< Pointer to the data to transfer to the FIFO. Do NOT include the CRC_A.
uint8_t send_len,        								///< Number of bytes in sendData.
bool accept_timeout       								///< True => A timeout is also success
) {
	rfid_status_t status;
	uint8_t cmd_buffer[18];								// We need room for 16 bytes data and 2 bytes CRC_A.

	// Sanity check
	if (send_data == NULL || send_len > 16) {
		return MI_NO_ROOM;
	}

	// Copy sendData[] to cmdBuffer[] and add CRC_A

	memcpy(cmd_buffer, send_data, send_len);
	status = rfid_calc_crc(cmd_buffer, send_len, &cmd_buffer[send_len]);

	if (status != MI_OK) {
		return status;
	}

	send_len += 2;
	uint8_t waitIRq = 0x30;
	uint8_t valid_bits = 0;
	uint8_t cmd_buffer_size = 18;
	status = rfid_to_card(CMD_TRANSCEIVE, waitIRq, cmd_buffer, send_len, cmd_buffer, &cmd_buffer_size, &valid_bits, 0,
	false);
	if ((accept_timeout && status) == MI_TIMEOUT) {
		return MI_OK;
	}

	if (status != MI_OK) {
		return status;
	}

	// The PICC must reply with a 4 bit ACK
	if (cmd_buffer_size != 1 || valid_bits != 4) {
		return MI_ERR;
	}

	if (cmd_buffer[0] != MIF_ACK) {
		return MI_NACK;
	}

	return MI_OK;
}

rfid_status_t rfid_card_read(uint8_t block_address, uint8_t *buffer, uint8_t *buffer_size) {
	rfid_status_t status;

	// Sanity check
	if (buffer == NULL || *buffer_size < 18) {
		return MI_NO_ROOM;
	}

	// Build command buffer
	buffer[0] = MIF_READ;
	buffer[1] = block_address;
	// Calculate CRC_A
	status = rfid_calc_crc(buffer, 2, &buffer[2]);
	if (status != MI_OK) {
		return status;
	}

	// Transmit the buffer and receive the response, validate CRC_A.
	return rfid_transcive_data(buffer, 4, buffer, buffer_size, 0, 0, true);
}

rfid_status_t rfid_authenticate(uint8_t command, 		///< PICC_CMD_MF_AUTH_KEY_A or PICC_CMD_MF_AUTH_KEY_B
uint8_t sector, 										///< The block number. See numbering in the comments in the .h file.
const uint8_t *key,   									///< Pointer to the Crypteo1 key to use (6 bytes)
const uint8_t *uid										///< Pointer to Uid struct. The first 4 bytes of the UID is used.
) {
	uint8_t waitIRq = 0x10;        // IdleIRq

	// Build command buffer
	uint8_t sendData[12];
	sendData[0] = command;
	sendData[1] = sector;

	for (int i = 0; i < 6; i++) {    // 6 key bytes
		sendData[2 + i] = key[i];
	}
	// Use the last uid bytes as specified in http://cache.nxp.com/documents/application_note/AN10927.pdf
	// section 3.2.5 "MIFARE Classic Authentication".
	// The only missed case is the MF1Sxxxx shortcut activation,
	// but it requires cascade tag (CT) byte, that is not part of uid.
	for (int i = 0; i < 4; i++) {                // The last 4 bytes of the UID
		sendData[8 + i] = uid[i];
	}

	return rfid_to_card(CMD_AUTHENT, waitIRq, &sendData[0], 12, NULL, NULL, NULL, 0, false);

	// Start the authentication.
	//return PCD_CommunicateWithPICC(PCD_MFAuthent, waitIRq, &sendData[0], sizeof(sendData));
}

rfid_status_t rfid_select_tag(uint8_t *uid_tab, uint8_t *size, uint8_t *sak) {
	rfid_clear_bit_mask(REG_COLL, 0x80);

	bool uid_complete = false;

	uint8_t buffer[9];
	buffer[0] = MIF_SEL_CL1;
	uint8_t received_bits = 0;

	uint8_t tx_last_bits;
	uint8_t count;
	uint8_t buffer_size = 0;

	uint8_t index = 2;

	uint8_t *response_buffer;
	uint8_t response_size = 0;

	rfid_status_t status;

	while (!uid_complete) {
		if (received_bits >= 32) {
			buffer[1] = 0x70;
			buffer[6] = buffer[2] ^ buffer[3] ^ buffer[4] ^ buffer[5];
			status = rfid_calc_crc(buffer, 7, &buffer[7]);
			if (status != MI_OK) {
				return status;
			}
			buffer_size = 9;
			tx_last_bits = 0;
			response_buffer = &buffer[6];
			response_size = 3;
		} else {
			tx_last_bits = received_bits % 8;
			count = received_bits / 8; // Number of whole bytes in the UID part.
			index = 2 + count;        // Number of whole bytes: SEL + NVB + UIDs
			buffer[1] = (index << 4) + tx_last_bits; // NVB - Number of Valid Bits
			buffer_size = index + (tx_last_bits ? 1 : 0);
			response_buffer = &buffer[index];
			response_size = 9 - index;
		}

		rfid_write_register(REG_BIT_FRAMING, (tx_last_bits << 4) + tx_last_bits);
		status = rfid_transcive_data(buffer, buffer_size, response_buffer, &response_size, &tx_last_bits, tx_last_bits, false);

		if (status == MI_COLLISION) {

			uint8_t valueOfCollReg = rfid_read_register(
			REG_COLL); // CollReg[7..0] bits are: ValuesAfterColl reserved CollPosNotValid CollPos[4:0]

			if (valueOfCollReg & 0x20) { // CollPosNotValid
				return MI_COLLISION; // Without a valid collision position we cannot continue
			}

			uint8_t collisionPos = valueOfCollReg & 0x1F; // Values 0-31, 0 means bit 32.
			if (collisionPos == 0) {
				collisionPos = 32;
			}

			if (collisionPos <= received_bits) { // No progress - should not happen
				return MI_INTERNAL_ERROR;
			}

			received_bits = collisionPos;
			count = received_bits % 8; // The bit to modify

			index = 1 + (received_bits / 8) + (count ? 1 : 0); // First byte is index 0.
			buffer[index] |= (1 << ((received_bits - 1) % 8));
		} else if (status != MI_OK) {
			return status;
		} else {
			if (received_bits >= 32) {
				uid_complete = true;
			} else {
				received_bits = 32;
			}
		}
	}

	index = (buffer[2] == MIF_CT) ? 3 : 2; // source index in buffer[]
	uint8_t bytes_to_copy = (buffer[2] == MIF_CT) ? 3 : 4;
	for (count = 0; count < bytes_to_copy; count++) {
		uid_tab[count] = buffer[index++];
	}
	// Check response SAK (Select Acknowledge)
	if (response_size != 3 || tx_last_bits != 0) { // SAK must be exactly 24 bits (1 byte + CRC_A).
		return MI_ERR;
	}

	status = rfid_calc_crc(response_buffer, 1, &buffer[2]);
	if (status != MI_OK) {
		return status;
	}

	if ((buffer[2] != response_buffer[1]) || (buffer[3] != response_buffer[2])) {
		return MI_CRC_WRONG;
	}

	*sak = response_buffer[0];
	*size = 4;
	return MI_OK;
}

bool rfid_is_new_card() {
	uint8_t tag_buffer[2];
	uint8_t buffer_size = 2;

	// reset transmissions rates
	rfid_write_register(REG_TX_MODE, 0x00);
	rfid_write_register(REG_RX_MODE, 0x00);

	rfid_status_t status = rfid_reqa(tag_buffer, &buffer_size);
	xprintf("status in rfid new card: %d \r\n", status);

	if (status == MI_OK || status == MI_COLLISION) {
		return true;
	}

	return false;
}

rfid_status_t rfid_reqa(uint8_t *response, uint8_t *response_size) {
	return rfid_reqa_or_wupa(MIF_REQA, response, response_size);
}

// MIF_REQA or MIF_WUPA
rfid_status_t rfid_reqa_or_wupa(uint8_t command, uint8_t *response, uint8_t *response_size) {
	if (response == NULL || *response_size < 2) { // The ATQA response is 2 bytes long.
		return MI_NO_ROOM;
	}

	rfid_clear_bit_mask(REG_COLL, 0x80);
	//rfid_write_register(REG_BIT_FRAMING, 0x80);
	uint8_t valid_bits = 7;

	rfid_status_t status = rfid_transcive_data(&command, 1, response, response_size, &valid_bits, 0, false);

	if (status != MI_OK) {
		return status;
	}

	if (*response_size != 2 || valid_bits != 0) {
		return MI_ERR;
	}

	return MI_OK;
}

rfid_status_t rfid_transcive_data(uint8_t *send_data, 	///< Pointer to the data to transfer to the FIFO.
uint8_t send_len,       								///< Number of uint8_ts to transfer to the FIFO.
uint8_t *back_data,    									///< nullptr or pointer to buffer if data should be read back after executing the command.
uint8_t *back_len, 										///< In: Max number of uint8_ts to write to *backData. Out: The number of uint8_ts returned.
uint8_t *valid_bits,									///< In/Out: The number of valid bits in the last uint8_t. 0 for 8 valid bits.
uint8_t rx_align, 										///< In: Defines the bit position in backData[0] for the first bit received. Default 0.
bool check_CRC 											///< In: True => The last two uint8_ts of the response is assumed to be a CRC_A that must be validated.
) {
	return rfid_to_card(CMD_TRANSCEIVE, 0x30, send_data, send_len, back_data, back_len, valid_bits, rx_align, check_CRC);
}

rfid_status_t rfid_to_card(uint8_t command, 			///< The command to execute. One of the PCD_Command enums.
uint8_t waitIRq, 										///< The bits in the ComIrqReg register that signals successful completion of the command.
uint8_t *send_data,    									///< Pointer to the data to transfer to the FIFO.
uint8_t send_len,      								    ///< Number of uint8_ts to transfer to the FIFO.
uint8_t *back_data, 									///< nullptr or pointer to buffer if data should be read back after executing the command.
uint8_t *back_len, 										///< In: Max number of uint8_ts to write to *backData. Out: The number of uint8_ts returned.
uint8_t *valid_bits, 									///< In/Out: The number of valid bits in the last uint8_t. 0 for 8 valid bits.
uint8_t rx_align, 										///< In: Defines the bit position in backData[0] for the first bit received. Default 0.
bool check_CRC 											///< In: True => The last two uint8_ts of the response is assumed to be a CRC_A that must be validated.
) {
	// Prepare values for BitFramingReg
	uint8_t txLastBits = valid_bits ? *valid_bits : 0;
	uint8_t bitFraming = (rx_align << 4) + txLastBits;  // RxAlign = BitFramingReg[6..4]. TxLastBits = BitFramingReg[2..0]

	//rfid_write_register(REG_COMM_IE_N, 0x80);

	rfid_write_register(REG_COMMAND, CMD_IDLE);     	// Stop any active command.
	rfid_write_register(REG_COMM_IRQ, 0x7F); 			// Clear all seven interrupt request bits
	rfid_write_register(REG_FIFO_LEVEL, 0x80); 			// FlushBuffer = 1, FIFO initialization
	for (int i = 0; i < send_len; ++i) {
		rfid_write_register(REG_FIFO_DATA, send_data[i]);
	}
	//rfid_write_register(FIFODataReg, send_len, send_data);	// Write send_data to the FIFO
	rfid_write_register(REG_BIT_FRAMING, bitFraming);        	// Bit adjustments
	rfid_write_register(REG_COMMAND, command);            		// Execute the command
	if (command == CMD_TRANSCEIVE) {
		rfid_set_bit_mask(REG_BIT_FRAMING, 0x80); 				// StartSend=1, transmission of data starts
	}

	// Wait for the command to complete.
	// In PCD_Init() we set the TAuto flag in TModeReg. This means the timer automatically starts when the PCD stops transmitting.
	// Each iteration of the do-while-loop takes 17.86μs.
	// TODO check/modify for other architectures than Arduino Uno 16bit

	uint16_t i;
	for (i = 2000; i > 0; i--) {
		uint8_t n = rfid_read_register(REG_COMM_IRQ); 	// ComIrqReg[7..0] bits are: Set1 TxIRq RxIRq IdleIRq HiAlertIRq LoAlertIRq ErrIRq TimerIRq
		if (n & waitIRq) { 								// One of the interrupts that signal success has been set.
			break;
		}

		if (n & 0x01) {            						// Timer interrupt - nothing received in 25ms
			return MI_TIMEOUT;
		}
		//xprintf("n: %02x\r\n", n);
	}

	// 35.7ms and nothing happened. Communication with the MFRC522 might be down.

	if (i == 0) {
		//xprintf("nothing\r\n");
		return MI_TIMEOUT;
	}

	// Stop now if any errors except collisions were detected.
	uint8_t errorRegValue = rfid_read_register(
	REG_ERROR); // ErrorReg[7..0] bits are: WrErr TempErr reserved BufferOvfl CollErr CRCErr ParityErr ProtocolErr
	if (errorRegValue & 0x13) {     // BufferOvfl ParityErr ProtocolErr
		return MI_ERR;
	}

	uint8_t _validBits = 0;

	// If the caller wants data back, get it from the MFRC522.
	if (back_data && back_len) {
		uint8_t n = rfid_read_register(REG_FIFO_LEVEL); // Number of uint8_ts in the FIFO

		if (n > *back_len) {
			return MI_NO_ROOM;
		}
		*back_len = n;                            // Number of uint8_ts returned
		//rfid_read_register(FIFODataReg, n, back_data, rx_align);	// Get received data from FIFO

		rfid_read_register_many(REG_FIFO_DATA, n, back_data, rx_align);

		_validBits = rfid_read_register(REG_CONTROL) & 0x07; // RxLastBits[2:0] indicates the number of valid bits in the last received uint8_t. If this value is 000b, the whole uint8_t is valid.
		if (valid_bits) {
			*valid_bits = _validBits;
		}
	}

	// Tell about collisions
	if (errorRegValue & 0x08) {        // CollErr
		return MI_COLLISION;
	}

	// Perform CRC_A validation if requested.
	if (back_data && back_len && check_CRC) {

		// In this case a MIFARE Classic NAK is not OK.
		if (*back_len == 1 && _validBits == 4) {
			return MI_NACK;
		}
		// We need at least the CRC_A value and all 8 bits of the last uint8_t must be received.
		if (*back_len < 2 || _validBits != 0) {
			return MI_CRC_WRONG;
		}
		// Verify CRC_A - do our own calculation and store the control in controlBuffer.
		uint8_t controlBuffer[2];

		rfid_status_t status = rfid_calc_crc(&back_data[0], *back_len - 2, &controlBuffer[0]);
		// rfid_status_t = PCD_CalculateCRC(&back_data[0], *back_len - 2, &controlBuffer[0]);
		if (status != MI_OK) {
			return status;
		}
		if ((back_data[*back_len - 2] != controlBuffer[0]) || (back_data[*back_len - 1] != controlBuffer[1])) {
			return MI_CRC_WRONG;
		}
	}

	return MI_OK;
} // End PCD_CommunicateWithPICC()

rfid_status_t rfid_calc_crc(uint8_t *tab, uint8_t len, uint8_t *out) {

	rfid_clear_bit_mask(REG_DIV_IRQ, 0x04);                	// CRCIrq = 0
	rfid_set_bit_mask(REG_FIFO_LEVEL, 0x80);            	// Clear the FIFO pointer
	rfid_write_register(REG_COMMAND, CMD_IDLE);      		// Stop any active command.

	for (int i = 0; i < len; i++) {
		rfid_write_register(REG_FIFO_DATA, tab[i]);
	}
	rfid_write_register(REG_COMMAND, CMD_CALCCRC);

	uint8_t i = 0xFF;
	uint8_t n;
	do {
		n = rfid_read_register(REG_DIV_IRQ);
		i--;
	} while ((i != 0) && !(n & 0x04));           			//CRCIrq = 1

	if (i == 0) {
		return MI_TIMEOUT;
	}

	out[0] = rfid_read_register(REG_CRC_RESULT_L);
	out[1] = rfid_read_register(REG_CRC_RESULT_M);

	return MI_OK;

}

void rfid_stop_crypto() {
	rfid_clear_bit_mask(REG_STATUS_2, 0x08);
}
