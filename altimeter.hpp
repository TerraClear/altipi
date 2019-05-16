/*
 * Altimeter control class.
 * Copyright (C) 2019 TerraClear, Inc.
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

#ifndef ALTIMETER_HPP
#define ALTIMETER_HPP

#include <iostream>
#include <fstream>
#include <sstream> 
#include <queue>
#include <iostream>
#include <cmath>
#include <iomanip>
#include <numeric>
#include <list>


//statically include only specific files.. i.e. we dont need entire lib.
#ifndef TC_USE_SERIAL
    #define TC_USE_SERIAL
#endif

class altimeter
{
    public:
        /*
         * Construct altimeter with a filepath to be used for logging.
         */
        altimeter(std::string filepath);
        ~altimeter();
        
        // Create the logfile of the altimeter data, consumed by other clients.
        void create_altimeter_logfile(std::string filename);
        
        //create a new request for altimetry
        void create_request(uint32_t seqno, uint32_t millis_elapsed);
        
        // get the last altitude distance in meters.
        float last_altitude_m();
        
        // Interpret altimeter data from serial string.
        // Return false if message cannot be processed.
        bool processMessage(std::string serialmsg);
        
        // Verify that the altimeter is up and running.
        bool altimeter_ok();
    private:
  const std::string _version_string = "1.0";
        
        struct altitude_entry
        {
            int seqno = 0;
            uint32_t millis_elapsed = 0;
            float distance_meters;
        };
        
         /* Strings representing communication commands over serial connection. */
         const std::string   _Response_Distance = "?LD ";
         const std::string   _Response_Info = "? SF11";
         
         const int           _Max_Number_Of_Kept_Altitudes = 30;
        
         /* Member variables */
         std::queue<altitude_entry>  _entry_queue;
         std::list<float>    _last_seen_altitudes;
         bool                _altimeter_ok = false;
         std::string         _serial_data = "";
         std::string         _file_path = "";
         float               _last_altitude_m;
         
         // Log an entry into the logfile.
         void log_altitude_entry(altitude_entry* entry);
        
         // Check that the logfile exists.
         bool log_exists(std::string filename);

         //Append text to existing logfile
         bool append_to_log(std::string filename, std::string appendstring);


};

#endif /* ALTIMETER_HPP */

