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
#include "../libterraclear/src/thread_base.hpp"
#include "../libterraclear/src/basicserial.hpp"

#define FEET_IN_METERS 3.28084

class thread_serialrx : public terraclear::thread_base
{
    public:
        thread_serialrx(std::string file_path, std::string serial_path, uint32_t serial_baud);
        virtual ~thread_serialrx();
        
        //create a new request for altimetry
        void create_request(uint32_t seqno, uint32_t millis_elapsed);
        bool altimeter_ok();
    
    private:
        struct altitude_entry
        {
            int seqno = 0;
            uint32_t millis_elapsed = 0;
            float distance_meters;
        };
        
        const uint32_t      _Serial_Timeout = 250;
        const std::string   _Request_Distance = "?LD\r\n";
        const std::string   _Request_Info = "?\r\n";
        const std::string   _Response_Distance = "?LD ";
        const std::string   _Response_Info = "? SF11";
        
        std::queue<altitude_entry>  _entry_queue;
        std::string         _serial_path = "";
        std::string         _file_path = "";
        terraclear::basicserial _serial1;
        bool                _altimeter_ok = false;
        std::string         _serial_data = "";

        //implementation of base class pure virtual functions..
        void thread_runloop();
        
        //Process Serial Port response messages.
        void processMessage(std::string serialmsg);
        
        //Check if file exists
        bool fexists(std::string filename);

        //create blank file
        void createFile(std::string filename);
        
        //Append text to existing file
        bool appendFile(std::string filename, std::string appendstring);

};

#endif /* THREAD_SERIALRX_HPP */

