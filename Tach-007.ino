/*********
  12/04/2022: Version 6 of using Interrupts and an optical sensor
              Started over using a COMPA Timer to give a consistent windows of time to count pulses.
              Theory of operation:  Use an interupt to create a heartbeat of consistent time. (eg 1 Sec or 500mSec)
                                    Then count how many pulses occur during that tieme window. 
  12/04/2022: Development:  Built a prototype using a 128x64 OLED display, an IR Obstical avoidance sensor.
  12/04/2022: Added FreqCount library (V1.3.0)
*********/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <FreqCount.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define SENSORPIN 2 // Sensor IO Pin

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Define Variables
char strCodeName[] = "Tach-006";
char strCodeVersion[] = "1.1";
unsigned long rpmtime;
float rpmfloat;
unsigned int rpm;
int valSensorPin;
bool tooslow = 1;
bool debugmode = 0;
bool rpmfastmode = 1;

#define LED_PIN     (13)

void enableints() {
  FreqCount.end(); // Disable FreqCount 

  // Set up Interupt Timers
  TCCR1A = 0; //Initialize Timer "A"
  TCCR1B = 0; //Initialize Timer "B"
  TCCR1B |= (1 << CS12); //Prescaler 256
  TIMSK1 |= (1 << TOIE1); //enable timer overflow
 
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

void setup() {
  // Setup to read Sensor
  pinMode(SENSORPIN, INPUT);
  
  // setup for Monitor
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB
  }

  // Serial Print Program Name and Version
  Serial.print(strCodeName); 
  Serial.print(" "); 
  Serial.println(strCodeVersion);
  
  // Sensor Pin Enable
  pinMode(SENSORPIN, INPUT);

  // Check to see if the sensor is being "pressed" for debug mode:
  valSensorPin = digitalRead(SENSORPIN);   // read the input pin
  if ( valSensorPin == 0) {
    debugmode = 1;
  }
  // Enable OLED Display
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  // Clear the buffer
  // Display Text (V-Large)
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE,BLACK);
  display.setCursor(0, 0); // X*Y Graphical location (128x64)
  display.print(strCodeName);
  display.print(" ");
  display.println(strCodeVersion);
  if ( debugmode) {
    display.println("DEBUG MODE");
  }
  display.display(); 

  if (rpmfastmode) {
    FreqCount.begin(1000); // Initialize setting the gateway interval
  }
}

void loop() {
  if (FreqCount.available()) {
    unsigned long count = FreqCount.read()*60;  // Number of pulses in 1 minute
    Serial.println(count);

    // Set up OLED display
    display.setTextSize(2);
    display.setTextColor(WHITE,BLACK);
    display.setCursor(0, 24);
    display.fillRect(0, 32, display.width(), display.height()-40, WHITE); // Clear line
    // print right justified
    if (count < 9999) { display.print(" "); }
    if (count < 999) { 
      display.print(" "); 
      Serial.print("RPM Too Low.");
      //Serial.println(count);
    }
    if (count < 99) { display.print(" "); }
    if (count < 9) { display.print(" "); }
    display.print(count);
    display.println(" RPM");
    display.display(); 
  }

  // Test if the RPM is too low
//  if (count < 1000) {
//    Serial.print("RPM Too Low.");
//    Serial.println(count);
//  }
}
