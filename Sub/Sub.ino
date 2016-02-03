//set up the needed libraries.
#include <ESP8266WiFi.h>
#include <WiFiUDP.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#define motor 16 //define motor as pin 16

//create definitions for min and max pulse lengths for the servo
#define servomin 100
#define servomax 600

//declare the SSID and password
const char *ssid = "Sub";
const char *password = "appleluv";
//set port that UDP communication shoudl occur through.
unsigned int port = 2390;
WiFiUDP Udp;
char packetBuffer[255];
int control[] = {0,0,0,00};

Adafruit_PWMServoDriver driver = Adafruit_PWMServoDriver(0x40);

void setup() {
  delay(1000); 
  Wire.begin(0, 2); //0 is SDA, 2 is SCL
  pinMode(motor, OUTPUT); //set pin 16 as output
  driver.begin();
  driver.setPWMFreq(60);
  Serial.begin(115200);
  Serial.println();
  Serial.println("Boot complete, setting up network ");
  WiFi.softAP(ssid,password);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  Udp.begin(port);
  Udp.flush();
}

double setServoPulse(uint8_t n, int pulse) {
  double pulselength;
  pulselength = 1000000;   // 1,000,000 us per second
  pulselength /= 60;   // 60 Hz  
  pulselength /= 4096;  // 12 bits of resolution
  pulse /= pulselength;
  return pulse;
}

int degToPulse(int deg){
  return map(deg, -90, 90, servomin, servomax);
}

int readPacket(){
  int packetSize = Udp.parsePacket();
  if(packetSize) {
    Serial.println("Recieved Packet");
    int len = Udp.read(packetBuffer, 255);
    Serial.println(len);
      if (len > 0){
        packetBuffer[len] = 0;
       } 
      Serial.println(packetBuffer);
      }
  int message;
  message = packetBuffer[6]-'0';
  return message;
  }
  

int msgToArray(int msg){
  uint8_t front, back, rudder, throttle;
  front = msg / 10000;
  msg = msg - front * 10000;
  back = msg / 1000;
  msg = msg - back * 1000;
  rudder = msg / 100;
  msg = msg - rudder * 100;
  throttle = msg;
  control[0] = front;
  control[1] = back;
  control[2] = rudder;
  control[3] = throttle;
}


//servo mapping: 0= front left, 1 = front right, 2 = back right, 3 = back left, 4 = rudder;
void servoControl(){
  if(control[0] = 1){
    driver.setPWM(0, 0, degToPulse(45));
    driver.setPWM(1, 0, degToPulse(-45));
  }else if(control[1] = 2){
    driver.setPWM(0, 0, degToPulse(-45));
    driver.setPWM(1, 0, degToPulse(45));
  }else{
    driver.setPWM(0, 0, 0);
    driver.setPWM(1, 0, 0);
  }
  if(control[1] = 1){
    driver.setPWM(2, 0, degToPulse(-45));
    driver.setPWM(3, 0, degToPulse(45));
  }else if(control[2]){
    driver.setPWM(2, 0, degToPulse(45));
    driver.setPWM(3, 0, degToPulse(-45));
  }else{
    driver.setPWM(2, 0, 0);
    driver.setPWM(3, 0, 0);
  }
  if(control[2] = 1){
    driver.setPWM(4, 0, degToPulse(45));
  }else if(control[2]){
    driver.setPWM(4, 0, degToPulse(-45));
  }else{
    driver.setPWM(4, 0, 0);
  }
  if(control[3] < 50){
    analogWrite(motor, map(control[3], 49, 0, 0, 1023));
  }else if (control[3] > 50){
    analogWrite(motor, map(control[3], 50, 100, 0, 1023));
  }else if (control[3] == 0){
    analogWrite(control[3], 0);
  }
}
 
void loop() {
  // put your main code here, to run repeatedly:
  msgToArray(readPacket());
  servoControl();
}

