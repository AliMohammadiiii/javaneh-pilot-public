#include <iostream>
#include <thread>
#include <memory>
#include <string>
#include <can2kuksa.h>
#include <linux/can.h>







int main(int argc, char** argv) {
	
		if(argc < 3)
		{
			cout << "Input is unvalid! First input is Canbus address and second is signal path!" << endl;
		}
		else
		{
			cout << argv[1] << ":	 " << argv[2] << endl;
			Can2kuksa c = Can2kuksa(argv);
			c.run();
		}
		
	return 0;
}
