#include "Geomagnetism.h"

// setup for my Lilygo ESP32 SDRam card
#define BOARD_MISO_PIN                      (2)
#define BOARD_MOSI_PIN                      (15)
#define BOARD_SCK_PIN                       (14)
#define BOARD_SD_CS_PIN                     (13)

// file that MUST be in the root of the SDRam card
#define   WMM_FILENAME  "/WMM.COF"

GeoMag geoMag;

void setup() {
  Serial.begin(115200);
  delay(2000);
  // iniialize SDRam pins
  SPI.begin(BOARD_SCK_PIN, BOARD_MISO_PIN, BOARD_MOSI_PIN);
  
  while (!Serial && millis() < 4000);

  // mount the SDRam card
  if (! SD.begin(BOARD_SD_CS_PIN))
    Serial.println(F("Failed to mount card"));
}

void loop()
{
  MAGtype_CoordGeodetic CoordGeodetic;
  MAGtype_Date UserDate;

  CoordGeodetic.lambda = -79.5;   // Toronto
  CoordGeodetic.phi = 43.65;
  CoordGeodetic.HeightAboveEllipsoid = 0.1;
  CoordGeodetic.UseGeoid = 1;
  /*UserDate.Year = 2024;
  UserDate.Month = 11;
  UserDate.Day = 11; */
  UserDate.DecimalYear = 2024.8;

  double deviation = geoMag.MAG_CalculateDeviation(WMM_FILENAME, CoordGeodetic, UserDate);
  Serial.printf("\nMAG_CalculateDeviation Result %f\n\n", deviation);
  
  CoordGeodetic.lambda = -123.944;   // Nanaimo
  CoordGeodetic.phi = 49.16;
  CoordGeodetic.HeightAboveEllipsoid = 0.0;
  CoordGeodetic.UseGeoid = 1;
  /*UserDate.Year = 2024;
  UserDate.Month = 11;
  UserDate.Day = 11; */
  UserDate.DecimalYear = 2024.8;

  deviation = geoMag.MAG_CalculateDeviation(WMM_FILENAME, CoordGeodetic, UserDate);
  Serial.printf("\nMAG_CalculateDeviation Result %f\n\n", deviation);
  delay(250);
}


