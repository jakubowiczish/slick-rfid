void rfid_configure(
	SPI_HandleTypeDef *s,
	GPIO_TypeDef *cs_po,
	uint16_t cs_pi,
	GPIO_TypeDef *reset_po,
	uint16_t reset_pi
);