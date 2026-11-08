#include <iostream>
#include <thread>
#include <memory>
#include <string>
#include <drive.h>







int main(int argc, char** argv) {
	if (argc == 1)
	{
		printf("\n./driver [sqlite path]\n");
	}else{
		printf("using database : %s \n", argv[1]);
		Drive t = Drive(argv[1]);
		t.run();
	}
        
	
	

	
	return 0;
}
