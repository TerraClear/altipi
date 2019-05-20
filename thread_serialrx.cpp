/*
 * Base C++ cross platform threading class 
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

#include "thread_serialrx.hpp"

thread_serialrx::thread_serialrx(altimeter* p_altimeter, 
        std::string serial_path, uint32_t serial_baud) 
{
       _serial_path = serial_path;
       
        //open serial port - Throws XKError..
        _serial1.open(serial_path, (terraclear::Baud) serial_baud);
        
        _p_altimeter = p_altimeter;
        //request altimeter info
        _serial1.writeString(_Request_Info, _Serial_Timeout);
}

thread_serialrx::~thread_serialrx() 
{
    // we do not own the altimeter, so do not free it.
}

void thread_serialrx::create_request(uint32_t seqno, uint32_t millis_elapsed)
{

    if (_p_altimeter == nullptr)
    {
        // TODO (JK, log error here.)
        return;
    }
    //Add request entry in queue
    this->mutex_lock();
        _p_altimeter->create_request(seqno, millis_elapsed);
   
        //make request via serialport, response will come back async and pop queue
        _serial1.writeString(_Request_Distance, _Serial_Timeout);
        
    this->mutex_unlock();
    
}

void thread_serialrx::thread_runloop()
{
    //ensure serial port is open
    if (_serial1.isopen)
    {
        //read any data available 128 bytes at a time
        _serial_data += _serial1.readstring(128, _Serial_Timeout);
        
        //find newline
        size_t nlpos = _serial_data.find('\n') + 1;
        
        //check if string read is \n terminated.
        //if not, might be partial string, so keep and for next.
        if (nlpos > 0)
        {
              //cut out only up to \n terminator.
              std::string serialmsg = _serial_data.substr(0, nlpos);

              //keep remainder around or reset.
              _serial_data = (_serial_data.length() > nlpos) ? _serial_data.substr(nlpos, _serial_data.length()) : "";

              //Process Message..
              // TODO(JK, determine if mutex needed)
              this->mutex_lock();
                   bool messageParsed = _p_altimeter->processMessage(serialmsg);
              this->mutex_unlock();
              
              if (!messageParsed)
              {
                  //bad message response, altimeter not ok..
                  //request altimeter info to reset..
                  _serial1.writeString(_Request_Info, _Serial_Timeout);
              }
         }
    }
}

        
 bool thread_serialrx::altimeter_ok()
 {
     if (_p_altimeter != nullptr)
     {
         return _p_altimeter->altimeter_ok();
     }
     return false;
 }
