# WGL_2023

This is the codebase for the Well Geochemistry Logger (WGL) 2023 version. The WGL is a submersible sonde containing sensors for measuring depth, temperature, electrical conductivity (EC), and pH. The sonde is intended for deployment in groundwater monitoring wells measuring >2" in diameter. 

The WGL consists of a pressure-rated aluminum cylinder which contains the onboard electronics unit. This electronics unit is powered by the Arduino Nano Every microcontroller platform. The microcontroller is interfaced with a Blue Robotics Bar30 sensor, an Atlas Scientific K0.1 EC sensor, and an Atlas Scientific Gravity Analog pH Sensor. The pH sensor is electrically isolated with a Gravity Analog Isolator Board.

Version `1.0` of the WGL was successfully field-deployed in January 2023, being submerged up to 300m depth, and profiling electrical conductivity across a hyperalkaline well. Version `5.0` is a significantly downsized version. Its hull diameter has been reduced from 75mm to 50mm and its microcontroller platform changed from an Arduino Uno to an Arduino Nano Every.

There are two versions of the software: `WGL_EC_2023` and `WGL_EC_pH_2023`. The first only measures EC. The second measures both EC and pH.

This code was developed by Tristan Caro and Harry Brodsky at the University of Colorado Boulder, Department of Geological Sciences, with support from Alexis Templeton.