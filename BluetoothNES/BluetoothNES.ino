//===============================================================================
//  Header Files
//===============================================================================

#include <SoftwareSerial.h>

//===============================================================================
//  Constants
//===============================================================================
// Here we have a bunch of constants that will become clearer when we look at the
// readNesController() function. Basically, we will use these contents to clear
// a bit. These are chosen according to the table above.
const int A_BUTTON         = 0;
const int B_BUTTON         = 1;
const int SELECT_BUTTON    = 2;
const int START_BUTTON     = 3;
const int UP_BUTTON        = 4;
const int DOWN_BUTTON      = 5;
const int LEFT_BUTTON      = 6;
const int RIGHT_BUTTON     = 7;

const int A_KEY            = 0x04;
const int B_KEY            = 0x05;
const int LEFT_KEY         = 0x50;
const int RIGHT_KEY        = 0x4F;
const int UP_KEY           = 0x52;
const int DOWN_KEY         = 0x51;
const int ENTER_KEY        = 0x28;
const int BACKSPACE_KEY    = 0x2A;

//===============================================================================
//  Variables
//===============================================================================
byte nesRegister  = 0;    // We will use this to hold current button states
byte oldState     = 0;
byte moveButton   = 0;
byte actionButton = 0;

//===============================================================================
//  Pin Declarations
//===============================================================================
//Inputs:
int bluetoothRx   = 3;    // RX-I pin of bluetooth mate
int nesData       = 5;    // The data pin for the NES controller

//Outputs:
int bluetoothTx   = 2;    // TX-O pin of bluetooth mate
int nesLatch      = 7;    // The latch pin for the NES controller
int nesClock      = 6;    // The clock pin for the NES controller

SoftwareSerial bluetooth(bluetoothTx, bluetoothRx);

void setup()
{
  Serial.begin(9600);

  // Set appropriate pins to inputs
  pinMode(nesData, INPUT);

  // Set appropriate pins to outputs
  pinMode(nesClock, OUTPUT);
  pinMode(nesLatch, OUTPUT);

  // Set initial states
  digitalWrite(nesClock, LOW);
  digitalWrite(nesLatch, LOW);

  // The Bluetooth Mate defaults to 115200bps
  bluetooth.begin(115200);

  // Print three times individually
  bluetooth.print("$");
  bluetooth.print("$");
  bluetooth.print("$");

  // Short delay, wait for the Mate to send back CMD
  delay(500);

  // Temporarily Change the baudrate to 9600, no parity
  bluetooth.println("U,9600,N");

  // 115200 can be too fast at times for NewSoftSerial to relay the data reliably
  // Start bluetooth serial at 9600
  bluetooth.begin(9600);
}

void loop()
{
  oldState = nesRegister;
  nesRegister = readNesController();

  /*
  Serial.print(bitRead(nesRegister, A_BUTTON));
  Serial.print(bitRead(nesRegister, B_BUTTON));
  Serial.print(bitRead(nesRegister, SELECT_BUTTON));
  Serial.print(bitRead(nesRegister, START_BUTTON));
  Serial.print(bitRead(nesRegister, UP_BUTTON));
  Serial.print(bitRead(nesRegister, DOWN_BUTTON));
  Serial.print(bitRead(nesRegister, LEFT_BUTTON));
  Serial.print(bitRead(nesRegister, RIGHT_BUTTON));
  Serial.println();
  */

  if (bluetooth.available())  // If the bluetooth sent any characters
  {
    // Send any characters the bluetooth prints to the serial monitor
    Serial.print((char)bluetooth.read());
  }

  if (bitRead(nesRegister, A_BUTTON) == 0) {
    if (bitRead(oldState, A_BUTTON) != 0) {
      actionButton = A_KEY;
      writeBtKey();
      Serial.println("A");
    }
  } else {
    if (bitRead(oldState, A_BUTTON) == 0) {
      actionButton = 0;
      writeBtKey();
    }
  }

  if (bitRead(nesRegister, B_BUTTON) == 0) {
    if (bitRead(oldState, B_BUTTON) != 0) {
      actionButton = B_KEY;
      writeBtKey();
      Serial.println("B");
    }
  } else {
    if (bitRead(oldState, B_BUTTON) == 0) {
      actionButton = 0;
      writeBtKey();
    }
  }

  if (bitRead(nesRegister, LEFT_BUTTON) == 0) {
    if (bitRead(oldState, LEFT_BUTTON) != 0) {
      moveButton = LEFT_KEY;
      writeBtKey();
    }
  } else {
    if (bitRead(oldState, LEFT_BUTTON) == 0) {
      moveButton = 0;
      writeBtKey();
    }
  }

  if (bitRead(nesRegister, RIGHT_BUTTON) == 0) {
    if (bitRead(oldState, RIGHT_BUTTON) != 0) {
      moveButton = RIGHT_KEY;
      writeBtKey();
    }
  } else {
    if (bitRead(oldState, RIGHT_BUTTON) == 0) {
      moveButton = 0;
      writeBtKey();
    }
  }

  if (bitRead(nesRegister, UP_BUTTON) == 0) {
    if (bitRead(oldState, UP_BUTTON) != 0) {
      moveButton = UP_KEY;
      writeBtKey();
    }
  } else {
    if (bitRead(oldState, UP_BUTTON) == 0) {
      moveButton = 0;
      writeBtKey();
    }
  }

  if (bitRead(nesRegister, DOWN_BUTTON) == 0) {
    if (bitRead(oldState, DOWN_BUTTON) != 0) {
      moveButton = DOWN_KEY;
      writeBtKey();
    }
  } else {
    if (bitRead(oldState, DOWN_BUTTON) == 0) {
      moveButton = 0;
      writeBtKey();
    }
  }

  if (bitRead(nesRegister, SELECT_BUTTON) == 0) {
    if (bitRead(oldState, SELECT_BUTTON) != 0) {
      actionButton = BACKSPACE_KEY;
      writeBtKey();
    }
  } else {
    if (bitRead(oldState, SELECT_BUTTON) == 0) {
      actionButton = 0;
      writeBtKey();
    }
  }

  if (bitRead(nesRegister, START_BUTTON) == 0) {
    if (bitRead(oldState, START_BUTTON) != 0) {
      actionButton = ENTER_KEY;
      writeBtKey();
    }
  } else {
    if (bitRead(oldState, START_BUTTON) == 0) {
      actionButton = 0;
      writeBtKey();
    }
  }
  
  delay(50);
}

