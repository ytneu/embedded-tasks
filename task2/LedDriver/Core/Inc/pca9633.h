/*
 * pca9633.h
 *
 *  Created on: Apr 6, 2023
 *      Author: michalb
 */

#ifndef SRC_PCA9633_H_
#define SRC_PCA9633_H_

#include "stdint.h"
#include "string.h"
#include "math.h"

#include "i2c.h"

enum DriverRegisters {
	REG_MODE_1       = 0x00,
	REG_MODE_2       = 0x01,
	REG_PWM_0        = 0x02,
	REG_PWM_1        = 0x03,
	REG_PWM_2        = 0x04,
	REG_PWM_3        = 0x05,
	REG_GRP_PWM      = 0x06,
	REG_GRP_FREQ     = 0x07,
	REG_LED_OUT      = 0x08,
	REG_SUB_ADR_1    = 0x09,
	REG_SUB_ADR_2    = 0x0a,
	REG_SUB_ADR_3    = 0x0b,
	REG_ALL_CALL_ADR = 0x0c
};

class PCA9633 {
public:
	PCA9633();
	virtual ~PCA9633();
	bool init(uint8_t address);
	void printRGBW(uint8_t red_val, uint8_t green_val, uint8_t blue_val, uint8_t white_val);
	bool printHex(char* hex);
	void printRainbow(int delay_ms);
private:
	uint8_t i2c_address;
	bool writeRegister(uint8_t reg, uint8_t data);
	void printRed(uint8_t val);
	void printGreen(uint8_t val);
	void printBlue(uint8_t val);
	void printWhite(uint8_t val);
	void HSLToRGB(float hue, float saturation, float lightness, int* red, int* green, int* blue);
};

#endif /* SRC_PCA9633_H_ */
