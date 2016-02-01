#include <ESP8266WiFi.h>
#include <WiFiUDP.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#define onems 0.24576
#define twoms 0.49152
#define onepfivems = .36894
#define negfourfive =  0.3072
#define fourfive = .4227


//4us per tick on a 60hz system. 
//pulse start will be 0, end will be pulse length (in miliseconds) divided by # of microseconds per tick. 
//our servos: 1ms = -90, 1.5ms = 0, 2ms = 90.
//number value is postves to teh right. 

const char *ssid = "Sub";
const char *password = "appleluv";
unsigned int port = 1234;
WiFiUDP Udp;
char packetBuffer[255];
int control[] = {0,0,0,00};

Adafruit_PWMServoDriver driver = Adafruit_PWMServoDriver(0x40);

void setup() {
  delay(1000); 
  Wire.begin(0, 2); //0 is SDA, 2 is SCL
  driver.begin();
  driver.setPWMFreq(60);
  yield();
  Serial.begin(115200);
  Serial.println();
  Serial.print("Boot complete, setting up network");
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
  Serial.print(pulselength); Serial.println(" us per period"); 
  pulselength /= 4096;  // 12 bits of resolution
  Serial.print(pulselength); Serial.println(" us per bit"); 
  pulse /= pulselength;
  Serial.println(pulse);
  return pulse;
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

void servoControl(int fronts, int backs, int rudder, int throttle){
  
}
 
void loop() {
  // put your main code here, to run repeatedly:
  msgToArray(readPacket());
  if(control[1] = 1){
    
  }
}

