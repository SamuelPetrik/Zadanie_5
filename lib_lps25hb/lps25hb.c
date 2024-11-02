/*
 * lps25hb.c
 *
 *  Created on: Nov 3, 2024
 *      Author: petri
 */
#include "i2c.h"
//float citanie_teploty(uint8_t adresa_zariadenia, uint8_t adresa_registra){
float citanie_tlaku(uint8_t adresa_zariadenia){
	uint8_t hod_reg_tlak_XL = i2c_master_read_byte(adresa_zariadenia, 28, 1);
	uint8_t hod_reg_tlak_L = i2c_master_read_byte(adresa_zariadenia, 29, 1);
	uint8_t hod_reg_tlak_H = i2c_master_read_byte(adresa_zariadenia, 2A, 1);

	uint32_t hod_reg_tlak = (int32_t)(hod_reg_tlak_H << 16) | (int32_t)(hod_reg_tlak_L << 8) | hod_reg_tlak_XL;
	return hod_reg_tlak/8.0;
}

