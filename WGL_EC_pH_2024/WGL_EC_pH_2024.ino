/*

Well Geochemistry Logger
Version 5.0 | 12 Jan. 2024
Author: Tristan A. Caro
Author: Harry F. Brodsky
PI: Alexis S. Templeton

# Pin Config
# Board: Arduino Nano Every

## Isolated Carrier Board (Conductivity) [3.3V]
RX (SCL): D3      --- DONE
TX (SDA): D2      --- DONE
GND: GND (RIGHT)  --- DONE
VCC: 3.3V         --- DONE
-------------------------------------------------
## SD Card [5V]
VCC: 5V           --- DONE
GND: GND (RIGHT)  --- DONE
CS: D14 (A0)      --- DONE
SCK: D13          --- DONE
MOSI: D11         --- DONE
MISO/MOSO: D12    --- DONE
-------------------------------------------------
## Bar30 [5V]
+5V: 5V           --- DONE
SDA: SDA (A4)     --- DONE
SCL: SCL (A5)     --- DONE
GND: GND (LEFT)   --- DONE  
-------------------------------------------------
## Gravity Analog Isolator (pH) [3.3V]
+: 3.3V         --- DONE
-: GND (RIGHT)  --- DONE
A: A1 (D15)     --- DONE
D: D7           --- DONE
NOTE: ("D" isn't a D on the board, it is a square wave symbol)
-------------------------------------------------

- Type commands into the Arduino serial monitor to control the EC circuit.
- This code was written in the Arduino 2.0 IDE
*/

// Libraries and definitions
#include <Wire.h>                                     //This library allows you to communicate with I2C/TWI devices. (Bar30)
#include "MS5837.h"                                   //This is the library for the Blue Robotics Bar30 sensor
#include <SPI.h>                                      //This library allows you to communicate with SPI devices, with the Arduino as the controller device. (SD CARD)
#include <SD.h>                                       //Enables reading and writing on SD cards.
#include <SoftwareSerial.h>                           //we have to include the SoftwareSerial library, or else we can't use it
#define rx 2                                          //define what pin rx is going to be
#define tx 3                                          //define what pin tx is going to be

// Bar30 pressure sensor and SD card
MS5837 sensor;                                        //Instantiate the Bar30 sensor
File myFile;                                          //Instantiate myFile for SD

// SD card
SoftwareSerial myserial(rx, tx);                      //define how the soft serial port is going to work
const int chipSelect = 14;                            //set our Chip Select (CS) pin to 14

// EC sensor
String inputstring = "";                              //a string to hold incoming data from the PC
String sensorstring = "";                             //a string to hold the data from the Atlas Scientific product
boolean input_string_complete = false;                //have we received all the data from the PC
boolean sensor_string_complete = false;               //have we received all the data from the Atlas Scientific product

// pH sensor:
uint8_t user_bytes_received = 0;                
const uint8_t bufferlen = 32;                   
char user_data[bufferlen];


// pH sensor setup
// to use the Atlas gravity circuits with the gravity isolator board's pulse output 
// uncomment line below: #define USE_PULSE_OUT you can use any pins instead of just the analog ones
// but it must be recalibrated note that the isolator's analog output also provides isolation

// #define USE_PULSE_OUT

#ifdef USE_PULSE_OUT
  #include "ph_iso_grav.h"       
  Gravity_pH_Isolated pH = Gravity_pH_Isolated(A1);         
#else
  #include "ph_grav.h"             
  Gravity_pH pH = Gravity_pH(A1);   
#endif

// This function parses calibration commands to the pH sensor:
void parse_cmd(char* string) {                   
  strupr(string);                                
  if (strcmp(string, "CAL,7") == 0) {       
    pH.cal_mid();                                
    Serial.println("MID CALIBRATED");
  }
  else if (strcmp(string, "CAL,4") == 0) {            
    pH.cal_low();                                
    Serial.println("LOW CALIBRATED");
  }
  else if (strcmp(string, "CAL,10") == 0) {      
    pH.cal_high();                               
    Serial.println("HIGH CALIBRATED");
  }
  else if (strcmp(string, "CAL,CLEAR") == 0) { 
    pH.cal_clear();                              
    Serial.println("CALIBRATION CLEARED");
  }
}


void setup() {                                        //set up the hardware
  // SD setup
  Serial.begin(9600);                                 //set baud rate for the hardware serial port_0 to 9600
  Serial.print("Initializing SD card...");
  if (!SD.begin(chipSelect)) {                        // if it doesnt begin
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }
  File dataFile = SD.open("wgl_log.txt", FILE_WRITE); // open the file. note that only one file can be open at a time
  dataFile.println("cond_uscm,pH,pressure_mbar,temp_C,depth_m,alt_m");                     // write the data
  dataFile.close();  
  Serial.println("SD initialized.");
  
  // EC sensor setup
  myserial.begin(9600);                               //set baud rate for the software serial port to 9600
  inputstring.reserve(10);                            //set aside some bytes for receiving data from the PC
  sensorstring.reserve(30);                           //set aside some bytes for receiving data from Atlas Scientific product

  // pH sensor setup
  Serial.println(F("Use commands \"CAL,7\", \"CAL,4\", and \"CAL,10\" to calibrate the pH circuit to those respective values"));
  Serial.println(F("Use command \"CAL,CLEAR\" to clear the calibration"));
  if (pH.begin()) {                                     
    Serial.println("Loaded EEPROM");
  }

  // Bar30 pressure sensor setup
  Wire.begin();                                       //This function initializes the Wire library and join the I2C bus as a controller or a peripheral. This function should normally be called only once.
  while (!sensor.init()) {                            // Returns true if initialization was successful
    Serial.println("Init failed!");
    Serial.println("Are SDA/SCL connected correctly?");
    Serial.println("Blue Robotics Bar30: White=SDA, Green=SCL");
    Serial.println("\n\n\n");
    delay(5000);
  }
  
  sensor.setModel(MS5837::MS5837_30BA);
  sensor.setFluidDensity(997); // kg/m^3 (freshwater, 1029 for seawater)
  
}


