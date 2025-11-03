# Setup

This is turning into a shaggy dog story so it seems best to split it out into its own document.

## 2025-05-05 Setup

* <https://docs.espressif.com/projects/esp-idf/en/stable/esp32/get-started/index.html> Instructions for installing the tool chain and SDK. (I'm installing on Linux - Debian Bookworm.) With everything installed, I start the terminal session and copy the `blink` project files to this directory by:

```text
# set environment vars
. ~/esp/esp-idf/export.sh

# copy the files, not the directory.
cp -r $IDF_PATH/examples/get-started/blink/* .

# see the original README for suggested targets or run "idf.py set-target"
idf.py set-target esp32

# set GPIO for LED in "examples" to '2' - the builtin blue LED.
idf.py menuconfig 

idf.py build
idf.py flash monitor
```

Great success! (Blue flashing LED on my NodeMCU.)

Result looks like

```text
hbarta@olive:~/Programming/ESP32/ESP32-ESP-IDF-CLI-start$ idf.py monitor
Executing action: monitor
Serial port /dev/ttyUSB0
Connecting.....
Detecting chip type... Unsupported detection protocol, switching and trying again...
Connecting..........
Detecting chip type... ESP32
Running idf_monitor in directory /home/hbarta/Programming/ESP32/ESP32-ESP-IDF-CLI-start
Executing "/home/hbarta/.espressif/python_env/idf5.4_py3.11_env/bin/python /home/hbarta/esp/esp-idf/tools/idf_monitor.py -p /dev/ttyUSB0 -b 115200 --toolchain-prefix xtensa-esp32-elf- --target esp32 --revision 0 /home/hbarta/Programming/ESP32/ESP32-ESP-IDF-CLI-start/build/blink.elf -m '/home/hbarta/.espressif/python_env/idf5.4_py3.11_env/bin/python' '/home/hbarta/esp/esp-idf/tools/idf.py'"...
--- esp-idf-monitor 1.6.2 on /dev/ttyUSB0 115200
--- Quit: Ctrl+] | Menu: Ctrl+T | Help: Ctrl+T followed by Ctrl+H
ets Jun  8 2016 00:22:57

rst:0x1 (POWERON_RESET),boot:0x17 (SPI_FAST_FLASH_BOOT)
configsip: 0, SPIWP:0xee
clk_drv:0x00,q_drv:0x00,d_drv:0x00,cs0_drv:0x00,hd_drv:0x00,wp_drv:0x00
mode:DIO, clock div:2
load:0x3fff0030,len:6276
load:0x40078000,len:15748
load:0x40080400,len:4
--- 0x40080400: _init at ??:?

ho 8 tail 4 room 4
load:0x40080404,len:3860
entry 0x4008063c
I (31) boot: ESP-IDF v5.4.1 2nd stage bootloader
I (31) boot: compile time May  5 2025 16:13:50
I (31) boot: Multicore bootloader
I (32) boot: chip revision: v1.0
I (35) boot.esp32: SPI Speed      : 40MHz
I (39) boot.esp32: SPI Mode       : DIO
I (42) boot.esp32: SPI Flash Size : 2MB
I (46) boot: Enabling RNG early entropy source...
I (50) boot: Partition Table:
I (53) boot: ## Label            Usage          Type ST Offset   Length
I (59) boot:  0 nvs              WiFi data        01 02 00009000 00006000
I (66) boot:  1 phy_init         RF data          01 01 0000f000 00001000
I (72) boot:  2 factory          factory app      00 00 00010000 00100000
I (79) boot: End of partition table
I (82) esp_image: segment 0: paddr=00010020 vaddr=3f400020 size=0a03ch ( 41020) map
I (104) esp_image: segment 1: paddr=0001a064 vaddr=3ff80000 size=0001ch (    28) load
I (104) esp_image: segment 2: paddr=0001a088 vaddr=3ffb0000 size=02314h (  8980) load
I (111) esp_image: segment 3: paddr=0001c3a4 vaddr=40080000 size=03c74h ( 15476) load
I (121) esp_image: segment 4: paddr=00020020 vaddr=400d0020 size=1460ch ( 83468) map
I (151) esp_image: segment 5: paddr=00034634 vaddr=40083c74 size=08fc0h ( 36800) load
I (172) boot: Loaded app from partition at offset 0x10000
I (172) boot: Disabling RNG early entropy source...
I (182) cpu_start: Multicore app
I (191) cpu_start: Pro cpu start user code
I (191) cpu_start: cpu freq: 160000000 Hz
I (191) app_init: Application information:
I (191) app_init: Project name:     blink
I (194) app_init: App version:      1
I (198) app_init: Compile time:     May  5 2025 16:14:32
I (203) app_init: ELF file SHA256:  147a9dc35...
I (207) app_init: ESP-IDF:          v5.4.1
I (211) efuse_init: Min chip rev:     v0.0
I (215) efuse_init: Max chip rev:     v3.99 
I (219) efuse_init: Chip rev:         v1.0
I (223) heap_init: Initializing. RAM available for dynamic allocation:
I (229) heap_init: At 3FFAE6E0 len 00001920 (6 KiB): DRAM
I (234) heap_init: At 3FFB2BD8 len 0002D428 (181 KiB): DRAM
I (239) heap_init: At 3FFE0440 len 00003AE0 (14 KiB): D/IRAM
I (245) heap_init: At 3FFE4350 len 0001BCB0 (111 KiB): D/IRAM
I (250) heap_init: At 4008CC34 len 000133CC (76 KiB): IRAM
I (257) spi_flash: detected chip: generic
I (259) spi_flash: flash io: dio
W (262) spi_flash: Detected size(4096k) larger than the size in the binary image header(2048k). Using the size in the binary image header.
I (275) main_task: Started on CPU0
I (285) main_task: Calling app_main()
I (285) example: Example configured to blink GPIO LED!
I (285) gpio: GPIO[2]| InputEn: 0| OutputEn: 0| OpenDrain: 0| Pullup: 1| Pulldown: 0| Intr:0 
I (285) example: Turning the LED OFF!
I (1295) example: Turning the LED ON!
I (2295) example: Turning the LED OFF!
I (3295) example: Turning the LED ON!
I (4295) example: Turning the LED OFF!
I (5295) example: Turning the LED ON!
I (6295) example: Turning the LED OFF!
I (7295) example: Turning the LED ON!
```

