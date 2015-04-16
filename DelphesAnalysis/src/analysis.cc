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
	TClonesArray *branchParticle = treeReader->UseBranch("Particle");
	
	TH1InfoClass<TH1D> h1(opts.debug);
	h1.addNewTH1("Top_Mass3Jets",        "M(3j)",         "Mass", "Yields", "GeV", "", 350, 0, 350 );
	h1.addNewTH1("Top_Mass3MatchedJets", "M(matched 3j)", "Mass", "Yields", "GeV", "", 350, 0, 350 );
	h1.addNewTH1("Top_Mass3GenJets",      "M(gen 3j)",     "Mass", "Yields", "GeV", "", 350, 0, 350 );

	h1.CreateTH1();
	h1.Sumw2();

	vector<double> ax, ay, az;
	ax.push_back(1); ax.push_back(0); ax.push_back(0);
	ay.push_back(0); ay.push_back(1); ay.push_back(0);
	az.push_back(0); az.push_back(0); az.push_back(1);

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

		}

		// one b-jet and two non b-jet -> top decay
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

	}

	fout->Write();
	fout->Close();
	printf(">> Done!\n");
	printf(">> Output file: %s\n", opts.output.c_str());
}
