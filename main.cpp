#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <fstream>
#include <chrono>

#ifdef __APPLE__
    #include <json/json.h>
#else
    #include <jsoncpp/json/json.h>
#endif

#ifdef __arm__
    #include <wiringPi.h>
#else
    #include "wiringPi_stub.h"
#endif

#include "pipinmap.hpp"
#include "xkthread_serialrx.hpp"

//Pins for LED's and Trigger line.
#define LED_OK  GPIO_23
#define LED_ERR GPIO_18
#define PIN_TRIGGER GPIO_25 
#define DEBOUNCE_MS 100

//clock to measure time elapsed between triggers.
std::chrono::steady_clock::time_point _lastmeasure;

//serial thread
xkthread_serialrx* pThreadRX  = nullptr;

//sequence number of trigger 
uint32_t _seqno = 0;


//Interrupt code.
void trigger_pulse()
{
    //get milliseconds since previous trigger
    std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
    uint32_t millicount = std::chrono::duration_cast<std::chrono::milliseconds>(now - _lastmeasure).count();
    
    //restrict serial calls incase there is noise on the line.
    if (millicount > DEBOUNCE_MS)
    {
        _lastmeasure = std::chrono::steady_clock::now();

        //reset millis for 1st entry
        if (_seqno == 0)
            millicount = 0;

        //create altimetry log request.
        pThreadRX->create_request(_seqno, millicount);

        //increase seq numbers.
        _seqno++;
    }
}

int main(int argc, char** argv) 
{
        //Default output file
        std::string outfilename = "altimetry.txt";

        //Default serial
        std::string serial_path = "/dev/ttyS0";
        
        //Default Baud Rate
        uint32_t serial_baud = 115200;
        
        //if port is supplied use supplied path..
        if (argc > 1)
        {
           serial_path = argv[1];
        }

        //create searial port
        pThreadRX = new xkthread_serialrx(outfilename, serial_path, serial_baud);

        //start serial thread.
        pThreadRX->thread_start("serialrx");

	//setup wiringPi in GPIO pin numbering mode..
    	wiringPiSetupGpio();

	//set IO pin state
	pinMode(LED_OK, OUTPUT);
	pinMode(LED_ERR, OUTPUT);
	pinMode(PIN_TRIGGER, INPUT);

        //set LED Start state
        digitalWrite(LED_ERR, HIGH);
        digitalWrite(LED_OK, LOW);

        //wire up interrupt on trigger pin
        wiringPiISR(PIN_TRIGGER, INT_EDGE_FALLING, &trigger_pulse);
        
        //Flash LED
        bool toggle = false;
        while(1)
        {
            if (pThreadRX->altimeter_ok())
            {
                digitalWrite(LED_OK, HIGH);
                digitalWrite(LED_ERR, LOW);
            }
            else
            {
                //flash ERR LED
                toggle = !toggle;
                digitalWrite(LED_OK, LOW);
                digitalWrite(LED_ERR, toggle);
            }
            
            usleep(100000);        
        }

        //stop and delete thread.
        pThreadRX->thread_stopwait();
        delete pThreadRX;
        
    	return 0;

}

