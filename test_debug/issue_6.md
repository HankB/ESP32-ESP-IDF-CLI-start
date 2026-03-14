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

Moving the test setup to a location further from the AP. RSSI now -72 from -52. Perhaps that will provoke some comm issues. Perhaps bouncing or pausing the broker would also work. It did. Stopping the broker and restarting it from the command line resulted in a rapid flood of error messages from the console. When the broker was restarted, communication was established. That worked.

It is desirable to add some kind of delay to the retries to avoid flooding WiFi if the broker goes down and until it comes back. Since there may be multiple ESPs trying to reconnect, a variable delay also makes sense. For now, specify a delay between 5 and 15 seconds. This can be implemented in the callback. First several times time testing the result looked like

```text
I (56614) mqtt: sent publish successful, msg_id=0
E (57434) mqtt_client: esp_mqtt_handle_transport_read_error: transport_read(): EOF
E (57434) mqtt_client: esp_mqtt_handle_transport_read_error: transport_read() error: errno=128
I (57434) mqtt: MQTT_EVENT_ERROR
I (57444) mqtt: MQTT5 return code is 0
E (57444) mqtt: Last error reported from esp-tls: 0x8008
I (57454) mqtt: Last errno string (Success)
I (57454) mqtt: Last errno string (Success)
E (57464) mqtt_client: mqtt_process_receive: mqtt_message_receive() returned -2
I (57474) mqtt: MQTT_EVENT_DISCONNECTED
I (70474) mqtt: mqtt5_event_handler() delaying 13 s
I (70474) mqtt: esp_mqtt_client_reconnect(), returned 0
I (70474) mqtt: MQTT_EVENT_BEFORE_CONNECT
I (70514) mqtt: MQTT_EVENT_CONNECTED
I (70514) mqtt: sent publish successful, msg_id=0
I (71514) mqtt: sent publish successful, msg_id=0
```

For further testing I was getting ready to kick off:

```text
root@olive:~# while(:)
> do
> systemctl stop mosquitto;systemctl start mosquitto
> sleep 30
> done
```

Before I hit Return, there was a hickup that matched the problem issue. Of note, the first retry was not successful but the second one was. This seems good.

```text
I (59474) mqtt: sent publish successful, msg_id=0
W (70484) transport_base: Poll timeout or error, errno=Success, fd=54, timeout_ms=10000
E (70484) mqtt_client: Writing didn't complete in specified timeout: errno=0
I (70484) mqtt: MQTT_EVENT_DISCONNECTED
I (70494) mqtt: mqtt5_event_handler() delaying 7 s
I (77494) mqtt: esp_mqtt_client_reconnect(), returned 0
W (77494) mqtt_client: Publish: Losing qos0 data when client not connected
I (77494) mqtt: MQTT_EVENT_BEFORE_CONNECT
I (77504) mqtt: sent publish successful, msg_id=-1
E (87664) mqtt_client: esp_mqtt_connect: mqtt_message_receive() returned 0
E (87664) mqtt_client: MQTT connect failed
I (87664) mqtt: MQTT_EVENT_DISCONNECTED
I (87664) mqtt: mqtt5_event_handler() delaying 7 s
I (94674) mqtt: esp_mqtt_client_reconnect(), returned 0
I (94674) mqtt: MQTT_EVENT_BEFORE_CONNECT
I (95064) mqtt: MQTT_EVENT_CONNECTED
I (95064) mqtt: sent publish successful, msg_id=0
```

### Interruption test

The shell script above will be run to interrupt the 1/s message stream to determine if it reliably recovers. It ran for hours and recovered reliably (every time.)

### long publish interval test

Provide the capability to extend the interval between MQTT messages to see if a long idle interval results in a problem. Testing now with a 60s interval. Also fixed the missing "blink LED" (wrong GPIO.) I've added two more ESP32s for testing including a mini and ESP32-C3 mini. The ESP32-C3 is not getting time from an NTP source because I didn't configure that in `menuconfig`. That seems to get unset when running `idf.py set-target`

## 2026-03-13 test longer intervals

The 1 minute interval tests ran overnight with no disconnects. The interval is expanded to ten minutes and repeated. The first host to start began publishing messages before it had fetched time using NTP. And with three ESPs under test, it will be useful to include some ID with the published messages.

Code just modified to use the hostname alone for the topic and delay 10 minutes between MQTT messages and testing with three ESP32s continues.

At this point (and while testing continues) the performance seens good. Opening/closing the broker connection for each message has not yet been implemented. At this point I will begin to roll this out for applications derived from this template (and learn if Github does anything to facilitate this or if I just need to manually copy files.)

### update on 10 minute intervals

Seen in the broker logs:

```text
1773427710: Expiring client ESP32_998610 due to timeout.
1773428175: New connection from 10.10.0.128:52802 on port 1883.
1773428175: New client connected from 10.10.0.128:52802 as ESP32_998610 (p5, c1, k120, u'123').
1773428356: Client ESP32_998610 closed its connection.
1773428367: Expiring client ESP32_998610 due to timeout.
1773428383: New connection from 10.10.0.128:52803 on port 1883.
1773428383: New client connected from 10.10.0.128:52803 as ESP32_998610 (p5, c1, k120, u'123').
1773428637: Client ESP32_998610 closed its connection.
1773428648: Expiring client ESP32_998610 due to timeout.
```

And calculating the deltas

|timestamp|delta|mm:ss|event|
|---|---|---|---|
|1773427710|||Expiring client ESP32_998610 due to timeout.|
|1773428175|65|1:05|New connection from 10.10.0.128:52802 on port 1883.|
|1773428175|0|0:00|New client connected from 10.10.0.128:52802 as ESP32_998610 (p5, c1, k120, u'123').|
|1773428356|181|3:01|Client ESP32_998610 closed its connection.|
|1773428367|11|0:11|Expiring client ESP32_998610 due to timeout.|
|1773428383|16|0:16|New connection from 10.10.0.128:52803 on port 1883.|
|1773428383|0|0:00|New client connected from 10.10.0.128:52803 as ESP32_998610 (p5, c1, k120, u'123').|
|1773428637|254|4:14|Client ESP32_998610 closed its connection.|
|1773428648|11|0:11|Expiring client ESP32_998610 due to timeout.|

Can't say this makes any sense to me except:

* The client is not coded to close the connection. I can't rule out that some resource exhaustion results in the stack closing the connection on behalf of the app.
* The broker times out 11s following this notice.
* The "new connection" repeats following a 208s delay which does not seem to match the publish interval 600s.

It seems clear that the open->publish->close strategy must be implemented when the intervals between published messages is longer than some interval.

## 2026-03-14 happy Pi day - back to work

Three ESPs were left running with the long publish interval overnight. In trhe morning only two were still publishing with `ESP32_998610` AWOL. Resetting them one at a time to match the host name to device and identified the ESP32-C3 as the one that stopped publishing.
