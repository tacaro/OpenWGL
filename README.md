# WGL_2023

This is the codebase for the Well Geochemistry Logger (WGL) 2023 version. The WGL is a submersible sonde containing sensors for measuring depth, temperature, electrical conductivity (EC), and pH. The sonde is intended for deployment in groundwater monitoring wells measuring \>2" in diameter.

The WGL consists of a pressure-rated aluminum cylinder which contains the onboard electronics unit. This electronics unit is powered by the Arduino Nano Every microcontroller platform. The microcontroller is interfaced with a Blue Robotics Bar30 sensor, an Atlas Scientific K0.1 EC sensor, and an Atlas Scientific Gravity Analog pH Sensor. The pH sensor is electrically isolated with a Gravity Analog Isolator Board.

Version `1.0` of the WGL was successfully field-deployed in January 2023, being submerged up to 300m depth, and profiling electrical conductivity across a hyperalkaline well. Version `5.0` is a significantly downsized version. Its hull diameter has been reduced from 75mm to 50mm and its microcontroller platform changed from an Arduino Uno to an Arduino Nano Every.

There are two versions of the software: `WGL_EC_2023` and `WGL_EC_pH_2023`. The first only measures EC. The second measures both EC and pH.

This code was developed by Tristan Caro and Harry Brodsky at the University of Colorado Boulder, Department of Geological Sciences, with support from Alexis Templeton.

## Bill of Components

**Hull**

-   Aluminum - 300 mm x 50mm (ID) watertight enclosure. Depth rating: 950 m. (Blue Robotics).

-   Aluminum Flange Cap - 50mm (ID) - Blank. (Blue Robotics).

-   Aluminum Flange Cap - 50mm (ID) - 4 x M10 hole. (Blue Robotics).

-   Pressure Relief Valve (M10). (Blue Robotics).

-   WetLink Cable Penetrator (M10, 4.5mm, HC). (Blue Robotics).

-   M10 Connector Blank. (Blue Robotics).

**Electronics**

-   Arduino Nano Every. (Arduino).

-   Atlas Scientific K0.1 Kit. (Atlas Scientific)

    -   EZO Conductivity Circuit

    -   K.01 Conductivity Probe

    -   EZO Carrier Board

-   HiLetgo Micro SD TF Card Reader Module (6 Pin SPI Interface).

-   Micro SD Card.

-   Bar30 High-Resolution 300m Depth/Pressure Sensor. (Blue Robotics).

-   JST GH to DF13 Adapter. (Blue Robotics).
