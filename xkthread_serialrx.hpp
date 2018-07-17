#ifndef XKTHREAD_SERIALRX_HPP
#define XKTHREAD_SERIALRX_HPP

#include <iostream>
#include <fstream>
#include <sstream> 
#include <queue>

#include "../tcpserial/xkthread.hpp"
#include "../tcpserial/xkserial.hpp"

class xkthread_serialrx : public xk::xkthread
{
    public:
        xkthread_serialrx(std::string file_path, std::string serial_path, uint32_t serial_baud);
        virtual ~xkthread_serialrx();
        
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
        xk::xkserial        _serial1;
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

#endif /* XKTHREAD_SERIALRX_HPP */