## 2025-05-05 add WiFi

Copy WiFi files from <https://github.com/HankB/ESP32-ESP-IDF-PlatformIO-start>. No. the APIs seem to have changed. :-/

## 2025-05-06 WiFi from Espressif example

* <https://github.com/espressif/esp-idf/blob/master/examples/wifi/getting_started/station>

And start naming files and headers in a way that avoids potential conflicts. `wifi.c` is just a bit too generic. Let's make that `proj_wifi.c`

```text
cp $IDF_PATH/examples/wifi/getting_started/station/main/station_example_main.c main/proj_wifi.c
```

And add to `main/CMakeLists.txt`

```text
hbarta@olive:~/Programming/ESP32/ESP32-ESP-IDF-CLI-start$ cat main/CMakeLists.txt 
idf_component_register(SRCS "blink_example_main.c" proj_wifi.c
                       INCLUDE_DIRS ".")
hbarta@olive:~/Programming/ESP32/ESP32-ESP-IDF-CLI-start$ 
```

First build try results in undefined macros. Three of these are covered by the example's `Kconfig.projbuild` and I'll just define them in `secrets.h` (SSID and password and other things that do not get committed or pushed) or `proj_wifi.h`.

### WiFi associates and gets IP address

Apparently uses the host name `espressif`.  WiFi related log messages are:

