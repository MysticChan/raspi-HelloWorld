#include <cstdlib>
#include <iostream>
//#include <wiringPi.h>
#include <RF24/RF24.h>

using namespace std;

// Radio pipe addresses for the 2 nodes to communicate.
const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };

// CE Pin, CSN Pin, SPI Speed
// Setup for GPIO 22 CE and CE0 CSN with SPI Speed @ 250KBPS
RF24 radio(RPI_V2_GPIO_P1_22, 0, RF24_250KBPS);


int main(int argc, char** argv)
{
    uint8_t len;

    // Refer to RF24.h or nRF24L01 DS for settings
    radio.begin();
    //radio.enableDynamicPayloads();
    //radio.setPALevel(RF24_PA_LOW);
    radio.setRetries(15,15);
    radio.setDataRate(RF24_250KBPS);
    radio.setPayloadSize(8);
    //radio.setAutoAck(1);
    radio.setChannel(13);
    //radio.setCRCLength(RF24_CRC_16);

    // Open 6 pipes for readings ( 5 plus pipe0, also can be used for reading )
    radio.openWritingPipe(pipes[0]);
    radio.openReadingPipe(1,pipes[1]);

    //
    // Start listening
    //
    radio.startListening();

    //
    // Dump the configuration of the rf unit for debugging
    //
    radio.printDetails();

    printf("Output below : \n");
    delay(1);

    // Pin 27 muss exportiert sein (gpio export 27 out)
//    int pin = 27;

    // http://wiringpi.com/reference/setup/
    // wiringPiSetupSys uses Broadcom GPIO pin numbers directly with no re-mapping
//    if (wiringPiSetupSys() == -1) {
//        exit (1);
//    }
//    pinMode(pin, OUTPUT);
//
//    delay(1);

    int counter{0};

    while(1)
    {
        int16_t receivePayload = 0;
        uint8_t pipe = 1;

        // Start listening


        if(radio.available()) {
            while ( radio.available() )
            {
                //len = radio.getDynamicPayloadSize();
                len = 2;
                radio.read( &receivePayload, sizeof(receivePayload));
                delay(20);

//            if(receivePayload == 111) {
                printf("Recv: size=%i payload=%d pipe=%i counter=%d",len,receivePayload,1,counter);
//            }
                // Display it on screen
//            printf("Recv: size=%i payload=%s pipe=%i",len,receivePayload,1);
//            digitalWrite(pin, 1);
//            delay(250);
//            digitalWrite(pin, 0);

                // Send back payload to sender
                radio.stopListening();

                // if pipe is 7, do not send it back
                radio.write(&receivePayload,len);
                receivePayload = 0;
                printf("\t Send: size=%i payload=%d pipe:%i\n",len,receivePayload,0);

                radio.startListening();
                counter++;
            }

        }

        delayMicroseconds(20);
    }

    return 0;
}
