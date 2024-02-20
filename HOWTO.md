# HOWTO: Well Geochemistry Logger (v. FEB 2024)

## Setup
Readings automatically begin when the power source is connected. To be safe, a fresh battery should be connected between deployments.

Data is written to a `.txt` file onboard the SD card. This file is *not* overwritten when the system restarts, and new data is concatenated to the end with a new header. Here is the recommended procedure:

1. Connect power. Data acquisition starts.
2. Deploy the WGL
3. Recover the WGL. Remove the power source. Data acquisition stops.
4. Remove the SD and connect it to laptop. Rename the file with the deployment information. E.g., "07JAN2023_NSHQ14.TXT".
5. Backup the file to your laptop and make a copy. 
> Ensure the data quality is good by looking at the raw values (or plotting it). Now, new data will not be added onto the file you just renamed.
6. Delete the file on the SD card and re-insert it into the WGL.

> Keep an eye out for aberrant data, as this could suggest an electronics failure.

> The pH sensor is *untested* at these deployment depths. Look for faulty data. If the data is garbage or causing problems, it should not affect the EC data. EC data can be accurately acquired at the depths of Oman wells.

## Deploying the WGL

1. Connect the SD card.
2. Connect the power source (NOTE: SD card must be inserted first, or the software will throw an error!)
3. Load the electronics unit into the WGL.
4. Make sure the blue pressure release valve is OPEN. 
5. Seal the bulkhead using the teflon locking cord. IMPORTANT: Ensure that there is no debris on the inside of the bulkhead or the O-rings on the bulkhead seal. Ensure that there is ample silicone lubricant to make sealing the WGL easier.
6. Seal the blue pressure release valve on the outside of the bulkhead. Just past finger-tight should be sufficient.
7. Bon voyage!

> Hint: If you want to be sure that data is being successfully acquired before sealing the bulkhead, you can have the WGL connected to your computer via USB. You should see the data stream in the serial monitor.

## Recovering the WGL

1. Dry off the exterior with a rag or paper towels (so the electronics are safe from drops)
2. IMPORTANT: Open the pressure release valve. 

> This will release any built-up pressure from the deployment. There should not be pressure - however - in the event that there was an electrical issue or water leak, the contents of the container have the potential to be pressurized. This is a conservative safety precaution.

3. Remove the teflon locking cable. Open the bulkhead. 
4. Remove the electronics unit.
5. Remove power source. Do this before removing the SD card or the software will throw an error.
6. Remove SD card and offload data.