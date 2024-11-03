/*
 * hts221.c
 *
 *  Created on: Nov 2, 2024
 *      Author: petri
 */

#include "i2c.h"
#include "hts221.h"

uint8_t adresa_hts221 = HTS221_DEVICE_ADDRESS_0;
float HTS221_HumiditySlope;
float HTS221_HumidityZero;
float HTS221_TemperatureSlope;
float HTS221_TemperatureZero;

// Read multiple bytes
void hts221_citaj(uint8_t adresa_registra, uint8_t* hodnoty, size_t dlzka) {
    i2c_master_citaj(hodnoty, dlzka, adresa_registra, adresa_hts221, 1);
}

// Write multiple bytes
void hts221_zapisuj(uint8_t adresa_registra, uint8_t* hodnoty, size_t dlzka) {
    i2c_master_zapisuj(hodnoty, dlzka, adresa_registra, adresa_hts221, 0);
}

void hts221_init(void) {
    uint8_t zistenie_senzoru;

    hts221_citaj(HTS221_WHO_AM_I_ADDRESS, &zistenie_senzoru, 1);

    if (!(zistenie_senzoru == HTS221_WHO_AM_I_VALUE)) {
        adresa_hts221 = HTS221_DEVICE_ADDRESS_1;
        hts221_citaj(HTS221_WHO_AM_I_ADDRESS, &zistenie_senzoru, 1);
    }

    uint8_t ctrl_setting = 134;
    hts221_zapisuj(HTS221_CTRL1, &ctrl_setting, 1);
}

// Combined humidity calibration and reading function
void hts221_vlhkost(float* vystup_vlhkost) {
    static uint8_t calibrated = 0;  // Flag to check if calibration is done

    if (!calibrated) {
        uint8_t calibration_data[2]; // Buffer to hold H0_rH and H1_rH
        uint8_t h0T0Out_data[2], h1T0Out_data[2]; // Buffers for H0_T0_OUT and H1_T0_OUT
        int16_t h0_t0_out, h1_t0_out;

        hts221_citaj(adresa_registra, hodnoty, dlzka)(HTS221_H0_rH_x2 | 0x80, calibration_data, 2);
        uint8_t h0_rH = calibration_data[0] >> 1;
        uint8_t h1_rH = calibration_data[1] >> 1;

        hts221_citaj(HTS221_H0_T0_OUT_H | 0x80, h0T0Out_data, 2);
        hts221_citaj(HTS221_H0_T0_OUT_L | 0x80, h1T0Out_data, 2);

        h0_t0_out = (int16_t)((uint16_t)h0T0Out_data[1] << 8 | (uint16_t)h0T0Out_data[0]);
        h1_t0_out = (int16_t)((uint16_t)h1T0Out_data[1] << 8 | (uint16_t)h1T0Out_data[0]);

        HTS221_HumiditySlope = (float)(h1_rH - h0_rH) / (h1_t0_out - h0_t0_out);
        HTS221_HumidityZero = h0_rH - HTS221_HumiditySlope * h0_t0_out;

        calibrated = 1; // Mark calibration as done
    }

    uint8_t h_out_data[2];
    int16_t h_out;
    hts221_citaj(HTS221_HUMIDITY_OUT_L, h_out_data, 2);
    h_out = (int16_t)(h_out_data[0] | (h_out_data[1] << 8));

    *vystup_vlhkost = (h_out * HTS221_HumiditySlope + HTS221_HumidityZero);
}

// Combined temperature calibration and reading function
void hts221_teplota(float* vystup_teplota) {
    static uint8_t calibrated = 0;  // Flag to check if calibration is done

    if (!calibrated) {
        uint8_t calibration_data[4]; // Buffer for T0/T1 calibration values
        uint8_t t_out_data[4];       // Buffer for T0_OUT/T1_OUT
        int16_t t0_out, t1_out;
        uint16_t t0_degC, t1_degC;

        hts221_citaj(HTS221_T0_degC_x8 | 0x80, calibration_data, 4);

        t0_degC = ((uint16_t)(calibration_data[0]) | ((uint16_t)(calibration_data[2] & 0x03) << 8));
        t1_degC = ((uint16_t)(calibration_data[1]) | ((uint16_t)(calibration_data[2] & 0x0C) << 8));

        hts221_citaj(HTS221_T0_OUT_L | 0x80, t_out_data, 4);

        t0_out = (int16_t)(t_out_data[1] << 8 | t_out_data[0]);
        t1_out = (int16_t)(t_out_data[3] << 8 | t_out_data[2]);

        HTS221_TemperatureSlope = (t1_degC - t0_degC) / (8.0 * (t1_out - t0_out));
        HTS221_TemperatureZero = (t0_degC / 8.0) - HTS221_TemperatureSlope * t0_out;

        calibrated = 1; // Mark calibration as done
    }

    uint8_t t_out_buffer[2];
    int16_t t_out;
    hts221_citaj(HTS221_TEMP_OUT_L | 0x80, t_out_buffer, 2);

    t_out = (int16_t)((uint16_t)t_out_buffer[1] << 8 | t_out_buffer[0]);

    *vystup_teplota = (t_out * HTS221_TemperatureSlope) + HTS221_TemperatureZero;
}
