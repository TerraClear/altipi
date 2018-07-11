/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: koos
 *
 * Created on July 10, 2018, 12:05 PM
 */

#include <cstdlib>
#include <iostream>
#include <unistd.h>

#include <wiringPi.h>
#include "pipinmap.hpp"

#include "../tcpserial/xkserial.hpp"

xk::xkserial _serial1;

void interrupttest()
{
    std::cout << "ISR Called!!" << std::endl;

    //write client msg to serial port..
    _serial1.writeString("?LD\r\n", 500);

    std::string retstr = _serial1.readstring(32, 500);
    
    std::cout << retstr;
}

int main(int argc, char** argv) 
{
        std::string serial_path = "/dev/ttyAMA0";

        //if port is supplied use supplied path..
        if (argc > 1)
        {
           serial_path = argv[1];
        }

        _serial1.open(serial_path, xk::BAUD_115200);

    
	//setup wiringPi in GPIO pin numbering mode..
    	wiringPiSetupGpio();

	//set IO pin state
	pinMode(GPIO_18, OUTPUT);
	pinMode(GPIO_23, INPUT);

        wiringPiISR(GPIO_23, INT_EDGE_FALLING, &interrupttest);
        
        bool toggle = false;

        while(1)
        {
            //enable LED
            toggle = !toggle;
            digitalWrite(GPIO_18, toggle);

            std::cout << "." ;
            usleep(500000);        
        }

       
    	return 0;

}

