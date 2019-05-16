/*
/*
 * This application measures distances with SF11/C Lidar.
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

#include <vector>

#include "altimeter.hpp"

altimeter::altimeter(std::string filepath) 
{
    this->_altimeter_ok = false;
    _file_path = filepath;
    create_altimeter_logfile(filepath);
    _last_altitude_m = 0.0;
    _last_seen_altitudes = {};
}

altimeter::~altimeter() 
{
}

void altimeter::create_altimeter_logfile(std::string filename)
{
    //file header string..
    
    std::string initstring = "#VERSION_0.03 - SEQUENCE,TIME_MS,DISTANCE\r\n";

    //create blank new file, overwriting any existing files
    std::ofstream outfile(filename);
    outfile << initstring;    
    outfile.close();    
}
        
void altimeter::log_altitude_entry(altitude_entry* entry)
{
    //write to file
    std::ostringstream strstrm;
    strstrm <<  entry->seqno << "," << entry->millis_elapsed << ","  
            <<  std::fixed << std::setprecision(2) << entry->distance_meters << std::endl;
    append_to_log(_file_path, strstrm.str());
}

void altimeter::create_request(uint32_t seqno, uint32_t millis_elapsed)
{
    altitude_entry entry;
    entry.seqno = seqno;
    entry.millis_elapsed = millis_elapsed;
    
    //save request to queue
    _entry_queue.push(entry);
    
     // response will come back async, handled by thread_serialRX
}

float altimeter::last_altitude_m()
{
    return this->_last_altitude_m;
}

bool altimeter::processMessage(std::string serialmsg)
{
    //check if serial message is distance response or other..
    if (serialmsg.substr(0, _Response_Distance.length()) == _Response_Distance) {


        //convert to float
        float distance_m = std::stof(serialmsg.substr(_Response_Distance.length(), serialmsg.length() - _Response_Distance.length()));

        if (_last_seen_altitudes.size() > _Max_Number_Of_Kept_Altitudes) {
            // kick out oldest value
            _last_seen_altitudes.pop_back();
        }
        // add altitude to last seen altitudes
        _last_seen_altitudes.push_front(distance_m);

        float average_distance_m = std::accumulate(
                _last_seen_altitudes.begin(),
                _last_seen_altitudes.end(),
                0.0) / static_cast<float> (_last_seen_altitudes.size());
               
        //std::cout << "Distance is " << average_distance_m 
        //      << "; size is " << _last_seen_altitudes.size() << std::endl;


        _last_altitude_m = average_distance_m;

    }
    else if (serialmsg.substr(0, _Response_Info.length()) == _Response_Info)
    {
        //received info string, i.e. serial is good..
        std::cout << "Altimeter OK: " << serialmsg ;
        _altimeter_ok = true;
        
    }
    else
    {
        //bad message response, altimeter not ok..
        std::cout << "Altimeter ERROR: " << serialmsg ;
        _altimeter_ok = false;
        
        return false;
    }

    //check if there is any corresponding requests waiting.
    // if there are, log a distance measurement.
    if (_entry_queue.size() > 0) {

        //pop FIFO item.
        altitude_entry entry = _entry_queue.back();
        _entry_queue.pop();

        //distance in meters..
        entry.distance_meters = _last_altitude_m;
        std::cout << "Distance logged is " << _last_altitude_m << std::endl;

        log_altitude_entry(&entry);
    }
    return true;
}
        

 bool altimeter::altimeter_ok()
 {
     return _altimeter_ok;
 }

 bool altimeter::log_exists(std::string filename)
{
    std::ifstream ifile(filename.c_str());
    return (bool)ifile;
}

//Append text to existing file
bool altimeter::append_to_log(std::string filename, std::string appendstring)
{
    bool retval = false;
    try
    {
        std::ofstream outfile;
        outfile.open(filename, std::ios_base::app);
        outfile << appendstring; 
        retval = true;
    }
    catch (std::exception ex)
    {
        std::cout << "ERROR Appending File: " << ex.what();
    }

    return retval;
}

