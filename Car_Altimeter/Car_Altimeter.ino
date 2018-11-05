#include <SparkFunMPL3115A2.h>
#include <Wire.h>
//#include "MPL3115A2.h"
#include <MicroView.h>
#include <HMC5883L.h>

//Create an instance of the object
MPL3115A2 myPressure;
HMC5883L compass;

boolean inverted = 0;//change this variable to change wheather the screen is inverted or not


int altitude;
float heading;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setup()
{
  uView.begin();	// start MicroView

  if(inverted == 1)
  {
  //Flip  and invert Screen
  uView.flipHorizontal(1);
  uView.flipVertical(1);
  }
  
  uView.clear(PAGE);	// clear page
  //uView.print("HelloWorld");	// display HelloWorld
  uView.display();
  
  Wire.begin();        // Join i2c bus
  Serial.begin(9600);  // Start serial for output
  
  compass = HMC5883L(); //new instance of HMC5883L library
  setupHMC5883L(); //setup the HMC5883L

  myPressure.begin(); // Get sensor online

  //Configure the sensor
  myPressure.setModeAltimeter(); // Measure altitude above sea level in meters
  //myPressure.setModeBarometer(); // Measure pressure in Pascals from 20 to 110 kPa

  myPressure.setOversampleRate(7); // Set Oversample to the recommended 128
  myPressure.enableEventFlags(); // Enable all three pressure and temp event flags 
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void loop()
{
  heading = getHeading();
  
  uView.setCursor(0,5);
  uView.clear(PAGE);	// clear page
  
  uView.setFontType(0);

  altitude = myPressure.readAltitudeFt();
  
  uView.print("Alt: ");
  uView.println(altitude);

  float temperature = myPressure.readTempF();
  
  uView.print("Tf: ");
  uView.println(temperature);

  uView.print("H: ");
  uView.println(heading);
  
  uView.println();
  
  uView.setFontType(1);
  
  //heading = getHeading();

  if(inverted == 0)
  {
    //uView.println(heading);
    
    if(heading >= 338 || heading <= 22)
    {
      uView.println(" North");
    }
    else if(heading >= 23 && heading <= 67)
    {
      uView.println("  NE");
    }
    else if(heading >= 68 && heading <= 112)
    {
      uView.println(" East");
    }
    else if(heading >= 113 && heading <= 157)
    {
      uView.println("  SE");
    }
    else if(heading >= 158 && heading <= 202)
    {
      uView.println(" South");
    }
      else if(heading >= 203 && heading <= 247)
    {
      uView.println("  SW");
    }
    else if(heading >= 248 && heading <= 292)
    {
      uView.println(" West");
    }
    else if(heading >= 293 && heading <= 337)
    {
      uView.println("  NW");
    }
    else
    {
      //uView.println("No Head");
    }
  }

  if(inverted == 1)
  {
    if(heading >= 338 || heading <= 22)
    {
      uView.println(" North");
    }
    else if(heading >= 23 && heading <= 67)
    {
      uView.println("  NW");
    }
    else if(heading >= 68 && heading <= 112)
    {
      uView.println(" West");
    }
    else if(heading >= 113 && heading <= 157)
    {
      uView.println("  SW");
    }
    else if(heading >= 158 && heading <= 202)
    {
      uView.println(" South");
    }
      else if(heading >= 203 && heading <= 247)
    {
      uView.println("  SE");
    }
    else if(heading >= 248 && heading <= 292)
    {
      uView.println(" East");
    }
    else if(heading >= 293 && heading <= 337)
    {
      uView.println("  NE");
    }
    else
    {
      uView.println("No Head");
    }
  }
  
  uView.display();


  delay(200);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setupHMC5883L(){
  //Setup the HMC5883L, and check for errors
  int error;  
  error = compass.SetScale(1.3); //Set the scale of the compass.
  if(error != 0) Serial.println(compass.GetErrorText(error)); //check if there is an error, and print if so

  error = compass.SetMeasurementMode(Measurement_Continuous); // Set the measurement mode to Continuous
  if(error != 0) Serial.println(compass.GetErrorText(error)); //check if there is an error, and print if so
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
float getHeading(){
  //Get the reading from the HMC5883L and calculate the heading
  MagnetometerScaled scaled = compass.ReadScaledAxis(); //scaled values from compass.
  float heading = atan2(scaled.YAxis, scaled.XAxis);

  // Correct for when signs are reversed.
  if(heading < 0) heading += 2*PI;
  if(heading > 2*PI) heading -= 2*PI;

  return heading * RAD_TO_DEG; //radians to degrees  Add 10 to account for Declination Angle of Boulder, CO, USA
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
