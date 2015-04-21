#include <vector>
#include "TLorentzVector.h"
#include "classes/DelphesClasses.h"
using namespace std;
//Matching functions
template <class delphesObj>
void sort2HighPt( vector<delphesObj> col, delphesObj &obj1, delphesObj &obj2 )
{
	int o1, o2;
	double pt1, pt2;	
	pt1=pt2=0;
	for( int i=0; i<col.size(); i++){
		if( pt1 < col[i].PT ){
			pt2=pt1;
			pt1=col[i].PT;
			o2=o1;
			o1=i;
		}else if( pt2 < col[i].PT ){
			pt2=col[i].PT;
			o2=i;
		}
	}
	obj1=col[o1];	
	obj2=col[o2];
}
template <class delphesObj>
void sort2HighPt( vector<delphesObj> col, vector<delphesObj>& obj12 )
{
	int o1, o2;
	double pt1, pt2;	
	pt1=pt2=0;
	for( int i=0; i<col.size(); i++){
		if( pt1 < col[i].PT ){
			pt2=pt1;
			pt1=col[i].PT;
			o2=o1;
			o1=i;
		}else if( pt2 < col[i].PT ){
			pt2=col[i].PT;
			o2=i;
		}
	}
	obj12.push_back(col[o1]);	
	obj12.push_back(col[o2]);	
}
void extractNonTopBjet( vector<Jet> jetCol1, vector<Jet> bjetCol, vector<Jet>& newjetCol)
{
	for( int i=0; i<jetCol1.size(); i++){
		bool isbjet=false;
		for( int j=0; j<bjetCol.size(); j++) 
			if( jetCol1[i].P4() == bjetCol[j].P4() ) isbjet=true; 
		if( !isbjet ) newjetCol.push_back( jetCol1[i] );
	}
}
Jet compareDeltaRHighPt( Jet jet, vector<Jet> jetCol )
{
	TLorentzVector jetP4 = jet.P4();
	double dR1=10000, dR2=10000;
	int j1, j2;
	for( int i=0; i<jetCol.size(); i++){
		if( jetP4 == jetCol[i].P4() ) continue;
		if( jetP4.DeltaR(jetCol[i].P4()) < dR1 )
		{ 
			dR2 = dR1;
			dR1 = jetP4.DeltaR( jetCol[i].P4() );
			j2 = j1;
			j1 = i;
		}else if( jetP4.DeltaR(jetCol[i].P4()) < dR2 ) {
			dR2 = jetP4.DeltaR( jetCol[i].P4() );
			j2 = i;
		}
	}
	if( jetCol[j1].PT > jetCol[j2].PT ) return jetCol[j1];
	else return jetCol[j2];
}
TLorentzVector compareDeltaRHighPt( TLorentzVector jetP4, vector<Jet> jetCol )
{
	double dR1=10000, dR2=10000;
	int j1, j2;
	for( int i=0; i<jetCol.size(); i++){
		if( jetP4 == jetCol[i].P4() ) continue;
		if( jetP4.DeltaR(jetCol[i].P4()) < dR1 )
		{ 
			dR2 = dR1;
			dR1 = jetP4.DeltaR( jetCol[i].P4() );
			j2 = j1;
			j1 = i;
		}else if( jetP4.DeltaR(jetCol[i].P4()) < dR2 ) {
			dR2 = jetP4.DeltaR( jetCol[i].P4() );
			j2 = i;
		}
	}
	if( jetCol[j1].PT > jetCol[j2].PT ) return jetCol[j1].P4();
	else return jetCol[j2].P4();
}
GenParticle *getMatchedGenParticle(Jet *jet, TClonesArray *branchParticle, int PID = -1)
{
    GenParticle *gen_matched = NULL;
    double dR_matched = 0.3; // threshold dR = 0.3
    
    for (int idx=0; idx<branchParticle->GetEntries(); idx++) {
        GenParticle *gen = (GenParticle*)branchParticle->At(idx);
        
        if (gen->Status!=3) continue;

        if (PID>0 && abs(gen->PID)!=PID) continue; //specific particle code
        if (PID==0) { // any light-quark jet
            if (abs(gen->PID)!=1 && abs(gen->PID)!=2 &&
                abs(gen->PID)!=3 && abs(gen->PID)!=4) continue;
        }
        if (PID<0) { // any jet
            if (abs(gen->PID)!=1 && abs(gen->PID)!=2 && abs(gen->PID)!=3 &&
                abs(gen->PID)!=4 && abs(gen->PID)!=5 && abs(gen->PID)!=21) continue;
        }

        double dR = gen->P4().DeltaR(jet->P4());
        if (dR<dR_matched) {
            dR_matched = dR;
            gen_matched = gen;
        }
    }
    return gen_matched;
}

