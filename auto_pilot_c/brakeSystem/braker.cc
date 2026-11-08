#include <iostream>
#include <thread>
#include <memory>
#include <string>
#include <brakeSystem.h>







int main(int argc, char** argv) {
	
	brakeSystem b = brakeSystem();
	b.run();
	return 0;
}
