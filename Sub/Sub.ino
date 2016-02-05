//set up the needed libraries.
#include <ESP8266WiFi.h>
#include <WiFiUDP.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
String stuff;
int msg;

#define motor 16 //define motor as pin 16

//create definitions for min and max pulse lengths for the servo
#define servomin 0
#define servomax 598

//declare the SSID and password
const char *ssid = "OhPLease";
const char *password = "appleluv";
//set port that UDP communication shoudl occur through.
unsigned int port = 2390;
WiFiUDP Udp;
char packetBuffer[6];
int control[6];

Adafruit_PWMServoDriver driver = Adafruit_PWMServoDriver(0x40);

void setup() {
  delay(1000); 
  Serial.begin(115200);
  Serial.println();
  WiFi.mode(WIFI_AP);
  Serial.println("Boot complete, setting up network ");
  WiFi.softAP(ssid,password);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Wire.begin(12, 13); //0 is SDA, 2 is SCL
  pinMode(motor, OUTPUT); //set pin 16 as output
  driver.begin();
  driver.setPWMFreq(60);
  Serial.println(myIP);
  Udp.begin(port);
  delay(1000);
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
      Serial.println(packetBuffer);
      }
}
  

int msgToArray(){
  uint8_t front, back, rudder, throttle;
  stuff = "";
  for(int k = 0; k<6; k++){
    stuff += packetBuffer[k];
  }
}



void servoControl(){
  if(stuff == "UDLF"){
    driver.setPWM(0, 0, degToPulse(45));
    driver.setPWM(1, 0, degToPulse(-45));
    driver.setPWM(2, 0, degToPulse(-45));
    driver.setPWM(3, 0, degToPulse(45));
    driver.setPWM(4, 0, degToPulse(45));
    //TODO: SET MOTOR
  }else if (stuff == "UDLB"){
    driver.setPWM(0, 0, degToPulse(45));
    driver.setPWM(1, 0, degToPulse(-45));
    driver.setPWM(2, 0, degToPulse(-45));
    driver.setPWM(3, 0, degToPulse(45));
    driver.setPWM(4, 0, degToPulse(45));
    //TODO: SET MOTOR
  }else if (stuff == "UDRF"){
    driver.setPWM(0, 0, degToPulse(45));
    driver.setPWM(1, 0, degToPulse(-45));
    driver.setPWM(2, 0, degToPulse(-45));
    driver.setPWM(3, 0, degToPulse(45));
    driver.setPWM(4, 0, degToPulse(-45));
    //TODO: SET MOTOR
  }else if (stuff == "UDRB"){
    driver.setPWM(0, 0, degToPulse(45));
    driver.setPWM(1, 0, degToPulse(-45));
    driver.setPWM(2, 0, degToPulse(-45));
    driver.setPWM(3, 0, degToPulse(45));
    driver.setPWM(4, 0, degToPulse(-45));
    //TODO: SET MOTOR
  }else if (stuff == "DULF"){
    driver.setPWM(0, 0, degToPulse(-45));
    driver.setPWM(1, 0, degToPulse(45));
    driver.setPWM(2, 0, degToPulse(45));
    driver.setPWM(3, 0, degToPulse(-45));
    driver.setPWM(4, 0, degToPulse(45));
    //TODO: SET MOTOR
  }else if (stuff == "DULB"){
    driver.setPWM(0, 0, degToPulse(-45));
    driver.setPWM(1, 0, degToPulse(45));
    driver.setPWM(2, 0, degToPulse(45));
    driver.setPWM(3, 0, degToPulse(-45));
    driver.setPWM(4, 0, degToPulse(45));
    //TODO: SET MOTOR
  }else if (stuff == "DURF"){
    driver.setPWM(0, 0, degToPulse(-45));
    driver.setPWM(1, 0, degToPulse(45));
    driver.setPWM(2, 0, degToPulse(45));
    driver.setPWM(3, 0, degToPulse(-45));
    driver.setPWM(4, 0, degToPulse(-45));
    //TODO: SET MOTOR
  }else if(stuff = "DURB"){
    driver.setPWM(0, 0, degToPulse(-45));
    driver.setPWM(1, 0, degToPulse(45));
    driver.setPWM(2, 0, degToPulse(45));
    driver.setPWM(3, 0, degToPulse(-45));
    driver.setPWM(4, 0, degToPulse(-45));
    //TODO: SET MOTOR
  }                
delay(100);
}
 
void loop() {
  // put your main code here, to run repeatedly:
  readPacket();
  msgToArray();
  servoControl();
  }

