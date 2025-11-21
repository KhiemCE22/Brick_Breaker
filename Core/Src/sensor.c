/*
 * sensor.c
 */

/* Includes */
#include "sensor.h"
#include "adc.h"

/* Variables */
uint16_t adc_receive[5];

/* Functions */
void sensor_init() {
	HAL_ADC_Start_DMA(&hadc1, (void*) adc_receive, 5);
}

void sensor_read() {
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*) adc_receive, 5);
}

uint16_t sensor_get_light() {
	return adc_receive[2];
}

uint16_t pot_get_median()
{
    uint16_t samples[5];

    // Lấy 5 mẫu liên tiếp từ DMA buffer
    for (int i = 0; i < 5; i++) {
        samples[i] = adc_receive[3];
    }

    // Sắp xếp 5 phần tử (bubble sort)
    for (int i = 0; i < 4; i++) {
        for (int j = i + 1; j < 5; j++) {
            if (samples[j] < samples[i]) {
                uint16_t t = samples[i];
                samples[i] = samples[j];
                samples[j] = t;
            }
        }
    }

    return samples[2];   // median
}


uint16_t sensor_get_potentiometer()
{
    static float smooth = 0;   // giá trị EMA
    const float alpha = 0.1f;  // độ mượt (0.05 – 0.2 tốt nhất)

    uint16_t median = pot_get_median();

    // EMA (Exponential Moving Average)
    smooth = alpha * median + (1.0f - alpha) * smooth;

    return (uint16_t)smooth;
}

float sensor_get_voltage() {
	return ((float) adc_receive[0] * 3.3 * 12) / (4095 * 1.565);
}

float sensor_get_current() {
	return (((float) adc_receive[1] * 3.3 * 1000) / (4095 * 0.647) - 2.5) * 5
			/ 2.5;
}

float sensor_get_temperature() {
	return ((float) adc_receive[4] * 330) / (4095);
}

