//===============================================================================
//  Header Files
//===============================================================================

#include <DigiJoystick.h>

//===============================================================================
//  Constants
//===============================================================================

const int A_BUTTON         = 0;
const int B_BUTTON         = 1;
const int SELECT_BUTTON    = 2;
const int START_BUTTON     = 3;
const int UP_BUTTON        = 4;
const int DOWN_BUTTON      = 5;
const int LEFT_BUTTON      = 6;
const int RIGHT_BUTTON     = 7;

//===============================================================================
//  Variables
//===============================================================================

//===============================================================================
//  Pin Declarations
//===============================================================================

/// Inputs
int nesDataPin = 5;

/// Outputs
int nesLatchPin = 0;
int nesClockPin = 1;

void setup() {
  // Set appropriate pins to inputs
  pinMode(nesDataPin, INPUT);

  // Set appropriate pins to outputs
  pinMode(nesClockPin, OUTPUT);
  pinMode(nesLatchPin, OUTPUT);

  // Set initial states
  digitalWrite(nesClockPin, LOW);
  digitalWrite(nesLatchPin, LOW);
}

void loop() {
  DigiJoystick.setButtons(readNesController(), (byte) 0);
  DigiJoystick.delay(10); 
}

byte readNesController()
{
  int tempData = 255;

  digitalWrite(nesLatchPin, HIGH);
  digitalWrite(nesLatchPin, LOW);

  for (int i = 0; i < 8; ++i) {
    if (digitalRead(nesDataPin) == LOW) {
      bitClear(tempData, i);
    }
    
    digitalWrite(nesClockPin, HIGH);
    digitalWrite(nesClockPin, LOW);
  }

  return ~tempData;
}
