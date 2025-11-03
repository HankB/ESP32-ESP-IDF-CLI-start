# ESP32 and ESP32-C3 using ESP-IDF from the command line.

*The starting point for this project is the `blink` example and the original README can be found at <https://github.com/espressif/esp-idf/blob/master/examples/get-started/blink/README.md>. Everything in this README is mine.

## Useful links

* <https://docs.espressif.com/projects/esp-idf/en/stable/esp32/index.html> Programming guide
* <https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/index.html> API reference
* <https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-guides/index.html> API guide

## Motivation

Using the ESP-IDF SDK provided by Espressif is the most direct way to leverage the packages they provide. In particular the DS18B20 drivers were near trivial with ESP-IDF and ESP-IDF on PlatformIO took me down rabbit holes. The ESP-IDF plugin for VS Code didn't configure because of decisions Debian has made about installing Python packages using `pip` (and which I agree with.) I do use VS Code for editing files and the CLI `idf.py` to build, flash and monitor.

## Status

See [Setup](./setup.md) for the shaggy dog story on setup and progress.

* 2025-10-21 MQTT cleanup complete for now.
* 2025-10-10 upgrade ESP_IDF 5.4.2 to 5.5.1. And tweak some things to build.
* 2025-07-21 Tweak MQTT to
    * not close the connection
    * not subscribe (?)
    * publish messages every second
    * Working with both ESP32-C3 and ESP32 architectures.
* 2025-07-20 Continue working with an ESP32 NodeMCU (There seem to be several variations)
* 2025-07-19 ESP32-C3 (RISC-V based) testing. Limited success. Working Blink and DS18B20. WiFi almost never connects. Boards on their way back to Amazon.
* 2025-05-06 MQTT5 pub/sub working.
* 2025-05-06 NTP time sync working.
* 2025-05-06 WiFi station example associates and gets IP address
* 2025-05-05 Blue flashing LED.

## Plans

See [Roadmap](./roadmap.md)

* Migrate to ESP32-C3 (should still work with Tensilica based ESP32.)
* Implement multiple MQTT brokers (e.g. if one does not work, try another.)
* Massive code cleanup

### Prep

See [Fun_with_ESP32"](https://github.com/HankB/Fun_with_ESP32/tree/main/ESP-IDF) and the Programming Guide linked above for instructions on installing the tool chain, configuriing environment variables and selecting the ESP32-C3 target.

```text
. ~/esp/esp-idf/export.sh
idf.py set-target [esp32|esp32c3] # or any listed by "idf.py set-target"
idf.py menuconfig # and check entries in "Example Configuration" and "Component config -> LWIP -> SNTP -> Request NTP servers from DHCP"
```

## Errata

Note: SSID and password does not come from example config but rather the `secrets.h` file which the user must provide. The example builds but does not connect to my AP. Setting this aside to try a WiFi example. No joy. Neither the `station` or the `softAP` examples provided any workable WiFi. These are going back to Amazon. Or not. I found this post that suggested reducing the TX power <https://forum.arduino.cc/t/no-wifi-connect-with-esp32-c3-super-mini/1324046/21> I tried it in the Arduino sketch and it worked. I commented the line out and WiFi still worked. I tried my ESP-IDF sketch and it worked. And now it is not. :-/ Occasionally it does connect but not reliably enough to be useful. Followup: I bought some other ESP32-C3 boards and they work with WiFi just fine.
