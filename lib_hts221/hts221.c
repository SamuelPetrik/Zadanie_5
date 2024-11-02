/*
 * hts221.c
 *
 *  Created on: Nov 2, 2024
 *      Author: petri
 */

#include "i2c.h"
//float citanie_teploty(uint8_t adresa_zariadenia, uint8_t adresa_registra){
float citanie_teploty(uint8_t adresa_zariadenia){
	uint8_t hod_reg_tep_L = i2c_master_read_byte(adresa_zariadenia, 2A, 1);
	uint8_t hod_reg_tep_H = i2c_master_read_byte(adresa_zariadenia, 2B, 1);

	uint16_t hod_reg_tep = (int16_t)(hod_reg_temp_H << 8) | hod_reg_temp_L;
	return hod_reg_tep/8.0;
}

float citanie_vlhkosti(uint8_t adresa_zariadenia){
	uint8_t hod_reg_vlh_L = i2c_master_read_byte(adresa_zariadenia, 28, 1);
	uint8_t hod_reg_vlh_H = i2c_master_read_byte(adresa_zariadenia, 29, 1);

	uint16_t hod_reg_vlh = (int16_t)(hod_reg_vlh_H << 8) | hod_reg_vlh_L;
	return hod_reg_vlh/8.0;
}
