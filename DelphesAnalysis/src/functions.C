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
double dotP3( TLorentzVector v1, TLorentzVector v2)
{
	return v1.Px()*v2.Px()+v1.Py()*v2.Py()+v1.Pz()*v2.Pz();
}
double dotP3( vector<double> v1, vector<double> v2)
{
	return v1[0]*v2[0]+v1[1]*v2[1]+v1[2]*v2[2];
}
double dotP3( TLorentzVector v1, vector<double> v2)
{
	return v1.Px()*v2[0]+v1.Py()*v2[1]+v1.Pz()*v2[2];
}
double dotP3( vector<double> v1, TLorentzVector v2)
{
	return v2.Px()*v1[0]+v2.Py()*v1[1]+v2.Pz()*v1[2];
}
vector<double> crossP3( TLorentzVector v1, TLorentzVector v2)
{
	vector<double> product;
	product.push_back( v1.Py()*v2.Pz()-v1.Pz()*v2.Py() ); //x
	product.push_back( v1.Pz()*v2.Px()-v1.Px()*v2.Pz() ); //y
	product.push_back( v1.Px()*v2.Py()-v1.Py()*v2.Px() ); //z 	
	return product;
}
vector<double> crossP3(  vector<double> v1, vector<double> v2)
{
	vector<double> product;
	product.push_back( v1[1]*v2[2]-v1[2]*v2[1] ); //x
	product.push_back( v1[2]*v2[0]-v1[0]*v2[2] ); //y
	product.push_back( v1[0]*v2[1]-v1[1]*v2[0] ); //z 	
	return product;
}
vector<double> crossP3( vector<double> v1, TLorentzVector v2)
{
	vector<double> product;
	product.push_back( v1[1]*v2.Pz()-v1[2]*v2.Py() ); //x
	product.push_back( v1[2]*v2.Px()-v1[0]*v2.Pz() ); //y
	product.push_back( v1[0]*v2.Py()-v1[1]*v2.Px() ); //z 	
	return product;
}
vector<double> crossP3(  TLorentzVector v1, vector<double> v2)
{
	vector<double> product;
	product.push_back( v1.Py()*v2[2]-v1.Pz()*v2[1] ); //x
	product.push_back( v1.Pz()*v2[0]-v1.Px()*v2[2] ); //y
	product.push_back( v1.Px()*v2[1]-v1.Py()*v2[0] ); //z 	
	return product;
}

vector<double> addP3( TLorentzVector v1, TLorentzVector v2, int isSubtract=0 )
{
	vector<double> newP3;
	if( isSubtract < 0 ){
		newP3.push_back( v1.Px()-v2.Px() );	
		newP3.push_back( v1.Py()-v2.Py() );	
		newP3.push_back( v1.Pz()-v2.Pz() );	
	}else{
		newP3.push_back( v1.Px()+v2.Px() );	
		newP3.push_back( v1.Py()+v2.Py() );	
		newP3.push_back( v1.Pz()+v2.Pz() );	
	}
	return newP3;
}
vector<double> addP3( vector<double> v1, vector<double> v2, int isSubtract=0 )
{
	vector<double> newP3;
	if( isSubtract < 0 ){
		newP3.push_back( v1[0]-v2[0] );	
		newP3.push_back( v1[1]-v2[1] );	
		newP3.push_back( v1[2]-v2[2] );	
	}else{
		newP3.push_back( v1[0]+v2[0] );	
		newP3.push_back( v1[1]+v2[1] );	
		newP3.push_back( v1[2]+v2[2] );	
	}
	return newP3;
}
vector<double> addP3( TLorentzVector v1, vector<double> v2, int isSubtract=0 )
{
	vector<double> newP3;
	if( isSubtract < 0 ){
		newP3.push_back( v1.Px()-v2[0] );	
		newP3.push_back( v1.Py()-v2[1] );	
		newP3.push_back( v1.Pz()-v2[2] );	
	}else{                             
		newP3.push_back( v1.Px()+v2[0] );	
		newP3.push_back( v1.Py()+v2[1] );	
		newP3.push_back( v1.Pz()+v2[2] );	
	}
	return newP3;
}
vector<double> addP3( vector<double> v1, TLorentzVector v2, int isSubtract=0 )
{
	vector<double> newP3;
	if( isSubtract < 0 ){
		newP3.push_back( v1[0]-v2.Px() );	
		newP3.push_back( v1[1]-v2.Py() );	
		newP3.push_back( v1[2]-v2.Pz() );	
	}else{                             
		newP3.push_back( v1[0]+v2.Px() );	
		newP3.push_back( v1[1]+v2.Py() );	
		newP3.push_back( v1[2]+v2.Pz() );	
	}
	return newP3;
}
