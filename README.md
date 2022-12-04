# IR-Tachometer
This is my learning experience building a very low cost tachometer that I wanted to build to measure teh RPM of the spindles of the various CNC Routers that I have been using. 
## Hardware 
- Arduino Mini-Pro ( $3.81)
These are now obsolete, but any arduino will do.  I just happen to have a few of these
- OLED Display (0.96" OLED Display Module 128X64 I2C SSD1306 for Arduino)(Uses Adafuit SSD1306 library)
- Infrared Sensor ($1.40) ( 3pin IR Infrared barrier Module obstacle avoidance sensor)
- Lithium Battery Charger Module ($1.00) [Aliexpress](https://www.aliexpress.com/item/32986135934.html)
- battery
## Code
### Version A - Uses interrupt on a sensor pusle and counts time between pulses
```
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define SENSORPIN 2 // Sensor IO Pin

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Define Variables
char strCodeName[] = "Tach-OLED-003C";
char strCodeVersion[] = "3.2";
unsigned long rpmtime;
float rpmfloat;
unsigned int rpm;
bool tooslow = 1;
int valSensorPin;

void setup() {
  // Sensor Pin Enable
  pinMode(SENSORPIN, INPUT);

  // Check to see if the sensor is being "pressed" for debug mode:
  valSensorPin = digitalRead(SENSORPIN);   // read the input pin

    if ( valSensorPin == 1) {
      Serial.begin(115200);
      while (!Serial) {
        ; // wait for serial port to connect. Needed for native USB
      }
  
      // Serial Print Program Name and Version
      Serial.print(strCodeName); 
      Serial.print(" "); 
      Serial.println(strCodeVersion);
    }
  
  // Set up Interupt Timers
  TCCR1A = 0; //Initialize Timer "A"
  TCCR1B = 0; //Initialize Timer "B"
  TCCR1B |= (1 << CS12); //Prescaler 256
  TIMSK1 |= (1 << TOIE1); //enable timer overflow

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if ( valSensorPin == 1) {
    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
      Serial.println(F("SSD1306 allocation failed"));
      for(;;); // Don't proceed, loop forever
    }
  }

  // Clear the buffer
  // Display Text (V-Large)
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE,BLACK);
  display.setCursor(0, 0); // X*Y Graphical location (128x64)
  display.println("DIY Tachometer");
  if ( valSensorPin == 0) {
    display.println("DEBUG MODE");
  }
  display.display(); 

  display.setTextSize(2);
  display.setCursor(0, 24); // Gra
  display.println(TCNT1);
  display.display(); 
  attachInterrupt(0, RPM, FALLING);
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(100);
  if (tooslow == 1) {
    //display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE,BLACK);
    display.setCursor(0, 24); 
    valSensorPin = digitalRead(SENSORPIN);   // read the input pin
    if (valSensorPin) {
      //display.drawRect(0, 40, display.width(), display.height()-40, WHITE);
      display.fillRect(0, 40, display.width(), display.height()-40, BLACK);
      display.println("No Pulses");
      display.setTextSize(1);
      display.println("Move closer to a");
      display.println("spinny thingy");
      display.setTextSize(2);
    } else {
      display.println("Too close");
      display.fillRect(0, 40, display.width(), display.height()-40, BLACK);
      display.setTextSize(1);
      //display.setTextColor(WHITE,BLACK);
      display.setCursor(0,40); 
      display.print("Move Back");
      //display.setTextSize(2);
    }
    display.display(); 
  } else {
    rpmfloat = 120 / (rpmtime/ 31250.00);
    rpm = round(rpmfloat);
    //display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE,BLACK);
    display.setCursor(0, 24); 
    display.println("         ");
    display.setCursor(0, 24); 
    display.println(rpm);
      display.fillRect(0, 40, display.width(), display.height()-40, BLACK);
    display.display(); 
  }
}

// Interrupt Service Routine
ISR(TIMER1_OVF_vect) {
  tooslow = 1;
}
                                         
// Interrupt Timer 
void RPM() {
  rpmtime = TCNT1;
    if ( valSensorPin == 1) {
      Serial.println(rpmtime);
    }
  TCNT1 = 0;
  tooslow = 0;
}
```


## Help
 - [Github-flavored Markdown info page.](http://github.github.com/github-flavored-markdown/)
 - [Mermaid diagrams](https://mermaid-js.github.io/mermaid/#/)
