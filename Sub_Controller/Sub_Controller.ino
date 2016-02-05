#include <ESP8266WiFi.h>;
#include <WiFiUDP.h>;

const char *ssid = "OhPLease";
const char *password = "appleluv";
unsigned int port = 2390;
WiFiUDP Udp;


//Format for transmission. front fins,back fins, rudder, 2 digit throttle)
//for fins: 1 means digitalRead(up), 2 means down
//for rudder, 1 means left, 2 means right
//throttle: 0-49 throttle backwards (0 is highest). 50-99 (99 is highest)

//declare message array that WiFill contain code sent to sub
char message[5];
//crate message integer for sending

//declare pins needed for the controller
int msg;
int up = 0;
int down = 12;
int left = 14;
int right = 5;
int throttle = A0;

//set main sub IP address. 
IPAddress SubIp(192, 168, 4, 1);

int status = WL_IDLE_STATUS;

void setup() {
  // put your setdigitalRead(up) code here, to run once:
  delay(1000);
  //setdigitalRead(up) serial monitor at baudrate of 115200, new line it to clear any crap from bootdigitalRead(up)
  Serial.begin(115200);
  Serial.println();
  WiFi.begin(ssid, password);
  //Connect to WiFifi network created by sub
  while(WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  //setdigitalRead(up) the pins for reading the fin conrtol
  pinMode(digitalRead(up), INPUT_PULLUP);
  pinMode(down, INPUT_PULLUP);
  pinMode(left, INPUT_PULLUP);
  pinMode(right, INPUT_PULLUP);
  //setup UDP layer for communication between devices
  Udp.begin(port);
  //wait one second, just to give things a chance to rectify themselves. 
  delay(1000);
}



//this function sends the data that is placed in syle beam(stuff_to_send)
void beam(char stuff[6]){
  //begin transmission of packet to the sub on the needed port
  Udp.beginPacket(SubIp, port);
  //write the data to the sub
  Udp.write(stuff);
  //end transmission, this actually sends the file!
  Udp.endPacket();  
  for(int i = 0; i <5; i++){
    Serial.print(message[i]);
  }
  Serial.println();
}

//this function reads the values of the pins and stitches them into a message to be sent by the beam() function
char controlRead(){
  String str;
  if(digitalRead(up) && !digitalRead(down) && !digitalRead(left) && digitalRead(right)){
    str = "UDLF";
    str.toCharArray(message, 5);
  }else if(digitalRead(up) && !digitalRead(down) && digitalRead(left) && !digitalRead(right)){
    str = "UDRF";
    str.toCharArray(message, 5);
  }else if (!digitalRead(up) && digitalRead(down) && !digitalRead(left) && digitalRead(right)){
    str = "DULF"; 
    str.toCharArray(message, 5);
  }else if(!digitalRead(up) && digitalRead(down) && digitalRead(left) && !digitalRead(right)){
    str = "DURF";
    str.toCharArray(message, 5);
  }
}


void loop() {
  //put your main code here, to run repeatedly:
  //send the char array created by controlRead() over the beam() function to the sub
  beam(message);
}
