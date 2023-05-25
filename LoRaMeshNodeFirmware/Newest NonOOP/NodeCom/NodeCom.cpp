#include "NodeCom.h"

enum NodeCom::ModemConfigChoice 
{   Bw125Cr45Sf128 = 0, 
    Bw500Cr45Sf128, 
    Bw31_25Cr48Sf512, 
    Bw125Cr48Sf4096, 
    Bw125Cr45Sf2048 };

enum NodeCom::RequestMessages 
{   ERROR = 0,
    IP_REQUEST,
    GET_STATUS,
    GET_MASTER_IP, 
    GET_ALL_SENSORS_DATA, 
    GET_SENSORS_DATA };

NodeCom::NodeCom(bool is_sensor_node):rf95(1,14), manager(rf95, 254)
{
    isSensorNode = is_sensor_node;
}

NodeCom::NodeCom(int ssn, int dio0, bool is_sensor_node, uint8_t ip):rf95(ssn,dio0), manager(rf95, ip)
{
    isSensorNode = is_sensor_node;
}

bool NodeCom::init(bool longRange)
{
    if (!manager.init() || (isSensorNode && !sensor.init()) )
    {
        return false; // false for initialisation error
    }

    
    setTxPower(2, false);
    setFrequency(433.0);
    setCADTimeout(500);

    // long range configuration requires for on-air time
    if (longRange) 
        {
        // custom configuration
        ModemConfig modem_config = {
        0x78, // Reg 0x1D: BW=125kHz, Coding=4/8, Header=explicit
        0xC4, // Reg 0x1E: Spread=4096chips/symbol, CRC=enable
        0x08  // Reg 0x26: LowDataRate=On, Agc=Off.  0x0C is LowDataRate=ON, ACG=ON
        };
        setModemRegisters(&modem_config);
        }
    else
        {
            if (!setModemConfig(Bw125Cr45Sf128))
            {
                return false;
            }
        }
    return true;
}

bool NodeCom::setFrequency(float centre)
{
    return rf95.setFrequency(centre);
}

void NodeCom::setTxPower(int8_t power, bool useRFO)
{
    rf95.setTxPower(power, useRFO);
}

void NodeCom::setCADTimeout (unsigned long cad_timeout)
{
    rf95.setCADTimeout(cad_timeout);
}

void NodeCom::setModemRegisters (const ModemConfig *config)
{
    const RH_RF95::ModemConfig config2 = {config->reg_1d, config->reg_1e, config->reg_26};
    rf95.setModemRegisters(&config2);
}

bool NodeCom::setModemConfig (ModemConfigChoice index)
{
    // Predefined configurations( bandwidth, coding rate, spread factor ):
    // Bw125Cr45Sf128     Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on. Default medium range
    // Bw500Cr45Sf128     Bw = 500 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on. Fast+short range
    // Bw31_25Cr48Sf512   Bw = 31.25 kHz, Cr = 4/8, Sf = 512chips/symbol, CRC on. Slow+long range
    // Bw125Cr48Sf4096    Bw = 125 kHz, Cr = 4/8, Sf = 4096chips/symbol, low data rate, CRC on. Slow+long range
    // Bw125Cr45Sf2048    Bw = 125 kHz, Cr = 4/5, Sf = 2048chips/symbol, CRC on. Slow+long range
    RH_RF95::ModemConfigChoice index2 = static_cast<RH_RF95::ModemConfigChoice>(index);
    if(rf95.setModemConfig(index2)) 
        return true;
    return false;
}

void NodeCom::setIsaRouter (bool isa_router)
{
    manager.setIsaRouter (isa_router);
}

uint8_t NodeCom::sendtoWait (uint8_t *buf, uint8_t len, uint8_t dest, uint8_t flags)
{
    //Return value and signification
    // RH_ROUTER_ERROR_NONE Message was routed and delivered to the next hop (not necessarily to the final dest address)
    // RH_ROUTER_ERROR_NO_ROUTE There was no route for dest in the local routing table
    // RH_ROUTER_ERROR_UNABLE_TO_DELIVER Not able to deliver to the next hop (usually because it dod not acknowledge due to being off the air or out of range
    return manager.sendtoWait (buf, len, dest, flags);
}

uint8_t NodeCom::sendtoWait (std::string *buf, uint8_t dest, uint8_t flags)
{
    //Return value and signification
    // RH_ROUTER_ERROR_NONE Message was routed and delivered to the next hop (not necessarily to the final dest address)
    // RH_ROUTER_ERROR_NO_ROUTE There was no route for dest in the local routing table
    // RH_ROUTER_ERROR_UNABLE_TO_DELIVER Not able to deliver to the next hop (usually because it dod not acknowledge due to being off the air or out of range
    const char* c_str = buf->c_str();
    uint8_t* buf2 = new uint8_t[buf->length()];
    std::memcpy(buf2, c_str, buf->length());
    bool status = manager.sendtoWait (buf2, buf->length(), dest, flags);
    delete[] buf2; // Free the memory when done using the array 
    return status;
}
 
