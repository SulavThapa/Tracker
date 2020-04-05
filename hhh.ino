#include <EEPROM.h>
#include <TinyGPS++.h> // library for GPS module
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>

#include <ESP8266HTTPClient.h>

TinyGPSPlus gps;  // The TinyGPS++ object
SoftwareSerial ss(4, 5); // The serial connection to the GPS device
const char* ssid = "Sulav21Silva"; //ssid of your wifi
const char* password = "sulavthapa"; //password of your wifi
const char* serverthink = "api.thingspeak.com";
String apiKey = "611SSKNHLTOSL0FL";
float latitude , longitude;
int  val;
String  lat_str , lng_str ;
int pm;
WiFiClient client;
WiFiServer server(80);

bool res_status = false;

      
void setup()
{
  
  Serial.begin(115200);
  ss.begin(9600);
  
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password); //connecting to wifi
  while (WiFi.status() != WL_CONNECTED)// while wifi not connected
  {
    delay(500);
    Serial.print("."); //print "...."
  }
  Serial.println("");
  Serial.println("WiFi connected");
  server.begin();
  Serial.println("Server started");
  Serial.println(WiFi.localIP());  // Print the IP address
  
}


void loop()
{

//  while(val!=4){
//    Serial.println("CRASHHHHHHHHHHH!!!!!!!!!!!!!!!!!");
//  }
    while (ss.available() > 0) { //while data is available
    
      gps.encode(ss.read()); //read gps data
      
        
        if (gps.location.isValid()) //check whether gps location is valid
        { 
          latitude = gps.location.lat();
          lat_str = (latitude , 6);// latitude location is stored in a string
          longitude = gps.location.lng();
          lng_str = String(longitude , 6); //longitude location is stored in a string
          Serial.println(lat_str);
          Serial.println(lng_str);
          sendData();
        }
        else{
          lat_str=("28.634102");
          lng_str=("77.445709");
        }
        
  
    
  }
 
 WiFiClient client = server.available(); // Check if a client has connected
  if (!client)
  {
    return;
  }
  else{
    Serial.println("Client visit Website !");
  }  
  // Prepare the response
  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n <!DOCTYPE html> <html> <head> <title>GPS DATA</title> <style>";
  s += "a:link {background-color: YELLOW;text-decoration: none;}";
  s += "table, th, td </style> </head> <body> <h1  style=";
  s += "font-size:300%;";
  s += " ALIGN=CENTER> GPS DATA</h1>";
  s += "<p ALIGN=CENTER style=""font-size:150%;""";
  s += "> <b>Location Details</b></p> <table ALIGN=CENTER style=";
  s += "width:50%";
  s += "> <tr> <th>Latitude</th>";
  s += "<td ALIGN=CENTER >";
  s += lat_str;
  s += "</td> </tr> <tr> <th>Longitude</th> <td ALIGN=CENTER >";
  s += lng_str;
  
  s += "</td>  </tr> </table> ";
 
  s += "</body> </html>";

  client.print(s); // all the values are send to the webpage
//  HTTPClient http;
//  http.begin("http://192.168.43.154:5000/gpsinfo/"+(String)lat_str+"/"+(String)lng_str);
//  http.addHeader("Content-Type", "text/plain");
//  http.GET();   //Send the request
//  String payload = http.getString();  //Get the response payload
//  Serial.println(payload);    //Print request response payload
//  http.end();
  
}

void sendData(){
   if (client.connect(serverthink,80))   //   "184.106.153.149" or api.thingspeak.com
     {  
    String postStr = apiKey;
    postStr +="&field1=";
    postStr += String(lat_str);
    postStr +="&field2=";
    postStr += String(lng_str);
 
    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);
 
    Serial.print("Latitude: ");
    Serial.print(lat_str);
    Serial.print("Longitude: ");
    Serial.print(lng_str);
    Serial.println("%. Send to Thingspeak.");

  delay(1000);                     

      }
  
  }
