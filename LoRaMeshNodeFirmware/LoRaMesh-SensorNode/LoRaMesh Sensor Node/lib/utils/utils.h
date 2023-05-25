#pragma once
#include <Arduino.h>

#define membersof(x) (sizeof(x) / sizeof(x[0]))

const uint8_t *String2uint8_tArr(String in)
{
    char outb[in.length() + 1];
    in.toCharArray(outb, in.length() + 1); // String to char*
    return (uint8_t *)outb;                // Char* to uint8_t*
}

void printHEX(String string)
{
    Serial.print("\t=> [");
    const uint8_t *bdata = String2uint8_tArr(string);
    for (uint8_t i = 0; i < membersof(bdata); i++)
    {
        Serial.print(bdata[i], HEX);
        Serial.print(", ");
    }
    Serial.println("]");
}