#ifndef MAIN_H
#define MAIN_H

#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
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

extern TaskHandle_t acq_adc_ext_task;
extern spi_device_handle_t adc_ext_spi_handler;

#endif
