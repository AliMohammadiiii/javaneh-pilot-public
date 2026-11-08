#include <conf.h>
#include <gps.h>

using namespace libconfig;


int main(int argc, char** argv)
{
    Conf *c = new Conf(argv[1]);
    const Setting& root = c->root();
    const Setting &gps = root["gpsd"];
    string deviceP, deviceN;
    gps.lookupValue("deviceP", deviceP);
    if (argc != 2)
	{
		printf("\n./gpsd config_file[../etc/config.cfg]\n");
	}else{
        Gps gps = Gps(deviceP);
        gps.run();
        return EXIT_SUCCESS ;
	}
}
