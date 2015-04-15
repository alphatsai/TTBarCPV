#include "../interface/TH1Info.h"
#include "../interface/TH1InfoClass.h"
#include <vector>
using namespace std;

//* Do it before call createTH1
template<typename TH1>
TH1InfoClass<TH1>::TH1InfoClass( bool deBug )
{
	debug=deBug;	
	//initTH1Info();
	size=Info.size();
	if( debug ) printf(">> [DEBUG] TH1InfoClass constructed, %d TH1s are prepared!\n", size);
}
template<typename TH1>
void TH1InfoClass<TH1>::addNewTH1(std::string name, std::string title, std::string xtitle, std::string ytitle, std::string xunit, std::string yunit, int bin, double min, double max)
{
	Info.push_back( TH1Info( name, title, xtitle, ytitle, xunit, yunit, bin, min, max ) );
	size=Info.size();
	if( debug ) printf(">> [DEBUG] Add new TH1 %s bin/min/max:%d/%.2f/%.2f, new size is %d\n", name.c_str(), bin, min, max, size);
}
/*
// This is example:
template<typename TH1>
void TH1InfoClass<TH1>::initTH1Info()
{
	// Info.push_back( TH1Info( Name,		Title,			xTitle, 	yTitle, 	xUnit,	yUnit, 	Bin, Min, Max ) );
	// Info.push_back( TH1Info( "JetPt",		"pT of Jet",	"p_{T}", 	"Yields", 	"GeV", 	"",		500, 0,   500 ) );
	//Info.push_back( TH1Info( "JetPt",		"pT of Jet",	"p_{T}", 	"Yields", 	"GeV", 	"",		500, 0,   500 ) );
	//Info.push_back( TH1Info( "JetMass",		"Mass of Jet",	"Mass", 	"Yields", 	"GeV", 	"",		500, 0,   500 ) );
}
*/

//* Create Histogram
template<typename TH1> 
void TH1InfoClass<TH1>::CreateTH1(){
	if( debug ) printf(">> [DEBUG] %d TH1s will be created...\n", size);
	for(int i=0; i<size; i++){
		if( debug ) printf(">> [DEBUG] %d %s created, in Bin/Min/Max:%d/%.2f/%.2f\n",i, Info[i].Name.c_str(), Info[i].Bin, Info[i].Min, Info[i].Max);
		indexTH1[Info[i].Name] = i;
		mapTH1[Info[i].Name] = new TH1(Info[i].Name.c_str(),"",Info[i].Bin, Info[i].Min, Info[i].Max);
	}
}
template<typename TH1> 
void TH1InfoClass<TH1>::CreateTH1( TFile* f, std::string dir_name){
	printf(">> %d TH1s will be got...\n", size);
	for(int i=0; i<size; i++){ 
		if( debug ) printf(">> [DEBUG] %d %s created, in Bin/Min/Max:%d/%.2f/%.2f\n",i, Info[i].Name.c_str(), Info[i].Bin, Info[i].Min, Info[i].Max);
		indexTH1[Info[i].Name] = i;
		mapTH1[Info[i].Name] =(TH1*)f->Get( (dir_name+Info[i].Name).c_str() );
	}
}

//* Set some option for Histogram
template<typename TH1> 
void TH1InfoClass<TH1>::SetTitles(){
	for(int i=0; i<size; i++){ 
		//mapTH1[Info[i].Name]->SetTile(Info[i]._title.c_str());
		string xt, yt;
		if( Info[i].xUnit.size() == 0 ) xt = Info[i].xTitle;
		else xt = Info[i].xTitle+" ["+Info[i].xUnit+"]";	
		if( Info[i].yUnit.size() == 0 ) yt = Info[i].yTitle;
		else yt = Info[i].yTitle+" ["+Info[i].yUnit+"]";	
		mapTH1[Info[i].Name]->SetXtitle( xt.c_str() );
		mapTH1[Info[i].Name]->SetYtitle( yt.c_str() );
	}
}

template<typename TH1> 
void TH1InfoClass<TH1>::Sumw2(){
	for(int i=0; i<size; i++){ 
		mapTH1.find(Info[i].Name)->second->Sumw2();
	}
}

//* Get Histogram
template<typename TH1> 
TH1* TH1InfoClass<TH1>::GetTH1(std::string name){
	TH1* th1;
	if( mapTH1.find(name) == mapTH1.end() ){
		printf(">> [ERROR] %s is not found in TH1InfoClass::GetTH1(std::string)\n", name.c_str());
	}else{	
		th1=mapTH1.find(name)->second;
	}
	return th1;
}

//* Get Info variables
template<typename TH1> 
TH1Info TH1InfoClass<TH1>::GetInfo(std::string name){
	TH1Info info;
	if( mapTH1.find(name) == mapTH1.end() ){
		printf(">> [ERROR] %s is not found in TH1InfoClass::GetInfo(std::string)\n", name.c_str());
	}else{	
		info=Info[indexTH1.find(name)->second];
	}
	return info;
}
template<typename TH1> 
TH1Info TH1InfoClass<TH1>::GetInfo(int index){
	TH1Info info;
	if( index >= size ){
		printf(">> [ERROR] %d is over the size(%d) in TH1InfoClass::GetInfo(int)\n", index, size);
	}else{
		info=Info[index];
	}
	return info;
}


