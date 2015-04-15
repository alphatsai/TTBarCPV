#ifndef TH1INFOCLASS_H
#define TH1INFOCLASS_H

#include "TH1Info.h"

using namespace std;

template<typename TH1> 
class TH1InfoClass{
	public:
		TH1InfoClass( bool deBug=0 );
		void addNewTH1(std::string name, std::string title, std::string xtitle, std::string ytitle, std::string xunit, std::string yunit, int bin, double min, double max);
		void initTH1Info(); // Be writen in custumer way
		void CreateTH1();
		void CreateTH1(TFile* f, std::string dir_name=""); 
		void SetTitles(); 
		void Sumw2();
		TH1* GetTH1(std::string name);
		TH1Info GetInfo(std::string name);
		TH1Info GetInfo(int index);
		
		bool debug;
		int size;

	private:
		map<std::string, TH1*> mapTH1;
		map<std::string, int> indexTH1;
		vector<TH1Info> Info; 
};
#endif

