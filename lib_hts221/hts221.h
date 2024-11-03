/*
 * hts221.h
 *
 *  Created on: Nov 2, 2024
 *      Author: petri
 */
#include "main.h"
#include "i2c.h"

#ifndef HTS221_H_
#define HTS221_H_

//#define 	HTS221_DEVICE_ADDRESS_WRITE		0xBEU
#define 	HTS221_DEVICE_ADDRESS_0			0xBEU
#define		HTS221_DEVICE_ADDRESS_1			0xBFU
#define 	HTS221_WHO_AM_I_VALUE			0xBCU
#define 	HTS221_WHO_AM_I_ADDRES			0x0FU

#define		HTS221_CTRL_REG1				0x20

// výstup teplota a vlhkost
#define		HTS221_HUMIDITY_OUT_L			0x28 //LSB
#define		HTS221_HUMIDITY_OUT_H			0x29 //MSB
#define		HTS221_TEMP_OUT_L				0x2A //LSB
#define		HTS221_TEMP_OUT_H				0x2B //MSB

// kalibrácia teplota
#define		HTS221_T0_degC_x8				0x32
#define		HTS221_T1_degC_x8				0x33
#define		HTS221_T0_OUT_L					0x3C
#define		HTS221_T0_OUT_H					0x3D
#define		HTS221_T1_OUT_L					0x3E
#define		HTS221_T1_OUT_H					0x3F

// kalibracia vlhkost
#define		HTS221_H0_rH_x2					0x30
#define		HTS221_H1_rH_x2					0x31
#define		HTS221_H0_T0_OUT_L				0x36
#define		HTS221_H0_T0_OUT_H				0x37
#define		HTS221_H1_T0_OUT_L				0x3A
#define		HTS221_H1_T0_OUT_H				0x3B

//todo upravit
//precita multibyte
void HTS221_citaj(uint8_t reg_addr, uint8_t* values, size_t length);
//zapise multibyte
void HTS221_zapisuj(uint8_t reg_addr, uint8_t value);
//inicializacia
void HTS221_init(void);
//void HTS221_get_humidity_calibration(void);
void HTS221_vlhkost(float* humidity_out);
//void HTS221_get_temperature_calibration(void);
void HTS221_teplota(float* temperature_out);

#endif /* HTS221_H_ */
