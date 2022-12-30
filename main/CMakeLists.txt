set(srcs "main_mcp.c" "gpio.c" "spi.c")
set(requires esp_adc_cal nvs_flash json mdns bt esp_http_server fatfs spiffs esp_https_server)

set(SRCS_PATH "./main")

idf_component_register(SRCS ${srcs}
                       INCLUDE_DIRS "." "../../"
                       REQUIRES ${requires}
                      )
