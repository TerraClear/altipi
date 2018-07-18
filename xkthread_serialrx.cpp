/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   xkthread_serialrx.cpp
 * Author: koos
 * 
 * Created on July 17, 2018, 10:39 AM
 */

#include "xkthread_serialrx.hpp"

xkthread_serialrx::xkthread_serialrx(std::string file_path, std::string serial_path, uint32_t serial_baud) 
{
       _serial_path = serial_path;
       _file_path = file_path;
       
        //open serial port - Throws XKError..
        _serial1.open(serial_path, (xk::XKBaud) serial_baud);
        
        //init file
        createFile(_file_path);
        
        //request altimeter info
        _serial1.writeString(_Request_Info, _Serial_Timeout);
}

xkthread_serialrx::~xkthread_serialrx() 
{
}

void xkthread_serialrx::create_request(uint32_t seqno, uint32_t millis_elapsed)
{
    altitude_entry entry;
    entry.seqno = seqno;
    entry.millis_elapsed = millis_elapsed;
    
    //Add request entry in queue
    this->mutex_lock();
        //save request to queue
        _entry_queue.push(entry);
   
        //make request via serialport, response will come back async and pop queue
        _serial1.writeString(_Request_Distance, _Serial_Timeout);
        
    this->mutex_unlock();
    
}

void xkthread_serialrx::thread_runloop()
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
            processMessage(serialmsg);
        }
            
       
    }
     
}

void xkthread_serialrx::processMessage(std::string serialmsg)
{
    //check if serial message is distance response or other..
    if (serialmsg.substr(0, _Response_Distance.length()) == _Response_Distance)
    {
        this->mutex_lock();
           //check if there is any corresponding requests waiting..
           if (_entry_queue.size() > 0)
           {
                std::cout << "Distance: " << serialmsg ;
                
               //pop FIFO item.
               altitude_entry entry = _entry_queue.back();
               _entry_queue.pop();

               //write to file
               std::ostringstream strstrm;
               strstrm <<  entry.seqno << "," << entry.millis_elapsed << "," << serialmsg.substr(_Response_Distance.length(), 4) << std::endl;
               
               appendFile(_file_path, strstrm.str());

           }
       this->mutex_unlock();
        
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
        
        //request altimeter info to reset..
        _serial1.writeString(_Request_Info, _Serial_Timeout);
    }

    
}
        
 bool xkthread_serialrx::altimeter_ok()
 {
     return _altimeter_ok;
 }

bool xkthread_serialrx::fexists(std::string filename)
{
    std::ifstream ifile(filename.c_str());
    return (bool)ifile;
}
 
void xkthread_serialrx::createFile(std::string filename)
{
    //file header string..
    std::string initstring = "\"SEQUENCE\",\"TIME\",\"DISTANCE\"\r\n";

    //create blank new file, overwriting any existing files
    std::ofstream outfile(filename);
    outfile << initstring;    
    outfile.close();    
}

//Append text to existing file
bool xkthread_serialrx::appendFile(std::string filename, std::string appendstring)
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
