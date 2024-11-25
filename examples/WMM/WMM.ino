#include "Geomagnetism.h"

#define BOARD_MISO_PIN                      (2)
#define BOARD_MOSI_PIN                      (15)
#define BOARD_SCK_PIN                       (14)
#define BOARD_SD_CS_PIN                     (13)

#define   WMM_FILENAME  "/WMM.COF"

GeoMag geoMag;
char err[80];

/*
  get user input
*/
void GetBuffer(char *buffer, size_t max)
{
  int ptr = 0;
  do {
    while(Serial.available() == 0) { };
    int data = Serial.read();
    if (data == '\n')
    {
      buffer[ptr] = '\0';
      break;
    }
    if (ptr < max - 1)
      buffer[ptr++] = (char)data;
  } while(true);
}

/*
  code converted from wmm_point.c
*/
void MAG_PrintUserDataWithUncertainty(MAGtype_GeoMagneticElements GeomagElements,
        MAGtype_GeoMagneticElements Errors,
        MAGtype_CoordGeodetic SpaceInput,
        MAGtype_Date TimeInput,
        MAGtype_MagneticModel *MagneticModel,
        MAGtype_Geoid *Geoid)
{
    char DeclString[100];
    char InclString[100];
    geoMag.MAG_DegreeToDMSstring(GeomagElements.Incl, 2, InclString);
    if(GeomagElements.H < 6000 && GeomagElements.H > 2000)
        MAG_Warnings(1, GeomagElements.H, MagneticModel);
    if(GeomagElements.H < 2000)
        MAG_Warnings(2, GeomagElements.H, MagneticModel);
    if(MagneticModel->SecularVariationUsed == TRUE)
    {
        geoMag.MAG_DegreeToDMSstring(GeomagElements.Decl, 2, DeclString);
        Serial.printf("\n Results For \n\n");
        if(SpaceInput.phi < 0)
            Serial.printf("Latitude	%.2fS\n", -SpaceInput.phi);
        else
            Serial.printf("Latitude	%.2fN\n", SpaceInput.phi);
        if(SpaceInput.lambda < 0)
            Serial.printf("Longitude	%.2fW\n", -SpaceInput.lambda);
        else
            Serial.printf("Longitude	%.2fE\n", SpaceInput.lambda);
        if(Geoid->UseGeoid == 1)
            Serial.printf("Altitude:	%.2f Kilometers above mean sea level\n", SpaceInput.HeightAboveGeoid);
        else
            Serial.printf("Altitude:	%.2f Kilometers above the WGS-84 ellipsoid\n", SpaceInput.HeightAboveEllipsoid);
        Serial.printf("Date:		%.1f\n", TimeInput.DecimalYear);
        Serial.printf("\n		Main Field\t\t\tSecular Change\n");
        Serial.printf("F	=	%9.1f +/- %5.1f nT\t\t Fdot = %5.1f\tnT/yr\n", GeomagElements.F, Errors.F, GeomagElements.Fdot);
        Serial.printf("H	=	%9.1f +/- %5.1f nT\t\t Hdot = %5.1f\tnT/yr\n", GeomagElements.H, Errors.H, GeomagElements.Hdot);
        Serial.printf("X	=	%9.1f +/- %5.1f nT\t\t Xdot = %5.1f\tnT/yr\n", GeomagElements.X, Errors.X, GeomagElements.Xdot);
        Serial.printf("Y	=	%9.1f +/- %5.1f nT\t\t Ydot = %5.1f\tnT/yr\n", GeomagElements.Y, Errors.Y, GeomagElements.Ydot);
        Serial.printf("Z	=	%9.1f +/- %5.1f nT\t\t Zdot = %5.1f\tnT/yr\n", GeomagElements.Z, Errors.Z, GeomagElements.Zdot);
        if(GeomagElements.Decl < 0)
            Serial.printf("Decl	=%20s  (WEST) +/-%3.0f Min Ddot = %.1f\tMin/yr\n", DeclString, 60 * Errors.Decl, 60 * GeomagElements.Decldot);
        else
            Serial.printf("Decl	=%20s  (EAST) +/-%3.0f Min Ddot = %.1f\tMin/yr\n", DeclString, 60 * Errors.Decl, 60 * GeomagElements.Decldot);
        if(GeomagElements.Incl < 0)
            Serial.printf("Incl	=%20s  (UP)   +/-%3.0f Min Idot = %.1f\tMin/yr\n", InclString, 60 * Errors.Incl, 60 * GeomagElements.Incldot);
        else
            Serial.printf("Incl	=%20s  (DOWN) +/-%3.0f Min Idot = %.1f\tMin/yr\n", InclString, 60 * Errors.Incl, 60 * GeomagElements.Incldot);
    } else
    {
        geoMag.MAG_DegreeToDMSstring(GeomagElements.Decl, 2, DeclString);
        Serial.printf("\n Results For \n\n");
        if(SpaceInput.phi < 0)
            Serial.printf("Latitude	%.2fS\n", -SpaceInput.phi);
        else
            Serial.printf("Latitude	%.2fN\n", SpaceInput.phi);
        if(SpaceInput.lambda < 0)
            Serial.printf("Longitude	%.2fW\n", -SpaceInput.lambda);
        else
            Serial.printf("Longitude	%.2fE\n", SpaceInput.lambda);
        if(Geoid->UseGeoid == 1)
            Serial.printf("Altitude:	%.2f Kilometers above MSL\n", SpaceInput.HeightAboveGeoid);
        else
            Serial.printf("Altitude:	%.2f Kilometers above WGS-84 Ellipsoid\n", SpaceInput.HeightAboveEllipsoid);
        Serial.printf("Date:		%.1f\n", TimeInput.DecimalYear);
        Serial.printf("\n	Main Field\n");
        Serial.printf("F	=	%-9.1f +/-%5.1f nT\n", GeomagElements.F, Errors.F);
        Serial.printf("H	=	%-9.1f +/-%5.1f nT\n", GeomagElements.H, Errors.H);
        Serial.printf("X	=	%-9.1f +/-%5.1f nT\n", GeomagElements.X, Errors.X);
        Serial.printf("Y	=	%-9.1f +/-%5.1f nT\n", GeomagElements.Y, Errors.Y);
        Serial.printf("Z	=	%-9.1f +/-%5.1f nT\n", GeomagElements.Z, Errors.Z);
        if(GeomagElements.Decl < 0)
            Serial.printf("Decl	=%20s  (WEST)+/-%4f\n", DeclString, 60 * Errors.Decl);
        else
            Serial.printf("Decl	=%20s  (EAST)+/-%4f\n", DeclString, 60 * Errors.Decl);
        if(GeomagElements.Incl < 0)
            Serial.printf("Incl	=%20s  (UP)+/-%4f\n", InclString, 60 * Errors.Incl);
        else
            Serial.printf("Incl	=%20s  (DOWN)+/-%4f\n", InclString, 60 * Errors.Incl);
    }

    if(SpaceInput.phi <= -55 || SpaceInput.phi >= 55)
        /* Print Grid Variation */
    {
        geoMag.MAG_DegreeToDMSstring(GeomagElements.GV, 2, InclString);
        Serial.printf("\n\n Grid variation =%20s\n", InclString);
    }

}/*MAG_PrintUserDataWithUncertainty*/


