#include <WiFi.h>

// wifi details
char ssid[] = "iot";
char password[] = "overtechnicality7petrophilous";

WiFiClient client;
// server details
char server[] = "3.250.38.184";
int port = 8000;

void connectToWiFi() {
  Serial.print("Connecting to network: ");
  Serial.print(ssid);
  Serial.flush();

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    Serial.flush();
    delay(300);
  }

  Serial.println();
  Serial.println("Connected");
  Serial.print("Obtaining IP address");
  Serial.flush();
  while (WiFi.localIP() == INADDR_NONE) {
    Serial.print(".");
    Serial.flush();
    delay(300);
  }
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

//connect to server
bool connectToServer() {
  Serial.print("Connecting to server: ");
  Serial.print(server);
  Serial.print(" on port ");
  Serial.print(port);
  
  if (!client.connect(server, port)) {
    Serial.println("error connecting to the server");
    return false;
  }

  Serial.println(" Connected to server :)");
  return true;
}
  void setup() {
    Serial.begin(9600);
    delay(2000);
    connectToWiFi();
    connectToServer();

    //close connection
    client.stop();
    Serial.println("Disconnected");
  }

void loop() {
  
}