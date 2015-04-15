#include <vector>
#include "TLorentzVector.h"
#include "classes/DelphesClasses.h"
using namespace std;
//Matching functions
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

