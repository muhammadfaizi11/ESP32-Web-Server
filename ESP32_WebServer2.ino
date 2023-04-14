#include <WiFi.h>
#include <WebServer.h>

// Replace with your network credentials
const char* ssid = "Remote-Crane-Gd4-No2";
const char* password = "craneGd4-SpindoU6";

unsigned long currentTime;
int stateAtas1 = 0;
String data;

int relay_Atas1 = 32;
int relay_Atas2 = 33;
int relay_Bawah1 = 34;
int relay_Bawah2 = 35;

WebServer server(80);

// Set your Static IP address
IPAddress local_IP(192, 168, 137, 100);
// Set your Gateway IP address
IPAddress gateway(192, 168, 137, 65);

IPAddress subnet(255, 255, 255, 0);

void handleRoot(){
  server.send(200, "text/plain", "Ready");
}

void handleGet(){
  if(server.hasArg("data")){
    data = server.arg("data");
    Serial.println("Data: " + data);
    if(data == "button_atas_speed1_on"){
      stateAtas1 = 1;
    }
    else if(data == "button_atas_speed1_off"){
      stateAtas1 = 0;
    }
  }
  server.send(200, "text/plain", "Data Received");
}

void handlePost(){
  server.send(200, "text/plain", "Processing Data");
}

void handleUpload(){
  HTTPUpload& upload = server.upload();
  if(upload.status == UPLOAD_FILE_START){
    Serial.println("Receiving data: ");
  }
  else if(upload.status == UPLOAD_FILE_WRITE){
    Serial.write(upload.buf, upload.currentSize);
  }
  else if(upload.status == UPLOAD_FILE_END){
    server.send(200, "teext/plain", "Data: ");
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // Configures static IP address
  if (!WiFi.config(local_IP, gateway, subnet)) {
  Serial.println("STA Failed to configure");
  }
  
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.on("/", handleRoot);
  server.on("/get", HTTP_GET, handleGet);
  server.on("/post", HTTP_POST, handlePost, handleUpload);
  server.begin();
  Serial.print("Ready");
}

void loop() {
  // put your main code here, to run repeatedly:
  currentTime = millis();
  if(stateAtas1 == 1){
    Serial.println("Arah Atas Speed 1 Nyala");
    digitalWrite(relay_Atas1, LOW);
  }
  else {
    Serial.println("Remote Tidak di Tekan");
    digitalWrite(relay_Atas1, HIGH);
  }
  server.handleClient();
}
