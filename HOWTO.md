# HOWTO: Well Geochemistry Logger (v. FEB 2024)

by Tristan Caro - 22 FEB 2024

## Deploying the WGL

1.  Connect the SD card and all of the sensor cables. `NOTE: if the SD card is absent or sensor cables are not connected, DATA ACQUISITION WILL NOT INITIATE.`
2.  Connect the power source.
3.  Load the electronics unit into the WGL.
4.  Make sure the blue pressure release valve is `OPEN.`
5.  Seal the bulkhead using the teflon locking cable. IMPORTANT: Ensure that there is no debris on the inside of the bulkhead or the O-rings on the bulkhead seal. Ensure that there is ample silicone lubricant to make sealing the WGL easier.
6.  Seal the blue pressure release valve on the outside of the bulkhead. Just past finger-tight should be sufficient.
7.  Bon voyage!

> Hint: If you want to be sure that data is being successfully acquired before sealing it inside the bulkhead, you can have the WGL connected to your computer via USB. You should see the data stream in the serial monitor.

## Recovering the WGL

1.  Dry off the exterior with a rag or paper towels.
2.  CRITICAL: Open the pressure release valve before attempting to open the bulkhead. Point it away from yourself when opening.

> Opening the pressure relief valve will release any built-up pressure inside the capsule. There should **not** be pressure. **However,** in the event that there was a water leak or electrical failure, the contents of the container could be pressurized. The pressure relief valve step is an important safety step.

3.  Remove the teflon locking cable. Shimmy open the bulkhead.
4.  Remove the electronics unit.
5.  Remove power source. Do this before removing the SD card or the software will throw an error.
6.  Remove SD card and offload data.

## Notes

-   Readings automatically begin when the power source is connected. To be safe, a fresh battery should be connected between deployments.

-   Data is written to a `.txt` file onboard the SD card. This file is *not* overwritten when the system restarts, and new data is concatenated to the end with a new header. I recommend deleting the file off the SD card after each deployment.

-   Keep an eye out for aberrant data, as this could suggest an electronics failure.

-   The pH sensor is ***untested*** at deployment depths \> 75 m. Look for faulty data which would indicate that the sensor is succumbing to pressure. If the pH data is garbage it should not affect the EC data. This EC sensor is rated to 350 meters depth and should not be a problem.
