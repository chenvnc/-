#include <SimpleEspNowConnection.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

Adafruit_MPU6050 mpu;
int fsrPin = 0;
SimpleEspNowConnection simpleEspConnection(SimpleEspNowRole::SERVER);

String clientAddress;

typedef struct struct_message{
  char type;
  int x;
  int y;
  int fsreading;
}struct_message;

void setup() {

  Serial.begin(115200);

   if(!mpu.begin()){
     Serial.println("Failed to find MPU6050 chip");
     while(1){
       delay(10);
     }
   }
  Serial.println("MPU6050 Found!");
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  Serial.print("Accelerometer range set to: ");
  switch (mpu.getAccelerometerRange()) {
    case MPU6050_RANGE_2_G:
      Serial.println("+-2G");
      break;
    case MPU6050_RANGE_4_G:
      Serial.println("+-4G");
      break;
    case MPU6050_RANGE_8_G:
      Serial.println("+-8G");
      break;
    case MPU6050_RANGE_16_G:
      Serial.println("+-16G");
      break;
  }
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  Serial.print("Gyro range set to: ");
  switch (mpu.getGyroRange()) {
    case MPU6050_RANGE_250_DEG:
      Serial.println("+- 250 deg/s");
      break;
    case MPU6050_RANGE_500_DEG:
      Serial.println("+- 500 deg/s");
      break;
    case MPU6050_RANGE_1000_DEG:
      Serial.println("+- 1000 deg/s");
      break;
    case MPU6050_RANGE_2000_DEG:
      Serial.println("+- 2000 deg/s");
      break;
  }

  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  Serial.print("Filter bandwidth set to: ");
  switch (mpu.getFilterBandwidth()) {
    case MPU6050_BAND_260_HZ:
      Serial.println("260 Hz");
      break;
    case MPU6050_BAND_184_HZ:
      Serial.println("184 Hz");
      break;
    case MPU6050_BAND_94_HZ:
      Serial.println("94 Hz");
      break;
    case MPU6050_BAND_44_HZ:
      Serial.println("44 Hz");
      break;
    case MPU6050_BAND_21_HZ:
      Serial.println("21 Hz");
      break;
    case MPU6050_BAND_10_HZ:
      Serial.println("10 Hz");
      break;
    case MPU6050_BAND_5_HZ:
      Serial.println("5 Hz");
      break;
  }
  clientAddress="ECFABCA5F68E";
  simpleEspConnection.begin();
  simpleEspConnection.onMessage(&OnMessage);
  simpleEspConnection.onPaired(&OnPaired);
  simpleEspConnection.onSendError(&OnSendError);
  simpleEspConnection.onConnected(&OnConnected);

  Serial.println(WiFi.macAddress());
}

void loop() {
  simpleEspConnection.loop();
  sendStructMessage();
  delay(100);
}

bool sendStructMessage(){
  struct_message myData;
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  myData.type='#';
  myData.x=a.acceleration.x*10;
  myData.y=a.acceleration.y*10;
  myData.fsreading = analogRead(fsrPin);
  return(simpleEspConnection.sendMessage((uint8_t *)&myData,sizeof(myData),clientAddress));
}
void OnSendError(uint8_t* ad){
  Serial.println("SENDING TO '"+simpleEspConnection.macToStr(ad)+"' WAS NOT POSSIBLE!");
}
void OnMessage(uint8_t* ad,const uint8_t* message,size_t len){
  if((char)message[0]=='#'){
    struct_message myData;
    memcpy(&myData,message,len);
    Serial.printf("Structure:\n");
    Serial.printf("x:%d\n",myData.x);
    Serial.printf("y:%d\n",myData.y);
    Serial.printf("fsreading:%d\n",myData.fsreading);
  }
  else Serial.printf("MESSAGE:[%d]%s from %s\n",len,(char *)message,simpleEspConnection.macToStr(ad).c_str());
}
void OnPaired(uint8_t *ga,String ad){
  Serial.println("EspNowConnection : Client '"+ad+"' paired! ");
  simpleEspConnection.endPairing();
  
  clientAddress = ad;
}
void OnConnected(uint8_t *ga, String ad)
{
  Serial.println("EspNowConnection : Client '"+ad+"' connected! ");

  simpleEspConnection.sendMessage((uint8_t *)"Message at OnConnected from Server", 34, ad);
}
