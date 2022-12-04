# IR-Tachometer
This is my learning experience building a very low cost tachometer that I wanted to build to measure the RPM of the spindles of the various CNC Routers that I have been using. 
## Hardware 
- Arduino Mini-Pro ( under $4.00 at [AliExpress](https://www.aliexpress.com/item/32821902128.html) )
These are now obsolete, but any arduino will do.  I just happen to have a few of these
- OLED Display ($3.50 at [AliEpress](https://www.aliexpress.com/item/32920071528.html) (Uses Adafuit SSD1306 library) 
- Infrared Sensor ($2.00 at [AliExpress](https://www.aliexpress.com/item/32952079917.html) ( 3pin IR Infrared barrier Module obstacle avoidance sensor) 
- Lithium Battery Charger Module ($1.00 at [Aliexpress](https://www.aliexpress.com/item/32986135934.html) )
- battery 3.7v 450mAh or so. ($3.65 at [AliExpress](https://www.aliexpress.com/item/4000908332181.html) )
## Code
There are several ways to do this.  I am testing all 3.
1. Count pulses for a period of time using a loop.  This is way too sketchy and was only used as a reference ( [See this article](https://how2electronics.com/digital-tachometer-ir-sensor-arduino/) )
2. Use interupts and measure the time between pulses.  My first attempt at this was less than satsfying and was iffy at best.  It resulted in many errors and sporatic results.  The issue is that you need to average the time with a few previous results.  otherwise you get a rather abrupt single snapshot in time.  "One of" as is were.  Not reliable.
3. Use interrupt to time a window and measure how many pulses occur during this window of time, such as 500msec or 1 sec. [See Blinky with Timer1 OVF](https://blog.podkalicki.com/arduino-blinky-with-timer1-ovf/)
4. Use a Frequency Count library such as from [PJRC for the Teensy](https://www.pjrc.com/teensy/td_libs_FreqCount.html)


## Help
 - [Github-flavored Markdown info page.](http://github.github.com/github-flavored-markdown/)
 - [Mermaid diagrams](https://mermaid-js.github.io/mermaid/#/)
