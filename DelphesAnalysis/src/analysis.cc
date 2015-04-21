#include <iostream>
#include <cmath>
#include <string>
#include <vector>

#include "TFile.h"
#include "TH1F.h"
#include "TClonesArray.h"
#include "TLorentzVector.h"
#include "TVector3.h"

#include "classes/DelphesClasses.h"
#include "ExRootAnalysis/ExRootTreeReader.h"
#include "ExRootAnalysis/ExRootTreeBranch.h"

#include "TH1InfoClass.cc"
#include "argvOpts.cc"
#include "functions.C"

const double MT=173.0;
const double MT2=MT*MT;
const double MT3=MT*MT*MT;
const double MT4=MT*MT*MT*MT;

using namespace std;
template<class TH1>
void setObservableHist(TH1* h, string ob ){
	h->GetXaxis()->SetBinLabel(1,(ob+"<0").c_str());
	h->GetXaxis()->SetBinLabel(2,(ob+">0").c_str());
}
template<class TH1>
void setLeptonSelHist(TH1* h){
	h->GetXaxis()->SetBinLabel(1,"1:0:0");
	h->GetXaxis()->SetBinLabel(2,"0:1:0");
	h->GetXaxis()->SetBinLabel(3,"0:0:1");
	h->GetXaxis()->SetBinLabel(4,"1:1:0");
	h->GetXaxis()->SetBinLabel(5,"0:1:1");
	h->GetXaxis()->SetBinLabel(6,"1:0:1");
}

