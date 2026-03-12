# Issue #6 Project derived from this stops publishing

## 2026-03-11 start testing

* Build and monitor. Usding an ESP32 WROOM clone of some sort. Initial plan is just to let it run and see if it stops publishing at some point. It's publishing 1/s so if the issue depends on number of messages sent, it should manifest within 20 minutes. Timestamp for the first message is 1773245252 (1107.) Last message was published at 1773245927 (1118.) Console output at that time was:

```text
I (787034) mqtt: sent publish successful, msg_id=0
I (788034) mqtt: sent publish successful, msg_id=0
W (799044) transport_base: Poll timeout or error, errno=Success, fd=54, timeout_ms=10000
E (799044) mqtt_client: Writing didn't complete in specified timeout: errno=0
I (799044) mqtt: MQTT_EVENT_DISCONNECTED
W (799054) mqtt_client: Publish: Losing qos0 data when client not connected
I (799054) mqtt: sent publish successful, msg_id=-1
W (800064) mqtt_client: Publish: Losing qos0 data when client not connected
I (800064) mqtt: sent publish successful, msg_id=-1
W (801064) mqtt_client: Publish: Losing qos0 data when client not connected
I (801064) mqtt: sent publish successful, msg_id=-1
W (802064) mqtt_client: Publish: Losing qos0 data when client not connected
I (802064) mqtt: sent publish successful, msg_id=-1
```

* Observations:
    * `MQTT_EVENT_DISCONNECTED` Not handled.
    * `sent publish successful, msg_id=-1` client did not recognize that message not sent.

* Adding `connection_messages true` to `mosquitto` broker configuration and bouncing before repeating the previous test. NB, the log does not include every message published.

The next blowup took a lot longer to manifest. Apparently the fault is not deterministic. From console output:

```text
I (4332004) mqtt: sent publish successful, msg_id=0
I (4333004) mqtt: sent publish successful, msg_id=0
I (4343634) mqtt: sent publish successful, msg_id=0
W (4354644) transport_base: Poll timeout or error, errno=Success, fd=54, timeout_ms=10000
E (4354644) mqtt_client: Writing didn't complete in specified timeout: errno=0
I (4354644) mqtt: MQTT_EVENT_DISCONNECTED
W (4354654) mqtt_client: Publish: Losing qos0 data when client not connected
I (4354654) mqtt: sent publish successful, msg_id=-1
```

Values in parenthesis are milliseconds The timestamp of the last successful message was 1773251349. Correlating the msed from the first message (6004) to the embedded timestamp (1773247021) the time stamp of the last interchanges can be determined to be 1773251349.

|ms|delta|msg|
|---|---|---|
|4332004|0|mqtt: sent publish successful, msg_id=0|
|4333004|1000|mqtt: sent publish successful, msg_id=0|
|4343634|1630|mqtt: sent publish successful, msg_id=0|
|4354644|1640|transport_base: Poll timeout or error, errno=Success, fd=54, timeout_ms=10000
|4354644|1640|mqtt_client: Writing didn't complete in specified timeout: errno=0|
|4354644|1640|mqtt: MQTT_EVENT_DISCONNECTED|
|4354654|1650|mqtt_client: Publish: Losing qos0 data when client not connected|
|4354654|1650|mqtt: sent publish successful, msg_id=-1

One odd thing here is the 0.6s difference between the last and nest to last message.

From the broker log:

```text
1773247021: New connection from 10.10.1.85:53416 on port 1883.
1773247021: New client connected from 10.10.1.85:53416 as ESP32_b9ACE8 (p5, c1, k120, u'123').
...
1773251405: Client ESP32_b9ACE8 closed its connection.
1773251416: Expiring client ESP32_b9ACE8 due to timeout.
```

According to the epoch time stamps, the broker reported the closed connection 56s after the client's last successful message. I'm not able to draw any conclusions about this. However it seems that the next thing to do is to work with error recovery since regardless if the connection is held open or opened just long enough for a message (or burst of messages) the error handling should recognize and recover from this issue.

Searching error messages:

