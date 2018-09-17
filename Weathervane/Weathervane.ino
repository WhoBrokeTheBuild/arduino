#include <Wire.h>
#include <Adafruit_BMP085.h>

//////////////////////
// Pin Definitions
//////////////////////
int humidPin     = 22;
int waterPin     = A0;

//////////////////////
// Variables
//////////////////////

Adafruit_BMP085 bmp;

void setup() {
  Serial.begin(9600);
  
  Serial.println("Weathervane 1.0");
  Serial.println();
  
  if (!bmp.begin()) {
    Serial.println("ERROR: No BMP180 detected");
  }
  
  pinMode(humidPin, INPUT);
}

/*
 *     0mm - 480
    5mm - 530
    10mm - 615
    15mm - 660
    20mm - 680
    25mm - 690
    30mm - 700
    35mm - 705
    40mm - 710
 */

int readWaterLevel() {
  float value = analogRead(A0);
  if (value <= 480){ 
    return 0;
  }
  else if (value>480 && value<=530){ 
    return 5;
  }
  else if (value>530 && value<=615){ 
    return 10;
  }
  else if (value>615 && value<=660){ 
    return 15;
  } 
  else if (value>660 && value<=680){ 
    return 20;
  }
  else if (value>680 && value<=690){ 
    return 25;
  }
  else if (value>690 && value<=700){ 
    return 30;
  }
  else if (value>700 && value<=705){ 
    return 35;
  }
  else if (value>705){ 
    return 40;
  }
}

void loop() {
    Serial.print("Temperature = ");
    Serial.print(bmp.readTemperature());
    Serial.println(" *C");
    
    Serial.print("Pressure = ");
    Serial.print(bmp.readPressure());
    Serial.println(" Pa");
  
    Serial.print("Altitude = ");
    Serial.print(bmp.readAltitude());
    Serial.println(" meters");

    Serial.print("Real altitude = ");
    Serial.print(bmp.readAltitude(101800));
    Serial.println(" meters");

    Serial.print("Rain level = ");
    Serial.print(readWaterLevel());
    Serial.println(" millimeters");

    Serial.println();
    
    delay(1000);
}
