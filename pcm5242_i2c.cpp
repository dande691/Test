#include "pcm5242_i2c.h"
#include <string.h>
#include "esp_log.h"

static const char* TAG = "PCM5242";

// PCM5242 Register Definitions
#define PAGE_SELECT_REG 0x00
#define BIQUAD_PAGE 44

// Starting register addresses for the biquad coefficients on page 44
static const uint8_t coeff_reg_addrs[] = {
    48, // N0
    52, // N1
    56, // N2
    60, // D1
    64  // D2
};

// Helper function to select a register page
static esp_err_t pcm5242_select_page(uint8_t page) {
    uint8_t write_buf[] = {PAGE_SELECT_REG, page};
    return i2c_master_write_to_device(I2C_MASTER_NUM, PCM5242_I2C_ADDR, write_buf, sizeof(write_buf), pdMS_TO_TICKS(1000));
}

// Helper function to write a 32-bit value to a register
static esp_err_t pcm5242_write_32bit_reg(uint8_t reg, uint32_t value) {
    uint8_t write_buf[5];
    write_buf[0] = reg;
    write_buf[1] = (value >> 24) & 0xFF;
    write_buf[2] = (value >> 16) & 0xFF;
    write_buf[3] = (value >> 8) & 0xFF;
    write_buf[4] = value & 0xFF;
    return i2c_master_write_to_device(I2C_MASTER_NUM, PCM5242_I2C_ADDR, write_buf, sizeof(write_buf), pdMS_TO_TICKS(1000));
}

esp_err_t i2c_master_init(void) {
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_MASTER_FREQ_HZ,
    };
    i2c_param_config(I2C_MASTER_NUM, &conf);
    return i2c_driver_install(I2C_MASTER_NUM, conf.mode, I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE, 0);
}

esp_err_t pcm5242_write_biquad_coeffs(const int16_t* coeffs) {
    esp_err_t ret;

    // Select the page containing the biquad coefficients
    ret = pcm5242_select_page(BIQUAD_PAGE);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to select page %d", BIQUAD_PAGE);
        return ret;
    }

    ESP_LOGI(TAG, "Successfully selected page %d", BIQUAD_PAGE);

    // Write each of the 5 coefficients
    for (int i = 0; i < 5; ++i) {
        // Convert 16-bit coefficient to 32-bit value (left-shifted by 16)
        uint32_t coeff_32bit = ((uint32_t)coeffs[i]) << 16;

        ret = pcm5242_write_32bit_reg(coeff_reg_addrs[i], coeff_32bit);
        if (ret != ESP_OK) {
            ESP_LOGE(TAG, "Failed to write coefficient %d to register %d", i, coeff_reg_addrs[i]);
            return ret;
        }
        ESP_LOGI(TAG, "Wrote coefficient %d (0x%04X -> 0x%08X) to register %d", i, (uint16_t)coeffs[i], coeff_32bit, coeff_reg_addrs[i]);
    }

    return ESP_OK;
}