* `transport_base: Poll timeout or error, errno=Success, fd=54, timeout_ms=10000` identifies this as a networking error and links <https://esp32.com/viewtopic.php?t=19683> for a similar issue. See also <https://github.com/espressif/esp-idf/issues/8587>. The ESP is pingable after this so it seems that the network transport does recover.
* `mqtt_client: Writing didn't complete in specified timeout: errno=0` seems to be the same as the previuos message"
* `mqtt: MQTT_EVENT_DISCONNECTED` is an application notification and where the situation can be handled. It seems that `esp_mqtt_client_reconnect()` is the correct call <https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/protocols/mqtt.html#_CPPv421esp_mqtt_client_start24esp_mqtt_client_handle_t>
* Before starting a long test increase the frequency of messages by 3 to see if this can be provoked sooner.

```text
I (1904) esp_netif_handlers: sta ip: 10.10.1.85, mask: 255.255.240.0, gw: 10.10.0.1
I (1904) wifi station: got ip:10.10.1.85
I (1904) wifi station: connected to ap SSID:??? password:???
I (1914) sntp: Initializing SNTP
I (1914) sntp: List of configured NTP servers:
I (1924) sntp: server 0: pool.ntp.org
I (1924) sntp: Time is not set yet. Connecting to WiFi and getting time over NTP.
I (1934) sntp: Waiting for system time to be set... (1/15)
I (3934) example: init_sntp(): 1
I (3934) mqtt: [APP] Startup..
I (3934) mqtt: [APP] Free memory: 229964 bytes
I (3934) mqtt: [APP] IDF version: v5.1.6-1451-ga247a4dbe1
I (3944) mqtt: MQTT_EVENT_BEFORE_CONNECT
I (3974) mqtt: MQTT_EVENT_CONNECTED
I (3974) mqtt: sent publish successful, msg_id=0
```

Broker log when the connection was interrupted to rebuild with a higher message frequency (and including stopping and restarting `mosquitto_sub`)

```text
1773262547: New connection from 10.10.1.85:55109 on port 1883.
1773262547: New client connected from 10.10.1.85:55109 as ESP32_b9ACE8 (p5, c1, k120, u'123').
1773262583: Client auto-83284BB8-3BCF-C898-EA57-DABE9389A72C disconnected.
1773262588: New connection from ::1:39766 on port 1883.
1773262588: New client connected from ::1:39766 as auto-2DB2687E-A568-8E1C-8706-23AA55B34A62 (p2, c1, k60).
1773262599: New connection from 10.10.1.85:53386 on port 1883.
1773262599: Client ESP32_b9ACE8 already connected, closing old connection.
1773262599: New client connected from 10.10.1.85:53386 as ESP32_b9ACE8 (p5, c1, k120, u'123').
1773262704: Client auto-2DB2687E-A568-8E1C-8706-23AA55B34A62 disconnected.
1773262708: New connection from ::1:40552 on port 1883.
1773262708: New client connected from ::1:40552 as auto-329F57E1-07EC-7D25-1599-BD79A9865999 (p2, c1, k60).
1773262725: New connection from 10.10.1.85:64116 on port 1883.
1773262725: Client ESP32_b9ACE8 already connected, closing old connection.
1773262725: New client connected from 10.10.1.85:64116 as ESP32_b9ACE8 (p5, c1, k120, u'123').
```

And the monitoring client `mosquitto_sub` output:

```text
hbarta@olive:~/Downloads/mqtt$ mosquitto_sub -v -t \#
/topic/repeating heap total:216700, used:76848, rssi:-46
/topic/repeating/C3 heap total:206780, used:75244, rssi:-61
hello dropped


topic/repeating/C3 ts 1773262724: heap total:219420, used:75512, rssi:-57
/topic/repeating/C3 ts 1773262725: heap total:218840, used:76056, rssi:-57
/topic/repeating/C3 ts 1773262725: heap total:218840, used:76056, rssi:-57
/topic/repeating/C3 ts 1773262725: heap total:218840, used:76056, rssi:-57
/topic/repeating/C3 ts 1773262726: heap total:218852, used:76044, rssi:-57
/topic/repeating/C3 ts 1773262726: heap total:218852, used:76044, rssi:-57
/topic/repeating/C3 ts 1773262726: heap total:218852, used:76044, rssi:-56
...
```

3 hours later and no blowup - returning to 1/second.

## 2026-03-12 overnight and no hiccup

Moving the test setup to a location further from the AP. RSSI now -72 from -52. Perhaps that will provoke some comm issues. Perhaps bounbcing or pausing the broker would also work.
