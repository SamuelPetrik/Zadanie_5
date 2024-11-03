/*
 * lps25hb.c
 *
 *  Created on: Nov 3, 2024
 *      Author: petri
 */
/*#include "i2c.h"
//float citanie_teploty(uint8_t adresa_zariadenia, uint8_t adresa_registra){
float citanie_tlaku(uint8_t adresa_zariadenia){
	uint8_t hod_reg_tlak_XL = i2c_master_read_byte(adresa_zariadenia, 28, 1);
	uint8_t hod_reg_tlak_L = i2c_master_read_byte(adresa_zariadenia, 29, 1);
	uint8_t hod_reg_tlak_H = i2c_master_read_byte(adresa_zariadenia, 2A, 1);

	uint32_t hod_reg_tlak = (int32_t)(hod_reg_tlak_H << 16) | (int32_t)(hod_reg_tlak_L << 8) | hod_reg_tlak_XL;
	return hod_reg_tlak/8.0;
}*/

#include "LPS25HB.h"

uint8_t LPS25HB_address = LPS25HB_DEVICE_ADDRESS_0;
uint16_t LPS25HB_PressureOffset;

void LPS25HB_citaj(uint8_t reg_addr, uint8_t* values, size_t length) {
    i2c_master_read(values, length, reg_addr, LPS25HB_address, 1);
    // Added a delay or a check for 'end_of_read_flag' to ensure data has been read before proceeding

}

void LPS25HB_zapisuj(uint8_t reg_addr, uint8_t* values, size_t length) {
    i2c_master_write_multi(values, length, reg_addr, LPS25HB_address, 0);
}


void LPS25HB_init() {
    uint8_t whoAmI = 0;
    LPS25HB_citaj(LPS25HB_WHO_AM_I_ADDRESS, &whoAmI, 1);

    if (!(whoAmI == LPS25HB_WHO_AM_I_VALUE)) {
        LPS25HB_address = LPS25HB_DEVICE_ADDRESS_1;

        LPS25HB_citaj(LPS25HB_WHO_AM_I_ADDRESS, &whoAmI, 1);
        if (whoAmI == LPS25HB_WHO_AM_I_VALUE) {
            uint8_t ctrl1 = 148;
            LPS25HB_zapisuj(LPS25HB_ADDRESS_CTRL1, &ctrl1, 1);
            LPS25HB_get_pressure_calibration();
        }
    }
}


void LPS25HB_tlak(float* tlak) {
    static int is_calibrated = 0;
    uint8_t buffer[3];

    // Ak kalibračné údaje ešte neboli načítané, načítame ich
    if (!is_calibrated) {
        uint8_t calibration_buffer[2];
        // Čítanie kalibračných údajov (RPDS_L a RPDS_H) s automatickým inkrementovaním adresy
        LPS25HB_citaj(LPS25HB_ADDRESS_RPDS_L | 0x80, calibration_buffer, 2);

        // Spojenie dvoch bajtov do 16-bitovej hodnoty kalibračného posunu tlaku
        LPS25HB_PressureOffset = (int16_t)(calibration_buffer[0] | (calibration_buffer[1] << 8));
        is_calibrated = 1; // Nastavenie príznaku, že kalibrácia bola načítaná
    }

    // Čítanie troch bajtov aktuálneho tlaku z registra PRESS_OUT_XL
    LPS25HB_citaj(LPS25HB_ADDRESS_PRESS_OUT_XL | 0x80, buffer, 3);

    // Spojenie troch bajtov do 24-bitovej hodnoty tlaku
    uint32_t p_out = (uint32_t)(buffer[2] << 16) | (buffer[1] << 8) | buffer[0];

    // Výpočet tlaku a uloženie do výstupnej premennej
    *pressure = p_out / 4096.0f;
}
