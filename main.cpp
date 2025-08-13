#include "pcm5242_i2c.h"
#include "esp_log.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

static const char* TAG = "MAIN";

// Example biquad coefficients (e.g., for a low-pass filter)
// These are just dummy values. You should replace them with your actual coefficients.
// These should be calculated based on your desired filter characteristics.
// For example, using a tool like MATLAB or an online biquad calculator.
// The values are signed 16-bit integers.
const int16_t biquad_coeffs[5] = {
    819,   // N0
    1638,  // N1
    819,   // N2
    -15327, // D1
    6028   // D2
};

// Main application entry point
void app_main(void) {
    ESP_LOGI(TAG, "Initializing I2C master...");
    esp_err_t ret = i2c_master_init();
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "I2C master initialization failed");
        return;
    }
    ESP_LOGI(TAG, "I2C master initialized successfully");

    // A small delay to ensure the PCM5242 is ready
    vTaskDelay(pdMS_TO_TICKS(100));

    ESP_LOGI(TAG, "Writing biquad coefficients to PCM5242...");
    ret = pcm5242_write_biquad_coeffs(biquad_coeffs);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to write biquad coefficients");
    } else {
        ESP_LOGI(TAG, "Biquad coefficients written successfully");
    }
}
