#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <fstream>
#include <chrono>

#ifdef __APPLE__
    #include <json/json.h>
#else
    #include <jsoncpp/json/json.h>
#endif

#ifdef __arm__
    #include <wiringPi.h>
#else
    #include "wiringPi_stub.h"
#endif

#include "pipinmap.hpp"
#include "../tcpserial/xkserial.hpp"


const std::string _outfilename = "altimetry.txt";

xk::xkserial _serial1;
std::chrono::steady_clock::time_point _lastmeasure;


//check if file exists..
bool fexists(const std::string& filename) 
{
  std::ifstream ifile(filename.c_str());
  return (bool)ifile;
}

void createFile(std::string filename)
{
    //init string..
    std::string initstring = "SEQUENCE\tTIME\tDISTANCE\r\n";

    //create blank new file
    std::ofstream outfile(filename);
    outfile << initstring;    
    outfile.close();
}



bool appendFile(std::string filename, std::string appendstring)
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
        std::cerr << ex.what();
    }
    
    return retval;
}


std::string readJSON(std::string jsonfile)
{
    std::string retstr = "";
    
    if (fexists(jsonfile.c_str()))
    {
        //read json file..
        Json::Value json_obj;
        std::ifstream json_fstream(jsonfile, std::ifstream::binary);
        json_fstream >> json_obj; 
        
        retstr = json_obj.toStyledString();
        
    }
    
    return retstr;
}

void createJSON(std::string jsonfile)
{
    //reset stopwatch / clock
   _lastmeasure = std::chrono::steady_clock::now();

    //Generate JSON Obj
    Json::Value altitude_json;

    altitude_json["trigger-count"] = 0;
    
     //array for measurements.                     
    Json::Value measurements(Json::arrayValue);

    //add initial box..
    Json::Value valinit;
    valinit["measure-id"] = 0;
    valinit["measure-time"] = 0;
    valinit["measure-distance"] = 0.00f;
       
    measurements.append(valinit);
    
    altitude_json["measurements"] = measurements;
    
    string json_string = altitude_json.toStyledString();

    std::ofstream json_file(jsonfile);
    json_file << json_string;    
    json_file.close();
}

void saveJSON(std::string jsonfile, float distanceFt)
{
    //std::string 
    //Generate JSON Obj
    //Json::Value bbox_json;
}

void trigger_pulse()
{
   std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();

   std::cout << "elapsed: "
             << std::chrono::duration_cast<std::chrono::microseconds>(now - _lastmeasure).count()
             << "us." << std::endl;

   _lastmeasure = std::chrono::steady_clock::now();

    //write client msg to serial port..
    //_serial1.writeString("?LD\r\n", 500);

    //std::string retstr = _serial1.readstring(32, 500);
    
    //    std::cout << retstr;
}

int main(int argc, char** argv) 
{

    createFile(_outfilename);
    appendFile(_outfilename, "1\t231455\t34.12\r\n");
    
    return 0;
        std::string serial_path = "/dev/ttyS0";

        //if port is supplied use supplied path..
        if (argc > 1)
        {
           serial_path = argv[1];
        }

        _serial1.open(serial_path, xk::BAUD_115200);

	//setup wiringPi in GPIO pin numbering mode..
    	wiringPiSetupGpio();

	//set IO pin state
	pinMode(GPIO_18, OUTPUT);
	pinMode(GPIO_23, INPUT);

        wiringPiISR(GPIO_23, INT_EDGE_FALLING, &trigger_pulse);
        
        bool toggle = false;
        while(1)
        {
            //enable LED
            toggle = !toggle;
            digitalWrite(GPIO_18, toggle);
            
            usleep(100000);        
        }

       
    	return 0;

}

