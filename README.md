# IR-Tachometer
This is my learning experience building a very low cost tachometer that I wanted to build to measure teh RPM of the spindles of the various CNC Routers that I have been using. 
## Hardware 
- Arduino Mini-Pro ( under $4.00 at [AliExpress](https://www.aliexpress.com/item/32821902128.html) )
These are now obsolete, but any arduino will do.  I just happen to have a few of these
- OLED Display ($3.50 at [AliEpress](https://www.aliexpress.com/item/32920071528.html) (Uses Adafuit SSD1306 library) 
- Infrared Sensor ($2.00 at [AliExpress](https://www.aliexpress.com/item/32952079917.html) ( 3pin IR Infrared barrier Module obstacle avoidance sensor) 
- Lithium Battery Charger Module ($1.00 at [Aliexpress](https://www.aliexpress.com/item/32986135934.html) )
- battery 3.7v 450mAh or so. ($3.65 at [AliExpress](https://www.aliexpress.com/item/4000908332181.html) )
## Code
There are 3 ways to do this.  I am testing all 3.

1. The first to use interupts and to measure the time between pulses.  My fisrt attempt at this was less than iffy at best and resulted in many errors and sporatic results.  The issue is that you need to average the time with a few previous results.  otherwise you get a rather abrupt single snapshot in time.  One of as is were.  Not reliable.
2. A better method is to measure how many pulses occur for a known time, such as 500msec or 1 sec. [See Blinky with Timer1 OVF](https://blog.podkalicki.com/arduino-blinky-with-timer1-ovf/)
3. Use a Grequency Count library such as from [PJRC for the Teensy](https://www.pjrc.com/teensy/td_libs_FreqCount.html)


## Help
 - [Github-flavored Markdown info page.](http://github.github.com/github-flavored-markdown/)
 - [Mermaid diagrams](https://mermaid-js.github.io/mermaid/#/)
