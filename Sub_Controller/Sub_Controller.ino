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
char message[6];
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
  // put your setup code here, to run once:
  delay(1000);
  //setup serial monitor at baudrate of 115200, new line it to clear any crap from bootup
  Serial.begin(115200);
  Serial.println();
  //Connect to wifi network created by sub
  WiFi.begin(ssid, password);
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
  }
  //setup the pins for reading the fin conrtol
  pinMode(up, INPUT_PULLUP);
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
/*  for(int i = 0; i <5; i++){
    Serial.print(message[i]);
  }
  Serial.println();*/
}

//this function reads the values of the pins and stitches them into a message to be sent by the beam() function
char controlRead(){
  //read the analog value from the throttle and store it in variable ts
  int ts = analogRead(throttle);
  //case statements to check when buttons are pushed.
  if(digitalRead(up) && !digitalRead(down)){ //if up button is pressed
    msg = 12000;
  }else if(!digitalRead(up) && digitalRead(down)){ //if down button is pressed
    msg = 21000;
  }else{  //if neither buttons, or both are pressed
    msg = 0;
  }
  if(!digitalRead(left) && digitalRead(right)){ //if left is pressed
    msg = msg + 100;
  }else if (digitalRead(left) && !digitalRead(right)){ //if right is pressed
    msg = msg + 200;
  }else{
    msg = msg; //if neither or both are pressed. 
  } 
  //remap the analog joystick value to a 2 digit value
  int speed = map(ts, 0, 1023, 0, 99);
  //add the analog value to the message generate preiously
  msg = msg + speed;
  //create a variable called string
  String str;
  //convert msg to a string
  str = String(msg);
  //convert the string to the char array needed to transmit the data over udp
  str.toCharArray(message,5);
}


void loop() {
  //put your main code here, to run repeatedly:
  //send the char array created by controlRead() over the beam() function to the sub
  beam(message);
}
