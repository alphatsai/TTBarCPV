#include <iostream>
#include <cmath>
#include <string>
#include <vector>

#include "TFile.h"
#include "TH1F.h"
#include "TClonesArray.h"
#include "TLorentzVector.h"

#include "classes/DelphesClasses.h"
#include "ExRootAnalysis/ExRootTreeReader.h"
#include "ExRootAnalysis/ExRootTreeBranch.h"

#include "TH1InfoClass.cc"
//#include "myTH1.cc"
#include "argvOpts.cc"
#include "functions.C"

using namespace std;

int main( int argc, char *argv[] )
{
	argvOpts opts( argc, argv);
	if( !opts.isVal ) return 0;

	////** Analysis 
	TFile *fout = new TFile(opts.output.c_str(),"recreate");
	TChain *chain = new TChain("Delphes");
	chain->Add(opts.input.c_str());

	ExRootTreeReader *treeReader = new ExRootTreeReader(chain);

	TClonesArray *branchJet = treeReader->UseBranch("Jet");
	TClonesArray *branchMuon = treeReader->UseBranch("Muon");
	TClonesArray *branchElectron = treeReader->UseBranch("Electron");
	TClonesArray *branchParticle = treeReader->UseBranch("Particle");
	
	TH1InfoClass<TH1D> h1(opts.debug);
	h1.addNewTH1("Top_Mass3Jets",        "M(3j)",         "Mass", "Yields", "GeV", "", 500, 0, 500 );
	h1.addNewTH1("Top_Mass3MatchedJets", "M(matched 3j)", "Mass", "Yields", "GeV", "", 500, 0, 500 );
	h1.addNewTH1("Top_Mass3GenJets",     "M(gen 3j)",     "Mass", "Yields", "GeV", "", 500, 0, 500 );
	h1.addNewTH1("Evt_CutFlow",        	 "",         	  "",     "Evetns", "", "",    7, 0, 7 );
	h1.addNewTH1("Evt_CutFlow_Mu",    	 "",         	  "",     "Evetns", "", "",    7, 0, 7 );
	h1.addNewTH1("Evt_CutFlow_El",     	 "",         	  "",     "Evetns", "", "",    7, 0, 7 );
	h1.addNewTH1("Evt_MuCut",     	 "isoMu:looseMu:looseEl",       	  "",     "Evetns", "", "",    7, 0, 7 );
	h1.addNewTH1("Evt_ElCut",     	 "isoEl:looseMu:looseEl",         	  "",     "Evetns", "", "",    7, 0, 7 );

	h1.CreateTH1();
	h1.Sumw2();

	h1.GetTH1("Evt_CutFlow")->GetXaxis()->SetBinLabel(1,"All");
	h1.GetTH1("Evt_CutFlow")->GetXaxis()->SetBinLabel(2,"1 isoLep");
	h1.GetTH1("Evt_CutFlow")->GetXaxis()->SetBinLabel(3,"veto(Loose #mu)");
	h1.GetTH1("Evt_CutFlow")->GetXaxis()->SetBinLabel(4,"veto(Loose e)");
	h1.GetTH1("Evt_CutFlow")->GetXaxis()->SetBinLabel(5,"#geq3 Jets");
	h1.GetTH1("Evt_CutFlow")->GetXaxis()->SetBinLabel(6,"=2 bjets");
	h1.GetTH1("Evt_CutFlow_Mu")->GetXaxis()->SetBinLabel(1,"All");
	h1.GetTH1("Evt_CutFlow_Mu")->GetXaxis()->SetBinLabel(2,"1 isoMu");
	h1.GetTH1("Evt_CutFlow_Mu")->GetXaxis()->SetBinLabel(3,"veto(Loose #mu)");
	h1.GetTH1("Evt_CutFlow_Mu")->GetXaxis()->SetBinLabel(4,"veto(Loose e)");
	h1.GetTH1("Evt_CutFlow_Mu")->GetXaxis()->SetBinLabel(5,"#geq3 Jets");
	h1.GetTH1("Evt_CutFlow_Mu")->GetXaxis()->SetBinLabel(6,"=2 bjets");
	h1.GetTH1("Evt_CutFlow_El")->GetXaxis()->SetBinLabel(1,"All");
	h1.GetTH1("Evt_CutFlow_El")->GetXaxis()->SetBinLabel(2,"1 isoEl");
	h1.GetTH1("Evt_CutFlow_El")->GetXaxis()->SetBinLabel(3,"veto(Loose #mu)");
	h1.GetTH1("Evt_CutFlow_El")->GetXaxis()->SetBinLabel(4,"veto(Loose e)");
	h1.GetTH1("Evt_CutFlow_El")->GetXaxis()->SetBinLabel(5,"#geq3 Jets");
	h1.GetTH1("Evt_CutFlow_El")->GetXaxis()->SetBinLabel(6,"=2 bjets");
	h1.GetTH1("Evt_MuCut")->GetXaxis()->SetBinLabel(1,"1:0:0");
	h1.GetTH1("Evt_MuCut")->GetXaxis()->SetBinLabel(2,"0:1:0");
	h1.GetTH1("Evt_MuCut")->GetXaxis()->SetBinLabel(3,"0:0:1");
	h1.GetTH1("Evt_MuCut")->GetXaxis()->SetBinLabel(4,"1:1:0");
	h1.GetTH1("Evt_MuCut")->GetXaxis()->SetBinLabel(5,"0:1:1");
	h1.GetTH1("Evt_MuCut")->GetXaxis()->SetBinLabel(6,"1:0:1");
	h1.GetTH1("Evt_MuCut")->GetXaxis()->SetBinLabel(7,"1:1:1");
	h1.GetTH1("Evt_ElCut")->GetXaxis()->SetBinLabel(1,"1:0:0");
	h1.GetTH1("Evt_ElCut")->GetXaxis()->SetBinLabel(2,"0:1:0");
	h1.GetTH1("Evt_ElCut")->GetXaxis()->SetBinLabel(3,"0:0:1");
	h1.GetTH1("Evt_ElCut")->GetXaxis()->SetBinLabel(4,"1:1:0");
	h1.GetTH1("Evt_ElCut")->GetXaxis()->SetBinLabel(5,"0:1:1");
	h1.GetTH1("Evt_ElCut")->GetXaxis()->SetBinLabel(6,"1:0:1");
	h1.GetTH1("Evt_ElCut")->GetXaxis()->SetBinLabel(7,"1:1:1");


	vector<double> ax, ay, az;
	ax.push_back(1); ax.push_back(0); ax.push_back(0);
	ay.push_back(0); ay.push_back(1); ay.push_back(0);
	az.push_back(0); az.push_back(0); az.push_back(1);

	int n_entries = treeReader->GetEntries();
	for(int entry = 0; entry < n_entries; entry++){
		treeReader->ReadEntry(entry);

		h1.GetTH1("Evt_Events")->Fill(1);

		if (((entry+1) % 100)==0 || entry+1==n_entries)
			printf("| processing %d/%d.\n",entry+1,n_entries);

		////* Lepton selection
		vector<Muon> selMuCol, looseMuCol_isoMu, looseMuCol_isoEl;
		vector<Electron> selElCol, looseElCol_isoMu, looseElCol_isoEl;
		//* Electron selections
		for( int idx=0; idx<branchElectron->GetEntries(); idx++){
			Electron* electron = (Electron*)branchElectron->At(idx);
			if( abs(electron->Eta) > 2.5 ) continue;
			if( abs(electron->Eta) < 1.5 && abs(electron->Eta) > 1.4442) continue;
			if( electron->PT < 15) continue;
				looseElCol_isoMu.push_back(*electron);	
			if( electron->PT < 20) continue;
			if( electron->PT < 45)
				looseElCol_isoEl.push_back(*electron);
			else
				selElCol.push_back(*electron);	
		}
		//* Muon selections
		for( int idx=0; idx<branchMuon->GetEntries(); idx++){
			Muon* muon = (Muon*)branchMuon->At(idx);
			if( abs(muon->Eta) > 2.5 ) continue;
			if( muon->PT < 10) continue; 
				looseMuCol_isoEl.push_back(*muon);	
			if( muon->PT < 35){ 
				looseMuCol_isoMu.push_back(*muon);	
			}else if( muon->PT >= 35 && abs(muon->Eta) < 2.1 ){
				selMuCol.push_back(*muon);
			}
		}
		//* Jets selections
		vector<Jet> seljetCol, bjetCol;
		for( int idx=0; idx<branchJet->GetEntries(); idx++){

			Jet *jet = (Jet*)branchJet->At(idx);
			h1.GetTH1("Jet_Pt")->Fill(jet->P4().Pt());			
			h1.GetTH1("Jet_Px")->Fill(jet->P4().Px());			
			h1.GetTH1("Jet_Py")->Fill(jet->P4().Py());			
			h1.GetTH1("Jet_Pz")->Fill(jet->P4().Pz());			
			h1.GetTH1("Jet_M")->Fill(jet->P4().M());			
			h1.GetTH1("Jet_E")->Fill(jet->P4().E());			
			h1.GetTH1("Jet_Eta")->Fill(jet->P4().Eta());			
			h1.GetTH1("Jet_Phi")->Fill(jet->P4().Phi());			
			h1.GetTH1("Jet_BTag")->Fill(jet->BTag);			

			if( jet->PT<30.) continue;
			if( abs(jet->Eta)>=2.4) continue;
			seljetCol.push_back(*jet);
			h1.GetTH1("SelJet_Pt")->Fill(jet->P4().Pt());			
			h1.GetTH1("SelJet_Px")->Fill(jet->P4().Px());			
			h1.GetTH1("SelJet_Py")->Fill(jet->P4().Py());			
			h1.GetTH1("SelJet_Pz")->Fill(jet->P4().Pz());			
			h1.GetTH1("SelJet_M")->Fill(jet->P4().M());			
			h1.GetTH1("SelJet_E")->Fill(jet->P4().E());			
			h1.GetTH1("SelJet_Eta")->Fill(jet->P4().Eta());			
			h1.GetTH1("SelJet_Phi")->Fill(jet->P4().Phi());			
			h1.GetTH1("SelJet_BTag")->Fill(jet->BTag);

			if (!jet->BTag) continue;
			Jet *bjet = (Jet*)branchJet->At(idx);
			bjetCol.push_back(*bjet);
			h1.GetTH1("bJet_Pt")->Fill(bjet->P4().Pt());			
			h1.GetTH1("bJet_Px")->Fill(bjet->P4().Px());			
			h1.GetTH1("bJet_Py")->Fill(bjet->P4().Py());			
			h1.GetTH1("bJet_Pz")->Fill(bjet->P4().Pz());			
			h1.GetTH1("bJet_M")->Fill(bjet->P4().M());			
			h1.GetTH1("bJet_E")->Fill(bjet->P4().E());			
			h1.GetTH1("bJet_Eta")->Fill(bjet->P4().Eta());			
			h1.GetTH1("bJet_Phi")->Fill(bjet->P4().Phi());			
			h1.GetTH1("bJet_BTag")->Fill(bjet->BTag);

			GenParticle *genb = getMatchedGenParticle(bjet,branchParticle,5); 

			// one b-jet and two non b-jet -> single top decay
			for (int idx1=0; idx1<branchJet->GetEntries(); idx1++) {
				for (int idx2=idx1+1; idx2<branchJet->GetEntries(); idx2++) {
					if (idx1==idx || idx2==idx) continue;
					Jet *jet1 = (Jet*)branchJet->At(idx1);
					Jet *jet2 = (Jet*)branchJet->At(idx2);
					// only 3 jets with only a b-tagger jet
					if (jet1->BTag) continue;
					if (jet2->BTag) continue;

					double Mjj = ((jet1->P4())+(jet2->P4())).Mag();
					if (Mjj<40. || Mjj>120.) continue;

					GenParticle *gen1 = getMatchedGenParticle(jet1,branchParticle,0);
					GenParticle *gen2 = getMatchedGenParticle(jet2,branchParticle,0);

					h1.GetTH1("Top_Mass3Jets")->Fill(((jet1->P4())+(jet2->P4())+(bjet->P4())).Mag());
					// Matching to Top quark, t->bw
					bool isFullMatched = false;
					if (gen1!=NULL && gen2!=NULL && genb!=NULL && gen1!=gen2 && gen1->M1==gen2->M1) {
						GenParticle *genw = (GenParticle*)branchParticle->At(gen1->M1);
						if (abs(genw->PID)==24 && genw->M1==genb->M1) {
							GenParticle *gent = (GenParticle*)branchParticle->At(genb->M1);
							if (abs(gent->PID)==6) isFullMatched = true;
						}
					}
					if (isFullMatched) {
						h1.GetTH1("Top_Mass3MatchedJets")->Fill(((jet1->P4())+(jet2->P4())+(bjet->P4())).Mag());
						h1.GetTH1("Top_Mass3GenJets")->Fill(((gen1->P4())+(gen2->P4())+(genb->P4())).Mag());
					}
				}
			}
		}

		//* Fill histo for event informations
		h1.GetTH1("Evt_NLeptons")->Fill((branchMuon->GetEntries()+branchElectron->GetEntries()));	
		h1.GetTH1("Evt_NSelLeptons")->Fill((selMuCol.size()+selElCol.size()));	
		h1.GetTH1("Evt_NMuons")->Fill(branchMuon->GetEntries());	
		h1.GetTH1("Evt_NSelMuons")->Fill(selMuCol.size());	
		h1.GetTH1("Evt_NLooseElIsoMu")->Fill(looseElCol_isoMu.size());
		h1.GetTH1("Evt_NLooseMuIsoMu")->Fill(looseMuCol_isoMu.size());
		h1.GetTH1("Evt_NElectrons")->Fill(branchElectron->GetEntries());	
		h1.GetTH1("Evt_NSelElectrons")->Fill(selElCol.size());	
		h1.GetTH1("Evt_NLooseElIsoEl")->Fill(looseElCol_isoEl.size());
		h1.GetTH1("Evt_NLooseMuIsoEl")->Fill(looseMuCol_isoEl.size());
		h1.GetTH1("Evt_NJets")->Fill(branchJet->GetEntries());	
		h1.GetTH1("Evt_NSelJets")->Fill(seljetCol.size());	
		h1.GetTH1("Evt_NbJets")->Fill(bjetCol.size());

		//* Fill cut flow
		h1.GetTH1("Evt_CutFlow")->Fill("All", 1);
		h1.GetTH1("Evt_CutFlow_Mu")->Fill("All", 1);
		h1.GetTH1("Evt_CutFlow_El")->Fill("All", 1);
		if( (selMuCol.size()+selElCol.size()) == 1  ){
			h1.GetTH1("Evt_CutFlow")->Fill("1 isoLep", 1);
			if( selMuCol.size() == 1 ){
				h1.GetTH1("Evt_CutFlow_Mu")->Fill("1 isoMu", 1);
				if( looseMuCol_isoMu.size() == 0 ){
					h1.GetTH1("Evt_CutFlow")->Fill("veto(Loose #mu)", 1);
					h1.GetTH1("Evt_CutFlow_Mu")->Fill("veto(Loose #mu)", 1);
					if( looseElCol_isoMu.size() == 0 ){
						h1.GetTH1("Evt_CutFlow")->Fill("veto(Loose e)", 1);
						h1.GetTH1("Evt_CutFlow_Mu")->Fill("veto(Loose e)", 1);
						if( seljetCol.size() >= 3 ){ 	
							h1.GetTH1("Evt_CutFlow")->Fill("#geq3 Jets", 1);
							h1.GetTH1("Evt_CutFlow_Mu")->Fill("#geq3 Jets", 1);
						}
						if( bjetCol.size() == 2 ){	
							h1.GetTH1("Evt_CutFlow")->Fill("=2 bjets", 1);
							h1.GetTH1("Evt_CutFlow_Mu")->Fill("=2 bjets", 1);
						}
					}	
				}
			}else{
				h1.GetTH1("Evt_CutFlow_El")->Fill("1 isoEl", 1);
				if( looseMuCol_isoEl.size() == 0 ){
					h1.GetTH1("Evt_CutFlow")->Fill("veto(Loose #mu)", 1);
					h1.GetTH1("Evt_CutFlow_El")->Fill("veto(Loose #mu)", 1);
					if( looseElCol_isoEl.size() == 0 ){
						h1.GetTH1("Evt_CutFlow")->Fill("veto(Loose e)", 1);
						h1.GetTH1("Evt_CutFlow_El")->Fill("veto(Loose e)", 1);
						if( seljetCol.size() >= 3 ){ 	
							h1.GetTH1("Evt_CutFlow")->Fill("#geq3 Jets", 1);
							h1.GetTH1("Evt_CutFlow_El")->Fill("#geq3 Jets", 1);
						}
						if( bjetCol.size() == 2 ){	
							h1.GetTH1("Evt_CutFlow")->Fill("=2 bjets", 1);
							h1.GetTH1("Evt_CutFlow_El")->Fill("=2 bjets", 1);
						}
					}	
				}				
			}
			
			//* Lepton cut
			if( selMuCol.size() == 1 && looseMuCol_isoMu.size() == 0 && looseElCol_isoMu.size() == 0 )
				h1.GetTH1("Evt_MuCut")->Fill("1:0:0", 1);
			if( selMuCol.size() == 0 && looseMuCol_isoMu.size() == 1 && looseElCol_isoMu.size() == 0 )
				h1.GetTH1("Evt_MuCut")->Fill("0:1:0", 1);
			if( selMuCol.size() == 0 && looseMuCol_isoMu.size() == 0 && looseElCol_isoMu.size() == 1 )
				h1.GetTH1("Evt_MuCut")->Fill("0:0:1", 1);
			if( selMuCol.size() == 1 && looseMuCol_isoMu.size() == 1 && looseElCol_isoMu.size() == 0 )
				h1.GetTH1("Evt_MuCut")->Fill("1:1:0", 1);
			if( selMuCol.size() == 1 && looseMuCol_isoMu.size() == 0 && looseElCol_isoMu.size() == 1 )
				h1.GetTH1("Evt_MuCut")->Fill("1:0:1", 1);
			if( selMuCol.size() == 0 && looseMuCol_isoMu.size() == 1 && looseElCol_isoMu.size() == 1 )
				h1.GetTH1("Evt_MuCut")->Fill("0:1:1", 1);
			if( selMuCol.size() == 1 && looseMuCol_isoMu.size() == 1 && looseElCol_isoMu.size() == 1 )
				h1.GetTH1("Evt_MuCut")->Fill("1:1:1", 1);
			if( selElCol.size() == 1 && looseMuCol_isoEl.size() == 0 && looseElCol_isoEl.size() == 0 )
				h1.GetTH1("Evt_ElCut")->Fill("1:0:0", 1);
			if( selElCol.size() == 0 && looseMuCol_isoEl.size() == 1 && looseElCol_isoEl.size() == 0 )
				h1.GetTH1("Evt_ElCut")->Fill("0:1:0", 1);
			if( selElCol.size() == 0 && looseMuCol_isoEl.size() == 0 && looseElCol_isoEl.size() == 1 )
				h1.GetTH1("Evt_ElCut")->Fill("0:0:1", 1);
			if( selElCol.size() == 1 && looseMuCol_isoEl.size() == 1 && looseElCol_isoEl.size() == 0 )
				h1.GetTH1("Evt_ElCut")->Fill("1:1:0", 1);
			if( selElCol.size() == 1 && looseMuCol_isoEl.size() == 0 && looseElCol_isoEl.size() == 1 )
				h1.GetTH1("Evt_ElCut")->Fill("1:0:1", 1);
			if( selElCol.size() == 0 && looseMuCol_isoEl.size() == 1 && looseElCol_isoEl.size() == 1 )
				h1.GetTH1("Evt_ElCut")->Fill("0:1:1", 1);
			if( selElCol.size() == 1 && looseMuCol_isoEl.size() == 1 && looseElCol_isoEl.size() == 1 )
				h1.GetTH1("Evt_ElCut")->Fill("1:1:1", 1);
		}
	}

	fout->Write();
	fout->Close();
	printf(">> Done!\n");
	printf(">> Output file: %s\n", opts.output.c_str());
}
