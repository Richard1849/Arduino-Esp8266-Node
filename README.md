# Arduino-Esp8266-Node-Demo
Arduino板使用Esp8266连接Tcp服务器，并且实现收发数据。

---



## 说明

由于该项目使用的arduino板是uno板，不支持esp8266库的编译，所以采用AT指令的形式连接TCP服务器。

---



## 硬件部分

- esp8266需要先使用USB-TTL烧录固件初始化，并且将波特率设置为9600

![1562489085032](C:\Users\TIMCOO~1\AppData\Local\Temp\1562489085032.png)

- 烧录好固件后，将esp8266与arduino版连接，注意不要接5V的口，会直接烧坏。

  ![1562489295712](C:\Users\TIMCOO~1\AppData\Local\Temp\1562489295712.png)

---

## Arduino代码部分

- 初始化串口部分

  ```c
  #include <SoftwareSerial.h>
  
  SoftwareSerial mySerial(10, 11); // RX, TX
  
  void setup()
  {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
  // wait for serial port to connect. Needed for Leonardo only
  }
  
  Serial.println("Serial Init");
  
  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);
  mySerial.println("mySerial Init");
  }
  
  void loop()
  {
  if (mySerial.available())
  Serial.write(mySerial.read());
  if (Serial.available())
  mySerial.write(Serial.read());
  }
  
  ```

  

- 使用AT指令连接wifi热点
  

  ```C
  // 通过AT指令连接热点
    mySerial.println("AT+CWJAP=\"wifi名称\",\"密码\"");  // 连接热点
    while (!getWifiSerialData("OK")); // 等待连接成功返回OK后，执行下一步
  ```

- 读取串口AT指令反馈数据函数

  

  ```C
  String _comdata_wifi = ""; // 定义读取反馈数据变量
  
  boolean getWifiSerialData(String val) {      //读取串口反馈的数据
    while (mySerial.available() > 0) {
      _comdata_wifi += char(mySerial.read());   //get wifi data
      delay(4);
      _comdata_wifi.trim();
    }
    Serial.println(_comdata_wifi);
    if (_comdata_wifi.endsWith("OK") || _comdata_wifi.endsWith(val)) {
      Serial.println("+++++++++++++++");
      _comdata_wifi = String("");
      return 1;
    } else return 0;
  }
  ```

  

- 连接TCP服务器部分

  ```C
    mySerial.println("AT+CIPSTART=\"TCP\",\"服务器地址\",端口"); // 连接TCP服务器AT指令
    while (!getWifiSerialData("Linked")); // 当返回Linked，表示连接成功
    mySerial.println("AT+CIPMODE=1"); // 开启透传模式，
    while (!getWifiSerialData("OK")); // 返回OK，表示开启成功
    mySerial.println("AT+CIPSEND"); // 发送数据
  ```

---

## TCP服务端主要部分

- 基于node的服务端

  ```js
  var net = require("net"); // 引入net模块
  
  // 创建TCP服务器
  var server = net.createServer(function(socket) {
   console.log('someone connects');
   // 向客户端发送数据
   socket.write('server_data');
   //  监听data事件
   socket.on('data', function(data) {
     ...
   })
  ```

  