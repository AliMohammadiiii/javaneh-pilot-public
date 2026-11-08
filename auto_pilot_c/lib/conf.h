#ifndef CONFIG_H
#define CONFIG_H
#endif
#include <iostream>
#include <libconfig.h++>

using namespace std;
using namespace libconfig;


class Conf
{				
	public:
		Config cfg;
		Conf();
		Conf(const char* config_file);
		Setting& root();
};