```text
I (507) wifi station: ESP_WIFI_MODE_STA
I (527) wifi:wifi driver task: 3ffc013c, prio:23, stack:6656, core=0
I (537) wifi:wifi firmware version: 79fa3f41ba
I (537) wifi:wifi certification version: v7.0
I (537) wifi:config NVS flash: enabled
I (537) wifi:config nano formatting: disabled
I (547) wifi:Init data frame dynamic rx buffer num: 32
I (547) wifi:Init static rx mgmt buffer num: 5
I (547) wifi:Init management short buffer num: 32
I (557) wifi:Init dynamic tx buffer num: 32
I (557) wifi:Init static rx buffer size: 1600
I (567) wifi:Init static rx buffer num: 10
I (567) wifi:Init dynamic rx buffer num: 32
I (577) wifi_init: rx ba win: 6
I (577) wifi_init: accept mbox: 6
I (577) wifi_init: tcpip mbox: 32
I (577) wifi_init: udp mbox: 6
I (587) wifi_init: tcp mbox: 6
I (587) wifi_init: tcp tx win: 5760
I (587) wifi_init: tcp rx win: 5760
I (597) wifi_init: tcp mss: 1440
I (597) wifi_init: WiFi IRAM OP enabled
I (597) wifi_init: WiFi RX IRAM OP enabled
I (627) phy_init: phy_version 4860,6b7a6e5,Feb  6 2025,14:47:07
I (697) wifi:mode : sta (08:3a:f2:b9:ac:e8)
I (697) wifi:enable tsf
I (707) wifi station: wifi_init_sta finished.
I (1447) wifi:new:<6,1>, old:<1,0>, ap:<255,255>, sta:<6,1>, prof:1, snd_ch_cfg:0x0
I (1447) wifi:state: init -> auth (0xb0)
I (1487) wifi:state: auth -> assoc (0x0)
I (1507) wifi:state: assoc -> run (0x10)
I (11507) wifi:state: run -> init (0xcc00)
I (11517) wifi:new:<6,0>, old:<6,1>, ap:<255,255>, sta:<6,1>, prof:1, snd_ch_cfg:0x0
I (11517) wifi station: retry to connect to the AP
I (11517) wifi station: connect to the AP fail
I (11527) wifi:new:<6,1>, old:<6,0>, ap:<255,255>, sta:<6,1>, prof:1, snd_ch_cfg:0x0
I (11537) wifi:state: init -> auth (0xb0)
I (11547) wifi:state: auth -> init (0x8a0)
I (11547) wifi:new:<6,0>, old:<6,1>, ap:<255,255>, sta:<6,1>, prof:1, snd_ch_cfg:0x0
I (11557) wifi station: retry to connect to the AP
I (11557) wifi station: connect to the AP fail
I (13967) wifi station: retry to connect to the AP
I (13967) wifi station: connect to the AP fail
I (13977) wifi:new:<6,1>, old:<6,0>, ap:<255,255>, sta:<6,1>, prof:1, snd_ch_cfg:0x0
I (13987) wifi:state: init -> auth (0xb0)
I (14007) wifi:state: auth -> assoc (0x0)
I (14017) wifi:state: assoc -> run (0x10)
I (14047) wifi:connected with Giant Voice System, aid = 5, channel 6, 40U, bssid = 5c:e9:31:0a:d3:a4
I (14047) wifi:security: WPA2-PSK, phy: bgn, rssi: -65
I (14197) wifi:pm start, type: 1

I (14197) wifi:dp: 1, bi: 102400, li: 3, scale listen interval from 307200 us to 307200 us
I (14197) wifi:AP's beacon interval = 102400 us, DTIM period = 1
I (14237) wifi:<ba-add>idx:0 (ifx:0, 5c:e9:31:0a:d3:a4), tid:1, ssn:0, winSize:64
I (15257) esp_netif_handlers: sta ip: 192.168.1.187, mask: 255.255.255.0, gw: 192.168.1.1
I (15257) wifi station: got ip:192.168.1.187
I (15257) wifi station: connected to ap SSID:??? password:???
```

## 2025-05-06 NTP from other project

From <https://github.com/HankB/ESP32-ESP-IDF-PlatformIO-start/tree/main>

```text
cp ../ESP32-ESP-IDF-PlatformIO-start/src/sntp.c main/proj_sntp.c
cp ../ESP32-ESP-IDF-PlatformIO-start/src/sntp.h main/proj_sntp.h
```

It was necessary to run `idf.py menuconfig` "Component config -> LWIP -> SNTP" to enable "Request NTP servers from DHCP" so that `esp_sntp_servermode_dhcp` would be included and resolved.

## 2025-05-06 MQTT from other project

No joy, ran into too many errors in headers - starting with the example for mqtt5. (I tested against my Docker `mosquitto` installation and checked the capabilities of 2.0.11 which ships Debian Bookworm and both support mqtt5.)

```text
cp $IDF_PATH/examples/protocols/mqtt5/main/app_main.c main/proj_mqtt.c
```

Merge mqtt5 entries to `main/Kconfig.projbuild`. Add line from `sdkconfig.defaults` and run `idf.py menuconfig` to configure ESP_MQTT and example settings.

