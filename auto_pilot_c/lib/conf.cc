#include "conf.h"

Conf::Conf() {
       Conf("../etc/config.cfg");
}  
Conf::Conf(const char* config_file) {
       cfg.readFile(config_file);
}      
Setting& Conf::root(){
       return cfg.getRoot();
}

