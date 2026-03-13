#pragma once

// define macros that would otherwise be managed 
// in the project configuration
#define CONFIG_ESP_MAXIMUM_RETRY 5
#define CONFIG_ESP_WIFI_AUTH_WPA2_PSK 1
#define CONFIG_ESP_WPA3_SAE_PWE_HUNT_AND_PECK 1

void wifi_init_sta(void);
const char* get_hostname(void);