bool NodeCom::recvfromAck (uint8_t *buf, uint8_t *len, uint8_t *source, uint8_t *dest, uint8_t *id, uint8_t *flags, uint8_t *hops)
{
    return manager.recvfromAck (buf, len, source, dest, id, flags, hops);
}

bool NodeCom::recvfromAck (std::string *buf, uint8_t *len, uint8_t *source, uint8_t *dest, uint8_t *id, uint8_t *flags, uint8_t *hops)
{
    uint8_t* buf2 = new uint8_t[*len];
    bool status = manager.recvfromAck (buf2, len, source, dest, id, flags, hops);
    *buf = (reinterpret_cast<char*>(buf2), reinterpret_cast<char*>(buf2 + *len));//this conversion works with ASCII or UTF-8 characters
    delete[] buf2; // Free the memory when done using the array
    return status;
}
 
bool NodeCom::recvfromAckTimeout (uint8_t *buf, uint8_t *len, uint16_t timeout, uint8_t *source, uint8_t *dest, uint8_t *id, uint8_t *flags, uint8_t *hops)
{
    return manager.recvfromAckTimeout (buf, len, timeout, source, dest, id, flags, hops);
}

bool NodeCom::recvfromAckTimeout (std::string *buf, uint8_t *len, uint16_t timeout, uint8_t *source, uint8_t *dest, uint8_t *id, uint8_t *flags, uint8_t *hops)
{
    uint8_t* buf2 = new uint8_t[*len];
    bool status = manager.recvfromAckTimeout (buf2, len, timeout, source, dest, id, flags, hops);
    //std::string str = (reinterpret_cast<char*>(buf2), reinterpret_cast<char*>(buf2 + *len));//this conversion works with ASCII or UTF-8 characters
    *buf = std::string((char *)buf2);
    delete[] buf2; // Free the memory when done using the array
    return status;
}

int16_t NodeCom::lastRssi()
{
    return rf95.lastRssi();
}

bool NodeCom::sendRequest(uint8_t dest, RequestMessages message, uint8_t *buf, uint8_t *len, uint16_t timeout)
{
    uint8_t res;
    const uint8_t len2 = 10;
    uint8_t buf2[len2];
    for(int i = 0; i < len2; i++)
    {
        buf2[i] = message;
    }

    res = manager.sendtoWait(buf2, len2, dest);
    if ( res == RH_ROUTER_ERROR_NONE) // sent successfully, waiting for respond
    {
        if (manager.recvfromAckTimeout(buf, len, timeout, &dest))
        {
            return true;
        }
    }
    return false;
}

bool NodeCom::waitForResquest()
{
    const uint8_t len2 = 10;
    uint8_t buf2[len2];
    uint8_t from;
    uint8_t res;
    if (manager.recvfromAck(buf2, &len2, &from))
    {
        bool isError = false;
        for(int i = 1; i < len2; i++)
        {
            if(buf2[0] =! buf2[i])
                isError = true;
        }
        
        int message = buf2[0];
        if(isError)
            message = 0;

        
        if(message == IP_REQUEST)
        {
            //Do what.............
            return true;
        }

        else if(message == GET_STATUS)
        {
            //Do what.............
            return true;
        }

        else if(message == GET_MASTER_IP)
        {
            //Do what.............
            return true;
        }

        else if(message == GET_ALL_SENSORS_DATA)
        {
            if(isSensorNode)
            {
                //Do what.............
                return true;
            }
        }

        else if(message == GET_SENSORS_DATA)
        {
            if(isSensorNode)
            {
                std::string message_to_send = sensor.getData();
                res = manager.sendtoWait(&message_to_send, from);
                if ( res == RH_ROUTER_ERROR_NONE)
                {
                    return true;
                }
            }
        }
    }
    return false;
}

bool NodeCom::respondRequest(uint8_t* dest, RequestMessages message) // maybe useless
{
    
    
    
    
    if(message == IP_REQUEST)
    {

    }

    else if(message == GET_STATUS)
    {
        
    }

    else if(message == GET_MASTER_IP)
    {

    }

    else if(message == GET_ALL_SENSORS_DATA)
    {
        if(isSensorNode)
        {

        }
        else return false;
    }

    else if(message == GET_SENSORS_DATA)
    {
        if(isSensorNode)
        {

        }
        else return false;
    }
}

bool NodeCom::setAsMasterIp()
{

}

bool NodeCom::findMasterIp()
{

}

bool NodeCom::getIp()
{

}

bool NodeCom::giveIp()
{

}