
#include <PubSubClient.h>
#include <UIPEthernet.h>

///////////////////  ETHERNET PARAMETERS  /////////////////

//EthernetServer ethserver = EthernetServer(1000);
EthernetClient ethClient;



///////////////////  MQTT PARAMETERS  /////////////////
#define CLIENT_ID "BMS"
const char* broker = "192.168.1.13"; 
String msgString;                                        

void callback(char* topic, byte* payload, unsigned int length) {
    
  // Handle message arrived upon a publish from a MQTT client
  char message_buff[100];
  int i;
  
  for(i=0; i<length; i++) {
    message_buff[i] = payload[i];
  }
  
  message_buff[i] = '\0';
  msgString = String(message_buff);
  
  Serial.println("message arrived: " + msgString);          // Serial output of received callback payload

}

PubSubClient mqttClient(broker, 1883, callback, ethClient); 


///////////////////  TIMING PARAMETERS //////////////////

#define PUBLISH_DELAY   1000
long previousMillis;



///////////////////////////////////////////////////////////

float t = 12.23;

void setup()
{
  Serial.begin(9600);

  uint8_t mac[6] = {0x00,0x01,0x02,0x03,0x04,0x05};
  IPAddress myIP(192,168,1,8);

  Ethernet.begin(mac,myIP);

//  ethserver.begin();

  
}

void loop()
{
  mqttClient.loop();
  
  if (!mqttClient.connected()) 
  { 
    reconnect();
   }
  else
  {
   if(millis() - previousMillis > PUBLISH_DELAY) {
   sendData();
  }
}
}

void reconnect() {
  // Loop until we're reconnected
  while (!mqttClient.connected()) 
  {
    Serial.println("Attempting MQTT connection...");   
    // Connect to the MQTT broker
    if (mqttClient.connect(CLIENT_ID)) 
    {
      Serial.println("connected");
    } 
    else {      
      Serial.print("failed, ");
      // Print to know why the connection failed.
      // See https://pubsubclient.knolleary.net/api.html#state for the failure code explanation.
      Serial.println(mqttClient.state());
    }
  }
} 

  void sendData() {

  char msgBuffer[20];
      
    mqttClient.publish("BMS/TEMP1", dtostrf(t, 6, 2, msgBuffer));
    t = t + 0.1;
   
    previousMillis = millis();

  }
