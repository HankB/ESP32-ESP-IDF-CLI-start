/* Provide SNTP support for an SSP-IDF based applicatio0n.
 * Heavily cribbed from examples/protocols/sntp/main/sntp_example_main.c
 *
 * Further documentation at
 * https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/network/esp_netif_programming.html#esp-netif-sntp-api
 *
 * This example will configure SNTP to get the server from the DHCP assignment:https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/network/esp_netif_programming.html#use-dhcp-obtained-sntp-server-s
 * Or it will not. The 'sntp' example code compiles fine with NTP server from DHCP but this file finds
 * esp_netif_sntp_start() to not be included in the headers and I cannot figure out how to fix that so I'm switching
 * to statically defined NTP server.
 */

#include <time.h>
#include <sys/time.h>
#include <esp_system.h>
#include <esp_sntp.h>
#include <esp_log.h>
#include "esp_netif_sntp.h"

#include "proj_sntp.h"

/* The following include is excluded from the git project and
   must be crafted to meet your needs. It must define

#define SSID "your_SSID"
#define  PWD "your_password"
#define sntp_server "pfsense.localdomain" <<<<<<<<<<<<<<<
*/
#include "secrets.h" // user provided include

static const char *TAG = "sntp";

void time_sync_notification_cb(struct timeval *tv)
{
    ESP_LOGI(TAG, "Notification of a time synchronization event");
}

time_t init_sntp(void)
{
    time_t now;
    struct tm timeinfo;
    time(&now);
    localtime_r(&now, &timeinfo);
    ESP_LOGI(TAG, "Initializing SNTP");

    /*
    esp_sntp_config_t config = ESP_NETIF_SNTP_DEFAULT_CONFIG_MULTIPLE(0, {});
    config.start = false;            // start the SNTP service explicitly
    config.server_from_dhcp = false; // accept the NTP offer from the DHCP server
    esp_netif_sntp_init(&config);
    esp_netif_sntp_start();
    */

    esp_sntp_config_t config = ESP_NETIF_SNTP_DEFAULT_CONFIG_MULTIPLE(2,
                                                                      ESP_SNTP_SERVER_LIST("time.windows.com", "pool.ntp.org"));
    esp_netif_sntp_init(&config);
    sntp_set_time_sync_notification_cb(time_sync_notification_cb);
    /*    esp_sntp_setoperatingmode(SNTP_OPMODE_POLL);

        //esp_sntp_setservername(0, "2.us.pool.ntp.org");
        esp_sntp_init();
    */
    // Is time set? If not, tm_year will be (1970 - 1900).
    if (timeinfo.tm_year < (2016 - 1900))
    {
        ESP_LOGI(TAG, "Time is not set yet. Connecting to WiFi and getting time over NTP.");
        /*
        sntp_servermode_dhcp(true); // accept NTP offers from DHCP server, if any
        initialize_sntp();
        */

        // wait for time to be set
        time_t now = 0;
        struct tm timeinfo = {0};
        int retry = 0;
        const int retry_count = 15;
        while (sntp_get_sync_status() == SNTP_SYNC_STATUS_RESET && ++retry < retry_count)
        {
            ESP_LOGI(TAG, "Waiting for system time to be set... (%d/%d)", retry, retry_count);
            vTaskDelay(2000 / portTICK_PERIOD_MS);
        }
        time(&now);
        localtime_r(&now, &timeinfo);
        // update 'now' variable with current time
        time(&now);
    }
    return now;
}
