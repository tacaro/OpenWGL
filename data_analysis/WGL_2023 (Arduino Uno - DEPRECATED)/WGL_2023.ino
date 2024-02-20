#include <Wire.h>
#include "MS5837.h"
#include <SD.h>
#include <SPI.h>
#include <SoftwareSerial.h>
#define rx 2
#define tx 3

int pinCS = 10;
bool firstline = true;

SoftwareSerial myserial(rx, tx);
MS5837 sensor;
File myFile;

String inputstring = ""; 
String sensorstring = "";
boolean input_string_complete = false;
boolean sensor_string_complete = false;

void setup() {
//Setup
  Serial.begin(9600);
  myserial.begin(9600);
  pinMode(pinCS, OUTPUT);
  Wire.begin();
  Serial.println("WGL2023 Booting up.");
  inputstring.reserve(10);
  sensorstring.reserve(30);

//SD initialization
  if (SD.begin()){
    Serial.println("SD card is ready.");
  } 
  else {
    Serial.println("SD card initialization failed");
    return;
  }

//Overwrite file 
  SD.remove("DATA.txt");
  Serial.println("Preexisting DATA.txt removed.");
  
//Bar30 initialization
  while (!sensor.init()) {
    Serial.println("Initialization failed!");
    Serial.println("Are SDA/SCL connected correctly?");
    Serial.println("Blue Robotics Bar30: White=SDA, Green=SCL");
    Serial.println("\n\n\n");
    delay(5000);
  }
  sensor.setModel(MS5837::MS5837_30BA);
  sensor.setFluidDensity(997); // kg/m^3 (997 for freshwater, 1029 for seawater)
  Serial.println("Bar30 is ready.");
}

//Serial event
void serialEvent() {
  inputstring = Serial.readStringUntil(13);
  input_string_complete = true;
}

//Loop
void loop() {
  if (input_string_complete == true) {
    myserial.print(inputstring);
    myserial.print('\r');
    inputstring = "";
    input_string_complete = false;
  }

  if (myserial.available() > 0) {
    char inchar = (char)myserial.read();
    sensorstring += inchar;
    if (inchar == '\r') {
      sensor_string_complete = true;
    }
  }

  if (sensor_string_complete == true) {
    if (isdigit(sensorstring[0]) == false) {
      Serial.println("WARMING: EC String Incomplete!");
      Serial.println(sensorstring);
    }
    else {
      Serial.println("EC String Complete.");
      delay(1000);
//Open file
      myFile = SD.open("DATA.txt", FILE_WRITE);
      if (myFile) {
        Serial.println("File opened.");
      }
      if (firstline == true) {
        myFile.print("EC,PRESS,TEMP,DEPTH,ALT\n");
        firstline = false;
      }
//Parse conductivity sensor string
      char sensorstring_array[30];
      char *EC;
      char *TDS;
      char *SAL;
      char *GRAV;
      String jackBlack = "";
      float f_ec;
      
      sensorstring.toCharArray(sensorstring_array, 30);
      EC = strtok(sensorstring_array, ",");
      TDS = strtok(NULL, ",");
      SAL = strtok(NULL, ",");
      GRAV = strtok(NULL, ",");
      f_ec = atof(EC);
     myFile.print(f_ec);
     myFile.print(",");

//Read Bar30
        sensor.read();
        myFile.print(sensor.pressure());
        myFile.print(",");
        
        myFile.print(sensor.temperature()); 
        myFile.print(",");
        
        myFile.print(sensor.depth()); 
        myFile.print(",");
        
        myFile.print(sensor.altitude()); 
        myFile.print(",");
        // START A NEW LINE:
        myFile.print("\n");

        myFile.close();
        Serial.println("Data Recorded.");
    }
    sensorstring = "";
    sensor_string_complete = false;
  }
}