void serialEvent() {                                  //if the hardware serial port_0 receives a char
  inputstring = Serial.readStringUntil(13);           //read the string until we see a <CR>
  input_string_complete = true;                       //set the flag used to tell if we have received a completed string from the PC
}


void loop() {                                         //here we go...
  if (input_string_complete == true) {                //if a string from the PC has been received in its entirety
    myserial.print(inputstring);                      //send that string to the Atlas Scientific product
    myserial.print('\r');                             //add a <CR> to the end of the string
    inputstring = "";                                 //clear the string
    input_string_complete = false;                    //reset the flag used to tell if we have received a completed string from the PC
  }

  if (myserial.available() > 0) {                     //if we see that the Atlas Scientific product has sent a character
    char inchar = (char)myserial.read();              //get the char we just received
    sensorstring += inchar;                           //add the char to the var called sensorstring
    if (inchar == '\r') {                             //if the incoming character is a <CR>
      sensor_string_complete = true;                  //set the flag
    }
  }


  if (sensor_string_complete == true) {               //if a string from the Atlas Scientific product has been received in its entirety
    if (isdigit(sensorstring[0]) == false) {          //if the first character in the string is a digit
      Serial.println(sensorstring);                   //send that string to the PC's serial monitor
    }
    else                                              //if the first character in the string is NOT a digit
    {
      //print_EC_data();                                //then call this function 
      char sensorstring_array[30];                        //we make a char array
      char *EC;                                           //char pointer used in string parsing
      char *TDS;                                          //char pointer used in string parsing
      char *SAL;                                          //char pointer used in string parsing
      char *GRAV;                                         //char pointer used in string parsing
      float f_ec;                                         //used to hold a floating point number that is the EC
      
      sensorstring.toCharArray(sensorstring_array, 30);   //convert the string to a char array 
      EC = strtok(sensorstring_array, ",");               //let's parse the array at each comma
      TDS = strtok(NULL, ",");                            //let's parse the array at each comma
      SAL = strtok(NULL, ",");                            //let's parse the array at each comma
      GRAV = strtok(NULL, ",");                           //let's parse the array at each comma

      // Update pH reading
      if (Serial.available() > 0) {                                                      
        user_bytes_received = Serial.readBytesUntil(13, user_data, sizeof(user_data));   
      }
      if (user_bytes_received) {                                                      
        parse_cmd(user_data);                                                          
        user_bytes_received = 0;                                                        
        memset(user_data, 0, sizeof(user_data));                                         
      }
      //Serial.println(pH.read_ph());             // uncomment this to print the pH readings to serial monitor                                                  


      // Update pressure and temperature readings
      sensor.read(); 
      //create a datastring for writing
      String dataString = String(EC) +            // EC value
                          "," +                   // comma separator
                          pH.read_ph() +          // pH value
                          "," +                    // comma separator
                          sensor.pressure() +     // pressure value
                          "," +                   // you get the idea :)
                          sensor.temperature() +  // temperature value
                          "," +                   
                          sensor.depth() +        // depth value
                          "," +                   
                          sensor.altitude();      // altitude value

      File dataFile = SD.open("wgl_log.txt", FILE_WRITE); // open the file. note that only one file can be open at a time,
      if (dataFile) {                                     // if the file is available, write to it:
        dataFile.println(dataString);                     // write the data
        dataFile.close();                                 //close the file
        // print to the serial port too:
        Serial.println(dataString);                       //print to serialport
      }
      
      else {                                              // if the file isn't open, pop up an error:
        Serial.println("error opening wgl_log.txt");
      }
    //f_ec= atof(EC);                                     //uncomment this line to convert the char to a float
    }
    sensorstring = "";                                    //clear the string
    sensor_string_complete = false;                       //reset the flag used to tell if we have received a completed string from the Atlas Scientific product
  }
}





// This is the original EC sensor read function that is provided with the Atlas sample
// This function is not called in this version of the script, but is retained here for reference
// Can be removed if you need the memory space

void print_EC_data(void) {                            //this function will pars the string  

  char sensorstring_array[30];                        //we make a char array
  char *EC;                                           //char pointer used in string parsing
  char *TDS;                                          //char pointer used in string parsing
  char *SAL;                                          //char pointer used in string parsing
  char *GRAV;                                         //char pointer used in string parsing
  float f_ec;                                         //used to hold a floating point number that is the EC
  
  sensorstring.toCharArray(sensorstring_array, 30);   //convert the string to a char array 
  EC = strtok(sensorstring_array, ",");               //let's pars the array at each comma
  TDS = strtok(NULL, ",");                            //let's pars the array at each comma
  SAL = strtok(NULL, ",");                            //let's pars the array at each comma
  GRAV = strtok(NULL, ",");                           //let's pars the array at each comma

  Serial.print("EC:");                                //we now print each value we parsed separately
  Serial.println(EC);                                 //this is the EC value

  Serial.print("TDS:");                               //we now print each value we parsed separately
  Serial.println(TDS);                                //this is the TDS value

  Serial.print("SAL:");                               //we now print each value we parsed separately
  Serial.println(SAL);                                //this is the salinity value

  Serial.print("GRAV:");                              //we now print each value we parsed separately
  Serial.println(GRAV);                               //this is the specific gravity
  Serial.println();                                   //this just makes the output easier to read
  
//f_ec= atof(EC);                                     //uncomment this line to convert the char to a float
}
