```text
hbarta@olive:~/Programming/ESP32/ESP32-ESP-IDF-CLI-start$ cat main/Kconfig.projbuild 
menu "Example Configuration"

    orsource "$IDF_PATH/examples/common_components/env_caps/$IDF_TARGET/Kconfig.env_caps"

    choice BLINK_LED
        prompt "Blink LED type"
        default BLINK_LED_GPIO
        help
            Select the LED type. A normal level controlled LED or an addressable LED strip.
            The default selection is based on the Espressif DevKit boards.
            You can change the default selection according to your board.

        config BLINK_LED_GPIO
            bool "GPIO"
        config BLINK_LED_STRIP
            bool "LED strip"
    endchoice

    choice BLINK_LED_STRIP_BACKEND
        depends on BLINK_LED_STRIP
        prompt "LED strip backend peripheral"
        default BLINK_LED_STRIP_BACKEND_RMT if SOC_RMT_SUPPORTED
        default BLINK_LED_STRIP_BACKEND_SPI
        help
            Select the backend peripheral to drive the LED strip.

        config BLINK_LED_STRIP_BACKEND_RMT
            depends on SOC_RMT_SUPPORTED
            bool "RMT"
        config BLINK_LED_STRIP_BACKEND_SPI
            bool "SPI"
    endchoice

    config BLINK_GPIO
        int "Blink GPIO number"
        range ENV_GPIO_RANGE_MIN ENV_GPIO_OUT_RANGE_MAX
        default 8
        help
            GPIO number (IOxx) to blink on and off the LED.
            Some GPIOs are used for other purposes (flash connections, etc.) and cannot be used to blink.

    config BLINK_PERIOD
        int "Blink period in ms"
        range 10 3600000
        default 1000
        help
            Define the blinking period in milliseconds.

    config SNTP_TIME_SERVER
        string "SNTP server name"
        default "pool.ntp.org"
        help
            Hostname of the main SNTP server.

    choice SNTP_TIME_SYNC_METHOD
        prompt "Time synchronization method"
        default SNTP_TIME_SYNC_METHOD_IMMED
        help
            Time synchronization method.

        config SNTP_TIME_SYNC_METHOD_IMMED
            bool "update time immediately when received"
        config SNTP_TIME_SYNC_METHOD_SMOOTH
            bool "update time with smooth method (adjtime)"
        config SNTP_TIME_SYNC_METHOD_CUSTOM
            bool "custom implementation"
    endchoice

    config BROKER_URL
        string "Broker URL"
        default "mqtt://mqtt.localdomain"
        help
            URL of the broker to connect to

    config BROKER_URL_FROM_STDIN
        bool
        default y if BROKER_URL = "FROM_STDIN"

endmenu
hbarta@olive:~/Programming/ESP32/ESP32-ESP-IDF-CLI-start$ cat sdkconfig.defaults
CONFIG_BLINK_LED_GPIO=y
CONFIG_BLINK_GPIO=8
CONFIG_MQTT_PROTOCOL_5=y
hbarta@olive:~/Programming/ESP32/ESP32-ESP-IDF-CLI-start$ 
```

Build and resolve issues and it works with my MQTT broker (Mosquitto in a Docker container.) There was an issue when I first ran it:

```text
ESP_ERROR_CHECK failed: esp_err_t 0x103 (ESP_ERR_INVALID_STATE) at 0x400d9fb0
--- 0x400d9fb0: init_mqtt at /home/hbarta/Programming/ESP32/ESP32-ESP-IDF-CLI-start/main/proj_mqtt.c:284 (discriminator 1)

file: "./main/proj_mqtt.c" line 284
func: init_mqtt
expression: esp_event_loop_create_default()

abort() was called at PC 0x400897df on core 0
--- 0x400897df: _esp_error_check_failed at /home/hbarta/esp/esp-idf/components/esp_system/esp_err.c:49
```

It wasn't clear to me what `esp_event_loop_create_default()` did so I commented it out and the example was able to publish w/out difficulty. Switching back to `mqtt://mqtt.eclipseprojects.io` to see if subscriptions work. Seems OK:

