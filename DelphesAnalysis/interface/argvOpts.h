#ifndef ARGVOPTS_CC
#define ARGVOPTS_CC

#include <string>

using namespace std;

class argvOpts{
	public:
		argvOpts();
		argvOpts(char *argv[]);
		void usage(std::string comments="");
		void checking();
		std::string excFile; //argv[0]
		std::string input;   //argv[1]
		std::string output;  //argv[2]
		bool isVal;
		bool debug;         

	private:
		std::string debugstr;//argv[3]
};
#endif
