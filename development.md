---
layout: default
title: Development
---

# Development

This project will not be maintained or even acknowledged going forward. I created this in a short
amount of time for an IoT project as part of my course. What this project does is extremely simple
and much better done by anything else. The rest of this section is just some of my thoughts that
came up while developing this project.

## Personal Thoughts

The ArduinoIoTCloud library is really bad. It really just... *sucks*. While developing this I
encountered many strange and frustrating behaviours caused by this library. For instance, the
library connects to WiFi itself. This forbids you from using any other WiFi library, and forces use
of the built in one. The ArduinoCloud.begin() function connects to the cloud *asynchronously*,
which isn't a problem on its own, but does introduce some design considerations. For example, I had
to connect to an NTP server to synchronize the Arduino's RTC. This meant waiting for the WiFi
connection to be established, which requires polling the ArduinoCloud.connected() function in a
busy loop before continuing. Immediately proceeding after connecting to the cloud proved
problematic - for some ungodly reason, doing this would cause the ArduinoCloud.update() function to
completely hang later in the programs execution. The library is completely opaque, and nothing I
did seemed to affect it. I was able to "fix" this by introducing a 10 second delay *after*
connecting to WiFi. God only knows why this works. But wait, there's more! With this newfound long
startup time, I wanted to show a message on the LCD screen to say "Connecting", and maybe play a
little loading animation. ArduinoIoTCloud decided against that. It turns out that initializing the
LCD before calling ArduinoCloud.begin() would, again, just completely break it. The LCD would
initialize fine, and the message would display perfectly - but as soon as you called
ArduinoCloud.begin(), chaos would ensue. *Somehow* the ArduinoCloud library would write garbage
characters to the screen, and any further attempts to print to the screen wouldn't work. Then, just
as before, the ArduinoCloud.update() function would completely hang, with no output whatsoever. I
have absoluetly no idea how a library can be so poorly written that it just steps all over the
memory of another, but it seemed like that was what was happening. I found no fix for this, and
decided to just leave the LCD blank until after we connected.