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

//wiringPi
#include <wiringPi.h>

//locals
#include "pipinmap.hpp"
#include "thread_serialrx.hpp"

//Pins for LED's and Trigger line.
#define LED_OK  GPIO_12
#define LED_ERR GPIO_24
#define PIN_TRIGGER GPIO_21 
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

//check if file exists..
bool fexists(const std::string& filename) 
{
  std::ifstream ifile(filename.c_str());
  return (bool)ifile;
}

//generete next log file name in numbered sequence
std::string generate_filename(std::string file_name, std::string file_ext)
{
    int file_seq = 1;
    int max_tries = 1000;
    std::string default_file = file_name + "." + file_ext;
    std::string tmp_file = default_file;
    
    do 
    {
        std::stringstream strstrm;
        strstrm << file_name << "_" << file_seq << "." << file_ext;
        tmp_file = strstrm.str();
        
        file_seq++;
        max_tries --;

        //max file sequence generation reached, use default and overwrite.
        if (max_tries <= 0)
        {
            tmp_file = default_file;
            break;
        }
        
    } while (fexists(tmp_file));
    
    return tmp_file;
}

int main(int argc, char** argv) 
{
        //initiate the clock 
        _lastmeasure = std::chrono::steady_clock::now();
    
        //Default output file
        std::string outfile_name = "altimetry";
        std::string outfile_ext = "txt";
       
        //Default serial
        std::string serial_path = "/dev/ttyS0";
        
        //Default Baud Rate
        uint32_t serial_baud = 115200;
        
        //if port and path is supplied..
        if (argc > 1)
        {
           serial_path = argv[1];
        }

        if (argc > 2)
        {
            std::stringstream strstrm;
            strstrm << argv[2] << "/" << outfile_name;
            outfile_name = strstrm.str();
        }

        std::string outfile_full = generate_filename(outfile_name, outfile_ext);

        //create & start serial port comms
        std::cout << ">>> START SERIAL THREAD..." << std::endl;
        pThreadRX = new thread_serialrx(outfile_full, serial_path, serial_baud);
        pThreadRX->thread_start("serialrx");

	//setup wiringPi in GPIO pin numbering mode..
    std::cout << ">>> STARTUP PI GPIO..." << std::endl;
    	wiringPiSetupGpio();

	//set IO pin state
	pinMode(LED_OK, OUTPUT);
	pinMode(LED_ERR, OUTPUT);
	pinMode(PIN_TRIGGER, INPUT);

        //set LED Start state
    std::cout << ">>> DEFAULT LED STATES..." << std::endl;
        digitalWrite(LED_ERR, HIGH);
        digitalWrite(LED_OK, LOW);

        //wire up interrupt on trigger pin
    std::cout << ">>> WIRING ISR..." << std::endl;
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