void writeBtKey()
{
  bluetooth.write((byte)0xFD); //Start HID Report
  bluetooth.write((byte)0x9); //Length byte
  bluetooth.write((byte)0x1); //Descriptor byte
  bluetooth.write((byte)0x00); //Modifier byte
  bluetooth.write((byte)0x00); //-
  if (moveButton == 0) {
    bluetooth.write(actionButton); //Send KEY
    bluetooth.write((byte)0x00);
  } else {
    bluetooth.write(moveButton); //Send KEY 
    bluetooth.write(actionButton); //Send KEY
  }
  for (byte i = 0; i < 4; i++) { //Send five zero bytes
    bluetooth.write((byte)0x00);
  }
}

byte readNesController()
{
  // Pre-load a variable with all 1's which assumes all buttons are not
  // pressed. But while we cycle through the bits, if we detect a LOW, which is
  // a 0, we clear that bit. In the end, we find all the buttons states at once.
  int tempData = 255;

  // Quickly pulse the nesLatch pin so that the register grab what it see on
  // its parallel data pins.
  digitalWrite(nesLatch, HIGH);
  digitalWrite(nesLatch, LOW);

  // Upon latching, the first bit is available to look at, which is the state
  // of the A button. We see if it is low, and if it is, we clear out variable's
  // first bit to indicate this is so.
  if (digitalRead(nesData) == LOW)
    bitClear(tempData, A_BUTTON);

  // Clock the next bit which is the B button and determine its state just like
  // we did above.
  digitalWrite(nesClock, HIGH);
  digitalWrite(nesClock, LOW);
  if (digitalRead(nesData) == LOW)
    bitClear(tempData, B_BUTTON);

  // Now do this for the rest of them!

  // Select button
  digitalWrite(nesClock, HIGH);
  digitalWrite(nesClock, LOW);
  if (digitalRead(nesData) == LOW)
    bitClear(tempData, SELECT_BUTTON);

  // Start button
  digitalWrite(nesClock, HIGH);
  digitalWrite(nesClock, LOW);
  if (digitalRead(nesData) == LOW)
    bitClear(tempData, START_BUTTON);

  // Up button
  digitalWrite(nesClock, HIGH);
  digitalWrite(nesClock, LOW);
  if (digitalRead(nesData) == LOW)
    bitClear(tempData, UP_BUTTON);

  // Down button
  digitalWrite(nesClock, HIGH);
  digitalWrite(nesClock, LOW);
  if (digitalRead(nesData) == LOW)
    bitClear(tempData, DOWN_BUTTON);

  // Left button
  digitalWrite(nesClock, HIGH);
  digitalWrite(nesClock, LOW);
  if (digitalRead(nesData) == LOW)
    bitClear(tempData, LEFT_BUTTON);

  // Right button
  digitalWrite(nesClock, HIGH);
  digitalWrite(nesClock, LOW);
  if (digitalRead(nesData) == LOW)
    bitClear(tempData, RIGHT_BUTTON);

  // After all of this, we now have our variable all bundled up
  // with all of the NES button states.*/
  return tempData;
}
