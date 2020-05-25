#include "pxt.h"
#include <cstdint>
#include <math.h>

using namespace pxt;

namespace temperatursensor {

class microbitp : public MicroBitComponent
{
  public:
    void *pin;
    int id;
    int status;
    int val;
    PinCapability capability;
    uint8_t pullMode;
    PinName name;

    microbitp(int id, PinName name, PinCapability capability){
        this->id = id;
        this->name = name;
        this->capability = capability;
        this->pullMode = 1;
        this->status = 0x00;
        this->pin = NULL;
        this->val = 0;
    }
    void deletep(){
        val -= 1;
        //printf("val = %d\n",val);
        if (status & 0x01)
            delete ((DigitalIn *)pin);
        if (status & 0x02)
            delete ((DigitalOut *)pin);
    }

    void disconnect(){
        
        if (status & 0x01){
            delete ((DigitalIn *)pin);
            val -= 1;
        }
        if (status & 0x02){
            delete ((DigitalOut *)pin);
            val -= 1;
        }
    }

    int setDigitalValue(int value){
        if (!(status & 0x02)){
            disconnect();
            val += 1;
            pin = new DigitalOut(name);
            status = 0x02;
        }
        // Write the value.
        ((DigitalOut *)pin)->write(value);
        return 0;
    }

    int getDigitalValue(){
        ((DigitalIn *)pin)->mode(PullUp);
        status = 0x01;
        return ((DigitalIn *)pin)->read();
//          return 0;
    }
};
    

    microbitp  pin0(7, MICROBIT_PIN_P0, PIN_CAPABILITY_ALL);              //P1
    microbitp  pin1(8, MICROBIT_PIN_P1, PIN_CAPABILITY_ALL);              //P2
    microbitp  pin2(9, MICROBIT_PIN_P2, PIN_CAPABILITY_ALL);              //C16
    microbitp  pin8(15, MICROBIT_PIN_P8, PIN_CAPABILITY_STANDARD);        //C17
    microbitp  pin12(19,MICROBIT_PIN_P12,PIN_CAPABILITY_STANDARD);        //P0
    microbitp  pin16(23,MICROBIT_PIN_P16,PIN_CAPABILITY_ALL);              //P3
    microbitp  pin19(25, MICROBIT_PIN_P19, PIN_CAPABILITY_STANDARD);        //C18
    microbitp  pin20(24, MICROBIT_PIN_P20, PIN_CAPABILITY_STANDARD);        //C19
    
    microbitp  pin = pin0;

    uint8_t init() {
        pin.setDigitalValue(0);
        for (volatile uint16_t i = 0; i < 600; i++);
        pin.setDigitalValue(1);
        for (volatile uint8_t i = 0; i < 30; i++);
        int b = pin.getDigitalValue();
        for (volatile uint16_t i = 0; i < 600; i++);
        return b;
    }

    void writeBit(int b) {
        int delay1, delay2;
        if (b == 1) {
            delay1 = 1;
            delay2 = 80;
        } else {
            delay1 = 75;
            delay2 = 6;
        }
        pin.setDigitalValue(0);
        for (uint8_t i = 1; i < delay1; i++);
        pin.setDigitalValue(1);
        for (uint8_t i = 1; i < delay2; i++);
    }

    void writeByte(int byte) {
        int i;
        for (i = 0; i < 8; i++) {
            if (byte & 1) {
                writeBit(1);
            } else {
                writeBit(0);
            }
            byte = byte >> 1;
        }
    }

    int readBit() {
        volatile int i;
        pin.setDigitalValue(0);
        //pin.setDigitalValue(1);
        pin.getDigitalValue()
        for (i = 1; i < 10; i++);
        int b = pin.getDigitalValue();
        for (i = 1; i < 60; i++);
        return b;
    }

    int convert() {
        volatile int i;
        int j;
        writeByte(0x44);
        for (j = 1; j < 1000; j++) {
            for (i = 1; i < 900; i++) {
        };
        if (readBit() == 1)
            break;
        };
        return (j);
    }

    int readByte() {
        int byte = 0;
        int i;
        for (i = 0; i < 8; i++) {
            byte = byte | readBit() << i;
        };
        return byte;
    }

    //%
    int16_t Temperature(int p) {
        switch(p){
          case 0: pin = pin0; break;
          case 1: pin = pin1; break;
          case 2: pin = pin2; break;
          case 8: pin = pin8; break;
          case 12: pin = pin12; break;
          case 16: pin = pin16; break;
          case 19: pin = pin19; break;
          case 20: pin = pin20; break;
          default: pin = pin0;
        }
        init();
        writeByte(0xCC);
        convert();
        init();
        writeByte(0xCC);
        writeByte(0xBE);
        int b1 = readByte();
        int b2 = readByte();
        pin.deletep();
        int16_t temp = (b2 << 8 | b1);
        return temp * 100 / 16;
    }
}