```text
I (3925) mqtt5_example: [APP] Startup..
I (3925) mqtt5_example: [APP] Free memory: 227460 bytes
I (3925) mqtt5_example: [APP] IDF version: v5.4.1
I (3935) mqtt5_example: Other event id:7
I (3935) example: Turning the LED OFF at 1746654140!
I (4675) mqtt5_example: MQTT_EVENT_CONNECTED
I (4685) mqtt5_example: sent publish successful, msg_id=27744
I (4685) mqtt5_example: sent subscribe successful, msg_id=22531
I (4685) mqtt5_example: sent subscribe successful, msg_id=22489
I (4695) mqtt5_example: sent unsubscribe successful, msg_id=27281
I (4735) mqtt5_client: MQTT_MSG_TYPE_PUBACK return code is 0
I (4735) mqtt5_example: MQTT_EVENT_PUBLISHED, msg_id=27744
I (4935) example: Turning the LED ON at 1746654141!
I (5065) mqtt5_client: MQTT_MSG_TYPE_SUBACK return code is 0
I (5065) mqtt5_example: MQTT_EVENT_SUBSCRIBED, msg_id=22531
I (5075) mqtt5_example: sent publish successful, msg_id=0
I (5075) mqtt5_client: MQTT_MSG_TYPE_SUBACK return code is 2
I (5075) mqtt5_example: MQTT_EVENT_SUBSCRIBED, msg_id=22489
I (5085) mqtt5_example: sent publish successful, msg_id=0
I (5095) mqtt5_example: MQTT_EVENT_DATA
I (5095) mqtt5_example: key is board, value is esp32
I (5095) mqtt5_example: key is u, value is user
I (5105) mqtt5_example: key is p, value is password
I (5105) mqtt5_example: payload_format_indicator is 1
I (5115) mqtt5_example: response_topic is /topic/test/response
I (5115) mqtt5_example: correlation_data is 123456
I (5125) mqtt5_example: content_type is 
I (5125) mqtt5_example: TOPIC=/topic/qos1
I (5125) mqtt5_example: DATA=data_3
I (5135) mqtt5_client: MQTT_MSG_TYPE_UNSUBACK return code is 0
I (5135) mqtt5_example: MQTT_EVENT_UNSUBSCRIBED, msg_id=27281
I (5145) mqtt_client: Client asked to disconnect
I (5935) example: Turning the LED OFF at 1746654142!
I (6155) mqtt5_example: MQTT_EVENT_DISCONNECTED
```

## 2025-07-19 ESP32-C3 RISC-V

See also <https://github.com/HankB/Fun_with_ESP32/tree/main/ESP-IDF/blink#2025-07-19-local-addendum> for notes on using a previous example with the ESP32-C3. Note that `idf.py set-target esp32c3` will reset the configuration to defaults so everything previously configured will require reconfiguration.

Some cleanup and enhancements complete. Monitoring stack usage - seems good after days, but need to add an 'uptime' count to confirm. Monitor using:

```text
mosquitto_sub -t \# -v -h olive # probably need to use the name for your broker.
```

The output looks like the following after running about a week or so. I'm confident the example is not leaking memory under normal operation. When I deploy, I'll likely include things like uptime (seconds), heap, and RSSI in the eventual payload.

```text
/topic/repeating/C3 heap total:206780, used:75244, rssi:-62
/topic/repeating heap total:216684, used:76864, rssi:-60
/topic/repeating/C3 heap total:206780, used:75244, rssi:-61
/topic/repeating heap total:216684, used:76864, rssi:-61
/topic/repeating/C3 heap total:206780, used:75244, rssi:-61
/topic/repeating heap total:216684, used:76864, rssi:-63
```

## 2025-10-20 Repeating on another host

```text
hbarta@olive:~/Programming/ESP32/ESP32-ESP-IDF-CLI-start$ . ~/esp/esp-idf/export.sh
Checking "python3" ...
Python 3.13.5
"python3" has been detected

ERROR: ESP-IDF Python virtual environment "/home/hbarta/.espressif/python_env/idf5.4_py3.13_env/bin/python" not found. Please run the install script to set it up before proceeding.
hbarta@olive:~/Programming/ESP32/ESP32-ESP-IDF-CLI-start$ 
```

The install script is apparently `/home/hbarta/esp/esp-idf/install.sh` After running this `. ~/esp/esp-idf/export.sh` worked and the project built,l flashed and could be monitored. This seems likely a result of upgrading from Debian Bookworm to Trixie and getting a new Python version.
