/*
  Pulse Patch
  This code targets a Simblee
  I2C Interface with MAX30102 Sp02 Sensor Module
*/

#include <Wire.h>
#include "MAX30102_Definitions.h"

//This line added by Chip 2016-09-28 to enable plotting by Arduino Serial Plotter
const int PRINT_ONLY_FOR_PLOTTER = 1;  //Set this to zero to return normal verbose print() statements

const byte ledEnable = 9;

unsigned int LED_timer;
int LED_delayTime = 300;
boolean boardLEDstate = HIGH;
volatile boolean MAX_interrupt = false;
short interruptSetting;
short interruptFlags;
float Celcius;
float Fahrenheit;
char sampleCounter = 0;
int REDvalue;
int IRvalue;
char mode = SPO2_MODE;  // SPO2_MODE or HR_MODE
char readPointer;
char writePointer;
char ovfCounter;
int rAmp = 10;
int irAmp = 10;



//  TESTING
unsigned int thisTestTime;
unsigned int thatTestTime;

char sampleRate;
boolean useFilter = true;
int gain = 10;
float HPfilterInputRED[NUM_SAMPLES];
float HPfilterOutputRED[NUM_SAMPLES];
float LPfilterInputRED[NUM_SAMPLES];
float LPfilterOutputRED[NUM_SAMPLES];
float HPfilterInputIR[NUM_SAMPLES];
float HPfilterOutputIR[NUM_SAMPLES];
float LPfilterInputIR[NUM_SAMPLES];
float LPfilterOutputIR[NUM_SAMPLES];

void setup(){

  Wire.beginOnPins(SCL_PIN,SDA_PIN);
  Serial.begin(230400);
  pinMode(BOARD_LED,OUTPUT); digitalWrite(BOARD_LED, boardLEDstate);
  pinMode(MAX_INT,INPUT);
  pinMode(ledEnable,OUTPUT);

  attachPinInterrupt(MAX_INT,MAX_ISR,LOW);
  if (!PRINT_ONLY_FOR_PLOTTER) Serial.println("\nPulsePatch 01\n");
  LED_timer = millis();
  MAX_init(SR_100); // initialize MAX30102, specify sampleRate
  if (useFilter){ initFilter(); }
  if (!PRINT_ONLY_FOR_PLOTTER) {
    printAllRegisters();
    Serial.println("");
    printHelpToSerial();
    Serial.println("");
  } else {
    //when configured for the Arduino Serial Plotter, start the system running right away
    enableMAX30102(true);
    thatTestTime = micros();
  }
}


void loop(){

  if(MAX_interrupt){
    serviceInterrupts(); // go see what woke us up, and do the work
    if(sampleCounter == 0x00){  // rolls over to 0 at 200
      MAX30102_writeRegister(TEMP_CONFIG,0x01); // take temperature
    }
  }

  blinkBoardLED();


  eventSerial();
}

void eventSerial(){
  while(Serial.available()){
    char inByte = Serial.read();
    uint16_t intSource;
    switch(inByte){
      case 'h':
        printHelpToSerial();
        break;
      case 'b':
        Serial.println("start running");
        enableMAX30102(true);
        thatTestTime = micros();
        break;
      case 's':
        Serial.println("stop running");
        enableMAX30102(false);
        break;
      case 't':
        MAX30102_writeRegister(TEMP_CONFIG,0x01);
        break;
      case 'i':
        intSource = MAX30102_readShort(STATUS_1);
        Serial.print("intSource: 0x"); Serial.println(intSource,HEX);
        break;
      case 'v':
        getDeviceInfo();
        break;
      case '?':
        printAllRegisters();
        break;

      case 'f':
        useFilter = false;
        break;
      case 'F':
        useFilter = true;
        break;
      case '1':
        rAmp++; if(rAmp > 50){rAmp = 50;}
        setLEDamplitude(rAmp, irAmp);
        serialAmps();
        break;
      case '2':
        rAmp--; if(rAmp < 1){rAmp = 0;}
        setLEDamplitude(rAmp, irAmp);
        serialAmps();
        break;
      case '3':
        irAmp++; if(irAmp > 50){irAmp = 50;}
        setLEDamplitude(rAmp, irAmp);
        serialAmps();
        break;
      case '4':
        irAmp--; if(irAmp < 1){irAmp = 0;}
        setLEDamplitude(rAmp, irAmp);
        serialAmps();
      default:
        break;
    }
  }
}

void blinkBoardLED(){
  if(millis()-LED_timer > LED_delayTime){
      LED_timer = millis();
      boardLEDstate = !boardLEDstate;
      digitalWrite(BOARD_LED,boardLEDstate);
    }
}

//Print out all of the commands so that the user can see what to do
//Added: Chip 2016-09-28
void printHelpToSerial() {
  Serial.println(F("Commands:"));
  Serial.println(F("   'h'  Print this help information on available commands"));
  Serial.println(F("   'b'  Start the thing running at the sample rate selected"));
  Serial.println(F("   's'  Stop the thing running"));
  Serial.println(F("   't'  Initiate a temperature conversion. This should work if 'b' is pressed or not"));
  Serial.println(F("   'i'  Query the interrupt flags register. Not really useful"));
  Serial.println(F("   'v'  Verify the device by querying the RevID and PartID registers (hex 6 and hex 15 respectively)"));
  Serial.println(F("   '1'  Increase red LED intensity"));
  Serial.println(F("   '2'  Decrease red LED intensity"));
  Serial.println(F("   '3'  Increase IR LED intensity"));
  Serial.println(F("   '4'  Decrease IR LED intensity"));
  Serial.println(F("   '?'  Print all registers"));
  Serial.println(F("   'F'  Turn on filters"));
  Serial.println(F("   'f'  Turn off filters"));
}

int MAX_ISR(uint32_t dummyPin) { // gotta have a dummyPin...
  MAX_interrupt = true;
  return 0; // gotta return something, somehow...
}

void serialAmps(){
  Serial.print("PA\t");
  Serial.print(rAmp); printTab(); Serial.println(irAmp);
}
