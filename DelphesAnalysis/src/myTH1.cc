#include "../interface/TH1Info.h"
#include "../interface/TH1InfoClass.h"
#include "TH1InfoClass.cc"

using namespace std;

template<typename TH1>
void TH1InfoClass<TH1>::initTH1Info()
{
	// Info.push_back( TH1Info( Name,		Title,			xTitle, 	yTitle, 	xUnit,	yUnit, 	Bin, Min, Max ) );
	Info.push_back( TH1Info( "JetPt",		"pT of Jet",	"p_{T}", 	"Yields", 	"GeV", 	"",		500, 0,   500 ) );
	Info.push_back( TH1Info( "JetMass",		"Mass of Jet",	"Mass", 	"Yields", 	"GeV", 	"",		500, 0,   500 ) );

	size=Info.size();
}