template<class TH1>
void setDiscardEvtHist(TH1* h, string Oname){
	h->GetXaxis()->SetBinLabel(1,("PT40, "+Oname+"==0").c_str());
	h->GetXaxis()->SetBinLabel(2,("PT50, "+Oname+"==0").c_str());
	h->GetXaxis()->SetBinLabel(3,("PT60, "+Oname+"==0").c_str());
}
template<class TH1>
void setCutFlow(TH1* h, string channel){
	if( channel.compare("lj") == 0 || channel.compare("ljm") == 0 || channel.compare("lje") == 0){
		h->GetXaxis()->SetBinLabel(1,"All");
		//h->GetXaxis()->SetBinLabel(2,"#geq1 goodVtx");
		if( channel.compare("lj") == 0  ) h->GetXaxis()->SetBinLabel(2,"1 isoLep");
		if( channel.compare("ljm") == 0 ) h->GetXaxis()->SetBinLabel(2,"1 isoMu");
		if( channel.compare("lje") == 0 ) h->GetXaxis()->SetBinLabel(2,"1 isoEl");
		h->GetXaxis()->SetBinLabel(3,"veto(Loose #mu)");
		h->GetXaxis()->SetBinLabel(4,"veto(Loose e)");
		h->GetXaxis()->SetBinLabel(5,"#geq3 Jets");
		h->GetXaxis()->SetBinLabel(6,"#geq2 bjets");
		h->GetXaxis()->SetBinLabel(7,"=2 bjets");
	}
	if( channel.compare("mj") == 0 || channel.compare("mj2b") ){
		std::string bjAlg;
		if( channel.compare("mj") == 0 ) bjAlg="#geq2"; 
		else if( channel.compare("mj2b") == 0 ) bjAlg="==2";
		h->GetXaxis()->SetBinLabel(1,"All");
		//h->GetXaxis()->SetBinLabel(2,"#geq1 goodVtx");
		h->GetXaxis()->SetBinLabel(2,"veto(Hard Lep)");
		h->GetXaxis()->SetBinLabel(3,"#geq4 pT60 jets");
		h->GetXaxis()->SetBinLabel(4,"#geq5 pT50 jets");
		h->GetXaxis()->SetBinLabel(5,"#geq6 pT40 jets");
		h->GetXaxis()->SetBinLabel(6,(bjAlg+" pT40 bjets").c_str());
		h->GetXaxis()->SetBinLabel(7,(bjAlg+" pT50 bjets").c_str());
		h->GetXaxis()->SetBinLabel(8,(bjAlg+" pT60 bjets").c_str());
		//h->GetXaxis()->SetBinLabel(7,"#=2 bjets");
	}
}
int main( int argc, char *argv[] )
{
	argvOpts opts( argc, argv);
	if( !opts.isVal ) return 0;

	////** Analysis 
	TFile *fout = new TFile(opts.output.c_str(),"recreate");
	TChain *chain = new TChain("Delphes");
	chain->Add(opts.input.c_str());

	ExRootTreeReader *treeReader = new ExRootTreeReader(chain);

	//TClonesArray *branchVtx = treeReader->UseBranch("Vertex");
	TClonesArray *branchJet = treeReader->UseBranch("Jet");
	TClonesArray *branchMuon = treeReader->UseBranch("Muon");
	TClonesArray *branchElectron = treeReader->UseBranch("Electron");
	TClonesArray *branchParticle = treeReader->UseBranch("Particle");
	
	TH1InfoClass<TH1D> h1(opts.debug);
	if( opts.isLepJets ){
		h1.addNewTH1( "Evt_O7_Mu",		"O7",	  	"O_{7}", "Events", 	"", 	"",		40, -2,   2) ;
		h1.addNewTH1( "Evt_O7_El",		"O7",	  	"O_{7}", "Events", 	"", 	"",		40, -2,   2) ;
		h1.addNewTH1( "Evt_O7Asym_Mu",	"A_{O7}",	"", 	 "Events", 	"", 	"",		2, 0,   2) ;
		h1.addNewTH1( "Evt_O7Asym_El",	"A_{O7}",	"", 	 "Events", 	"", 	"",		2, 0,   2) ;
		h1.addNewTH1( "Evt_O2",			"O2",	  	"O_{2}", "Events", 	"", 	"",		40, -2,   2) ;
		h1.addNewTH1( "Evt_O2_Mu",		"O2",	  	"O_{2}", "Events", 	"", 	"",		40, -2,   2) ;
		h1.addNewTH1( "Evt_O2_El",		"O2",	  	"O_{2}", "Events", 	"", 	"",		40, -2,   2) ;
		h1.addNewTH1( "Evt_O2Asym",		"A_{O2}",	"", 	 "Events", 	"", 	"",		2, 0,   2) ;
		h1.addNewTH1( "Evt_O2Asym_Mu",	"A_{O2}",	"", 	 "Events", 	"", 	"",		2, 0,   2) ;
		h1.addNewTH1( "Evt_O2Asym_El",	"A_{O2}",	"", 	 "Events", 	"", 	"",		2, 0,   2) ;
		h1.addNewTH1( "Evt2b_O7",		"O7",	  	"O_{7}", "Events", 	"", 	"",		40, -2,   2) ;
		h1.addNewTH1( "Evt2b_O7Asym",	"A_{O7}",	"", 	 "Events", 	"", 	"",		2, 0,   2) ;
		h1.addNewTH1( "Evt2b_O7_Mu",		"O7",	  	"O_{7}", "Events", 	"", 	"",		40, -2,   2) ;
		h1.addNewTH1( "Evt2b_O7_El",		"O7",	  	"O_{7}", "Events", 	"", 	"",		40, -2,   2) ;
		h1.addNewTH1( "Evt2b_O7Asym_Mu",	"A_{O7}",	"", 	 "Events", 	"", 	"",		2, 0,   2) ;
		h1.addNewTH1( "Evt2b_O7Asym_El",	"A_{O7}",	"", 	 "Events", 	"", 	"",		2, 0,   2) ;
		h1.addNewTH1( "Evt2b_O2",			"O2",	  	"O_{2}", "Events", 	"", 	"",		40, -2,   2) ;
		h1.addNewTH1( "Evt2b_O2_Mu",		"O2",	  	"O_{2}", "Events", 	"", 	"",		40, -2,   2) ;
		h1.addNewTH1( "Evt2b_O2_El",		"O2",	  	"O_{2}", "Events", 	"", 	"",		40, -2,   2) ;
		h1.addNewTH1( "Evt2b_O2Asym",		"A_{O2}",	"", 	 "Events", 	"", 	"",		2, 0,   2) ;
		h1.addNewTH1( "Evt2b_O2Asym_Mu",	"A_{O2}",	"", 	 "Events", 	"", 	"",		2, 0,   2) ;
		h1.addNewTH1( "Evt2b_O2Asym_El",	"A_{O2}",	"", 	 "Events", 	"", 	"",		2, 0,   2) ;

		h1.addNewTH1( "Evt_NLeptons",	   "Num. of leptons",	  	   	"N(lep)", 		  "Events", 	"", 	"",		10, 0,   10) ;
		h1.addNewTH1( "Evt_NSelLeptons",   "Num. of selected leptons", 	"N(selected lep)","Events", 	"", 	"",		10, 0,   10) ;
		h1.addNewTH1( "Evt_NMuons",		   "Num. of muon",	  		   	"N(#mu)", 		  "Events", 	"", 	"",		10, 0,   10) ;
		h1.addNewTH1( "Evt_NSelMuons",	   "Num. of selected muon",    	"N(selected #mu)","Events", 	"", 	"",		10, 0,   10) ;
		h1.addNewTH1( "Evt_NLooseMuIsoMu", "Num. of loose muon", 	   	"N(loose #mu)",	  "Events", 	"", 	"",		10, 0,   10) ;
		h1.addNewTH1( "Evt_NLooseElIsoMu", "Num. of loose electron",   	"N(loose e)",	  "Events", 	"", 	"",		10, 0,   10) ;
		h1.addNewTH1( "Evt_NElectrons",	   "Num. of electron",	  	   	"N(e)", 		  "Events", 	"", 	"",		10, 0,   10) ;
		h1.addNewTH1( "Evt_NSelElectrons", "Num. of selected electron",	"N(selected e)",  "Events", 	"", 	"",		10, 0,   10) ;
		h1.addNewTH1( "Evt_NLooseMuIsoEl", "Num. of loose muon", 		"N(loose #mu)",	  "Events", 	"", 	"",		10, 0,   10) ;
		h1.addNewTH1( "Evt_NLooseElIsoEl", "Num. of loose electron", 	"N(loose e)",	  "Events", 	"", 	"",		10, 0,   10) ;
		h1.addNewTH1( "Evt_CutFlow_Mu",    "",         	  "",     "Evetns", "", "",    7, 0, 7 );
		h1.addNewTH1( "Evt_CutFlow_El",    "",         	  "",     "Evetns", "", "",    7, 0, 7 );
		h1.addNewTH1( "Evt_MuCut",     	 "isoMu:looseMu:looseEl",       	  "",     "Evetns", "", "",    7, 0, 7 );
		h1.addNewTH1( "Evt_ElCut",     	 "isoEl:looseMu:looseEl",         	  "",     "Evetns", "", "",    7, 0, 7 );
	}
	if( opts.isMultiJets ){
		h1.addNewTH1( "Evt_O7_PT50",		"O7",	  	"O_{7}", "Events", 	"", 	"",		40, -2,   2) ;
		h1.addNewTH1( "Evt_O7_PT60",		"O7",	  	"O_{7}", "Events", 	"", 	"",		40, -2,   2) ;
		h1.addNewTH1( "Evt_O7Asym_PT50",	"A_{O7}",	"", 	 "Events", 	"", 	"",		2, 0,   2) ;
		h1.addNewTH1( "Evt_O7Asym_PT60",	"A_{O7}",	"", 	 "Events", 	"", 	"",		2, 0,   2) ;
		h1.addNewTH1( "Evt_O7DiscardEvt", 	"",        	 "",   	 "Evetns", 	"", 	"",     3, 0, 3 );
		h1.addNewTH1( "Evt_O5",			"O5",	  	"O_{5}", "Events", 	"", 	"",		40, -2,   2) ;
		h1.addNewTH1( "Evt_O5_PT50",		"O5",	  	"O_{5}", "Events", 	"", 	"",		40, -2,   2) ;
		h1.addNewTH1( "Evt_O5_PT60",		"O5",	  	"O_{5}", "Events", 	"", 	"",		40, -2,   2) ;
		h1.addNewTH1( "Evt_O5Asym",		"A_{O5}",	"", 	 "Events", 	"", 	"",		2, 0,   2) ;
		h1.addNewTH1( "Evt_O5Asym_PT50",	"A_{O5}",	"", 	 "Events", 	"", 	"",		2, 0,   2) ;
		h1.addNewTH1( "Evt_O5Asym_PT60",	"A_{O5}",	"", 	 "Events", 	"", 	"",		2, 0,   2) ;
		h1.addNewTH1( "Evt_O5DiscardEvt", 	"",        	 "",   	 "Evetns", 	"", 	"",     3, 0, 3 );
		h1.addNewTH1( "Evt_NSelJets_PT40",	"Num. of jets pT>40",	  	  "N(j)", 	"Events", 	"", 	"",		20, 0,   20) ;
		h1.addNewTH1( "Evt_NSelJets_PT50",	"Num. of jets pT>50",	  	  "N(j)", 	"Events", 	"", 	"",		20, 0,   20) ;
		h1.addNewTH1( "Evt_NSelJets_PT60",	"Num. of jets pT>60",	  	  "N(j)", 	"Events", 	"", 	"",		20, 0,   20) ;
		h1.addNewTH1( "Evt_NbJets_PT40_evtSelNJets",	"Num. of jets pT>60",	  	  "N(j)", 	"Events", 	"", 	"",		20, 0,   20) ;
		h1.addNewTH1( "Evt_NbJets_PT50_evtSelNJets",	"Num. of jets pT>60",	  	  "N(j)", 	"Events", 	"", 	"",		20, 0,   20) ;
		h1.addNewTH1( "Evt_NbJets_PT60_evtSelNJets",	"Num. of jets pT>60",	  	  "N(j)", 	"Events", 	"", 	"",		20, 0,   20) ;
		h1.addNewTH1( "Evt2b_CutFlow", "",        	  "",     			"Evetns", 	"", 	"",     8, 0, 8 );
		h1.addNewTH1( "Evt2b_O7",		"O7",	  	"O_{7}", "Events", 	"", 	"",		40, -2,   2) ;
		h1.addNewTH1( "Evt2b_O7_PT50",		"O7",	  	"O_{7}", "Events", 	"", 	"",		40, -2,   2) ;
		h1.addNewTH1( "Evt2b_O7_PT60",		"O7",	  	"O_{7}", "Events", 	"", 	"",		40, -2,   2) ;
		h1.addNewTH1( "Evt2b_O7Asym",		"A_{O7}",	"", 	 "Events", 	"", 	"",		2, 0,   2) ;
		h1.addNewTH1( "Evt2b_O7Asym_PT50",	"A_{O7}",	"", 	 "Events", 	"", 	"",		2, 0,   2) ;
		h1.addNewTH1( "Evt2b_O7Asym_PT60",	"A_{O7}",	"", 	 "Events", 	"", 	"",		2, 0,   2) ;
		h1.addNewTH1( "Evt2b_O7DiscardEvt", 	"",        	 "",   	 "Evetns", 	"", 	"",     3, 0, 3 );
		h1.addNewTH1( "Evt2b_O5",			"O5",	  	"O_{5}", "Events", 	"", 	"",		40, -2,   2) ;
		h1.addNewTH1( "Evt2b_O5_PT50",		"O5",	  	"O_{5}", "Events", 	"", 	"",		40, -2,   2) ;
		h1.addNewTH1( "Evt2b_O5_PT60",		"O5",	  	"O_{5}", "Events", 	"", 	"",		40, -2,   2) ;
		h1.addNewTH1( "Evt2b_O5Asym",		"A_{O5}",	"", 	 "Events", 	"", 	"",		2, 0,   2) ;
		h1.addNewTH1( "Evt2b_O5Asym_PT50",	"A_{O5}",	"", 	 "Events", 	"", 	"",		2, 0,   2) ;
		h1.addNewTH1( "Evt2b_O5Asym_PT60",	"A_{O5}",	"", 	 "Events", 	"", 	"",		2, 0,   2) ;
		h1.addNewTH1( "Evt2b_O5DiscardEvt", 	"",        	 "",   	 "Evetns", 	"", 	"",     3, 0, 3 );
	
		h1.addNewTH1( "SelJet_PT40_Pt",	"pT of selected Jet pT>40",	  "p_{T}(selected j)", 	"Yields", 	"GeV", 	"",		500, 0,   500 );
		h1.addNewTH1( "SelJet_PT50_Pt",	"pT of selected Jet pT>50",	  "p_{T}(selected j)", 	"Yields", 	"GeV", 	"",		500, 0,   500 );
		h1.addNewTH1( "SelJet_PT60_Pt",	"pT of selected Jet pT>60",	  "p_{T}(selected j)", 	"Yields", 	"GeV", 	"",		500, 0,   500 );
	}

	h1.CreateTH1();
	h1.Sumw2();

	h1.GetTH1("Evt_Channel")->GetXaxis()->SetBinLabel(1,"Lepton+Jets");
	h1.GetTH1("Evt_Channel")->GetXaxis()->SetBinLabel(2,"MultiJets");

	if( opts.isLepJets ){
		h1.GetTH1("Evt_Channel")->Fill("Lepton+Jets", 1);
		setObservableHist(h1.GetTH1("Evt_O7Asym"),    "O_{7}");
		setObservableHist(h1.GetTH1("Evt_O7Asym_Mu"), "O_{7}");
		setObservableHist(h1.GetTH1("Evt_O7Asym_El"), "O_{7}");
		setObservableHist(h1.GetTH1("Evt_O2Asym"),    "O_{2}");
		setObservableHist(h1.GetTH1("Evt_O2Asym_Mu"), "O_{2}");
		setObservableHist(h1.GetTH1("Evt_O2Asym_El"), "O_{2}");
		setObservableHist(h1.GetTH1("Evt2b_O7Asym"),    "O_{7}");
		setObservableHist(h1.GetTH1("Evt2b_O7Asym_Mu"), "O_{7}");
		setObservableHist(h1.GetTH1("Evt2b_O7Asym_El"), "O_{7}");
		setObservableHist(h1.GetTH1("Evt2b_O2Asym"),    "O_{2}");
		setObservableHist(h1.GetTH1("Evt2b_O2Asym_Mu"), "O_{2}");
		setObservableHist(h1.GetTH1("Evt2b_O2Asym_El"), "O_{2}");
		setCutFlow(h1.GetTH1("Evt_CutFlow"),    "lj");
		setCutFlow(h1.GetTH1("Evt_CutFlow_Mu"), "ljm");
		setCutFlow(h1.GetTH1("Evt_CutFlow_El"), "lje");
		setLeptonSelHist(h1.GetTH1("Evt_MuCut"));
		setLeptonSelHist(h1.GetTH1("Evt_ElCut"));
	}
	if( opts.isMultiJets ){
		h1.GetTH1("Evt_Channel")->Fill("MultiJets", 1);
		setCutFlow(h1.GetTH1("Evt_CutFlow"),    "mj");
		setCutFlow(h1.GetTH1("Evt2b_CutFlow"),    "mj2b");
		setObservableHist(h1.GetTH1("Evt_O7Asym"),       "O_{7}");
		setObservableHist(h1.GetTH1("Evt_O7Asym_PT50"),  "O_{7}");
		setObservableHist(h1.GetTH1("Evt_O7Asym_PT60"),  "O_{7}");
		setObservableHist(h1.GetTH1("Evt_O5Asym"),    	 "O_{5}");
		setObservableHist(h1.GetTH1("Evt_O5Asym_PT50"),  "O_{5}");
		setObservableHist(h1.GetTH1("Evt_O5Asym_PT60"),  "O_{5}");
		setObservableHist(h1.GetTH1("Evt2b_O7Asym"),     "O_{7}");
		setObservableHist(h1.GetTH1("Evt2b_O7Asym_PT50"),"O_{7}");
		setObservableHist(h1.GetTH1("Evt2b_O7Asym_PT60"),"O_{7}");
		setObservableHist(h1.GetTH1("Evt2b_O5Asym"),     "O_{5}");
		setObservableHist(h1.GetTH1("Evt2b_O5Asym_PT50"),"O_{5}");
		setObservableHist(h1.GetTH1("Evt2b_O5Asym_PT60"),"O_{5}");
		setDiscardEvtHist(h1.GetTH1("Evt_O7DiscardEvt"), "O_{7}");
		setDiscardEvtHist(h1.GetTH1("Evt_O5DiscardEvt"), "O_{5}");
		setDiscardEvtHist(h1.GetTH1("Evt2b_O7DiscardEvt"), "O_{7}");
		setDiscardEvtHist(h1.GetTH1("Evt2b_O5DiscardEvt"), "O_{5}");
	}

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

		/*// Vertex selection
		vector<Vertex> selVertex;
		for( int idx=0; idx<branchVtx->GetEntries(); idx++){
			Vertex *vtx = (Vertex*)branchVtx->At(idx);
			if( abs(vtx->Z)>24 ) continue;
			if( sqrt((vtx->X*vtx->X)+(vtx->Y*vtx->Y))>2 ) continue;
			selVertex.push_back(*vtx);
		}
		*/

		////* Jets selections
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

			if( abs(jet->Eta)>=2.4) continue;
			if( ( opts.isLepJets && jet->PT>30. )|| ( opts.isMultiJets && jet->PT>40. )){ 
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
			}

			if (  ( opts.isLepJets && jet->PT>30. && jet->BTag)
				||( opts.isMultiJets && jet->PT>40. && jet->BTag) 
			){ 
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
			}
		}
		h1.GetTH1("Evt_NJets")->Fill(branchJet->GetEntries());	
		h1.GetTH1("Evt_NSelJets")->Fill(seljetCol.size());	
		h1.GetTH1("Evt_NbJets")->Fill(bjetCol.size());
	
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
		/////* Lepton + Jets Channel ------------------------------------------------------
		if( opts.isLepJets ){
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

			//* Fill cut flow
			bool isMuCh=false; 
			bool isElCh=false;
			bool is2bMuCh=false; 
			bool is2bElCh=false;
			Jet bjet1, bjet2, jet1; 
			Muon isoMu;
			Electron isoEl;

			h1.GetTH1("Evt_CutFlow")->Fill("All", 1);
			h1.GetTH1("Evt_CutFlow_Mu")->Fill("All", 1);
			h1.GetTH1("Evt_CutFlow_El")->Fill("All", 1);

			//if( selVertex.size() >= 1 ){
			//	h1.GetTH1("Evt_CutFlow")->Fill("#geq1 goodVtx", 1);
				if( (selMuCol.size()+selElCol.size()) == 1  ){
					h1.GetTH1("Evt_CutFlow")->Fill("1 isoLep", 1);
					if( selMuCol.size() == 1 ){
						isoMu=selMuCol[0];
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
									// Lable the hardest non_bjet 
									int j1;
									double pt1=0;	
									for( int i=0; i<seljetCol.size(); i++){
										if( !seljetCol[i].BTag && pt1<seljetCol[i].PT ){
											j1=i;
											pt1=seljetCol[i].PT;
										}
									}
									jet1=seljetCol[j1];

									if( bjetCol.size() >= 2 ){
										isMuCh=true;	
										h1.GetTH1("Evt_CutFlow")->Fill("#geq2 bjets", 1);
										h1.GetTH1("Evt_CutFlow_Mu")->Fill("#geq2 bjets", 1);
										// Lable bjet by Pt
										sort2HighPt( bjetCol, bjet1, bjet2 );

										h1.GetTH1("bJet12_Px")->Fill(bjet1.P4().Px());
										h1.GetTH1("bJet12_Px")->Fill(bjet2.P4().Px());
										h1.GetTH1("bJet12_Py")->Fill(bjet1.P4().Py());
										h1.GetTH1("bJet12_Py")->Fill(bjet2.P4().Py());
										h1.GetTH1("bJet12_Pz")->Fill(bjet1.P4().Pz());
										h1.GetTH1("bJet12_Pz")->Fill(bjet2.P4().Pz());
									}

									if( bjetCol.size() == 2 ){	
										is2bMuCh=true;	
										h1.GetTH1("Evt_CutFlow")->Fill("=2 bjets", 1);
										h1.GetTH1("Evt_CutFlow_Mu")->Fill("=2 bjets", 1);
									}
								}
							}	
						}
					}else{
						isoEl=selElCol[0];
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
									// Lable the hardest non_bjet 
									int j1;
									double pt1=0;	
									for( int i=0; i<seljetCol.size(); i++){
										if( !seljetCol[i].BTag && pt1<seljetCol[i].PT ){
											j1=i;
											pt1=seljetCol[i].PT;
										}
									}
									jet1=seljetCol[j1];

									if( bjetCol.size() >= 2 ){ 
										isElCh=true;	
										h1.GetTH1("Evt_CutFlow")->Fill("#geq2 bjets", 1);
										h1.GetTH1("Evt_CutFlow_El")->Fill("#geq2 bjets", 1);
										// Lable bjet by Pt
										sort2HighPt( bjetCol, bjet1, bjet2 );
										h1.GetTH1("bJet12_Px")->Fill(bjet1.P4().Px());
										h1.GetTH1("bJet12_Px")->Fill(bjet2.P4().Px());
										h1.GetTH1("bJet12_Py")->Fill(bjet1.P4().Py());
										h1.GetTH1("bJet12_Py")->Fill(bjet2.P4().Py());
										h1.GetTH1("bJet12_Pz")->Fill(bjet1.P4().Pz());
										h1.GetTH1("bJet12_Pz")->Fill(bjet2.P4().Pz());
									}
									if( bjetCol.size() == 2 ){
										is2bElCh=true;	
										h1.GetTH1("Evt_CutFlow")->Fill("=2 bjets", 1);
										h1.GetTH1("Evt_CutFlow_El")->Fill("=2 bjets", 1);
									}
								}
							}	
						}				
					}
				//}
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

			//* Fill observables O7 and O2
			//* bJets >= 2
			if( isMuCh && !isElCh){
				vector<double> O2_1v = addP3( bjet1.P4(), bjet2.P4() );
				vector<double> O2_2v = crossP3( isoMu.P4(), jet1.P4() );
				double O2 = dotP3( O2_1v, O2_2v );
				h1.GetTH1("Evt_O2")->Fill(O2/MT3);	
				h1.GetTH1("Evt_O2_Mu")->Fill(O2/MT3);
				if( O2 > 0 ){
					h1.GetTH1("Evt_O2Asym")->Fill("O_{2}>0",1);
					h1.GetTH1("Evt_O2Asym_Mu")->Fill("O_{2}>0",1);
				}else{
					h1.GetTH1("Evt_O2Asym")->Fill("O_{2}<0",1);
					h1.GetTH1("Evt_O2Asym_Mu")->Fill("O_{2}<0",1);
				}

				double O7_1z = dotP3( az, addP3( bjet1.P4(), bjet2.P4(),-1 ));
				double O7_2z = dotP3( az, crossP3( bjet1.P4(), bjet2.P4() ));
				double O7 = O7_1z*O7_2z;
				h1.GetTH1("Evt_O7")->Fill(O7/MT3);	
				h1.GetTH1("Evt_O7_Mu")->Fill(O7/MT3);
				h1.GetTH1("Evt_O7_term1")->Fill(O7_1z);
				h1.GetTH1("Evt_O7_term2")->Fill(O7_2z);
				if( O7 > 0 ){
					h1.GetTH1("Evt_O7Asym")->Fill("O_{7}>0",1);
					h1.GetTH1("Evt_O7Asym_Mu")->Fill("O_{7}>0",1);
				}else{
					h1.GetTH1("Evt_O7Asym")->Fill("O_{7}<0",1);
					h1.GetTH1("Evt_O7Asym_Mu")->Fill("O_{7}<0",1);
				}
			}else if( !isMuCh && isElCh){
				vector<double> O2_1v = addP3( bjet1.P4(), bjet2.P4() );
				vector<double> O2_2v = crossP3( isoEl.P4(), jet1.P4() );
				double O2 = dotP3( O2_1v, O2_2v );
				h1.GetTH1("Evt_O2")->Fill(O2/MT3);	
				h1.GetTH1("Evt_O2_El")->Fill(O2/MT3);
				if( O2 > 0 ){
					h1.GetTH1("Evt_O2Asym")->Fill("O_{2}>0",1);
					h1.GetTH1("Evt_O2Asym_El")->Fill("O_{2}>0",1);
				}else{
					h1.GetTH1("Evt_O2Asym")->Fill("O_{2}<0",1);
					h1.GetTH1("Evt_O2Asym_El")->Fill("O_{2}<0",1);
				}

				double O7_1z = dotP3(az, addP3(bjet1.P4(), bjet2.P4(),-1));
				double O7_2z = dotP3(az, crossP3(bjet1.P4(), bjet2.P4()));
				double O7 = O7_1z*O7_2z;
				h1.GetTH1("Evt_O7")->Fill(O7/MT3);	
				h1.GetTH1("Evt_O7_El")->Fill(O7/MT3);	
				h1.GetTH1("Evt_O7_term1")->Fill(O7_1z);
				h1.GetTH1("Evt_O7_term2")->Fill(O7_2z);
				if( O7 > 0 ){
					h1.GetTH1("Evt_O7Asym")->Fill("O_{7}>0", 1);
					h1.GetTH1("Evt_O7Asym_El")->Fill("O_{7}>0",1);
				}else{
					h1.GetTH1("Evt_O7Asym")->Fill("O_{7}<0",1);
					h1.GetTH1("Evt_O7Asym_El")->Fill("O_{7}<0",1);
				}
			}
			//* bJets == 2
			if( is2bMuCh && !is2bElCh){
				vector<double> O2_1v = addP3( bjetCol[0].P4(), bjetCol[1].P4() );
				vector<double> O2_2v = crossP3( isoMu.P4(), jet1.P4() );
				double O2 = dotP3( O2_1v, O2_2v );
				h1.GetTH1("Evt2b_O2")->Fill(O2/MT3);	
				h1.GetTH1("Evt2b_O2_Mu")->Fill(O2/MT3);
				if( O2 > 0 ){
					h1.GetTH1("Evt2b_O2Asym")->Fill("O_{2}>0",1);
					h1.GetTH1("Evt2b_O2Asym_Mu")->Fill("O_{2}>0",1);
				}else{
					h1.GetTH1("Evt2b_O2Asym")->Fill("O_{2}<0",1);
					h1.GetTH1("Evt2b_O2Asym_Mu")->Fill("O_{2}<0",1);
				}

				double O7_1z = dotP3( az, addP3( bjetCol[0].P4(), bjetCol[1].P4(),-1 ));
				double O7_2z = dotP3( az, crossP3( bjetCol[0].P4(), bjetCol[1].P4() ));
				double O7 = O7_1z*O7_2z;
				h1.GetTH1("Evt2b_O7")->Fill(O7/MT3);	
				h1.GetTH1("Evt2b_O7_Mu")->Fill(O7/MT3);
				if( O7 > 0 ){
					h1.GetTH1("Evt2b_O7Asym")->Fill("O_{7}>0",1);
					h1.GetTH1("Evt2b_O7Asym_Mu")->Fill("O_{7}>0",1);
				}else{
					h1.GetTH1("Evt2b_O7Asym")->Fill("O_{7}<0",1);
					h1.GetTH1("Evt2b_O7Asym_Mu")->Fill("O_{7}<0",1);
				}
			}else if( !is2bMuCh && is2bElCh){
				vector<double> O2_1v = addP3( bjetCol[0].P4(), bjetCol[1].P4() );
				vector<double> O2_2v = crossP3( isoEl.P4(), jet1.P4() );
				double O2 = dotP3( O2_1v, O2_2v );
				h1.GetTH1("Evt2b_O2")->Fill(O2/MT3);	
				h1.GetTH1("Evt2b_O2_El")->Fill(O2/MT3);
				if( O2 > 0 ){
					h1.GetTH1("Evt2b_O2Asym")->Fill("O_{2}>0",1);
					h1.GetTH1("Evt2b_O2Asym_El")->Fill("O_{2}>0",1);
				}else{
					h1.GetTH1("Evt2b_O2Asym")->Fill("O_{2}<0",1);
					h1.GetTH1("Evt2b_O2Asym_El")->Fill("O_{2}<0",1);
				}

				double O7_1z = dotP3(az, addP3( bjetCol[0].P4(), bjetCol[1].P4(),-1));
				double O7_2z = dotP3(az, crossP3( bjetCol[0].P4(), bjetCol[1].P4()));
				double O7 = O7_1z*O7_2z;
				h1.GetTH1("Evt2b_O7")->Fill(O7/MT3);	
				h1.GetTH1("Evt2b_O7_El")->Fill(O7/MT3);	
				if( O7 > 0 ){
					h1.GetTH1("Evt2b_O7Asym")->Fill("O_{7}>0", 1);
					h1.GetTH1("Evt2b_O7Asym_El")->Fill("O_{7}>0",1);
				}else{
					h1.GetTH1("Evt2b_O7Asym")->Fill("O_{7}<0",1);
					h1.GetTH1("Evt2b_O7Asym_El")->Fill("O_{7}<0",1);
				}
			}
		} //* Lepton + Jets Channel END

		/////* Multi-Jets Channel ------------------------------------------------------
		if( opts.isMultiJets ){
			//// Jet additional selections 
			vector<Jet> seljetCol2, seljetCol3, bjetCol2, bjetCol3;
			for( int j=0; j<seljetCol.size(); j++){
				Jet jet = seljetCol.at(j);
				h1.GetTH1("SelJet_PT40_Pt")->Fill(jet.PT);
				if( jet.PT<50. ) continue;
				seljetCol2.push_back(jet);
				h1.GetTH1("SelJet_PT50_Pt")->Fill(jet.PT);
				if( jet.PT<60. ) continue;
				seljetCol3.push_back(jet);
				h1.GetTH1("SelJet_PT60_Pt")->Fill(jet.PT);
			}
			for( int j=0; j<bjetCol.size(); j++){
				Jet bjet = bjetCol.at(j);
				if( bjet.PT<50. ) continue;
				bjetCol2.push_back(bjet);
				if( bjet.PT<60. ) continue;
				bjetCol3.push_back(bjet);
			}
			//* Fill histo for event informations
			h1.GetTH1("Evt_NSelJets_PT40")->Fill(seljetCol.size());
			h1.GetTH1("Evt_NSelJets_PT50")->Fill(seljetCol2.size());
			h1.GetTH1("Evt_NSelJets_PT60")->Fill(seljetCol3.size());
			//* Fill cut flow
			vector<Jet> bjet12pT40, bjet12pT50, bjet12pT60;
			vector<Jet> nonbjetColPT40, nonbjetColPT50, nonbjetColPT60; 

			bool isMutijetPT40, isMutijetPT50, isMutijetPT60;
			bool isMutijet2bPT40, isMutijet2bPT50, isMutijet2bPT60;
			isMutijetPT40=isMutijetPT50=isMutijetPT60=false;
			isMutijet2bPT40=isMutijet2bPT50=isMutijet2bPT60=false;

			h1.GetTH1("Evt_CutFlow")->Fill("All", 1);
			h1.GetTH1("Evt2b_CutFlow")->Fill("All", 1);
			if( ( selMuCol.size() + selElCol.size() ) == 0 ){
				h1.GetTH1("Evt_CutFlow")->Fill("veto(Hard Lep)", 1);
				h1.GetTH1("Evt2b_CutFlow")->Fill("veto(Hard Lep)", 1);
				if( seljetCol3.size() >= 4 ){
					h1.GetTH1("Evt_CutFlow")->Fill("#geq4 pT60 jets", 1);
					h1.GetTH1("Evt2b_CutFlow")->Fill("#geq4 pT60 jets", 1);
					if( seljetCol2.size() >= 5 ){
						h1.GetTH1("Evt_CutFlow")->Fill("#geq5 pT50 jets", 1);
						h1.GetTH1("Evt2b_CutFlow")->Fill("#geq5 pT50 jets", 1);
						if( seljetCol.size() >= 6 ){
							h1.GetTH1("Evt_CutFlow")->Fill("#geq6 pT40 jets", 1);
							h1.GetTH1("Evt2b_CutFlow")->Fill("#geq6 pT50 jets", 1);
							h1.GetTH1("Evt_NbJets_PT40_evtSelNJets")->Fill(bjetCol.size());
							h1.GetTH1("Evt_NbJets_PT50_evtSelNJets")->Fill(bjetCol2.size());
							h1.GetTH1("Evt_NbJets_PT60_evtSelNJets")->Fill(bjetCol3.size());
							if( bjetCol.size() >= 2 ){
								isMutijetPT40=true;
								sort2HighPt( bjetCol, bjet12pT40 );
								extractNonTopBjet( seljetCol, bjet12pT40, nonbjetColPT40 );
								h1.GetTH1("Evt_CutFlow")->Fill("#geq2 pT40 bjets", 1);

								if( bjetCol.size() == 2 ){
									isMutijet2bPT40=true;
									h1.GetTH1("Evt2b_CutFlow")->Fill("==2 pT40 bjets", 1);
								}
							}	
							if( bjetCol2.size() >= 2 ){
								isMutijetPT50=true;
								sort2HighPt( bjetCol2, bjet12pT50 );
								extractNonTopBjet( seljetCol, bjet12pT50, nonbjetColPT50 );
								h1.GetTH1("Evt_CutFlow")->Fill("#geq2 pT50 bjets", 1);

								if( bjetCol2.size() == 2 ){
									isMutijet2bPT50=true;
									h1.GetTH1("Evt2b_CutFlow")->Fill("==2 pT50 bjets", 1);
								}
							}		
							if( bjetCol3.size() >= 2 ){
								isMutijetPT60=true;
								sort2HighPt( bjetCol3, bjet12pT60 );
								extractNonTopBjet( seljetCol, bjet12pT60, nonbjetColPT60 );
								h1.GetTH1("Evt_CutFlow")->Fill("#geq2 pT60 bjets", 1);

								if( bjetCol3.size() == 2 ){
									isMutijet2bPT60=true;
									h1.GetTH1("Evt2b_CutFlow")->Fill("==2 pT60 bjets", 1);
								}
							}
						}
					}
				}
			}
			//* Fill observables O7 and O5
			//* bjets PT>40 
			if( isMutijetPT40 || isMutijet2bPT40 ){
				// O2
				TLorentzVector bjet1 = bjet12pT40[0].P4();
				TLorentzVector bjet2 = bjet12pT40[1].P4();
				TLorentzVector jet1 = compareDeltaRHighPt( bjet1, nonbjetColPT40 );
				TLorentzVector jet2 = compareDeltaRHighPt( bjet2, nonbjetColPT40 );
				TVector3 bbCM = -( bjet1 + bjet2 ).BoostVector();
				bjet1.Boost(bbCM); // bjet2.Boost(bbCM); // In CM(b,b~), b and b~ will back-to-back, i.e vector(b)=-vector(b~)
				 jet1.Boost(bbCM);  jet2.Boost(bbCM);
				double O5 = dotP3( bjet1, crossP3( jet1, jet2 ));
				// * O5_1 will always be the same with O5_2.
				// double O5_1 = dotP3( bjet1, crossP3( jet1, jet2 )); 
				// double O5_2 = dotP3( bjet2, crossP3( jet2, jet1 ));
				// printf("bjet1 ( %6.2f, %6.2f, %6.2f ) -> ( %6.2f, %6.2f, %6.2f )\n", bjet12pT40[0].P4().Px(), bjet12pT40[0].P4().Py(), bjet12pT40[0].P4().Pz(), bjet1.Px(), bjet1.Py(), bjet1.Pz());
				// printf("bjet2 ( %6.2f, %6.2f, %6.2f ) -> ( %6.2f, %6.2f, %6.2f )\n", bjet12pT40[1].P4().Px(), bjet12pT40[1].P4().Py(), bjet12pT40[1].P4().Pz(), bjet2.Px(), bjet2.Py(), bjet2.Pz());
				// printf("O5_1: %6.2f, O5_2: %6.2f\n\n", O5_1, O5_2);

				// O7
				double O7_1z = dotP3( az, addP3( bjet12pT40[0].P4(), bjet12pT40[1].P4(),-1 ));
				double O7_2z = dotP3( az, crossP3( bjet12pT40[0].P4(), bjet12pT40[1].P4() ));
				double O7 = O7_1z*O7_2z;

				// fill histo
				if( isMutijetPT40 ){
					if( O5 != 0 ){ 
						h1.GetTH1("Evt_O5")->Fill(O5/MT3);	
						if( O5 > 0 )
							h1.GetTH1("Evt_O5Asym")->Fill("O_{5}>0",1);
						else
							h1.GetTH1("Evt_O5Asym")->Fill("O_{5}<0",1);
					}else{
						h1.GetTH1("Evt_O5DiscardEvt")->Fill("PT40, O_{5}==0",1);
					}

					if( O7 != 0 ){ 
						h1.GetTH1("Evt_O7")->Fill(O7/MT3);	
						if( O7 > 0 )
							h1.GetTH1("Evt_O7Asym")->Fill("O_{7}>0",1);
						else
							h1.GetTH1("Evt_O7Asym")->Fill("O_{7}<0",1);
					}else{
						h1.GetTH1("Evt_O7DiscardEvt")->Fill("PT40, O_{7}==0",1);
					}
				}
				if( isMutijet2bPT40 ){
					if( O5 != 0 ){ 
						h1.GetTH1("Evt2b_O5")->Fill(O5/MT3);	
						if( O5 > 0 )
							h1.GetTH1("Evt2b_O5Asym")->Fill("O_{5}>0",1);
						else
							h1.GetTH1("Evt2b_O5Asym")->Fill("O_{5}<0",1);
					}else{
						h1.GetTH1("Evt2b_O5DiscardEvt")->Fill("PT40, O_{5}==0",1);
					}

					if( O7 != 0 ){ 
						h1.GetTH1("Evt2b_O7")->Fill(O7/MT3);	
						if( O7 > 0 )
							h1.GetTH1("Evt2b_O7Asym")->Fill("O_{7}>0",1);
						else
							h1.GetTH1("Evt2b_O7Asym")->Fill("O_{7}<0",1);
					}else{
						h1.GetTH1("Evt2b_O7DiscardEvt")->Fill("PT40, O_{7}==0",1);
					}
				}	
			}
			//* bjets PT>50 
			if( isMutijetPT50 || isMutijet2bPT50 ){
				// O2
				TLorentzVector bjet1 = bjet12pT50[0].P4();
				TLorentzVector bjet2 = bjet12pT50[1].P4();
				TLorentzVector jet1 = compareDeltaRHighPt( bjet1, nonbjetColPT50 );
				TLorentzVector jet2 = compareDeltaRHighPt( bjet2, nonbjetColPT50 );
				TVector3 bbCM = -( bjet1 + bjet2 ).BoostVector();
				bjet1.Boost(bbCM); // bjet2.Boost(bbCM); // In CM(b,b~), b and b~ will back-to-back, i.e vector(b)=-vector(b~)
				 jet1.Boost(bbCM);  jet2.Boost(bbCM);
				double O5 = dotP3( bjet1, crossP3( jet1, jet2 ));

				// O7
				double O7_1z = dotP3( az, addP3( bjet12pT50[0].P4(), bjet12pT50[1].P4(),-1 ));
				double O7_2z = dotP3( az, crossP3( bjet12pT50[0].P4(), bjet12pT50[1].P4() ));
				double O7 = O7_1z*O7_2z;

				// fill histo
				if( isMutijetPT50 ){
					if( O5 != 0 ){ 
						h1.GetTH1("Evt_O5_PT50")->Fill(O5/MT3);	
						if( O5 > 0 )
							h1.GetTH1("Evt_O5Asym_PT50")->Fill("O_{5}>0",1);
						else
							h1.GetTH1("Evt_O5Asym_PT50")->Fill("O_{5}<0",1);
					}else{
						h1.GetTH1("Evt_O5DiscardEvt")->Fill("PT50, O_{5}==0",1);
					}

					if( O7 != 0 ){ 
						h1.GetTH1("Evt_O7_PT50")->Fill(O7/MT3);	
						if( O7 > 0 )
							h1.GetTH1("Evt_O7Asym_PT50")->Fill("O_{7}>0",1);
						else
							h1.GetTH1("Evt_O7Asym_PT50")->Fill("O_{7}<0",1);
					}else{
						h1.GetTH1("Evt_O7DiscardEvt")->Fill("PT50, O_{7}==0",1);
					}
				}
				if( isMutijet2bPT50 ){
					if( O5 != 0 ){ 
						h1.GetTH1("Evt2b_O5_PT50")->Fill(O5/MT3);	
						if( O5 > 0 )
							h1.GetTH1("Evt2b_O5Asym_PT50")->Fill("O_{5}>0",1);
						else 
							h1.GetTH1("Evt2b_O5Asym_PT50")->Fill("O_{5}<0",1);
					}else{
						h1.GetTH1("Evt2b_O5DiscardEvt")->Fill("PT50, O_{5}==0",1);
					}

					if( O7 != 0 ){ 
						h1.GetTH1("Evt2b_O7_PT50")->Fill(O7/MT3);	
						if( O7 > 0 )
							h1.GetTH1("Evt2b_O7Asym_PT50")->Fill("O_{7}>0",1);
						else 
							h1.GetTH1("Evt2b_O7Asym_PT50")->Fill("O_{7}<0",1);
					}else{
						h1.GetTH1("Evt2b_O7DiscardEvt")->Fill("PT50, O_{7}==0",1);
					}
				}
			}
			//* bjets PT>60 
			if( isMutijetPT60 || isMutijet2bPT60 ){
				// O2
				TLorentzVector bjet1 = bjet12pT60[0].P4();
				TLorentzVector bjet2 = bjet12pT60[1].P4();
				TLorentzVector jet1 = compareDeltaRHighPt( bjet1, nonbjetColPT60 );
				TLorentzVector jet2 = compareDeltaRHighPt( bjet2, nonbjetColPT60 );
				TVector3 bbCM = -( bjet1 + bjet2 ).BoostVector();
				bjet1.Boost(bbCM); // bjet2.Boost(bbCM); // In CM(b,b~), b and b~ will back-to-back, i.e vector(b)=-vector(b~)
				 jet1.Boost(bbCM);  jet2.Boost(bbCM);
				double O5 = dotP3( bjet1, crossP3( jet1, jet2 ));

				// O7
				double O7_1z = dotP3( az, addP3( bjet12pT60[0].P4(), bjet12pT60[1].P4(),-1 ));
				double O7_2z = dotP3( az, crossP3( bjet12pT60[0].P4(), bjet12pT60[1].P4() ));
				double O7 = O7_1z*O7_2z;
				if( isMutijetPT60 ){
					if( O5 != 0 ){ 
						h1.GetTH1("Evt_O5_PT60")->Fill(O5/MT3);					
						if( O5 > 0 )
							h1.GetTH1("Evt_O5Asym_PT60")->Fill("O_{5}>0",1);
						else
							h1.GetTH1("Evt_O5Asym_PT60")->Fill("O_{5}<0",1);
					}else{
						h1.GetTH1("Evt_O5DiscardEvt")->Fill("PT60, O_{5}==0",1);
					}

					if( O7 != 0 ){ 
						h1.GetTH1("Evt_O7_PT60")->Fill(O7/MT3);					
						if( O7 > 0 )
							h1.GetTH1("Evt_O7Asym_PT60")->Fill("O_{7}>0",1);
						else 
							h1.GetTH1("Evt_O7Asym_PT60")->Fill("O_{7}<0",1);
					}else{
						h1.GetTH1("Evt_O7DiscardEvt")->Fill("PT60, O_{7}==0",1);
					}
				}
				if( isMutijet2bPT60 ){
					if( O5 != 0 ){ 
						h1.GetTH1("Evt2b_O5_PT60")->Fill(O5/MT3);	
						if( O5 > 0 )
							h1.GetTH1("Evt2b_O5Asym_PT60")->Fill("O_{5}>0",1);
						else 
							h1.GetTH1("Evt2b_O5Asym_PT60")->Fill("O_{5}<0",1);
					}else{
						h1.GetTH1("Evt2b_O5DiscardEvt")->Fill("PT60, O_{5}==0",1);
					}

					if( O7 != 0 ){ 
						h1.GetTH1("Evt2b_O7_PT60")->Fill(O7/MT3);	
						if( O7 > 0 )
							h1.GetTH1("Evt2b_O7Asym_PT60")->Fill("O_{7}>0",1);
						else 
							h1.GetTH1("Evt2b_O7Asym_PT60")->Fill("O_{7}<0",1);
					}else{
						h1.GetTH1("Evt2b_O7DiscardEvt")->Fill("PT60, O_{7}==0",1);
					}
				}
			}
		 }//* Multi-Jets Channel END
	}//* Events end

	fout->Write();
	fout->Close();
	printf(">> Done!\n");
	printf(">> Output file: %s\n", opts.output.c_str());
}
