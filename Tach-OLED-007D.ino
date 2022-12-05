/*********
  12/09/2021: PDS - Configure Tachometer by picking items from DIY Tach script
  12/30/2021: PDS - Updated to use Sensor as a select switch.  Hold finger over sensor while powering on triggers a debug menu. 
                  - Also, turn off Serial unless a menu (above) has been triggered. 
  12/01/2022: PDS - Version 7C.  Uses pulse width.  Can handle very slow pulses as well as fast.
                  - Added setTextColor(WHITE,BLACK) instead if just WHITE
                  - Added a "erase a rectangle" to clear old messages below readings
                  - *DONE* TODO: Verify that the tach is calculating correctly (x/factor?)
                  - Add an averaging function to smooth out the RPM jumping around. ( https://www.aranacorp.com/en/implementation-of-the-moving-average-in-arduino/ )
*********/

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
char strCodeName[] = "Tach-OLED";
char strCodeVersion[] = "7.3";
unsigned long rpmtime;
float rpmfloat;
unsigned int rpm;
bool tooslow = 1;
int valSensorPin;

  // Running Average
  const int numReadings  = 10;
  int readings [numReadings];
  int readIndex  = 0;
  long total  = 0;

void setup() {
  // Sensor Pin Enable
  pinMode(SENSORPIN, INPUT);

  // Check to see if the sensor is being "pressed" for debug mode:
  valSensorPin = digitalRead(SENSORPIN);   // read the input pin

  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB
  }
  // Serial Print Program Name and Version
  Serial.print(strCodeName); 
  Serial.print(" "); 
  Serial.println(strCodeVersion);
  if ( valSensorPin == 1) {
    Serial.println("SenserPin HIGH ...");
  }
  
  // Set up Interupt Timers
  TCCR1A = 0; //Initialize Timer "A"
  TCCR1B = 0; //Initialize Timer "B"
  TCCR1B |= (1 << CS12); //Prescaler 256
  TIMSK1 |= (1 << TOIE1); //enable timer overflow

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
  }

  // Setup Display
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE,BLACK);
  display.setCursor(0, 0); // X*Y Graphical location (128x64)
  display.print(strCodeName);
  display.print(" ");
  display.println(strCodeVersion);
  if ( valSensorPin == 0) {
    display.println("DEBUG MODE");
  } else {
    display.println("Slow MODE");
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
    //rpmfloat = 120 / (rpmtime/ 15000.00);
    rpm = round(rpmfloat);
    //display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE,BLACK);
    display.fillRect(0, 24, display.width(), display.height()-24, BLACK);
    display.setCursor(0, 24); 
    display.print(rpm);
    display.println(" RPM");
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
    //if ( valSensorPin == 1) {
      Serial.println(rpmtime);
    //}
  TCNT1 = 0;
  tooslow = 0;
}

long smooth(rpm ) { /* function smooth */
  ////Perform average on sensor readings
  long average;
  // subtract the last reading:
  total = total - readings[readIndex];
  // read the sensor:
  readings[readIndex] = rpm;
  // add value to total:
  total = total + readings[readIndex];
  // handle index
  readIndex = readIndex + 1;
  if (readIndex >= numReadings) {
    readIndex = 0;
  }
  // calculate the average:
  average = total / numReadings;

  return average;
}