int MAG_ValidateDMSstring(char *input, int min, int max, char *Error)

/* Validates a latitude DMS string, and returns 1 for a success and returns 0 for a failure.
It copies an error message to the Error string in the event of a failure.

INPUT : input (DMS string)
OUTPUT : Error : Error string
CALLS : none
 */
{
    int degree, minute, second, j = 0, n, max_minute = 60, max_second = 60;
    int i;
    degree = -1000;
    minute = -1;
    second = -1;
    n = (int) strlen(input);

    for(i = 0; i <= n - 1; i++) /*tests for legal characters*/
    {
        if((input[i] < '0' || input[i] > '9') && (input[i] != ',' && input[i] != ' ' && input[i] != '-' && input[i] != '\0' && input[i] != '\n'))
        {
            strcpy(Error, "\nError: Input contains an illegal character, legal characters for Degree, Minute, Second format are:\n '0-9' ',' '-' '[space]' '[Enter]'\n");
            return FALSE;
        }
        if(input[i] == ',')
            j++;
    }
    if(j == 2)
        j = sscanf(input, "%d, %d, %d", &degree, &minute, &second); /*tests for legal formatting and range*/
    else
        j = sscanf(input, "%d %d %d", &degree, &minute, &second);
    if(j == 1)
    {
        minute = 0;
        second = 0;
        j = 3;
    }
    if(j != 3)
    {
        strcpy(Error, "\nError: Not enough numbers used for Degrees, Minutes, Seconds format\n or they were incorrectly formatted\n The legal format is DD,MM,SS or DD MM SS\n");
        return FALSE;
    }
    if(degree > max || degree < min)
    {
        sprintf(Error, "\nError: Degree input is outside legal range\n The legal range is from %d to %d\n", min, max);
        return FALSE;
    }
    if(degree == max || degree == min)
        max_minute = 0;
    if(minute > max_minute || minute < 0)
    {
        strcpy(Error, "\nError: Minute input is outside legal range\n The legal minute range is from 0 to 60\n");
        return FALSE;
    }
    if(minute == max_minute)
        max_second = 0;
    if(second > max_second || second < 0)
    {
        strcpy(Error, "\nError: Second input is outside legal range\n The legal second range is from 0 to 60\n");
        return FALSE;
    }
    return TRUE;
} /*MAG_ValidateDMSstring*/


