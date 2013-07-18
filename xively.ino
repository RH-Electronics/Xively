#include <CountingStream.h>
#include <Xively.h>
#include <XivelyClient.h>
#include <XivelyDatastream.h>
#include <XivelyFeed.h>

#include <SPI.h>
#include <Ethernet.h>
#include <HttpClient.h>
#include <Xively.h>

// MAC address for your Ethernet shield
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

// Your Xively key to let you upload data
char xivelyKey[] = "";

volatile int CNT;         // variable for counting interrupts from dosimeter 
unsigned long lastConnectionTime = 0;                // last time we connected to Xively
const unsigned long connectionInterval = 60000;      // delay between connecting to Xively in milliseconds
//int sensorValue;


// Define the strings for our datastream IDs
char sensorId[] = "CPM";
XivelyDatastream datastreams[] = {
  XivelyDatastream(sensorId, strlen(sensorId), DATASTREAM_FLOAT),
};
// Finally, wrap the datastreams into a feed
XivelyFeed feed(122314, datastreams, 1 /* number of datastreams */);
EthernetClient client;
XivelyClient xivelyclient(client);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  CNT = 0;
  attachInterrupt(0,GetEvent,RISING);  // Interrupt from GM Tube on pin 2

  while (Ethernet.begin(mac) != 1)
  {
    Serial.println("Error getting IP address via DHCP, trying again...");
    delay(15000);
  }
}

void loop() {
  
    if (millis() - lastConnectionTime > connectionInterval) {        //if 60 seconds over then send CPM data to Xively
    int sensorValue = CNT;
    datastreams[0].setFloat(sensorValue);
    int ret = xivelyclient.put(feed, xivelyKey);   // not sure if need this line....??
    sensorValue = 0; 
    lastConnectionTime = millis();
    CNT = 0;

    }
    
}

    void GetEvent(){                         // Get interrupts from Geiger Kit
  CNT++;                                     // Count interrupts
}  
