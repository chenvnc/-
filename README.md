### ***简单无线控制机械臂***
***
> *​项目实现用的环境为Arduino*

> > Adafruit_MPU6050,SimpleEspNowConnection和Wire文件夹为Arduino库文件，直接导入就可

> > receive​与send文件夹里面为项目代码，后缀为ino,分别是​发送端和接收端
***
*使用的设备有：*  
* ESP8266 D1 R2无线WiFi开发板两个  
* 机械臂一个  
* MPU6050六轴传感器一个  
* 压力传感器一个  
* 9g180舵机四个 
***
> 两板数据传输使用espnow协议

>MPU6050六轴传感器只是调用库简单的运用
