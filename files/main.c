#include <stdio.h>
#include "assembly_execution.h"

int main(int argc, char *argv[]) {
	
	if (argc == 1)
	{
		run_in_interactive();
	}
	else if (argc == 2){
		printf("Parameters are incorrect, please read readme.user.");
	}
	else if (argc == 3){
		if ( string_compare( argv[2], "-pas" , 0) )
		{
			run_a_file(argv[1],"output.hex", NULL);
		}
		else if( string_compare( argv[2], "-compile" , 0) ){
			compile_file(argv[1],"output_debug.hex");
		}
		else{
			printf("Parameters are incorrect, please read readme.user.");
		}
	}
	else if (argc == 4){
		run_a_file(argv[1], argv[2], argv[3]);
	}
	else {
		printf("Parameters are incorrect, please read readme.user.");
	}

	return 0;
}