int MAG_Warnings(int control, double value, MAGtype_MagneticModel *MagneticModel)

/*Return value 0 means end program, Return value 1 means get new data, Return value 2 means continue.
  This prints a warning to the screen determined by the control integer. It also takes the value of the parameter causing the warning as a double.  This is unnecessary for some warnings.
  It requires the MagneticModel to determine the current epoch.

 INPUT control :int : (Warning number)
                value  : double: Magnetic field strength
                MagneticModel
OUTPUT : none
CALLS : none

 */
{
    char ans[20];
    strcpy(ans, "");
    
    switch(control) {
      case 1:/* Horizontal Field strength low */
        Serial.printf("\nCaution: location is approaching the blackout zone around the magnetic pole as\n");
        Serial.printf("      defined by the WMM military specification \n");
        Serial.printf("      (https://www.ngdc.noaa.gov/geomag/WMM/data/MIL-PRF-89500B.pdf). Compass\n");
        Serial.printf("      accuracy may be degraded in this region.\n");
        Serial.printf("Press enter to continue...\n");
        GetBuffer(ans, sizeof(ans));
        break;
      case 2:/* Horizontal Field strength very low */
        Serial.printf("\nWarning: location is in the blackout zone around the magnetic pole as defined\n");
        Serial.printf("      by the WMM military specification \n");
        Serial.printf("      (https://www.ngdc.noaa.gov/geomag/WMM/data/MIL-PRF-89500B.pdf). Compass\n");
        Serial.printf("      accuracy is highly degraded in this region.\n");
        GetBuffer(ans, sizeof(ans));
        break;
      case 3:/* Elevation outside the recommended range */
        Serial.printf("\nWarning: The value you have entered of %.1f km for the elevation is outside of the recommended range.\n Elevations above -10.0 km are recommended for accurate results. \n", value);
        GetBuffer(ans, sizeof(ans));
        break;
      case 4:/*Date outside the recommended range*/
        Serial.printf("\nWARNING - TIME EXTENDS BEYOND INTENDED USAGE RANGE\n CONTACT NCEI FOR PRODUCT UPDATES:\n");
        Serial.printf("	National Centers for Environmental Information\n");
        Serial.printf("	NOAA E/NE42\n");
        Serial.printf("	325 Broadway\n");
        Serial.printf("\n	Boulder, CO 80305 USA");
        Serial.printf("	Attn: Manoj Nair or Arnaud Chulliat\n");
        Serial.printf("	Phone:	(303) 497-4642 or -6522\n");
        Serial.printf("	Email:	geomag.models@noaa.gov\n");
        Serial.printf("	Web: http://www.ngdc.noaa.gov/geomag/WMM/DoDWMM.shtml\n");
        Serial.printf("\n VALID RANGE  = %d - %d\n", (int) MagneticModel->epoch, (int) MagneticModel->CoefficientFileEndDate);
        Serial.printf(" TIME   = %f\n", value);
        GetBuffer(ans, sizeof(ans));
        break;
      case 5:/*Elevation outside the allowable range*/
        Serial.printf("\nError: The value you have entered of %f km for the elevation is outside of the recommended range.\n Elevations above -10.0 km are recommended for accurate results. \n", value);
        GetBuffer(ans, sizeof(ans));
        break;
    }
    return 2;
} /*MAG_Warnings*/

