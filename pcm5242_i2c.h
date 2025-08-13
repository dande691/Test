#ifndef PCM5242_I2C_H
#define PCM5242_I2C_H

#include "driver/i2c.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// Define the I2C address for the PCM5242.
// This is a common default, but you may need to change it based on your hardware configuration.
#define PCM5242_I2C_ADDR 0x4C

// I2C configuration
#define I2C_MASTER_SCL_IO           22      /*!< gpio number for I2C master clock */
#define I2C_MASTER_SDA_IO           21      /*!< gpio number for I2C master data  */
#define I2C_MASTER_NUM              I2C_NUM_0 /*!< I2C port number for master dev */
#define I2C_MASTER_FREQ_HZ          100000  /*!< I2C master clock frequency */
#define I2C_MASTER_TX_BUF_DISABLE   0       /*!< I2C master doesn't need buffer */
#define I2C_MASTER_RX_BUF_DISABLE   0       /*!< I2C master doesn't need buffer */

/**
 * @brief Initialize the I2C master bus.
 *
 * @return esp_err_t ESP_OK on success, otherwise an error code.
 */
esp_err_t i2c_master_init(void);

/**
 * @brief Write biquad coefficients to the PCM5242.
 *
 * This function writes five 16-bit coefficients for a single biquad filter.
 * The coefficients are for N0, N1, N2, D1, and D2.
 *
 * @param coeffs An array of 5 int16_t coefficients.
 * @return esp_err_t ESP_OK on success, otherwise an error code.
 */
esp_err_t pcm5242_write_biquad_coeffs(const int16_t* coeffs);

#ifdef __cplusplus
}
#endif

#endif // PCM5242_I2C_H
