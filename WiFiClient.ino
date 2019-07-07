#include <SoftwareSerial.h>
#include <dht.h>         //引用dht11库文件，使得下面可以调用相关参数
#define flame A0 
#define MQ2 A1//指定模拟端口A1
#define DHT11PIN 13       //定义温湿度针脚号
#define buzzer 5
#define LEDG 6
#define LEDR 7
#define FAN 9
#define pinInterrupt 2

dht DHT;               //实例化一个对象
int flame_detected;


// 网络通信
SoftwareSerial mySerial(10, 11); // RX, TX
String _comdata_wifi = "";
String _server_data = "";
int temp = 100;
int wet = 200;
int state = 0;
unsigned long lasttime = 0;
void connect_server () {
  delay(1000);
  mySerial.println("AT+CWJAP=\"iphone\",\"wwwcom123\"");
  while (!getWifiSerialData("OK"));
  mySerial.println("AT+CIPSTART=\"TCP\",\"47.102.102.23\",3001");
  while (!getWifiSerialData("Linked"));
  mySerial.println("AT+CIPMODE=1");
  while (!getWifiSerialData("OK"));
  mySerial.println("AT+CIPSEND");
}
void send_data (int tem, int hum) {
  String Wet = String(hum);
  String Temp = String(tem);
  mySerial.println("{\"temp\":"+Temp+",\"wet\":"+Wet+"}");
  delay(1000);
}
boolean getServerData() {                              //读取服务器反馈的数据
  while (mySerial.available() > 0) {
    _server_data += char(mySerial.read());   //get server data
    delay(4);
    _server_data.trim();
    // Serial.println(_server_data);
  }
  Serial.println(_server_data);
  if (_server_data.indexOf("closeLight")+1) {
    Serial.println("》》》》》》");
    // Serial.println(_server_data);
    state = 0;
    digitalWrite(FAN,LOW);
    _server_data = String("");
    return 1;
  } else if (_server_data.indexOf("openLight")+1) {
    Serial.println("《《《《《");
    // Serial.println(_server_data);
    state = 0;
    _server_data = String("");
    digitalWrite(FAN,HIGH);
    return 1;
  } else return 0;
}
boolean getWifiSerialData(String val) {      //读取wifi反馈的数据
  while (mySerial.available() > 0) {
    _comdata_wifi += char(mySerial.read());   //get wifi data
    delay(4);
    _comdata_wifi.trim();
    // Serial.println(_comdata_wifi);
  }
  Serial.println(_comdata_wifi);
  if (_comdata_wifi.endsWith("OK") || _comdata_wifi.endsWith(val)) {
    Serial.println("+++++++++++++++");
    _comdata_wifi = String("");
    return 1;
  } else return 0;
}

boolean getWifiData(String val) {      //读取wifi反馈的数据
  while (mySerial.available() > 0) {
    _comdata_wifi += char(mySerial.read());   //get wifi data
    delay(4);
    _comdata_wifi.trim();
    //Serial.println(_comdata_wifi);
  }
  Serial.println(_comdata_wifi);

  if (_comdata_wifi.endsWith("OK") || _comdata_wifi.endsWith(val) || _comdata_wifi.endsWith("ERROR") || _comdata_wifi.endsWith("CON") || _comdata_wifi.endsWith("CLOSED")) {
    Serial.println("+++++++++++++++");
    _comdata_wifi = String("");
    return 1;
  } else return 0;
}

void reconnect_server () {  // 重新连接服务器
  mySerial.print("+++");//退出发送模式
  mySerial.println("AT+CIPSTART=\"TCP\",\"47.102.102.23\",3001");
  while (!getWifiData("CONNECTED"));
  mySerial.println("AT+CIPMODE=1");
  while (!getWifiSerialData("OK"));
  mySerial.println("AT+CIPSEND");
  while (!getWifiSerialData("OK>"));
}

void button()
{
      digitalWrite(buzzer,LOW); 
      digitalWrite(LEDR,LOW);
      digitalWrite(LEDG,LOW); 
      digitalWrite(FAN,LOW);
      Serial.println("Key Up"); 
      delay(1000);
      return;
      
}
void setup()
{
  // 硬件部分
  pinMode(MQ2,INPUT);
  pinMode(flame,INPUT);
  pinMode(pinInterrupt,INPUT);
  pinMode(DHT11PIN,INPUT);    //定义输出口
  pinMode(buzzer,OUTPUT);      //设置数字IO脚模式，OUTPUT为输出
  pinMode(LEDG,OUTPUT);
  pinMode(LEDR,OUTPUT);
  pinMode(FAN,OUTPUT);
  attachInterrupt(digitalPinToInterrupt(pinInterrupt),button,CHANGE);
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  Serial.println("Goodnight moon!");
  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);
  mySerial.print("+++");//退出发送模式
  connect_server(); // 连接服务器
}
void loop() // run over and over
{
  // 硬件部分
  
  int chk = DHT.read11(DHT11PIN);     //将读取到的值赋给chk
  int tem = (float)DHT.temperature;   //将温度值赋值给te
  int hum = (float)DHT.humidity;      //将湿度值赋给hum
  int val = analogRead(MQ2);                //读取A1口的电压值并赋值到val
  flame_detected = analogRead(flame); //读取火焰传感器的模拟值
  if(val>200)
  {
   digitalWrite(LEDG,HIGH);
   digitalWrite(buzzer,HIGH);
  // digitalWrite(FAN,HIGH);
   delay(1000);
   digitalWrite(buzzer,LOW);
   digitalWrite(LEDG,LOW);
  }
  if(flame_detected<900)
  {
   digitalWrite(LEDR,HIGH);
   digitalWrite(buzzer,HIGH);
   delay(1000);
   digitalWrite(buzzer,LOW);
   digitalWrite(LEDR,LOW);
  }
  
  Serial.print("val=");
  Serial.print(val);//串口发送val值
  Serial.print("\t");
  Serial.print("flame="); 
  Serial.print(flame_detected); 
  Serial.print("\t");
  Serial.print("wd=");
  Serial.print(tem);                //打印温度结果 
  Serial.print("\t");                 //打印湿度结果 
  Serial.print("sd=");
  Serial.print(hum);
  Serial.print("\n");
  delay(500);

  
  // 网络部分
  getServerData();  // 监听服务器有无发出指令
  send_data(tem, hum);
    
  if (mySerial.available())
    Serial.write(mySerial.read());
  if (Serial.available())
    mySerial.write(Serial.read());
}