void MAG_GetDeg(char* Query_String, double* latitude, double bounds[2]) {
	/*Gets a degree value from the user using the standard input*/
	char buffer[64], Error_Message[255];
	int done, i, j;
	String str;

	Serial.printf("%s", Query_String);
  GetBuffer(buffer, sizeof(buffer));
  for(i = 0, done = 0, j = 0; i <= 64 && !done; i++)
  {
        if(buffer[i] == '.')
        {
            j = sscanf(buffer, "%lf", latitude);
            if(j == 1)
                done = 1;
            else
                done = -1;
        }
        if(buffer[i] == ',')
        {
            if(MAG_ValidateDMSstring(buffer, bounds[0], bounds[1], Error_Message))
            {
                geoMag.MAG_DMSstringToDegree(buffer, latitude);
                done = 1;
            } else
                done = -1;
        }
        if(buffer[i] == ' ')/* This detects if there is a ' ' somewhere in the string,
		if there is the program tries to interpret the input as Degrees Minutes Seconds.*/
        {
            if(MAG_ValidateDMSstring(buffer, bounds[0], bounds[1], Error_Message))
            {
                geoMag.MAG_DMSstringToDegree(buffer, latitude);
                done = 1;
            } else
                done = -1;
        }
        if(buffer[i] == '\0' || done == -1)
        {
            if(MAG_ValidateDMSstring(buffer, bounds[0], bounds[1], Error_Message) && done != -1)
            {
                sscanf(buffer, "%lf", latitude);
                done = 1;
            } else
            {
                Serial.printf("%s", Error_Message);
                strcpy(buffer, "");
                Serial.printf("\nError encountered, please re-enter as '(-)DDD,MM,SS' or in Decimal Degrees DD.ddd:\n");
                GetBuffer(buffer, sizeof(buffer));
                i = -1;
                done = 0;
            }
        }
    }
}

int MAG_GetAltitude(char* Query_String, MAGtype_Geoid *Geoid, MAGtype_CoordGeodetic* coords, int bounds[2], int AltitudeSetting){
	int done, j, UpBoundOn;
	char tmp;
	char buffer[64];
	double value;
	done = 0;
    if(bounds[1] != NO_ALT_MAX){
        UpBoundOn = TRUE;    
    } else {
        UpBoundOn = FALSE;
    }
    Serial.printf("%s", Query_String);
	
    while(!done)
    {
        GetBuffer(buffer, sizeof(buffer));
        j = 0;
        if((AltitudeSetting != MSLON) && (buffer[0] == 'e' || buffer[0] == 'E' || AltitudeSetting == WGS84ON)) /* User entered height above WGS-84 ellipsoid, copy it to CoordGeodetic->HeightAboveEllipsoid */
        {
          if(buffer[0]=='e' || buffer[0]=='E') {
            j = sscanf(buffer, "%c%lf", &tmp, &coords->HeightAboveEllipsoid);
          } else {
            j = sscanf(buffer, "%lf", &coords->HeightAboveEllipsoid);
          }
          if(j == 2)
              j = 1;
          Geoid->UseGeoid = 0;
          coords->HeightAboveGeoid = coords->HeightAboveEllipsoid;
    			value = coords->HeightAboveEllipsoid;
          Serial.printf("j %d\n", j);
        } else /* User entered height above MSL, convert it to the height above WGS-84 ellipsoid */
        {
          Geoid->UseGeoid = 1;
          j = sscanf(buffer, "%lf", &coords->HeightAboveGeoid);
          geoMag.MAG_ConvertGeoidToEllipsoidHeight(coords, Geoid);
		    	value = coords->HeightAboveGeoid;
        }
        Serial.printf("j %d value %lf\n", j, value);
        if(j == 1)
            done = 1;
        else
            Serial.printf("\nIllegal Format, please re-enter as '(-)HHH.hhh:'\n");
        if((value < bounds[0] || (value > bounds[1] && UpBoundOn)) && done == 1) {
          if(UpBoundOn) {
            done = 0;
            printf("\nWarning: The value you have entered of %f km for the elevation is outside of the required range.\n", value);
            printf(" An elevation between %d km and %d km is needed. \n", bounds[0], bounds[1]);
            if (AltitudeSetting == WGS84ON){
                Serial.printf("Please enter height above WGS-84 Ellipsoid (in kilometers):\n");
            } else if (AltitudeSetting==MSLON){
                Serial.printf("Please enter height above mean sea level (in kilometers):\n");
            } else {
                Serial.printf("Please enter height in kilometers (prepend E for height above WGS-84 Ellipsoid):");
            }
          } else {
            switch(MAG_Warnings(3, value, NULL)) {
              case 0:
                return USER_GAVE_UP;
              case 1:
                done = 0;
                Serial.printf("Please enter height above sea level (in kilometers):\n");
                break;
              case 2:
                break;
            }
          }
        }
    }
    return 0;
}


