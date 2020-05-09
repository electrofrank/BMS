/*
 * UIPEthernet EchoServer example.
 *
 * UIPEthernet is a TCP/IP stack that can be used with a enc28j60 based
 * Ethernet-shield.
 *
 * UIPEthernet uses the fine uIP stack by Adam Dunkels <adam@sics.se>
 *
 *      -----------------
 *
 * This Hello World example sets up a server at 192.168.1.6 on port 1000.
 * Telnet here to access the service.  The uIP stack will also respond to
 * pings to test if you have successfully established a TCP connection to
 * the Arduino.
 *
 * This example was based upon uIP hello-world by Adam Dunkels <adam@sics.se>
 * Ported to the Arduino IDE by Adam Nielsen <malvineous@shikadi.net>
 * Adaption to Enc28J60 by Norbert Truchsess <norbert.truchsess@t-online.de>
 */
#include <PubSubClient.h>
#include <UIPEthernet.h>

EthernetServer ethserver = EthernetServer(1000);
EthernetClient ethClient;



// MQTT PARAMETERS
const char* broker = "192.168.1.13"; 
String msgString;                                          // Initialize msgString variable

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


void setup()
{
  Serial.begin(9600);

  uint8_t mac[6] = {0x00,0x01,0x02,0x03,0x04,0x05};
  IPAddress myIP(192,168,1,8);

  Ethernet.begin(mac,myIP);

  ethserver.begin();

  
}

void loop()
{
  mqttClient.loop();
  
  if (!mqttClient.connected()) 
  { 
    reconnect();
   }
   
}

void reconnect() {
  // Loop until we're reconnected
  while (!mqttClient.connected()) 
  {
    Serial.println("Attempting MQTT connection...");   
    // Connect to the MQTT broker
    if (mqttClient.connect("bms")) 
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
