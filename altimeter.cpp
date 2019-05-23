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

altimeter::altimeter(std::string filepath, std::string debug_filepath) 
{
    this->_altimeter_ok = false;
    _file_path = filepath;
    _debug_file_path = debug_filepath;
    create_altimeter_logfile(filepath);
    create_altimeter_logfile(_debug_file_path);
    _last_altitude_m = 0.0;
    _last_seen_altitudes = {};
    _last_entry.seqno = 0;
    _last_entry.millis_elapsed = 0;
    _last_entry.distance_meters = -1.0;  // IE undefined
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
        
void altimeter::log_altitude_entry(altitude_entry* entry, std::string filename)
{
    //write to file
    std::ostringstream strstrm;
    strstrm <<  entry->seqno << "," << entry->millis_elapsed << ","  
            <<  std::fixed << std::setprecision(2) << entry->distance_meters << std::endl;
    append_to_log(filename, strstrm.str());
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
    // A message is a distance if it is preceeded by _Response_Distance or is a float.
    auto checkIfFloat = [this, serialmsg]() {
        try {
             float distance_m = std::stof(serialmsg.substr(1, serialmsg.length()));
        } catch (std::exception ex)
        {
            
            std::cout << "Error converting to float: " << ex.what();
            return false;
        }
        return true;
    };
   
    if (serialmsg.substr(0, _Response_Info.length()) == _Response_Info ||
        (serialmsg.compare(_Response_Continuous_Mode) == 0))
    {
        //received info string, i.e. serial is good..
        std::cout << "Altimeter OK: " << serialmsg ;
        _altimeter_ok = true;
        
    }
    //check if serial message is distance response or other..
    else if (serialmsg.substr(0, _Response_Distance.length()) == _Response_Distance ||
             checkIfFloat() ){
        //convert to float

        float distance_m = std::stof(serialmsg.substr(1, serialmsg.length()));

        if (_last_seen_altitudes.size() > _Max_Number_Of_Kept_Altitudes) {
            // kick out oldest value
            _last_seen_altitudes.pop_back();
        }
        
        // add altitude to last seen altitudes
        _last_seen_altitudes.push_front(distance_m); 
              
        // If we have a full queue, sanity check value, if it is outside some number 
        // of standard deviations, it is likely a bad reading.
        // If we don't have a full queue, we are at the beginning of the flight and 
        // need to build up our data.
        if (_last_seen_altitudes.size() < _Max_Number_Of_Kept_Altitudes || 
                is_within_two_standard_deviations(distance_m))
        {
              
              _last_altitude_m = distance_m;
              _last_entry.distance_meters = distance_m;
        }
        
        // TODO(JK, Log distance seen here.)
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

        log_altitude_entry(&entry, _file_path);
        
        _last_entry = entry;
    }
    
    // log the distance for debug reasons in a separate file.
    if (_last_entry.distance_meters > -1)
    {
        log_altitude_entry(&_last_entry, _debug_file_path);
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

float altimeter::get_mean_altitude()
{
     return std::accumulate(
                _last_seen_altitudes.begin(),
                _last_seen_altitudes.end(),
                0.0) / static_cast<float> (_last_seen_altitudes.size());
}
         
float altimeter::get_median_altitude()
{
    // copy list into array so we can use std::sort on it.
    float copy_altitudes[_last_seen_altitudes.size()];
        
    std::copy(_last_seen_altitudes.begin(), _last_seen_altitudes.end(), copy_altitudes);
        
    std::sort(copy_altitudes, 
         copy_altitudes+sizeof(copy_altitudes)/sizeof(copy_altitudes[0]));
        
    // get the middle value of the sorted array
    float median_distance_m = copy_altitudes[(_last_seen_altitudes.size() + 1)/2];
    
    //std::cout << "median is " << median_distance_m << std::endl;
    
    return median_distance_m;
}
         
bool altimeter::is_within_two_standard_deviations(float latest_altitude) {
   
    float mean = get_mean_altitude();

    std::vector<float> diff(_last_seen_altitudes.size());
    std::transform(_last_seen_altitudes.begin(),
            _last_seen_altitudes.end(),
            diff.begin(), 
            [mean](float x) { return x - mean; });
    float sq_sum = std::inner_product(diff.begin(), diff.end(), diff.begin(), 0.0);
    float stdev = std::sqrt(sq_sum / _last_seen_altitudes.size());
    
    //std::cout << "mean = " << mean << " and stdev = " << stdev << std::endl;
    
    
    /* This is some math that checks if the stdev is 0.  We have to use
       an epsilon to check because we are dealing with imprecise floating
       point numbers.
       If the stdev is 0, we are saying that all the altitude readings are the same,
       which is a valid case if the aircraft is say, being held statically above ground.
       It is an edge case. 
     */
    auto checkIfStdevNearZero = [stdev]() {
        const double epsilon = 1e-5;
        return std::abs(stdev - 0.0) <= epsilon * std::abs(0.0);
    };
    
    if (checkIfStdevNearZero())
    {
        // latest altitude is within one standard deviation of previous values.
        //std::cout << "Usable altitude" << std::endl;
        return true; // All our numbers are the same.
    }
    
    // Check that the new value is within one standard deviation of the mean.
    if (latest_altitude <= (mean + 2*stdev) && 
            latest_altitude >= (mean - 2*stdev) )
    {
        // latest altitude is within one standard deviation of previous values.
        //std::cout << "Usable altitude" << std::endl;
        return true;
    }
    
        // latest altitude is within one standard deviation of previous values.
        //std::cout << "THROWING OUT ALTITUDE " << latest_altitude << std::endl;
    return false;
}