int MAG_GetUserInput(MAGtype_MagneticModel *MagneticModel, MAGtype_Geoid *Geoid, MAGtype_CoordGeodetic *CoordGeodetic, MAGtype_Date *MagneticDate)

/*
This prompts the user for coordinates, and accepts many entry formats.
It takes the MagneticModel and Geoid as input and outputs the Geographic coordinates and Date as objects.
Returns 0 when the user wants to exit and 1 if the user enters valid input data.
INPUT :  MagneticModel  : Data structure with the following elements used here
                        double epoch;       Base time of Geomagnetic model epoch (yrs)
                : Geoid Pointer to data structure MAGtype_Geoid (used for converting HeightAboveGeoid to HeightABoveEllipsoid

OUTPUT: CoordGeodetic : Pointer to data structure. Following elements are updated
                        double lambda; (longitude)
                        double phi; ( geodetic latitude)
                        double HeightAboveEllipsoid; (height above the ellipsoid (HaE) )
                        double HeightAboveGeoid;(height above the Geoid )

                MagneticDate : Pointer to data structure MAGtype_Date with the following elements updated
                        int	Year; (If user directly enters decimal year this field is not populated)
                        int	Month;(If user directly enters decimal year this field is not populated)
                        int	Day; (If user directly enters decimal year this field is not populated)
                        double DecimalYear;      decimal years

CALLS: 	MAG_DMSstringToDegree(buffer, &CoordGeodetic->lambda); (The program uses this to convert the string into a decimal longitude.)
                MAG_ValidateDMSstringlong(buffer, Error_Message)
                MAG_ValidateDMSstringlat(buffer, Error_Message)
                MAG_Warnings
                MAG_ConvertGeoidToEllipsoidHeight
                MAG_DateToYear

 */
{
  char Error_Message[255];
  char buffer[40];
  int i, j, a, b, c, done = 0;
  double lat_bound[2] = {LAT_BOUND_MIN, LAT_BOUND_MAX};
  double lon_bound[2] = {LON_BOUND_MIN, LON_BOUND_MAX};
  int alt_bound[2] = {ALT_BOUND_MIN, NO_ALT_MAX}; 
  char Qstring[1024];
  String str;
  strcpy(buffer, ""); /*Clear the input    */
	strcpy(Qstring, "\nPlease enter latitude\nNorth latitude positive, For example:\n30, 30, 30 (D,M,S) or 30.508 (Decimal Degrees) (both are north)\n");
	MAG_GetDeg(Qstring, &CoordGeodetic->phi, lat_bound);
  strcpy(buffer, ""); /*Clear the input*/
  strcpy(Qstring,"\nPlease enter longitude\nEast longitude positive, West negative.  For example:\n-100.5 or -100, 30, 0 for 100.5 degrees west\n");
	MAG_GetDeg(Qstring, &CoordGeodetic->lambda, lon_bound);
	    
	strcpy(Qstring,"\nPlease enter height above mean sea level (in kilometers):\n[For height above WGS-84 ellipsoid prefix E, for example (E20.1)]\n");
  if(MAG_GetAltitude(Qstring, Geoid, CoordGeodetic, alt_bound, FALSE)==USER_GAVE_UP)
      return FALSE;
  strcpy(buffer, "");
  Serial.printf("\nPlease enter the decimal year or calendar date\n (YYYY.yyy, MM DD YYYY or MM/DD/YYYY):\n");
  GetBuffer(buffer, sizeof(buffer));
  for(i = 0, done = 0; i <= 40 && !done; i++)
  {
    if(buffer[i] == '.')
    {
      j = sscanf(buffer, "%lf", &MagneticDate->DecimalYear);
      if(j == 1)
        done = 1;
      else
        buffer[i] = '\0';
    }
    if(buffer[i] == '/')
    {
      sscanf(buffer, "%d/%d/%d", &MagneticDate->Month, &MagneticDate->Day, &MagneticDate->Year);
      if(!geoMag.MAG_DateToYear(MagneticDate, Error_Message))
      {
        Serial.printf("%s", Error_Message);
        Serial.printf("\nPlease re-enter Date in MM/DD/YYYY or MM DD YYYY format, or as a decimal year\n");
        while (NULL == fgets(buffer, 40, stdin)) {
          Serial.printf("\nPlease re-enter Date in MM/DD/YYYY or MM DD YYYY format, or as a decimal year\n");
        }
        i = 0;
      } else
        done = 1;
    }
    if((buffer[i] == ' ' && buffer[i + 1] != '/') || buffer[i] == '\0')
    {
      if(3 == sscanf(buffer, "%d %d %d", &a, &b, &c))
      {
        MagneticDate->Month = a;
        MagneticDate->Day = b;
        MagneticDate->Year = c;
        MagneticDate->DecimalYear = 99999;
        Serial.printf("a %d b %d c %d\n", a, b, c);
      } else if(1 == sscanf(buffer, "%d %d %d", &a, &b, &c))
      {
        MagneticDate->DecimalYear = a;
        done = 1;
      }
      if(!(MagneticDate->DecimalYear == a))
      {
        if(!geoMag.MAG_DateToYear(MagneticDate, Error_Message))
        {
          Serial.printf("%s", Error_Message);
          strcpy(buffer, "");
          Serial.printf("\nError encountered, please re-enter Date in MM/DD/YYYY or MM DD YYYY format, or as a decimal year\n");
          GetBuffer(buffer, sizeof(buffer));
          i = -1;
        } else
          done = 1;
      }
    }
    if(buffer[i] == '\0' && i != -1 && done != 1)
    {
      strcpy(buffer, "");
      Serial.printf("\nError encountered, please re-enter as MM/DD/YYYY, MM DD YYYY, or as YYYY.yyy:\n");
      GetBuffer(buffer, sizeof(buffer));
      i = -1;
    }
    Serial.printf("Done %d\n", done);
    if(done)
    {
      if(MagneticDate->DecimalYear > MagneticModel->CoefficientFileEndDate || MagneticDate->DecimalYear < MagneticModel->epoch)
      {
        Serial.println("ruhroh");
        switch(MAG_Warnings(4, MagneticDate->DecimalYear, MagneticModel)) {
          case 0:
            return 0;
          case 1:
            done = 0;
            i = -1;
            strcpy(buffer, "");
            Serial.printf("\nPlease enter the decimal year or calendar date\n (YYYY.yyy, MM DD YYYY or MM/DD/YYYY):\n");
            GetBuffer(buffer, sizeof(buffer));
            break;
          case 2:
            break;
        }
      }
    }
  }
  Serial.println("Finished");
  return TRUE;
} /*MAG_GetUserInput*/


