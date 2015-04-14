#include <iostream>
#include <cmath>
#include <string>

#include "TFile.h"
#include "TH1F.h"
#include "TClonesArray.h"
#include "TLorentzVector.h"

#include "classes/DelphesClasses.h"

#include "ExRootAnalysis/ExRootTreeReader.h"
#include "ExRootAnalysis/ExRootTreeBranch.h"

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

using namespace std;
int main( int argc, char *argv[] )
{
	////** Check input and output
	printf("|\n");
    if( argv[1] == NULL ){
        printf("| Please input file.\n"); 
		printf("| Ex: %s tag_1_delphes_events.root\n", argv[0]);
		printf("|\n");
        return 0;
    }else{
		cout<<"| Loaded file: "<<argv[1]<<endl;
	} 
    if( argv[2] == NULL ){
		argv[2]="result.root";
	}else{
		string str(argv[2]);
		if( str.find(".root") == std::string::npos){ 
			printf("| [ERROR]: \"%s\" is not root file for output\n", argv[2]);
			printf("| Please do like: %s %s result.root\n", argv[0], argv[1]);
			printf("|\n");
			return 0;
		}
	}

	////** Analysis 
    TFile *fout = new TFile(argv[2],"recreate");
    TChain *chain = new TChain("Delphes");
    chain->Add(argv[1]);
    
    ExRootTreeReader *treeReader = new ExRootTreeReader(chain);

    TClonesArray *branchJet = treeReader->UseBranch("Jet");
    TClonesArray *branchParticle = treeReader->UseBranch("Particle");
    
    TH1D *histMass1 = new TH1D("histMass1", "M(j_{1}, j_{2})", 80, 40.0, 200.0);
    TH1D *histMass2 = new TH1D("histMass2", "M(gen b_{1}, gen b_{2})", 80, 40.0, 200.0);

    TH1D *histMass3 = new TH1D("histMass3", "M(3j)", 140, 50.0, 330.0);
    TH1D *histMass4 = new TH1D("histMass4", "M(matched 3j)", 140, 50.0, 330.0);
    TH1D *histMass5 = new TH1D("histMass5", "M(gen 3j)", 140, 50.0, 330.0);
    
    TH1D *histR_pt  = new TH1D("histR_pt", "Pt(gen b)/Pt(j) versus Pt(j)", 10, 20.,120.);
    TH1D *histR2_pt  = new TH1D("histR2_pt", "", 10, 20.,120.);
    TH1D *histC_pt  = new TH1D("histC_pt", "", 10, 20.,120.);
    
    TH1D *histR_eta = new TH1D("histR_eta", "Pt(gen b)/Pt(j) versus Eta(j)", 5, 0.,2.5);
    TH1D *histR2_eta = new TH1D("histR2_eta", "", 5, 0.,2.5);
    TH1D *histC_eta = new TH1D("histC_eta", "", 5, 0.,2.5);
    
    int n_entries = treeReader->GetEntries();
    for(int entry = 0; entry < n_entries; entry++) {
        treeReader->ReadEntry(entry);
        
        if (((entry+1) % 100)==0 || entry+1==n_entries)
            printf("| processing %d/%d.\n",entry+1,n_entries);
        
        for (int idx=0; idx<branchJet->GetEntries(); idx++) {
            Jet *jet = (Jet*)branchJet->At(idx);
            if (!jet->BTag) continue;
            
            GenParticle *gen = getMatchedGenParticle(jet,branchParticle,5);
            if (gen==NULL) continue;
            
            double R = jet->PT/gen->PT;
            
            histR_pt->Fill(jet->PT,R);
            histR2_pt->Fill(jet->PT,R*R);
            histC_pt->Fill(jet->PT,1.);

            histR_eta->Fill(fabs(jet->Eta),R);
            histR2_eta->Fill(fabs(jet->Eta),R*R);
            histC_eta->Fill(fabs(jet->Eta),1.);
        }
        
        for (int idx1=0; idx1<branchJet->GetEntries(); idx1++) {
            for (int idx2=idx1+1; idx2<branchJet->GetEntries(); idx2++) {
                Jet *jet1 = (Jet*)branchJet->At(idx1);
                Jet *jet2 = (Jet*)branchJet->At(idx2);
                
                if (!jet1->BTag) continue;
                if (!jet2->BTag) continue;
                
                GenParticle *gen1 = getMatchedGenParticle(jet1,branchParticle,5);
                GenParticle *gen2 = getMatchedGenParticle(jet2,branchParticle,5);
                
                histMass1->Fill(((jet1->P4())+(jet2->P4())).Mag());
                
                if (gen1!=NULL && gen2!=NULL && gen1!=gen2)
                    histMass2->Fill(((gen1->P4())+(gen2->P4())).Mag());
            }
        }
        
        for (int idx=0; idx<branchJet->GetEntries(); idx++) {
            
            Jet *bjet = (Jet*)branchJet->At(idx);
            if (!bjet->BTag) continue;
            if (bjet->PT<40.) continue;
       
            GenParticle *genb = getMatchedGenParticle(bjet,branchParticle,5); 
            
            for (int idx1=0; idx1<branchJet->GetEntries(); idx1++) {
                for (int idx2=idx1+1; idx2<branchJet->GetEntries(); idx2++) {

                    if (idx1==idx || idx2==idx) continue;
                    Jet *jet1 = (Jet*)branchJet->At(idx1);
                    Jet *jet2 = (Jet*)branchJet->At(idx2);
                    if (jet1->BTag) continue;
                    if (jet2->BTag) continue;
                    
                    double Mjj = ((jet1->P4())+(jet2->P4())).Mag();
                    if (Mjj<40. || Mjj>120.) continue;
                    
                    GenParticle *gen1 = getMatchedGenParticle(jet1,branchParticle,0);
                    GenParticle *gen2 = getMatchedGenParticle(jet2,branchParticle,0);
                
                    histMass3->Fill(((jet1->P4())+(jet2->P4())+(bjet->P4())).Mag());
                    
                    bool isFullMatched = false;
                    if (gen1!=NULL && gen2!=NULL && genb!=NULL && gen1!=gen2 && gen1->M1==gen2->M1) {
                        GenParticle *genw = (GenParticle*)branchParticle->At(gen1->M1);
                        if (abs(genw->PID)==24 && genw->M1==genb->M1) {
                            GenParticle *gent = (GenParticle*)branchParticle->At(genb->M1);
                            if (abs(gent->PID)==6) isFullMatched = true;
                        }
                    }
                
                    if (isFullMatched) {
                        histMass4->Fill(((jet1->P4())+(jet2->P4())+(bjet->P4())).Mag());
                        histMass5->Fill(((gen1->P4())+(gen2->P4())+(genb->P4())).Mag());
                    }
                }
            }
        }
    
    }

    histR_pt->Divide(histC_pt);
    histR2_pt->Divide(histC_pt);
    for(int i=1;i<=histR2_pt->GetNbinsX();i++)
        histR_pt->SetBinError(i,sqrt(histR2_pt->GetBinContent(i)-histR_pt->GetBinContent(i)*histR_pt->GetBinContent(i)));

    histR_eta->Divide(histC_eta);
    histR2_eta->Divide(histC_eta);
    for(int i=1;i<=histR2_eta->GetNbinsX();i++)
        histR_eta->SetBinError(i,sqrt(histR2_eta->GetBinContent(i)-histR_eta->GetBinContent(i)*histR_eta->GetBinContent(i)));
    
    fout->Write();
    fout->Close();
	printf("| Done!\n");
	printf("| Output file: %s\n", argv[2]);
	printf("|\n");
}
