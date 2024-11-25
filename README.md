# GeoMagnetism

This library was developed targeting an **`Arduino`** ESP32 with an SDRam card.

The objective of this library is to calculate the magnetic deviation from any given point in the world.
You provide lat/lon altitude and date/time.  The NOAA WMM calculation returns magnetic deviation in decimal degrees. (among other things such as magnetic inclination)

The WMM.ino file is for testing the library.  It's almost an exact conversion of wmm_point.c from the WMM NOAA web source.  It prompts for lat/lon altitude and time/date.

The MAG_CalculateDeviation(const char* fileName, MAGtype_CoordGeodetic CoordGeodetic, MAGtype_Date UserDate)
  function can be used if you already have your lat/lon time/date and altitude values and you just want the Magnetic Deviation (for course headings etc...)

The SDRam card MUST have WMM.cof and EGM9615.bin files in the root folder.  The library uses these files to calculate the deviation.  

I don't believe EGM9615.bin file will change, but WMM.cof is only good up until the end of 2025 I believe.
Hopefully NOAA will publish a new file before that time.

## Installation

1. [Download](https://github.com/DMT-Services/GeoMagnetism/archive/GeoMagnetism.zip) the Latest release from gitHub.
2. Unzip and modify the Folder name to "LinkedList" (Remove the '-version')
3. Paste the modified folder on your Library folder (On your `Libraries` folder inside Sketchbooks or Arduino software).
4. Reopen the Arduino software.

### Version History

* `1.0 (2024-11-25)`: Original release
