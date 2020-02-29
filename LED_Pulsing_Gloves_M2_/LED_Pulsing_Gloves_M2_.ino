/**************************************************************************/
/*!
    @file     LED_Pulsing_Gloves1.h
    @author   Oliver Heilmann
    v1.0  - First release
*/
/**************************************************************************/
#include <Wire.h>
#include <Adafruit_MMA8451.h>
#include <Adafruit_Sensor.h>

Adafruit_MMA8451 mma = Adafruit_MMA8451();
int i = 0;
float first_val = 0;
bool start = true;
int LED = 13;

// Curve Parameters
int samples = 15; float avgCurve[15]; byte arrayIndex = 0;
float percentage = 0.05;
float sumavg = 0;

void setup(void) {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED, OUTPUT);
  
  Serial.begin(9600);
  //Serial.println("Adafruit MMA8451 test!");
  
  if (! mma.begin()) {
    //Serial.println("Couldnt start");
    while (1);
  }
  //Serial.println("MMA8451 found!");
  
  mma.setRange(MMA8451_RANGE_2_G);
  
  //Serial.print("Range = "); Serial.print(2 << mma.getRange());  
  //Serial.println("G");
  delay(2000);
}

void loop() {
  // Read the 'raw' data in 14-bit counts
  mma.read();

  /* Get a new sensor event */ 
  sensors_event_t event; 
  mma.getEvent(&event);

  float x = event.acceleration.x; 
  float y = event.acceleration.y;
  float z = event.acceleration.z;
  float avg = (abs(x)+abs(y)+abs(z))/3.0;

  // Appending list with values
  if(arrayIndex <= samples){
    avgCurve[arrayIndex] = avg;
    arrayIndex++;
  }else{
    arrayIndex = 0;
    avgCurve[arrayIndex] = avg;
    arrayIndex++;
    }
  
  // Sum of Average
  sumavg = 0;
  for (int i = 0 ; i < samples ; i++){
     sumavg += avgCurve[i];
  }

  /*
  // Compare end and end -2 results
  float diff = abs(abs(avgCurve[12]) - abs(avgCurve[14]));
  float average_sum = (abs(avgCurve[12]) + abs(avgCurve[14])) / 2.0;
  if(diff/average_sum < percentage ){
    //digitalWrite(LED, HIGH);
    Serial.print(avg);
    Serial.print("   ");
    Serial.println(avgCurve[13]);
  }else{
    //digitalWrite(LED, LOW);
  }
  */

  if(avg > 8.0){
    digitalWrite(LED, HIGH);
    delay(100);
  }else{
    digitalWrite(LED, LOW);
  }

  Serial.print(avg);
  Serial.print("   ");
  Serial.println(sumavg/samples);
  
  
  //Serial.println("AVG: \t"); Serial.print(avg); Serial.print("\t");
  //Serial.print("X: \t"); Serial.print(x); Serial.print("\t");
  //Serial.print("Y: \t"); Serial.print(event.acceleration.y); Serial.print("\t");
  //Serial.print("Z: \t"); Serial.print(event.acceleration.z); Serial.print("\t");
  //Serial.println();
 
  /* Get the orientation of the sensor */
  //uint8_t o = mma.getOrientation(); 
  //delay(100);
}
