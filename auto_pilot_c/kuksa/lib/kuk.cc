#include <iostream>
#include <string>

#include "kuksa.grpc.pb.h"
#include "kclient.h"
#include "kuk.h"

using namespace std;
using namespace Tiro;

using kuksa::AuthResponse;
using kuksa::GetResponse;




Kuk::Kuk()
{
    
}



void Kuk::run()
{
    std::string paths[2] = {"Vehicle.Speed", "Vehicle.TraveledDistance"};
    
    KukClient vehicle = KukClient();
    // authorize into kuksa server
    AuthResponse r = vehicle.authorize();

    
    // set a path with float value
    vehicle.set(paths[0], (float)23.5);

    // set a path with float value
    float values[2] = {345.0, 723.0} ;
    vehicle.set(paths, values, 2);
    
    while(true){
        // get single path value from kuksa server
        GetResponse g1 = vehicle.get(paths[0]);
        printf("speed : %f\n", 
            g1.values(0).valuefloat()
        );

        // get array of paths's value from kuksa server
        GetResponse g2 = vehicle.get(paths, 2);
        printf("speed : %f, traveled : %f\n", 
            g2.values(0).valuefloat(), 
            g2.values(1).valuefloat()
        );



        usleep(50000);
		
	}
}
