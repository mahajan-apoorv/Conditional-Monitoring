#include "TimerOne.h"
#include<SoftwareSerial.h>
SoftwareSerial mySerial1(9,8);
int temp_raw_value;
long int sum = 0;
long int sum1 = 0;
int EP=3;
 int i;
int currentPin = A1;
int sensitivity = 65;
int adcValue= 0;
int offsetVoltage = 2500;
float adcVoltage = 0;
float currentValue;
int vibr_Pin =12;
int temp;
float vol_raw_value=0;
long long int avg=0;

unsigned int counter=0;
int a[5]={0,0,0,0,0};

void docount()
{
  counter++;  
} 

void timerIsr()
{
  Timer1.detachInterrupt();  //stop the timer
  int rotation = (counter / 2)*60;  // divide by number of holes in Disc
  a[4] = rotation;
  counter=0;  //  reset counter to zero
  Timer1.attachInterrupt( timerIsr ); // enable the timer
}


void setup() {
  
  Serial.begin(9600);
  mySerial1.begin(9600);
  delay(7000);
  pinMode(vibr_Pin, INPUT);
  pinMode(A1, INPUT);
  Timer1.initialize(1000000); // set timer for 1sec
  attachInterrupt(1, docount, RISING);  // increase counter when speed sensor pin goes High
  Timer1.attachInterrupt( timerIsr ); // enable the timer
   
  
}

void loop() {
  
  long measurement = TP_init();

  a[0] = measurement;                         //vibration

                                                                                          //temp sensor
  for(int i=1;i<=50;i++)
  {
  temp_raw_value = analogRead(A0);
  temp_raw_value = map(temp_raw_value,0,1023,0,5050)+20;
  sum = sum + temp_raw_value;
  }

  temp_raw_value = sum/50;

  a[1] = temp_raw_value/10;
  sum = 0;
 
   for (i =0;i<50;i++) {                                                                                   //current sensor
  adcValue = analogRead(currentPin);
  //Serial.println(adcValue);
  adcVoltage = (adcValue / 1024.0) * 5000;
  //Serial.println(adcVoltage);
  currentValue = ((( adcVoltage - offsetVoltage) / sensitivity)*1000)+150;
  sum1 = sum1 + currentValue;
  }
  
  currentValue = (sum1/50);
  a[2] = currentValue;
  sum1= 0;
  
  for(i=1;i<=50;i++)                                                                   //voltage 
  {
  vol_raw_value = analogRead(A2);
  vol_raw_value = map(vol_raw_value,0,1023,0,5200);
  sum = sum + vol_raw_value;
  }
  a[3] = (sum/50)+100;
  sum = 0;
    
  
 Serial.print("vib "); 
 Serial.println(a[0]);
 
 Serial.print("temp "); 
 Serial.println(a[1]);
 
 Serial.print("current "); 
 Serial.println(a[2]);
 
 Serial.print("voltage "); 
 Serial.println(a[3]);
 
 Serial.print("rpm "); 
 Serial.println(a[4]);

 Serial.println("\n");
 
 for (i=0;i<5;i++)
 { 
  mySerial1.println(a[i]);
  //mySerial1.println('\n');
  }
  
  if (a[0]> 1000)
  {
    digitalWrite(4,HIGH);
    digitalWrite(5,HIGH);
  }
  else
  {
   // digitalWrite(4,LOW);
    digitalWrite(5,LOW);
  }
  if (a[1]>=32)
  {
    digitalWrite(4,HIGH);
    digitalWrite(6,HIGH);
  }
   else
  {
//    digitalWrite(4,LOW);
    digitalWrite(6,LOW);
  }
  if (a[2]> 1000)
  {
    digitalWrite(4,HIGH);
    digitalWrite(7,HIGH);
  }
   else
  {
  //  digitalWrite(4,LOW);
    digitalWrite(7,LOW);
  }
  if (a[3]> 1400)
  {
    digitalWrite(4,HIGH);
    digitalWrite(10,HIGH);
  }
   else
  {
    //digitalWrite(4,LOW);
    digitalWrite(10,LOW);
  }
  if (a[4]> 7000 )
  {
    digitalWrite(4,HIGH);
    digitalWrite(11,HIGH);
  }
   else
  {
    //digitalWrite(4,LOW);
    digitalWrite(11,LOW);
  }
  if(a[0]<1000&&a[1]<32 && a[2]<1000 &&a[3]<1400 && a[4]<7000)
  {
    digitalWrite(4,LOW);
  }

   delay(13000);
}
long TP_init(){
  long measurement=pulseIn (EP, HIGH);  //wait for the pin to get HIGH and returns measurement
 // measurement=measurement/1000;
  return measurement;
}
