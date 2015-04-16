#include <string>
#include <cstring>
#include "../interface/argvOpts.h"

using namespace std;

argvOpts::argvOpts( int argc, char *argv[] )
{
	argvSize = argc;
	excFile  = argv[0];
	input    = ( argv[1] == NULL ) ? "" : argv[1];
	output   = ( argv[2] == NULL ) ? "" : argv[2];
	debugstr = ( argv[3] == NULL ) ? "" : argv[3];
	debug=false;
	isVal=false;
	checking();
}

void argvOpts::checking()
{
	if( input.size() == 0 ){
		usage();
	}else{
		printf(">> Input: %s\n", input.c_str());
		isVal=true;
	}
	if( isVal ){
		if( output.size() == 0 ){
			usage("[ERROR] No output file");
			isVal=false;
		}else{
			if( output.find(".root") == std::string::npos){ 
				usage("[ERROR] "+output+" is not root file for output");
				isVal=false;
			}
		}
	}
	if( debugstr.compare("True") == 0 ||  debugstr.compare("true") == 0 || debugstr.compare("1") == 0 ) debug=true;
}

void argvOpts::usage(std::string comments)
{
	if( comments.size() != 0 ) printf(">> %s\n", comments.c_str());
	printf(">> Usage:\n");
	printf(">> Ex: %s input.root output.root [Ture]\n", excFile.c_str());
}
