using namespace std;
bool argvOpts( char *argv[] )
{
	bool isVal=false;
	if( argv[1] == NULL ){
		printf(">> Please input file.\n"); 
		printf(">> Ex: %s tag_1_delphes_events.root\n", argv[0]);
		isVal=false;
	}else{
		printf(">> Loaded file: %s\n", argv[1]);
		isVal=true;
	}
	if( isVal ){
		if( argv[2] == NULL ){
			argv[2]="result.root";
			isVal=true;
		}else{
			std::string str(argv[2]);
			if( str.find(".root") == std::string::npos){ 
				printf(">> [ERROR]: \"%s\" is not root file for output\n", argv[2]);
				printf(">> [ERROR]: Please do like: %s %s result.root\n", argv[0], argv[1]);
				isVal=false;
			}
		}
	}
	return isVal;
}
