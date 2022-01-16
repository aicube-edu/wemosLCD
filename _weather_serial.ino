#include <ESP8266WiFi.h>
 
#ifndef STASSID
#define STASSID "YangJipsa" //와이파이 검색했을때 뜨는 이름
#define STAPSK  "15151515" //패스워드
#endif
 
const char* ssid = STASSID;
const char* password = STAPSK;

//http://www.kma.go.kr/wid/queryDFSRSS.jsp?zone=1159068000
const char* host = "www.kma.go.kr";
const int httpsPort = 80;
 
String url = "/wid/queryDFSRSS.jsp?zone=1159068000";
 
void setup() {
  Serial.begin(9600);
  Serial.println();
  Serial.print("connecting to ");
  Serial.println(ssid);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  
  delay(2000);
  Serial.println("IP address: ");
  
  delay(2000);
  Serial.println(WiFi.localIP());
 
  Serial.print("connecting to ");
  Serial.println(host);
  Serial.print("requesting URL: ");
  Serial.println(url);
}
 
void loop() {
  delay(10000);
  String str[4];
  char* strKey[4] = {"hour","temp","pop","pty"};
  bool check[4] = {false, };
  String strTmp;

  WiFiClient client;
  
  if (!client.connect(host, httpsPort)) 
  {
    Serial.println("connection failed");
    return;
  }
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
 
  unsigned long t = millis(); //생존시간
  while(1){
    if(client.available()) break;
    if(millis() - t > 10000) break;
  }
 
  //Serial.println("request sent");
  //Serial.print("data : ");
  while(client.available())
  {
    String data = client.readStringUntil('\n'); //readline();
    //Serial.println(data);
    delay(1);

    for (int i=0 ; i<4 ; i++)
    {
      strTmp = dataParsing(data, strKey[i], &check[i]);
      if(strTmp!= "e") str[i] = strTmp;
    }
  }
  Serial.print("Time : "); Serial.print(str[0]); 
  Serial.print(" , Temp : "); Serial.print(str[1]);
  Serial.print(" , POP : "); Serial.print(str[2]); 
  Serial.print(" , PTY : "); Serial.println(str[3]);
}

String dataParsing(String DATA, char* str, bool *check)
{ // HOUR, TEMP, POP, PTY
  int s = -1;
  int e = -1;
  
  if (str=="hour"){
    String temp = "<hour>";
    s = DATA.indexOf(temp) + temp.length();
    e = DATA.indexOf("</hour>");
  }
  else if (str=="temp"){
    String temp = "<temp>";
    s = DATA.indexOf(temp) + temp.length();
    e = DATA.indexOf("</temp>");
  }
  else if (str=="pop"){
    String temp = "<pop>";
    s = DATA.indexOf(temp) + temp.length();
    e = DATA.indexOf("</pop>");
  }
  else if (str=="pty"){
    String temp = "<pty>";
    s = DATA.indexOf(temp) + temp.length();
    e = DATA.indexOf("</pty>");
  }

  
  if( (s != -1) && (e != -1) && !(*check) ) 
  {
    (*check) = true;
    /*Serial.println(DATA);
    Serial.print("s:"); Serial.print(s);
    Serial.print(",e:"); Serial.println(e);*/
    return DATA.substring(s,e);
  }
  return "e"; 
}  