void setup() {
  float_value f1;
  float_value f2;
  byte buf[4];

  Serial.begin(115200);
  delay(2000);
  // iniialize SDRam pins
  SPI.begin(BOARD_SCK_PIN, BOARD_MISO_PIN, BOARD_MOSI_PIN);
  
  while (!Serial && millis() < 4000);

  // mount the SDRam card
  if (! SD.begin(BOARD_SD_CS_PIN))
    Serial.println(F("Failed to mount card"));

  Serial.printf("WMM Tests. Pass lat/lon/altitude and time date to return Magnetic Deviation and Inclination\n");
}


void loop() {
  strcpy(err, "");
    geoMag.begin(err);    // setup error string
    MAGtype_MagneticModel *MagneticModels, *TimedMagneticModel;
    MAGtype_Ellipsoid Ellip;
    MAGtype_CoordSpherical CoordSpherical;
    MAGtype_CoordGeodetic CoordGeodetic;
    MAGtype_Date UserDate;
    MAGtype_GeoMagneticElements GeoMagneticElements, Errors;
    MAGtype_Geoid Geoid;
    char VersionDate[12];
    int NumTerms, Flag = 1, nMax = 0;
    int epochs = 1;

    /* Memory allocation */
    strncpy(VersionDate, VERSIONDATE_LARGE + 39, 11);
    VersionDate[11] = '\0';
    //GeoidHeightBuffer = ConstGeoidHeightBuffer;
    MagneticModels = geoMag.MAG_robustReadMagModels(WMM_FILENAME, epochs);
    if (strlen(err))
      Serial.println(err);
    if(nMax < MagneticModels->nMax) nMax = MagneticModels->nMax;
    NumTerms = ((nMax + 1) * (nMax + 2) / 2);
    TimedMagneticModel = geoMag.MAG_AllocateModelMemory(NumTerms); // For storing the time modified WMM Model parameters
    if (strlen(err))
      Serial.println(err);
    if(MagneticModels == NULL || TimedMagneticModel == NULL)
    {
        geoMag.MAG_Error(2);
        Serial.println(err);    // memory issue
    }
    geoMag.MAG_SetDefaults(&Ellip, &Geoid); // Set default values and constants
    // Check for Geographic Poles

    // Set EGM96 Geoid parameters
    Geoid.Geoid_Initialized = 1;

    // Set EGM96 Geoid parameters END
    if(MAG_GetUserInput(MagneticModels, &Geoid, &CoordGeodetic, &UserDate)) // Get User Input
    {
      Serial.println("In Sph");
        geoMag.MAG_GeodeticToSpherical(Ellip, CoordGeodetic, &CoordSpherical); // Convert from geodetic to Spherical Equations: 17-18, WMM Technical report
      Serial.println("In Timely");
        geoMag.MAG_TimelyModifyMagneticModel(UserDate, MagneticModels, TimedMagneticModel); // Time adjust the coefficients, Equation 19, WMM Technical report 
      Serial.println("In GeoMag");
        geoMag.MAG_Geomag(Ellip, CoordSpherical, CoordGeodetic, TimedMagneticModel, &GeoMagneticElements); // Computes the geoMagnetic field elements and their time change
      Serial.println("In Grid");
        geoMag.MAG_CalculateGridVariation(CoordGeodetic, &GeoMagneticElements);
      Serial.println("In Error Calc");
        geoMag.MAG_WMMErrorCalc(GeoMagneticElements.H, &Errors);
      Serial.println("In Print");
        MAG_PrintUserDataWithUncertainty(GeoMagneticElements, Errors, CoordGeodetic, UserDate, TimedMagneticModel, &Geoid); // Print the results
    }


    geoMag.MAG_FreeMagneticModelMemory(TimedMagneticModel);
    
    geoMag.MAG_FreeMagneticModelMemory(MagneticModels);
    // use the do all function and test it
    double deviation = geoMag.MAG_CalculateDeviation(WMM_FILENAME, CoordGeodetic, UserDate);
    Serial.printf("\nMAG_CalculateDeviation Result %f\n\n", deviation);
    
    delay(500);
}
