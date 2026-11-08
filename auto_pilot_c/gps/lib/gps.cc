#include <iostream>

#include <string>
#include <thread>
#include <unistd.h>
#include <nmea.h>
#include <nmea/gpgll.h>
#include <nmea/gpgga.h>
#include <nmea/gprmc.h>
#include <libserial/SerialPort.h>
#include <gps.h>


// using kuksa::AuthResponse;
// using kuksa::GetResponse;


using namespace std;
using namespace LibSerial;


Gps::Gps(string device)
{
	try
	{
		serial_port.Open(device) ;
	}
	catch (const OpenFailed&)
	{
		std::cerr << "The serial port did not open correctly." << std::endl ;
		exit(0);
	}
	serial_port.SetBaudRate(BaudRate::BAUD_921600) ;
	serial_port.SetCharacterSize(CharacterSize::CHAR_SIZE_8) ;
	serial_port.SetFlowControl(FlowControl::FLOW_CONTROL_NONE) ;
	serial_port.SetParity(Parity::PARITY_NONE) ;
	serial_port.SetStopBits(StopBits::STOP_BITS_1) ;
	while(!serial_port.IsDataAvailable()) 
	{
		usleep(1000) ;
	}

	AuthResponse r = vehicle.authorize();

	

}


void Gps::run(){
    int timeout_ms = 5000; // timeout value in milliseconds
    char next_char;      // variable to store the read result
	int counter = 0;
    char* msg = new char[256];
	while(true){
		serial_port.ReadByte( next_char, timeout_ms );
		if(next_char == '$'){
			msg[counter] = '\0';
				process(msg);
			counter = 0;
		}
		msg[counter] = next_char;
		counter++;
	}
}

void Gps::process(char *msg){
	nmea_s *data;
	data = nmea_parse(msg, strlen(msg), 0);
	if (NULL != data) {
		if (NMEA_GPGGA == data->type) {
			nmea_gpgga_s *g = (nmea_gpgga_s *) data;
			double _l;
			char l[64];
			if(g->latitude.degrees > 30/* && g->position_fix == 4*/)
			{
				_l = g->latitude.degrees + (g->latitude.minutes / 60);
				sprintf(l, "%.8f", _l);
				values[0] = atof(l);
			}
			if(g->longitude.degrees > 50/* && g->position_fix == 4*/)
			{
				_l = g->longitude.degrees + (g->longitude.minutes / 60);
				sprintf(l, "%.8f", _l);
				values[1] =  atof(l);
			}
			if(g->latitude.degrees > 30 && g->longitude.degrees > 50){
				fix = g->position_fix;
			}	
			update_kuksa();
			
		}
	}

}

void Gps::update_kuksa(){

	vehicle.set(paths, values, 2);	
}
