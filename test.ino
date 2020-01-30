
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>


#define ledpin 2


const char* ssid     = "502";//无线名称
const char* password = "RD35462959";//无线密码
const char* host = "api.heclouds.com";
const char* APIKEY = "TOxv0h2J8afXYA5vel0FqsIQPgk=";
int32_t deviceId = 578968512;
const char* LEDdata = "led";
//const char* DS_Temp = "wendu";

unsigned long lastCheckInTime = 0; //记录上次报到时间
const unsigned long postingInterval = 40000; // 每隔40秒向服务器报到一次
const int httpPort = 80;


WiFiClient client;


void tcp_test () {
  if (!client.connect(host, httpPort)) {//链接tcp服务器测试
    Serial.println("connection failed");
    return;
  }
}



void checkIn() {
  String url = "/devices/";
  url += String(deviceId);
  url += "/datapoints";           //http://open.iot.10086.cn/doc/art190.html#43
  //  String data = "{\"" + String(DS_Temp) + "\":" + String(val_t) + ",\"" + String(DS_Hum) + "\":" + String(val_h) + ",\"" +  String(DS_Baojing) + "\":" + String(val_t) + "}";
  String data = "{\"datastreams\":[{\"id\":\"test_stream\",\"datapoints\":[{\"value\":28}]}]}";
  Serial.print("data length:");
  Serial.println(String(data.length()));
  String post_data = "POST " + url + " HTTP/1.1\r\n" +
                     "api-key:" + APIKEY + "\r\n" +
                     "Host:" + host + "\r\n" +
                     "Content-Length: " + String(data.length()) + "\r\n" +
                     "Connection: close\r\n\r\n" +
                     data;
  client.print(post_data);
  lastCheckInTime = millis();


}
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(1000);

  WiFi.begin(ssid, password);
}

void loop() {
  // put your main code here, to run repeatedly:
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  if (millis() - lastCheckInTime > postingInterval || lastCheckInTime == 0) {
    tcp_test();
    if (!client.connected()) {
      if (!client.connect(host, httpPort)) {
        Serial.println("connection failed");
        delay(5000);
        return;
      }
    }
    checkIn();
    String status_code = client.readStringUntil('\r');        //读取GET数据，服务器返回的状态码，若成功则返回状态码200
    Serial.println(status_code);
  }





}
