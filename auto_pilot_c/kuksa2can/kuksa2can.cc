#include <iostream>
#include <thread>
#include <memory>
#include <string>
#include <kuk2can.h>







int main(int argc, char** argv) {
	
		if(argc < 3)
		{
			cout << "Input is unvalid! First input is Canbus address and second is signal path!" << endl;
		}
		else
		{
			cout << argv[1] << ":	 " << argv[2] << endl;
			kuk2can k = kuk2can(argv);
			k.run();
		}
		
	
        
	
	

	
	return 0;
}
