/*
 * This application measures distances with SF11/C Lidar.
 * Copyright (C) 2017 TerraClear, Inc.
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <fstream>
#include <chrono>

#include <wiringPi.h>
#include "pipinmap.hpp"

#include "thread_serialrx.hpp"

//Pins for LED's and Trigger line.
#define LED_OK  GPIO_23
#define LED_ERR GPIO_18
#define PIN_TRIGGER GPIO_25 
#define DEBOUNCE_MS 100

//clock to measure time elapsed between triggers.
std::chrono::steady_clock::time_point _lastmeasure;
uint32_t elapse_tof = 0;

//serial thread
thread_serialrx* pThreadRX  = nullptr;

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
        {
            millicount = 0;
            elapse_tof = 0;
        }
        else
        {
            elapse_tof += millicount;
        }
        //create altimetry log request.
        pThreadRX->create_request(_seqno, elapse_tof);

        //increase seq numbers.
        _seqno++;
    }
}

int main(int argc, char** argv) 
{
    cout << ">>> STARTUP..." << std::endl;
    
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

        //create & start serial port comms
    cout << ">>> START SERIAL THREAD..." << std::endl;
        pThreadRX = new thread_serialrx(outfilename, serial_path, serial_baud);
        pThreadRX->thread_start("serialrx");

	//setup wiringPi in GPIO pin numbering mode..
    cout << ">>> STARTUP PI GPIO..." << std::endl;
    	wiringPiSetupGpio();

	//set IO pin state
	pinMode(LED_OK, OUTPUT);
	pinMode(LED_ERR, OUTPUT);
	pinMode(PIN_TRIGGER, INPUT);

        //set LED Start state
    cout << ">>> DEFAULT LED STATES..." << std::endl;
        digitalWrite(LED_ERR, HIGH);
        digitalWrite(LED_OK, LOW);

        //wire up interrupt on trigger pin
    cout << ">>> WIRING ISR..." << std::endl;
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
            
            usleep(250000);        
        }

        //stop and delete thread.
        pThreadRX->thread_stopwait();
        delete pThreadRX;
        
    	return 0;

}

