#include "NodeCom.h"
#include <Arduino.h>

uint8_t NODE_ADDRESS = 1; // address of this node


NodeCom manager(1, 14, false, NODE_ADDRESS);
void setup()
{
    Serial.begin(9600);
    delay(3000);
    Serial.print(F("initializing node "));
    Serial.print(NODE_ADDRESS);
    if (!manager.init())
        {Serial.println(" init failed");} 
    else
        {Serial.println(" done");}
    manager.setIsaRouter(true);
    manager.setTxPower(18);

    Serial.println("RF95 ready");
}

uint8_t buf[25];
uint8_t res;
uint8_t data[]="hello to you";


void test_main_node(){
    uint8_t len = sizeof(buf);
    uint8_t from = 1;

    // static unsigned int count= 0;

    // if(count >= 1500000)
    // {
        res = manager.sendtoWait(data, sizeof(data), 1);
        if ( res != RH_ROUTER_ERROR_NONE)
        {
            Serial.print("sendtoWait failed:");
            Serial.println(res);

        }
        else if ( res == RH_ROUTER_ERROR_NONE)
        {
            
        }
    //     count = 0;
    // }
    

    //delay(2000);

    std::string res_message;

    if (manager.recvfromAckTimeout(buf, &len, 5000, &from))
    {
        Serial.print("message from node n.");
        Serial.print(from);
        Serial.print(": ");
        Serial.print((char*)buf);
        Serial.println(" rssi: ");
        //Serial.println(rf95.lastRssi()); 
    }
    //else Serial.println(" No reply ");

    // count++;

}

void test_middle_node(){
    uint8_t len = sizeof(buf);
    uint8_t from;
    std::string message = "I am only a middle node";
    if (manager.recvfromAck(buf, &len, &from))
    {
        Serial.print("message from node n.");
        Serial.print(from);
        Serial.print(": ");
        Serial.print((char*)buf);
        Serial.println(" rssi: ");
        //Serial.println(rf95.lastRssi()); 

        res = manager.sendtoWait(&message, from);
        if ( res != RH_ROUTER_ERROR_NONE)
        {
        Serial.print("sendtoWait failed:");
        Serial.println(res);
        }
    }
    else if(from == 1 || from == 3)Serial.println(from);
}

void test_sensor_node(){
    uint8_t len = sizeof(buf);
    uint8_t from;
    std::string message = "sending data from string";
    if (manager.recvfromAck(buf, &len, &from))
    {
        Serial.print("message from node n.");
        Serial.print(from);
        Serial.print(": ");
        Serial.print((char*)buf);
        Serial.println(" rssi: ");
        //Serial.println(rf95.lastRssi()); 
        
        res = manager.sendtoWait(&message, from);
        if ( res != RH_ROUTER_ERROR_NONE)
        {
        Serial.print("sendtoWait failed:");
        Serial.println(res);
        }
    }
}



void loop()
{
    //test_main_node(); //Node 3
    //test_middle_node(); //Node 2
    test_sensor_node(); // Node 1    
}

