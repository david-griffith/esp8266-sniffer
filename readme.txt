Aeroscout tag sniffer.

Switches the ESP8266 into promiscuous mode and reports the MAC and RSSI of
 any tags that broadcast to the Aeroscout destination address (01:0c:cc).

This information is pushed out the UART at 115200 baud, in a simplistic
format of MAC address and rssi. The python program sorts them by RSSI and 
creates a list, with a 10 second timeout to drop off tags that are no longer 
visible.

A great deal of this, most (notabll the packet capture struct and the general idea)
was pulled from another program - 
https://github.com/pulkin/esp8266-injection-example/blob/master/user/user_main.c

To build - 
Get the ESP Open SDK and drop this in a folder in the SDK called "/examples/sniffer",
 it should build without errors. My knowledge of makefiles is pretty sketchy, so 
things may not work if you're building this on windows.

Enjoy!

Dave
(angry.deity@gmail.com)

