#include <SoftwareSerial.h>
//SoftwareSerial HM10(10,11);
char data;
char inData = "";

#define RED_LED 6
#define BLUE_LED 5
#define GREEN_LED 9

int brightness = 255;
int gBright = 0;
int rBright = 0;
int bBright = 0;

int fadeSpeed = 10;

int black[3]  = { 0, 0, 0 };
int white[3]  = { 100, 100, 100 };
int red[3]    = { 100, 0, 0 };
int green[3]  = { 0, 100, 0 };
int blue[3]   = { 0, 0, 100 };
int yellow[3] = { 40, 95, 0 };
int dimWhite[3] = { 30, 30, 30 };
int purple[3] = { 40, 5, 66 };

int redVal = black[0];
int grnVal = black[1]; 
int bluVal = black[2];

int wait = 10;      // 10ms internal crossFade delay; increase for slower fades
int hold = 0;       // Optional hold when a color is complete, before the next crossFade
int DEBUG = 1;      // DEBUG counter; if set to 1, will write values back via serial
int loopCount = 60; // How often should DEBUG report?
int repeat = 3;     // How many times should we loop before stopping? (0 for no stop)
int j = 0;          // Loop counter for repeat

int prevR = redVal;
int prevG = grnVal;
int prevB = bluVal;

void setup()
{
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);

  Serial.begin(9600);
  Serial.println("Serial started at 9600");
  //HM10.begin(9600);
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
}


//void red()
//{
//  rBright = 0;
//  gBright = 0;
//  bBright = 0;
//  for(int i=0;i<256;i++)
//  {
//    
//    analogWrite(RED_LED, rBright);
//    analogWrite(BLUE_LED, bBright);
//    analogWrite(GREEN_LED, gBright);
//    rBright++;
//    bBright--;
//    gBright--;
//    delay(fadeSpeed);
//  }
//}
//void blue()
//{
//  rBright = 0;
//  gBright = 0;
//  bBright = 0;
//  for(int i=0;i<256;i++)
//  {
//    
//    analogWrite(RED_LED, rBright);
//    analogWrite(BLUE_LED, bBright);
//    analogWrite(GREEN_LED, gBright);
//    rBright--;
//    bBright++;
//    gBright--;
//    delay(fadeSpeed);
//  }
//}
//void green()
//{
//  rBright = 0;
//  gBright = 0;
//  bBright = 0;
//  for(int i=0;i<256;i++)
//  {
//    
//    analogWrite(RED_LED, rBright);
//    analogWrite(BLUE_LED, bBright);
//    analogWrite(GREEN_LED, gBright);
//    rBright--;
//    bBright--;
//    gBright++;
//    delay(fadeSpeed);
//  }
//}
int calculateStep(int prevValue, int endValue) {
  int step = endValue - prevValue; // What's the overall gap?
  if (step) {                      // If its non-zero, 
    step = 1020/step;              //   divide by 1020
  } 
  return step;
}

int calculateVal(int step, int val, int i) {

  if ((step) && i % step == 0) { // If step is non-zero and its time to change a value,
    if (step > 0) {              //   increment the value if step is positive...
      val += 1;           
    } 
    else if (step < 0) {         //   ...or decrement it if step is negative
      val -= 1;
    } 
  }
  // Defensive driving: make sure val stays in the range 0-255
  if (val > 255) {
    val = 255;
  } 
  else if (val < 0) {
    val = 0;
  }
  return val;
}

/* crossFade() converts the percentage colors to a 
*  0-255 range, then loops 1020 times, checking to see if  
*  the value needs to be updated each time, then writing
*  the color values to the correct pins.
*/

void crossFade(int color[3]) {
  // Convert to 0-255
  int R = (color[0] * 255) / 100;
  int G = (color[1] * 255) / 100;
  int B = (color[2] * 255) / 100;

  int stepR = calculateStep(prevR, R);
  int stepG = calculateStep(prevG, G); 
  int stepB = calculateStep(prevB, B);

  for (int i = 0; i <= 1020; i++) {
    redVal = calculateVal(stepR, redVal, i);
    grnVal = calculateVal(stepG, grnVal, i);
    bluVal = calculateVal(stepB, bluVal, i);

    analogWrite(RED_LED, redVal);   // Write current values to LED pins
    analogWrite(GREEN_LED, grnVal);      
    analogWrite(BLUE_LED, bluVal); 

    delay(wait); // Pause for 'wait' milliseconds before resuming the loop

    if (DEBUG) { // If we want serial output, print it at the 
      if (i == 0 or i % loopCount == 0) { // beginning, and every loopCount times
        Serial.print("Loop/RGB: #");
        Serial.print(i);
        Serial.print(" | ");
        Serial.print(redVal);
        Serial.print(" / ");
        Serial.print(grnVal);
        Serial.print(" / ");  
        Serial.println(bluVal); 
      } 
      DEBUG += 1;
    }
  }
  // Update current values for next loop
  prevR = redVal; 
  prevG = grnVal; 
  prevB = bluVal;
  delay(hold); // Pause for optional 'wait' milliseconds before resuming the loop
}


void loop() 
{
  

  //while(Serial.available() > 0)
  //{
  //  data = HM10.read();
  //  inData = data;
  //  Serial.write(data);
    
  //}

  if(Serial.available())
  {
    inData = Serial.read();
    delay(10);
    //HM10.write(Serial.read());
  }

  switch (inData)
  {
    case 'r':
      //red();  
      crossFade(red);    
      break;
    case 'b':
      //blue();
      crossFade(blue);
      break;
    case 'g':
      //green();
      crossFade(green);
      break;   
      case 'y':
      //red();  
      crossFade(yellow);    
      break;
    case 'w':
      //blue();
      crossFade(white);
      break;
    case 'p':
      //green();
      crossFade(purple);
      break;
    case 'o':
      crossFade(black);
      break;      
  }
  inData = '1';
  Serial.flush();
  //int gBright = 0;
  //int rBright = 0;
  //int bBright = 0;
  

}
