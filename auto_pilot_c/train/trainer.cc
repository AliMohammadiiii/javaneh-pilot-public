#include <iostream>
#include <thread>
#include <memory>
#include <string>
#include <train.h>







int main(int argc, char** argv) {
	if (argc == 1)
	{
		printf("\n./trainer [sqlite path]\n");
	}else{
		Train t = Train(argv[1]);
		t.run();
	}
	
	return 0;
}
