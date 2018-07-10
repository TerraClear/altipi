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

using namespace std;

#define GPIO4 7

int main(int argc, char** argv) 
{
	//setup wiringPi
    	wiringPiSetup();

	//set IO pin state
	pinMode(GPIO4, OUTPUT);

	//enable LED
	digitalWrite(GPIO4, HIGH);
	
	printf("done..\n");

    	return 0;

}

