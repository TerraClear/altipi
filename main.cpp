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
#include "pipinmap.hpp"

using namespace std;

int main(int argc, char** argv) 
{
	//setup wiringPi
    	wiringPiSetup();

	//set IO pin state
	pinMode(GPIO_18, OUTPUT);

	//enable LED
	digitalWrite(GPIO_18, HIGH);
	
	std::cout << "Enabled GPIO 18 " << std::endl;

    	return 0;

}

