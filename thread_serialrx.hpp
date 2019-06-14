/*
 * Serial Comms threading class 
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

#define _VERSION_STRING "0.03"

#ifndef THREAD_SERIALRX_HPP
#define THREAD_SERIALRX_HPP

#include <iostream>
#include <fstream>
#include <sstream> 
#include <queue>
#include <iostream>
#include <cmath>
#include <iomanip>

//statically include only specific files.. i.e. we dont need entire lib.
#ifndef TC_USE_SERIAL
    #define TC_USE_SERIAL
#endif
#include "libterraclear/src/basicserial.hpp"
#include "libterraclear/src/thread_base.hpp"
#include "altimeter.hpp"

class thread_serialrx : public terraclear::thread_base
{
    public:
        thread_serialrx(altimeter* p_altimeter, std::string serial_path, uint32_t serial_baud);
        virtual ~thread_serialrx();
        
        //create a new request for altimetry
        void create_request(uint32_t seqno, uint32_t millis_elapsed);
        bool altimeter_ok();
    
    private:
        const std::string _version_string = _VERSION_STRING;
        
        
        const uint32_t      _Serial_Timeout = 250;
        const std::string   _Request_Continuous_Mode = "#SU,1\r\n";
        const std::string   _Request_Median_Filter_On = "#FM, 1\r\n";
        const std::string   _Request_Median_Filter_Use_20_Values = "#FS, 20\r\n";
        const std::string   _Request_Distance = "?LD\r\n";
        const std::string   _Request_Info = "?\r\n";
        
        std::string         _serial_path = "";
        terraclear::basicserial _serial1;
        std::string         _serial_data = "";
        altimeter*          _p_altimeter;

        //implementation of base class pure virtual functions..
        void thread_runloop();

};

#endif /* THREAD_SERIALRX_HPP */

