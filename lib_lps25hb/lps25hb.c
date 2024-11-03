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

#include "lps25hb.h"

uint8_t LPS25HB_address = LPS25HB_DEVICE_ADDRESS_0;
uint16_t LPS25HB_PressureOffset;

void lps25hb_citaj(uint8_t adresa_registra, uint8_t* hodnoty, size_t dlzka) {
    i2c_master_citaj(hodnoty, dlzka, adresa_registra, LPS25HB_address, 1);
    // Added a delay or a check for 'end_of_read_flag' to ensure data has been read before proceeding

}

void lps25hb_zapisuj(uint8_t adresa_registra, uint8_t* hodnoty, size_t dlzka) {
    i2c_master_zapisuj(hodnoty, dlzka, adresa_registra, LPS25HB_address, 0);
}


/*void lps25hb_init() {
    uint8_t whoAmI = 0;
    lps25hb_citaj(LPS25HB_WHO_AM_I_ADDRESS, &whoAmI, 1);

    if (!(whoAmI == LPS25HB_WHO_AM_I_VALUE)) {
        LPS25HB_address = LPS25HB_DEVICE_ADDRESS_1;

        lps25hb_citaj(LPS25HB_WHO_AM_I_ADDRESS, &whoAmI, 1);
        if (whoAmI == LPS25HB_WHO_AM_I_VALUE) {
            uint8_t ctrl1 = 148;
            lps25hb_zapisuj(LPS25HB_ADDRESS_CTRL1, &ctrl1, 1);
            lps25hb_tlak();
        }
    }
}


void lps25hb_tlak(float* tlak) {
    static int is_calibrated = 0;
    uint8_t buffer[3];

    // Ak kalibračné údaje ešte neboli načítané, načítame ich
    if (!is_calibrated) {
        uint8_t calibration_buffer[2];
        // Čítanie kalibračných údajov (RPDS_L a RPDS_H) s automatickým inkrementovaním adresy
        lps25hb_citaj(LPS25HB_ADDRESS_RPDS_L | 0x80, calibration_buffer, 2);

        // Spojenie dvoch bajtov do 16-bitovej hodnoty kalibračného posunu tlaku
        LPS25HB_PressureOffset = (int16_t)(calibration_buffer[0] | (calibration_buffer[1] << 8));
        is_calibrated = 1; // Nastavenie príznaku, že kalibrácia bola načítaná
    }

    // Čítanie troch bajtov aktuálneho tlaku z registra PRESS_OUT_XL
    lps25hb_citaj(LPS25HB_ADDRESS_PRESS_OUT_XL | 0x80, buffer, 3);

    // Spojenie troch bajtov do 24-bitovej hodnoty tlaku
    uint32_t p_out = (uint32_t)(buffer[2] << 16) | (buffer[1] << 8) | buffer[0];

    // Výpočet tlaku a uloženie do výstupnej premennej
    *tlak = p_out / 4096.0f;
}*/
void LPS25HB_init() {
    uint8_t whoAmI = 0;
    lps25hb_citaj(LPS25HB_WHO_AM_I_ADDRESS, &whoAmI, 1);

    if (!(whoAmI == LPS25HB_WHO_AM_I_VALUE)) {
        LPS25HB_address = LPS25HB_DEVICE_ADDRESS_1;

        lps25hb_citaj(LPS25HB_WHO_AM_I_ADDRESS, &whoAmI, 1);
        if (whoAmI == LPS25HB_WHO_AM_I_VALUE) {
            uint8_t ctrl1 = 148;
            lps25hb_zapisuj(LPS25HB_ADDRESS_CTRL1, &ctrl1, 1);
            LPS25HB_get_pressure_calibration();
        }
    }
}


void LPS25HB_get_pressure_calibration(void){
    uint8_t buffer[2];
    // Read two bytes from RPDS_L register with auto-increment to RPDS_H
    lps25hb_citaj(LPS25HB_ADDRESS_RPDS_L | 0x80, buffer, 2);

    // Combine the two bytes into a single 16-bit value
    LPS25HB_PressureOffset = (int16_t)(buffer[0] | (buffer[1] << 8));
}

void LPS25HB_get_pressure(float* pressure) {
    uint8_t buffer[3];
    // Read three bytes from PRESS_OUT_XL register with auto-increment to PRESS_OUT_H
    lps25hb_citaj(LPS25HB_ADDRESS_PRESS_OUT_XL | 0x80, buffer, 3);

    // Combine the three bytes into a single 24-bit value
    uint32_t p_out = (uint32_t)(buffer[2] << 16) | (buffer[1] << 8) | buffer[0];

    // Calculate the actual pressure value
    *pressure = p_out / 4096.0f;
}
