#include "Arduino.h" // Must be here for harware defines to be linked first
#include "../can_api.h"
#include "SimpleCAN.h"

#if defined(STM32F4xx)

#include <string.h>

SimpleCAN can;

void _initCAN(int tx, int rx) {
    //Initialize configuration structures using macro initializers
    can.init(tx, rx, BAUD_500K, CanMode::NormalCAN); 
    can.filterAcceptAll();
    can.begin();
    
}

void _transmitCAN(uint32_t identifier, uint8_t *data, uint8_t length) {
    //Configure message to transmit
    can_message_t message;
    message.id = identifier;
    //message.extd = 1;
    message.isStandard = 0;
    message.dlc = length;
    for (int i = 0; i < length; i++) {
        message.data[i] = data[i];
    }

    //Queue message for transmission
    can.transmit(&message);
    
}

bool _receiveCAN(uint32_t *identifier, uint8_t *data, uint8_t *length) {
    
    can_message_t message;
    if (can.receive(&message) != CAN_OK) {
       return false; // no message
    }

    if (message.isStandard == 1) {    // 29 bit
        return false; // 11 bit
    }

    if (!message.isRTR) {
        *identifier = message.id;
        memcpy(data, message.data, message.dlc);
        *length = message.dlc;
        
        return true;
    }
    return false;

}

void _getUniqueID(uint8_t * id) {
	// set ID

}

#endif