#include <iostream>
#include <thread>
#include <memory>
#include <string>
#include <lidar.h>







int main(int argc, char** argv) {
	
	if (argc != 4)
	{
		printf("\n./scan_lidar serial_port1 serial_port2 budrate\n");
	}else{
		Lidar t = Lidar(argv[1],argv[2], atoi(argv[3]));
		t.run();
	}
        
	
	

	
	return 0;
}