//3D vector functions
double dotP3( double x1, double y1, double z1, double x2, double y2, double z2 )
{
	return (x1*x2)+(y1*y2)+(z1*z2);
}
double dotP3( TLorentzVector v1, TLorentzVector v2 ){ return dotP3( v1.Px(), v1.Py(), v1.Pz(), v2.Px(), v2.Py(), v2.Pz() ); }
double dotP3( TLorentzVector v1, vector<double> v2 ){ return dotP3( v1.Px(), v1.Py(), v1.Pz(), v2[0], v2[1], v2[2] ); }
double dotP3( vector<double> v1, TLorentzVector v2 ){ return dotP3( v1[0], v1[1], v1[2], v2.Px(), v2.Py(), v2.Pz() ); }
double dotP3( vector<double> v1, vector<double> v2 ){ return dotP3( v1[0], v1[1], v1[2], v2[0], v2[1], v2[2] ); }

vector<double> crossP3( double x1, double y1, double z1, double x2, double y2, double z2)
{
	vector<double> product;
	product.push_back( (y1*z2)-(z1*y2) ); //x
	product.push_back( (z1*x2)-(x1*z2) ); //y
	product.push_back( (x1*y2)-(y1*x2) ); //z 	
	return product;
}
vector<double> crossP3( TLorentzVector v1, TLorentzVector v2 ){ return crossP3( v1.Px(), v1.Py(), v1.Pz(), v2.Px(), v2.Py(), v2.Pz() ); }
vector<double> crossP3( TLorentzVector v1, vector<double> v2 ){ return crossP3( v1.Px(), v1.Py(), v1.Pz(), v2[0], v2[1], v2[2] ); }
vector<double> crossP3( vector<double> v1, TLorentzVector v2 ){ return crossP3( v1[0], v1[1], v1[2], v2.Px(), v2.Py(), v2.Pz() ); }
vector<double> crossP3( vector<double> v1, vector<double> v2 ){	return crossP3( v1[0], v1[1], v1[2], v2[0], v2[1], v2[2] ); }

vector<double> addP3( double x1, double y1, double z1, double x2, double y2, double z2, int isSubtract=0 )
{
	vector<double> newP3;
	if( isSubtract < 0 ){
		newP3.push_back( x1-x2 );	
		newP3.push_back( y1-y2 );	
		newP3.push_back( z1-z2 );	
	}else{
		newP3.push_back( x1+x2 );	
		newP3.push_back( y1+y2 );	
		newP3.push_back( z1+z2 );	
	}
	return newP3;
}
vector<double> addP3( TLorentzVector v1, TLorentzVector v2, int isSubtract=0 ){ return addP3( v1.Px(), v1.Py(), v1.Pz(), v2.Px(), v2.Py(), v2.Pz(), isSubtract ); }
vector<double> addP3( TLorentzVector v1, vector<double> v2, int isSubtract=0 ){ return addP3( v1.Px(), v1.Py(), v1.Pz(), v2[0], v2[1], v2[2], isSubtract ); }
vector<double> addP3( vector<double> v1, TLorentzVector v2, int isSubtract=0 ){ return addP3( v1[0], v1[1], v1[2], v2.Px(), v2.Py(), v2.Pz(), isSubtract ); }
vector<double> addP3( vector<double> v1, vector<double> v2, int isSubtract=0 ){ return addP3( v1[0], v1[1], v1[2], v2[0], v2[1], v2[2], isSubtract ); }

