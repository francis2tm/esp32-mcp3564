#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include "nvs.h"
#include "nvs_flash.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_log.h"
#include "esp_bt_main.h"
#include "esp_gap_bt_api.h"
#include "esp_bt_device.h"
#include "esp_spp_api.h"
#include "time.h"
#include "sys/time.h"
#include "driver/timer.h"
#include "esp_adc_cal.h"
#include "driver/spi_master.h"
#include "cJSON.h"

#include "gpio.h"
#include "spi.h"

#define ADC_MCP 2
#define _ADC_EXT_ ADC_MCP

#define ACQ_ADC_EXT_PRIORITY 11

TaskHandle_t acq_adc_ext_task;
spi_device_handle_t adc_ext_spi_handler;

void IRAM_ATTR acqAdcExtTask();


void app_main(void){
    gpioInit();
    xTaskCreatePinnedToCore(&acqAdcExtTask, "acqAdcExtTask", 2048, NULL, ACQ_ADC_EXT_PRIORITY, &acq_adc_ext_task, 1);
}

void IRAM_ATTR acqAdcExtTask(){
    uint8_t channel_mask = 0b01;

    adcExtInit();
    mcpSetupRoutine(channel_mask);
    adcExtStart();

    while(1){
        if(ulTaskNotifyTake(pdTRUE, portMAX_DELAY)){
            gpio_intr_disable(MCP_DRDY_IO);
            int32_t raw_data = mcpReadRegister(REG_ADCDATA, 4);

            //Put CH_ID in right place
            //sample = (raw_data & 0xF0000000) >> 8 | (raw_data & 0x00F00000) << 8 | (raw_data & 0x0F0FFFFF);

            //channel = (uint32_t)sample >> 28;

            //Convert scale from -Vref/2 <-> +Vref/2 to 0 <-> +Vref
            //adc_ext_samples[channel] = sample + 0x01000000;

            int8_t sign = (raw_data >> 24) & 0x01;      // bits [27:34] represent the sign (extended)
            int8_t channel = (raw_data >> 28) & 0x0F;   // 4 MSBs [31-28] represent the CH (SCAN MODE)
            int32_t sample = raw_data & 0x01FFFFFF;     // 
            
            const int32_t VREF = 3.3;

            /* como a eletronica de momento não suporta resolucoes elevadas 
            * esta conversao simula um conversor AD de N bits (10 bits devem chegar para validar nesta fase) */
            const uint8_t N_BITS = 10;
            int32_t sample_n_bits = (raw_data & 0x00FFFFFF) >> (24 - N_BITS); 	// descartar N bits
            float voltage = 0.0;

            if(true)
            //if(channel == 4)  	// útil caso estejas a excitar apenas um canal com tensão conhecida (aumentar o número de amostras "N_SAMPLES")
            {
                if(sign){
                    printf("CH: %d data_hex: 0x%.7x raw: 0x%.8x sign: -\n", channel, sample, raw_data);
                }else{
                    /* só fazer a conversão para tensões positivas*/
                    voltage = ((float) sample_n_bits) * (VREF*2 ) / (pow(2, N_BITS) - 1);

                    printf("CH: %d data_hex: 0x%.7x raw: 0x%.8x s_10b: 0x%.3x voltage: %.3f V\n", channel, sample, raw_data, sample_n_bits, voltage);
                }
            }

            gpio_intr_enable(MCP_DRDY_IO);
        }
    }
}
