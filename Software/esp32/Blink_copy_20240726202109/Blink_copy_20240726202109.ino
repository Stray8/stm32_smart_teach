/*
  Blink

  Turns an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO
  it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN is set to
  the correct LED pin independent of which board is used.
  If you want to know what pin the on-board LED is connected to on your Arduino
  model, check the Technical Specs of your board at:
  https://www.arduino.cc/en/Main/Products

  modified 8 May 2014
  by Scott Fitzgerald
  modified 2 Sep 2016
  by Arturo Guadalupi
  modified 8 Sep 2016
  by Colby Newman

  This example code is in the public domain.

  https://www.arduino.cc/en/Tutorial/BuiltInExamples/Blink
*/
#include <HTTPClient.h>
#include <WiFi.h>
#include <ArduinoJson.h>


//uart
HardwareSerial mySerial1(1);

//wifi 修改WiFi账号和密码
const char* ssid = "iPhone";
const char* passport = "88888888";
// const char* ntpServer = "pool.ntp.org"; //网络时间服务器
// const long gmtOffset_sec = 8 * 3600;
// const int daylightOffset_sec = 0;

//weather
String url = "https://restapi.amap.com/v3/weather/weatherInfo";
String adcode = "110101";//330100
String key = "2a439b91c01b63f297aa5d2234dff433";
String extensions = "base";
String output     = "JSON";
/* 需要的响应数据 */
String province;
String city;
String weather;
String temperature;
String t;


// void printLocalTime()
// {
//   struct tm timeinfo;
//   if(!getLocalTime(&timeinfo)){
//     return;
//   }
//   Serial.print(&timeinfo, "%F");
//   mySerial1.print(&timeinfo, "%F");
//   mySerial1.print("-");

// }

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(2, OUTPUT);
  Serial.begin(115200);
  mySerial1.begin(115200,SERIAL_8N1,5,18);
  WiFi.begin(ssid, passport);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi Connected");  
  digitalWrite(2, HIGH);  // turn the LED on (HIGH is the voltage level)

  // configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  // printLocalTime();

  /* 创建 HTTPClient 对象 */
  HTTPClient http;
  /* 指定要发送请求的URL */
  http.begin(url+"?city="+adcode+"&key="+key+"&extensions="+extensions+"&output="+output);
  /* GET请求，函数返回状态码 */
  int http_code=http.GET();
  /* 获取响应数据的字符串格式 */
  String response=http.getString();
  /* 关闭连接 */
  http.end();
 
  /* 创建 DynamicJsonDocument 对象 */
  DynamicJsonDocument doc(1024);
  /* 解析Json 数据 */
  deserializeJson(doc,response);
  // 用串口打印数据会发现lives是一个数组，[0]是数组下标
  province=doc["lives"][0]["province"].as<String>();
  city=doc["lives"][0]["city"].as<String>();
  weather=doc["lives"][0]["weather"].as<String>();
  temperature=doc["lives"][0]["temperature"].as<String>();
  t=doc["lives"][0]["reporttime"].as<String>();
  if(weather == "多云")
    weather = "yun";
  else if(weather == "雨")
    weather = "rainy";
  else if(weather == "阴")
    weather = "yin";
  else if(weather == "晴")
    weather = "qin";
  Serial.println(t+ ","+weather);
  
  mySerial1.println(t+ ","+weather+".");
}


// the loop function runs over and over again forever
void loop() {
  delay(1000);                      // wait for a second
}
