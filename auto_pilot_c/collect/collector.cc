/**
 *  @example serial_port_read.cpp
 */

#include <libserial/SerialPort.h>

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <data_collect.h>

int main()
{
    DataCollector collect = DataCollector();
    collect.run();
    return EXIT_SUCCESS ;
}
