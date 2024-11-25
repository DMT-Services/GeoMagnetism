# GeoMagnetism

This library was developed targeting an **`Arduino`** ESP32 with an SDRam card.

The objective of this library is to calculate the magnetic deviation from any given point in the world.
You provide lat/lon altitude and date/time.  The NOAA WMM calculation returns magnetic deviation in decimal degrees. (among other things such as magnetic inclination)

The WMM.ino example file will allow you to enter the parameters and is taken virtually verbatim from the WMM NOAA example wmm_point.c.
It prompts for lat/lon altitude and time/date.

The simple.ino example file uses the function MAG_CalculateDeviation(const char* fileName, MAGtype_CoordGeodetic CoordGeodetic, MAGtype_Date UserDate)
 and tests with Toronto, Canada and Nanaimo, Canada coords.  
  function can be used if you already have your lat/lon time/date and altitude values and you just want the Magnetic Deviation (for course headings etc...)

The SDRam card MUST have WMM.cof and EGM9615.bin files in the root folder.  The library uses these files to calculate the deviation.  
I don't believe the EGM9615.bin file will change, but WMM.cof is only good up until the end of 2025 I believe.  Hopefully NOAA will publish a new file before that time.
A future enhancement could be to use a 2nd WMM.cof file when the new one is available.  Those files are tiny in comparison to the EGM9615 file.

## Installation

1. [Download](https://github.com/DMT-Services/GeoMagnetism/archive/GeoMagnetism.zip) the Latest release from gitHub.
2. Unzip and copy the Folder GeoMagnetism
3. Paste the folder on your Library folder (On your `Libraries` folder inside Sketchbooks or Arduino software).
4. Reopen the Arduino software.

-------------------------

### Version History

* `1.0 (2024-11-25)`: Original release

