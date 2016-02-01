#include <ESP8266WiFi.h>
#include <WiFiUDP.h>


const char *ssid = "Sub";
const char *password = "appleluv";
unsigned int port = 1234;
WiFiUDP Udp;


//Format for transmission. front fins,back fins, rudder, 2 digit throttle)
//for fins: 1 means up, 2 means down
//for rudder, 1 means left, 2 means right
//throttle: 0-49 throttle backwards (0 is highest). 50-99 (99 is highest)
char message[] = "00000";
int msg;
int up = 2;
int down = 0;
int left = 4;
int right = 5;
int throttle;
int buttonStates;

IPAddress SubIp(129, 6, 15, 28);


void setup() {
  // put your setup code here, to run once:
  delay(1000);
  Serial.begin(115200);
  Serial.println();
  WiFi.begin(ssid, password);
  pinMode(up, INPUT);
  pinMode(down, INPUT);
  pinMode(left, INPUT);
  pinMode(right, INPUT);
  delay(1000);
}



void beam(char stuff){
  Udp.beginPacket(SubIp, 1234);
  Udp.write(stuff);
  Udp.endPacket();  
}

char controlRead(){
  buttonStates = 0;
  int UPs = digitalRead(up);
  int DNs = 10 * digitalRead(down);
  int Ls = 100 * digitalRead(left);
  int Rs = 1000 * digitalRead(right);
  int ts = analogRead(throttle);
  buttonStates = UPs + DNs + Ls + Rs;
  switch(buttonStates){
    case(0001): //up button
      msg = 12000;
    break;
    case(0010): //down button
      msg = 21000;
    break;
    case(0100): //left button
      msg = 00100;
    break;
    case(1000): //right button
      msg = 00200;
    break;
    case(0101):
      msg = 12100; //up and left
    break;
    case(1001): //up right
      msg = 12200;
    break;
    case(0110): //down left
      msg = 21100;
    break;
    case(1010): //down right
      msg = 21200;
    break;
    case(0000): //no buttons pressed
      msg = 00000;
    break;
    default:
    break;
  }
  int speed = map(ts, 0, 255, 0, 100);
  msg = msg + speed;
  String str;
  str = String(msg);
  str.toCharArray(message,2);
  return msg;
}

void loop() {
  // put your main code here, to run repeatedly:
  beam(controlRead());
}
