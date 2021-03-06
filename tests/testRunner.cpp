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

#include "altimeter.hpp"

/*
 * Test that true is true-- and that we have Googletest up and running.
 */
TEST(AltiPiTest, DefaultThreadSerialRXIsGood)
{
    /* SETUP */
  
    altimeter* p_testAltimeter = new altimeter(
            "testAltimeter.txt", "testDebugAltimeter.txt");
    
    //Default serial
    std::string serial_path = "/dev/ttyS0";
        
    //Default Baud Rate
    uint32_t serial_baud = 115200;

    //create & start serial port comms
    thread_serialrx* pThreadRX = new thread_serialrx(p_testAltimeter,
            serial_path, serial_baud);


    /* RUN TEST */
    EXPECT_NE(pThreadRX, nullptr);

    EXPECT_EQ(pThreadRX->altimeter_ok(), false);

    //stop and delete thread.
    pThreadRX->thread_stopwait();
    delete p_testAltimeter;
    p_testAltimeter = nullptr;
    
    delete pThreadRX;
    pThreadRX = nullptr;

    // Pointer should be nulled now.
    EXPECT_EQ(pThreadRX, nullptr);
}


/*
 * Test that the altimeter starts up not OK.
 */
TEST(AltimeterTest, DefaultsOk)
{
    /* SETUP */
    altimeter testAltimeter = altimeter("testAltimeter.txt", "testDebugAltimeter.txt");
    
    /* RUN TEST */
    EXPECT_EQ(testAltimeter.altimeter_ok(), false);
}

/*
 * Test that the altimeter starts up not OK.
 */
TEST(AltimeterTest, ProcessMessageTest)
{
    /* SETUP */
    altimeter testAltimeter = altimeter("testAltimeter.txt", "testDebugAltimeter.txt");
    
    /* RUN TEST */
    EXPECT_EQ(testAltimeter.altimeter_ok(), false);
    
    std::string sample_distance = "?LD 98.75";
    
    EXPECT_EQ(true, testAltimeter.processMessage(sample_distance));
    EXPECT_EQ(testAltimeter.altimeter_ok(), false);
    
    std::string sample_info_response = "? SF11/C V7 Firmware: V1.0.0";
    
    EXPECT_EQ(true, testAltimeter.processMessage(sample_info_response));
    EXPECT_EQ(testAltimeter.altimeter_ok(), true);
    
    std::string bad_response = "badbadbad";
    
    EXPECT_EQ(false, testAltimeter.processMessage(bad_response));
    EXPECT_EQ(testAltimeter.altimeter_ok(), false);
    
}


/*
 * Test that the altimeter averaging works correctly.
 */
TEST(AltimeterTest, AltimterDistanceAverageAllSameTest)
{
    /* SETUP */
    altimeter testAltimeter = altimeter("testAltimeter.txt", "testDebugAltimeter.txt");
    
    /* RUN TEST */
    EXPECT_EQ(testAltimeter.altimeter_ok(), false);
    
    // Test distances to try.
    std::vector<std::string> distance{
            "?LD 5.0",  // 0
            "?LD 5.0",  // 1
            "?LD 5.0",  // 2
            "?LD 5.0",  // 3
            "?LD 5.0",  // 4
            "?LD 5.0",  // 5
            "?LD 5.0",  // 6
            "?LD 5.0",  // 7
            "?LD 5.0",  // 8
            "?LD 5.0",  // 9
            "?LD 5.0"  // 10
    };
    
    // Note: Google Test recommends using ASSERT_FLOAT_EQ to test float equality.
    ASSERT_FLOAT_EQ(0.0, testAltimeter.last_altitude_m());
    
    
    // This test shows that we handled up to 10 items correctly.
    for (std::string dist: distance)
    {
        EXPECT_EQ(true, testAltimeter.processMessage(dist));
    }
    
    ASSERT_FLOAT_EQ(5.0, testAltimeter.last_altitude_m());
    
}


/*
 * Test that the altimeter averaging works correctly.
 */
TEST(AltimeterTest, AltimterDistanceAverageChangingTest)
{
    /* SETUP */
    altimeter testAltimeter = altimeter("testAltimeter.txt", "testDebugAltimeter.txt");
    
    /* RUN TEST */
    EXPECT_EQ(testAltimeter.altimeter_ok(), false);
    
    // Test distances to try.
    std::vector<std::string> distance{
            "?LD 2.5",  // 0
            "?LD 2.0",  // 1
            "?LD 3.0",  // 2
            "?LD 4.0",  // 3
            "?LD 4.0",  // 4
    };

    // Note: Google Test recommends using ASSERT_FLOAT_EQ to test float equality.
    ASSERT_FLOAT_EQ(0.0, testAltimeter.last_altitude_m());
    
    
    // This test shows that we handled up to 10 items correctly.
    for (std::string dist: distance)
    {
      EXPECT_EQ(true, testAltimeter.processMessage(dist));
    }
    
    // Calculated 3.1 as the average of the distance values.
    ASSERT_FLOAT_EQ(4, testAltimeter.last_altitude_m());
    
}

/*
 * Test that the altimeter averaging works correctly.
 */
