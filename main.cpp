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

#include <wiringPi.h>
#include <unistd.h>
#include "pipinmap.hpp"

using namespace std;

void interrupttest()
{
 
    std::cout << "ISR Called!!" << std::endl;
}

int main(int argc, char** argv) 
{
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


            usleep(500000);        
        }

       
    	return 0;

}

