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

/* 
 * File:   testRunner.cpp
 * Author: Jennifer Kowalsky <Jennifer.Kowalsky@TerraClear.com>
 * Description: Unit tests for the AltiPi project.
 * Created on May 15, 2019, 12:40 PM
 */

#include <stdlib.h>
#include <iostream>
#include "gtest/gtest.h"
#include "pipinmap.hpp"
#include "thread_serialrx.hpp"


/*
 * Test that true is true-- and that we have Googletest up and running.
 */
TEST(AltiPiTest, DefaultThreadSerialRXIsGood)
{
    /* SETUP */
  
    //Default serial
    std::string serial_path = "/dev/ttyS0";
        
    //Default Baud Rate
    uint32_t serial_baud = 115200;

    std::string outfile_full = "testaltimetry.txt";

    //create & start serial port comms
    std::cout << ">>> START SERIAL THREAD..." << std::endl;
    thread_serialrx* pThreadRX = new thread_serialrx(outfile_full, serial_path, serial_baud);


    /* RUN TEST */
    EXPECT_NE(pThreadRX, nullptr);

    EXPECT_EQ(pThreadRX->altimeter_ok(), false);

    //stop and delete thread.
    pThreadRX->thread_stopwait();
    delete pThreadRX;
    pThreadRX = nullptr;

    // Pointer should be nulled now.
    EXPECT_EQ(pThreadRX, nullptr);
}