TEST(AltimeterTest, AltimterAverageExpectedCase)
{
    /* SETUP */
    altimeter testAltimeter = altimeter("testAltimeter.txt", "testDebugAltimeter.txt");
    
    /* RUN TEST */
    EXPECT_EQ(testAltimeter.altimeter_ok(), false);
    
    // Test distances to try.
    std::vector<std::string> distance{
            "?LD 200.0",  // 0
            "?LD 200.0",  // 1
            "?LD 200.0",  // 2
            "?LD 200.0",  // 3
            "?LD 200.0",  // 4
            "?LD 200.0",  // 5
            "?LD 200.0",  // 6
            "?LD 200.0",  // 7
            "?LD 200.0",  // 8
            "?LD 200.0",  // 9
            "?LD 200.0"  // 10
    };
    
    // Note: Google Test recommends using ASSERT_FLOAT_EQ to test float equality.
    ASSERT_FLOAT_EQ(0.0, testAltimeter.last_altitude_m());
    
    
    // This test shows that we handled up to 10 items correctly.
    for (std::string dist: distance)
    {
        EXPECT_EQ(true, testAltimeter.processMessage(dist));
    }
    
    ASSERT_FLOAT_EQ(200.0, testAltimeter.last_altitude_m());
    
}


/*
 * Test that the altimeter averaging works correctly.
 */
TEST(AltimeterTest, AltimeterBadValueCase)
{
    /* SETUP */
    altimeter testAltimeter = altimeter("testAltimeter.txt", "testDebugAltimeter.txt");
    
    /* RUN TEST */
    EXPECT_EQ(testAltimeter.altimeter_ok(), false);
    
    // Test distances to try.
    std::vector<std::string> distance{
            "?LD 200.0",  // 0
            "?LD 100.0",  // 1
    };
    
    // Note: Google Test recommends using ASSERT_FLOAT_EQ to test float equality.
    ASSERT_FLOAT_EQ(0.0, testAltimeter.last_altitude_m());
    
    
    // This test shows that we handled up to 10 items correctly.
    for (int i = 0; i < 11; i++)
    {
        EXPECT_EQ(true, testAltimeter.processMessage(distance[0]));
    }
    
    ASSERT_FLOAT_EQ(200.0, testAltimeter.last_altitude_m());
    

    EXPECT_EQ(true, testAltimeter.processMessage(distance[1]));
    //ASSERT_FLOAT_EQ(200.0, testAltimeter.last_altitude_m());
    
}

/*
 * Test that the altimeter averaging works correctly.
 */
TEST(AltimeterTest, AltimeterWithStdev)
{
    /* SETUP */
    altimeter testAltimeter = altimeter("testAltimeter.txt", "testDebugAltimeter.txt");
    
    /* RUN TEST */
    EXPECT_EQ(testAltimeter.altimeter_ok(), false);
    
    // Test distances to try.
    std::vector<std::string> distance{
            "?LD 210.10",  // 0
            "?LD 203.18",  // 1
            "?LD 199.93",  // 2
            "?LD 197.87",  // 3
            "?LD 197.54",  // 4
            "?LD 195.80",  // 5
            "?LD 195.24",  // 6
            "?LD 194.69",  // 7
            "?LD 195.83",  // 8
            "?LD 197.97",  // 9
            "?LD 201.61"  // 10
    };
    
    
    // Note: Google Test recommends using ASSERT_FLOAT_EQ to test float equality.
    ASSERT_FLOAT_EQ(0.0, testAltimeter.last_altitude_m());
    
    
    // This test shows that we handled up to 10 items correctly.
    for (std::string dist: distance)
    {
      EXPECT_EQ(true, testAltimeter.processMessage(dist));
    }
    
    ASSERT_FLOAT_EQ(201.61, testAltimeter.last_altitude_m());
    
}

/*
 * Test that the altimeter averaging works correctly.
 */
TEST(AltimeterTest, AltimeterWithStdevWithOutsideValues)
{
    /* SETUP */
    altimeter testAltimeter = altimeter("testAltimeter.txt", "testDebugAltimeter.txt");
    
    /* RUN TEST */
    EXPECT_EQ(testAltimeter.altimeter_ok(), false);
    
    // Test distances to try.
    std::vector<std::string> distance{
            "?LD 64.04",    // 0
            "?LD 61.9292",  // 1
            "?LD 60.9386",  // 2
            "?LD 60.3107",  // 3
            "?LD 60.2101",  // 4
            "?LD 59.68",    // 5
            "?LD 59.5091",  // 6
            "?LD 59.3415",  // 7
            "?LD 59.6889",  // 8
            "?LD 60.3412",  // 9
            "?LD 30.1599",  // 10
            "?LD 30.0746",  // 11
            "?LD 30.0746",  // 12
            "?LD 61.4507"   // 13
    };
    
    
    // Note: Google Test recommends using ASSERT_FLOAT_EQ to test float equality.
    ASSERT_FLOAT_EQ(0.0, testAltimeter.last_altitude_m());
    
    
    // This test shows that we handled up to 10 items correctly.
    for (std::string dist: distance)
    {
        EXPECT_EQ(true, testAltimeter.processMessage(dist));
    }
    
    ASSERT_FLOAT_EQ(61.4507, testAltimeter.last_altitude_m());
    
}
