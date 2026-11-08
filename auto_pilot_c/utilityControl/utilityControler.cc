#include <iostream>
#include <thread>
#include <memory>
#include <string>
#include <utilityControl.h>







int main(int argc, char** argv) {
	
	utilityControl uc = utilityControl();
	uc.run();
	return 0;
}
