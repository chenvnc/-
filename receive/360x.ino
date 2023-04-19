#include <SimpleEspNowConnection.h>
#include<Servo.h>
#include <Wire.h>
Servo base, left, right, head;

const int movespace=5;

SimpleEspNowConnection simpleEspConnection(SimpleEspNowRole::CLIENT);
String serverAddress;

typedef struct struct_message{
  char type;
  int x;
  int y;
  int fsreading;
}struct_message;

const int baseMin = 0;
const int baseMax = 180;
const int leftMin = 90;
const int leftMax = 180;
const int rightMin = 40;
const int rightMax = 160;
const int headMin = 35;
const int headMax = 135;

int DSD = 0;

void setup() {
  base.attach(2, 500, 2500);
  left.attach(14, 500, 2500);
  right.attach(12, 500, 2500);
  head.attach(13, 500, 2500);
  base.write(90);
  left.write(180);
  right.write(40);
  head.write(35);
  Serial.begin(115200);

  simpleEspConnection.begin();
  serverAddress="CC50E323893C";
  simpleEspConnection.setServerMac(serverAddress);
  simpleEspConnection.onMessage(&OnMessage);
  Serial.println(WiFi.macAddress());
}
void loop() {
  simpleEspConnection.loop();
}

void Noneline(int x,int y,int fsreading){
  int Pos;
    if(y>50){
    Serial.println("Recieved Command:Base Turn Left");
    Pos=base.read()+movespace;
    servofast('b',Pos,DSD);
  }
  if(y<-50){
    Serial.println("Recieved Command:Base Turn right");
    Pos=base.read()-movespace;
    servofast('b',Pos,DSD);
  }
  if(x>70){
    Serial.println("Recieved Command:left front");
    Pos=left.read()-movespace;
    servofast('l',Pos,DSD);
  }
  else if(x>20){
    Serial.println("Recieved Command:Right front");
    Pos=right.read()+movespace;
    servofast('r',Pos,DSD);
  }
  if(x<-70){
    Serial.println("Recieved Command:Right back");
    Pos=right.read()-movespace;
    servofast('r',Pos,DSD);
  }
  else if(x<-30){
    Serial.println("Recieved Command:left back");
    Pos=left.read()+movespace;
    servofast('l',Pos,DSD);
  }
  if(fsreading<512){
    Serial.println("Recieved Command:head close");
    Pos=head.read()+movespace;
    servofast('h',Pos,DSD);
  }
  else{
    Serial.println("Recieved Command:head open");
    Pos=head.read()-movespace;
    servofast('h',Pos,DSD);
  }
}

void servofast(char serialCmd, int serialData, int dsd) {
  Servo newservo;

  Serial.println();
  Serial.print("Command servo :");
  Serial.print(serialCmd);
  Serial.print(" to ");
  Serial.print(serialData);
  Serial.println();
  int fromPos;
  switch (serialCmd) {
    case 'b':
      if (serialData <= baseMax && serialData >= baseMin) {
        newservo = base;
        fromPos = base.read();
        break;
      }
      else {
        Serial.println("+warning:Base Servo Value Out Of Limit!");
        return;
      }
    case 'l':
      if (serialData <= leftMax && serialData >= leftMin) {
        newservo = left;
        fromPos = left.read();
        break;
      }
      else {
        Serial.println("+warning:Left Servo Value Out Of Limit!");
        return;
      }
    case 'r':
      if (serialData <= rightMax && serialData >= rightMin) {
        newservo = right;
        fromPos = right.read();
        break;
      }
      else {
        Serial.println("+warning:Right Servo Value Out Of Limit!");
        return;
      }
    case 'h':
      if (serialData <= headMax && serialData >= headMin) {
        newservo = head;
        fromPos = head.read();
        break;
      }
      else {
        Serial.println("+warning:Head Servo Value Out Of Limit!");
        return;
      }
  }

  if (fromPos <= serialData) {
    for (int i = fromPos; i <= serialData; i++) {
      newservo.write(i);
      delay(dsd);
    }
  }
  else {
    for (int i = fromPos; i >= serialData; i--) {
      newservo.write(i);
      delay(dsd);
    }
  }
  switch(serialCmd){
    case 'b':base=newservo;break;
    case 'l':left=newservo;break;
    case 'r':right=newservo;break;
    case 'h':head=newservo;break;
  }
}
void OnMessage(uint8_t* ad,const uint8_t* message,size_t len){
  if((char)message[0]=='#'){
    struct_message myData;
    memcpy(&myData,message,len);
    Noneline(myData.x,myData.y,myData.fsreading);
  }
  else Serial.printf("MESSAGE:[%d]%s from %s\n",len,(char *)message,simpleEspConnection.macToStr(ad).c_str());
}
//void armDataCmd(char serialCmd,int serialData) {
//  if (serialCmd == 'b' || serialCmd == 'l' || serialCmd == 'r' || serialCmd == 'h') {
//    servofast(serialCmd, serialData, DSD);
//  }
//  else {
//    switch (serialCmd) {
//      case 'o': reportStatus(); break;
//      case 'i': initialServo(); break;
//      default: Serial.println("None point!"); break;
//    }
//  }
//}
//void armJoyCmd(char serialCmd){
//  int Pos;
//  switch(serialCmd){
//    case 'a':
//      Serial.println("Recieved Command:Base Turn Left");
//      Pos=base.read()+movespace;
//      servofast('b',Pos,DSD);
//      break;
//     case 'd':
//      Serial.println("Recieved Command:Base Turn Right");
//      Pos=base.read()-movespace;
//      servofast('b',Pos,DSD);
//      break;
//     case 'w':
//      Serial.println("Recieved Command:Right up");
//      Pos=right.read()+movespace;
//      servofast('r',Pos,DSD);
//      break;
//     case 's':
//      Serial.println("Recieved Command:Right down");
//      Pos=right.read()-movespace;
//      servofast('r',Pos,DSD);
//      break;
//     case 'i':
//      Serial.println("Recieved Command:left up");
//      Pos=left.read()-movespace;
//      servofast('l',Pos,DSD);
//      break;
//     case 'k':
//      Serial.println("Recieved Command:left down");
//      Pos=left.read()+movespace;
//      servofast('l',Pos,DSD);
//      break;
//     case 'j':
//      Serial.println("Recieved Command:head open");
//      Pos=head.read()-movespace;
//      servofast('h',Pos,DSD);
//      break;
//     case 'l':
//      Serial.println("Recieved Command:head close");
//      Pos=head.read()+movespace;
//      servofast('h',Pos,DSD);
//      break;
//     case 'o':
//      reportStatus();
//      break;
//     case 'p':
//      initialServo();
//      break;
//     default:
//      Serial.println("Unknown Command");
//      return;
//  }
//}
//void initialServo() {
//  int iniData[4][2] = {
//    {'b', 90},
//    {'l', 180},
//    {'r', 40},
//    {'h', 135}
//  };
//  for (int i = 0; i < 4; i++) {
//    servofast(iniData[i][0], iniData[i][1], DSD);
//  }
//}
//

//
//void reportStatus() {
//  Serial.println("");
//  Serial.println("");
//  Serial.println("+++STATUS REPORT CURREANTLY+++");
//  Serial.print("base :"); Serial.println(base.read());
//  Serial.print("left ："); Serial.println(left.read());
//  Serial.print("right :"); Serial.println(right.read());
//  Serial.print("head :"); Serial.println(head.read());
//  Serial.println("+++++++++++++++++++++++++++++++");
//  Serial.println();
//}
