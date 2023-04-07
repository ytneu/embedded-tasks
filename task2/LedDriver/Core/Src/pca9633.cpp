/*
 * pca9633.cpp
 *
 *  Created on: Apr 6, 2023
 *      Author: michalb
 */

#include "pca9633.h"

PCA9633::PCA9633() {
	// TODO Auto-generated constructor stub

}

PCA9633::~PCA9633() {
	// TODO Auto-generated destructor stub
}

bool PCA9633::init(uint8_t address) {
	i2c_address = (address >> 1);
	bool success = true;
	success = success && writeRegister(REG_MODE_1, 0x00);
	success = success && writeRegister(REG_MODE_2, (0x01 << 2));
	//value 0x03 -> LED driver x individual brightness can be controlled through its PWMx register.
	success = success && writeRegister(REG_LED_OUT, (0x03 << 6 | 0x03 << 4 | 0x03 << 2 | 0x03));

	return success;
}

bool PCA9633::writeRegister(uint8_t reg, uint8_t data) {
	HAL_StatusTypeDef status = HAL_OK;
	status = HAL_I2C_Mem_Write(&hi2c1, i2c_address, reg, 1, &data, sizeof(data), HAL_MAX_DELAY);
	if (status != HAL_OK) {
		return false;
	}
	return true;
}

void PCA9633::printRed(uint8_t val) {
	writeRegister(REG_PWM_0, val);
}

void PCA9633::printGreen(uint8_t val) {
	writeRegister(REG_PWM_1, val);
}

void PCA9633::printBlue(uint8_t val) {
	writeRegister(REG_PWM_2, val);
}

void PCA9633::printWhite(uint8_t val) {
	writeRegister(REG_PWM_3, val);
}

void PCA9633::printRGBW(uint8_t redVal, uint8_t greenVal, uint8_t blueVal, uint8_t whiteVal) {
	printRed(redVal);
	printGreen(greenVal);
	printBlue(blueVal);
	printWhite(whiteVal);
}

bool PCA9633::printHex(char* hex) {
	if (strlen(hex) == 9 && hex[0] == '#') {
		uint32_t intValue = strtoul(hex+1, NULL, 16);
		uint8_t redVal = (uint8_t)((intValue >> 24) & 0xFF);
		uint8_t greenVal = (uint8_t)((intValue >> 16) & 0xFF);
		uint8_t blueVal = (uint8_t)((intValue >> 8) & 0xFF);
		uint8_t whiteVal = (uint8_t)(intValue & 0xFF);
		printRGBW(redVal, greenVal, blueVal, whiteVal);
		return true;
	}
	return false;
}

// hue [0, 360], saturation [0, 100], lightness [0, 100]
void PCA9633::HSLToRGB(float hue, float saturation, float lightness, int* red, int* green, int* blue) {
	float redN = 0.0f;
	float greenN = 0.0f;
	float blueN = 0.0f;

	float saturationN = saturation / 100.0f;
	float lightnessN = lightness / 100.0f;

	float chroma = (1.0f - fabs(2.0f * lightnessN - 1.0f)) * saturationN;
	float hueSector = hue / 60.0f;
	int hueSectorInt = floor(hueSector);
	float intermediateVal = chroma * (1.0f - fabs(fmod(hueSector, 2) - 1.0f));
	float lightnessAdj = lightnessN - chroma / 2.0f;

	switch(hueSectorInt) {
		case 0:
	      redN = chroma;
	      greenN = intermediateVal;
	      break;
	    case 1:
	      redN = intermediateVal;
	      greenN = chroma;
	      break;
	    case 2:
	      greenN = chroma;
	      blueN = intermediateVal;
	      break;
	    case 3:
	      greenN = intermediateVal;
	      blueN = chroma;
	      break;
	    case 4:
	      redN = intermediateVal;
	      blueN = chroma;
	      break;
	    case 5:
	      redN = chroma;
	      blueN = intermediateVal;
	      break;
	}

	*red = round((redN + lightnessAdj) * 255.0f);
	*green = round((greenN + lightnessAdj) * 255.0f);
	*blue = round((blueN + lightnessAdj) * 255.0f);
}

void PCA9633::printRainbow(int delay_ms) {
	// hue = 0 is red and hue = 240 is blue
	int redVal, greenVal, blueVal;
	for (int i = 0; i < 240; i++) {
		HSLToRGB((float)i, 100.0f, 50.0f, &redVal, &greenVal, &blueVal);
		printRGBW(redVal, greenVal, blueVal, 0);
		HAL_Delay(delay_ms);
	}
}
