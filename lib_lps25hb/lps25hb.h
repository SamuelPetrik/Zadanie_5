/*
 * lps25hb.h
 *
 *  Created on: Nov 3, 2024
 *      Author: petri
 */
#include "main.h"
#include "i2c.h"

#ifndef LPS25HB_H_
#define LPS25HB_H_

#define 	LPS25HB_DEVICE_ADDRESS_0			0xB8
#define 	LPS25HB_DEVICE_ADDRESS_1			0xBA
#define 	LPS25HB_DEVICE_ADDRESS				0xB8
#define 	LPS25HB_WHO_AM_I_VALUE				0xBCU
#define 	LPS25HB_WHO_AM_I_ADDRESS				0x0FU

#define 	LPS25HB_ADDRESS_CTRL1				0x20

#define 	LPS25HB_ADDRESS_PRESS_OUT_XL		0x28
#define 	LPS25HB_ADDRESS_PRESS_OUT_L			0x29
#define 	LPS25HB_ADDRESS_PRESS_OUT_H			0x2A

#define		LPS25HB_ADDRESS_RPDS_L				0x39
#define		LPS25HB_ADDRESS_RPDS_H				0x3A

void lps25hb_citaj(uint8_t adresa_registra, uint8_t* hodnoty, size_t dlzka);
void lps25hb_zapisuj(uint8_t adresa_registra, uint8_t* hodnoty, size_t dlzka);
void lps25hb_init(void);
void lps25hb_get_pressure_calibration(void);
void lps25hb_get_pressure(float* pressure);

//void lps25hb_tlak(float* tlak);

#endif /* LPS25HB_H_ */
