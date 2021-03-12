/* Sim800 GSM module relay control example.
 
Code by Belabbaci Nazim (https://github.com/NazimBL)
GSM module Ordered from Deadline Store: https://www.facebook.com/thedeadlinestore/

*/

#define LED 13
#define PUMP 4

#include<SoftwareSerial.h>
//sRx---simTX,sTX----simRX
SoftwareSerial sim(2,3);

const String PUMP_NUMBER="0542306807";
const String CONTROL_NUMBER="0796770812";
String simOutput;
bool state=false;

void setup() {
  
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(LED,OUTPUT);
  digitalWrite(LED,state);
  digitalWrite(PUMP,state);
  sim.begin(9600);
  delay(100);
  sim.println("AT+CLIP=1");
  delay(10);
  sim.println("AT+CMGF=1");
  delay(10);
  sim.println("AT+CNMI=1,2,0,0,0");
}

void loop() {
  
  // put your main code here, to run repeatedly:
   if(Serial.available()){
    sim.write(Serial.read());
  }
  if(sim.available()>0){
    simOutput = sim.readStringUntil('\n');
    Serial.println(simOutput);

    if(contains(simOutput,"+CLIP:") && contains(simOutput,CONTROL_NUMBER)){

      //Write Pump & LED state 
      digitalWrite(LED,state);
      digitalWrite(PUMP,state);
      //Hang up
      sim.println("ATH");
      delay(2000);
      //Send confirmation sms
      SMSconfirm(state);
      //toggle state for next command
      state=!state;    
      } 
  }

}

void SMSconfirm(bool tag){
  sim.println("AT+CMGF=1");
  delay(200);
  sim.println("AT+CMGS=\""+CONTROL_NUMBER+"\"\r");
  delay(200);
  if(tag==1){
    sim.println("Turned ON Confirmation");
  }else{
    sim.println("Turned OFF Confirmation");
  }
  delay(100);
  sim.println((char)26);
  delay(200);
}

bool contains(String cmd,String resp){

bool tag=true;
int cmd_length=cmd.length();
int resp_length=resp.length();
char cmd_buff[cmd_length];
char resp_buff[resp_length];
byte i=0;
while(cmd.charAt(i)!='\0'){
 cmd_buff[i]=cmd.charAt(i);
 i++;
}
i=0;
while(resp.charAt(i)!='\0'){
 resp_buff[i]=resp.charAt(i);
 i++;
}
i=0;
int index=cmd.indexOf(resp);
int end_of_me=cmd_length-index;

while(resp.charAt(i)!='\0'){
if(resp.charAt(i)!=cmd.charAt(index)){
  tag=false;
   break;
}  
else {
  i++;
  index++;
}
}
return tag;
}
