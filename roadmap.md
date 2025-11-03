# Roadmap

Since others have found this project useful it seems appropriate to identify a roadmap for both their and my benefit.

The initial goal was to provide a starting point for my projects that encompasses functionality that I intend to use in all of them. That has, in a crude way, been achieved. The various modules were mostly copied form some examples which provide more functionality than I desire. Much further work will be to eliminate the extras. Anyone needing any of that can look at the original examples and add it back in.

## MQTT

There is a *lot* of code from the example that can be removed. Further, there is a bug <https://github.com/HankB/ESP32-ESP-IDF-CLI-start/issues/3> that needs to be addressed.

In addition, it seems wise to provide the capability to switch to an alternate broker if the original cannot be reached. 

Lastly, for my applications, the typical publish interval will be some number of minutes. It seems wasteful to keep the connection open between messages. This could be coded to establish the connection each time a message is to be published (and the code factored in a way that this can be reverted if that turns out not to be a good idea.) Interesting discussion on this at <http://www.steves-internet-guide.com/connected-or-disconnect/>. For now the ESP will retain the connection as my application requires relatively low traffic. 

The next effort will be to alternate between multiple brokers to maintain continuity of service.
