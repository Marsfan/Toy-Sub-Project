#include <ESP8266WiFi.h>
#include <WiFiUDP.h>


const char *ssid = "Sub";
const char *password = "appleluv";
unsigned int port = 2390;
WiFiUDP Udp;


//Format for transmission. front fins,back fins, rudder, 2 digit throttle)
//for fins: 1 means up, 2 means down
//for rudder, 1 means left, 2 means right
//throttle: 0-49 throttle backwards (0 is highest). 50-99 (99 is highest)

//declare message array that will contain code sent to sub
char message[] = "00000";
//crate message integer for sending

//declare pins needed for the controller
int msg;
int up = 13;
int down = 12;
int left = 14;
int right = 5;
int throttle = A0;

//set main sub IP address. 
IPAddress SubIp(192, 168, 4, 1);


void setup() {
  // put your setup code here, to run once:
  delay(1000);
  Serial.begin(115200);
  Serial.println();
  WiFi.begin(ssid, password);
  pinMode(up, INPUT_PULLUP);
  pinMode(down, INPUT_PULLUP);
  pinMode(left, INPUT_PULLUP);
  pinMode(right, INPUT_PULLUP);
  delay(1000);
}



void beam(char stuff){
  Udp.beginPacket(SubIp, 1234);
  Udp.write(stuff);
  Udp.endPacket();  
}

char controlRead(){
  msg = 0;
  buttonStates = 0;
  if(digitalRead(up) && !digitalRead(down)){
    msg = 12000;
  }else if(!digitalRead(up) && digitalRead(down)){
    msg = 21000;
  }else{
    msg = 0;
  }
  if(!digitalRead(left) && digitalRead(right)){
    msg = msg + 100;
  }else if (digitalRead(left) && !digitalRead(right)){
    msg = msg + 200;
  }else{
    msg = msg;
  } 
  int speed = map(ts, 0, 1023, 0, 99);
  msg = msg + speed;
  String str;
  str = String(msg);
  str.toCharArray(message,2);
 // return msg;
  Serial.println(msg);
}

void loop() {
  //put your main code here, to run repeatedly:
  //beam(controlRead());
 controlRead();
}
