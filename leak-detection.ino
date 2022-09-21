#include <LiquidCrystal.h>


float vol = 0.0,l_minute;
float vol2 = 0.0,l_minute2;
unsigned char flowsensor = 3; // Sensor Input
unsigned char flowsensor2 = 2;
unsigned long currentTime;
unsigned long cloopTime;
unsigned long flow_frequency;
unsigned long flow_frequency2;
int motor_pin=9;
float var=0;

LiquidCrystal lcd(12, 11, 6, 5, 4, 10);

void flow () // Interrupt function to increment flow
{
   flow_frequency++;
}
void flow2(){
  flow_frequency2++;
}
void setup()
{
   Serial.begin(9600);
   pinMode(flowsensor, INPUT);
    pinMode(flowsensor2, INPUT);
   digitalWrite(flowsensor, HIGH); // Optional Internal Pull-Up
   digitalWrite(flowsensor2, HIGH); // Optional Internal Pull-Up
   attachInterrupt(digitalPinToInterrupt(flowsensor), flow, RISING); // Setup Interrupt
   attachInterrupt(digitalPinToInterrupt(flowsensor2), flow2, RISING); // Setup Interrupt
   pinMode(motor_pin,OUTPUT);
   lcd.begin(16, 2);
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print("Water FlowMeter");
   lcd.setCursor(0,1);
   lcd.print(flow_frequency2);
   delay(500);
   currentTime = millis();
   cloopTime = currentTime;
}

void loop ()
{
   currentTime = millis();
   // Every second, calculate and print litres/hour
   if(currentTime >= (cloopTime + 1000))
   {
    cloopTime = currentTime; // Updates cloopTime
    if(flow_frequency != 0 || flow_frequency2 != 0)
    {
      
       l_minute = (flow_frequency / 7.5); // (Pulse frequency x 60 min) / 7.5Q = flowrate in L/hour
       l_minute2 = (flow_frequency2 / 7.5); // (Pulse frequency x 60 min) / 7.5Q = flowrate in L/hour
      
       
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Rate: ");
      lcd.print(l_minute);
      lcd.print(" L/M");
      lcd.setCursor(0,1);
      lcd.print("Rate2: ");
      lcd.print(l_minute2);
      lcd.print(" L/M");
      delay(500);
      l_minute = l_minute/60;
      l_minute2 = l_minute2/60;
      if((0.9*l_minute)>l_minute2){
         lcd.clear();
         lcd.setCursor(0,0);
         lcd.print("LEAK");
         lcd.setCursor(0,1);
         lcd.print("FlOW STOPPING..");
         delay(500);
         lcd.clear();
         lcd.setCursor(0,0);
         lcd.print("RATE 1: 0 L/M");
         digitalWrite(motor_pin,HIGH);
          exit(0);
        
      }
      
      flow_frequency = 0; // Reset Counter
      flow_frequency2 = 0; // Reset Counter
      Serial.print(l_minute, DEC); // Print litres/hour
      Serial.println(" L/Sec");
      Serial.print(l_minute2, DEC); // Print litres/hour
      Serial.println(" L/Sec");
    }
    else {
      Serial.println(" flow rate = 0 ");
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Rate: ");
      lcd.print( flow_frequency );
      lcd.print(" L/M");
      lcd.setCursor(0,1);
      lcd.print("Vol:");
      lcd.print(vol);
      lcd.print(" L");
    }
   }
}
