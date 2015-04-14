void do_make_plot(char *name, char *xtitle, char *ytitle, char* option = "ratio")
{
    //TFile *fin = new TFile("result.root");
    TFile *fin = new TFile("result_TTtoLepJet.root");
    
    TCanvas *c1 = new TCanvas("c1","c1",0,0,700,500);
    c1->SetFrameFillColor(10);
    c1->SetFrameBorderMode(0);
    c1->SetFillColor(10);
    c1->SetBorderMode(0);
    c1->SetFrameLineWidth(2);
    c1->SetTopMargin(0.08);
    c1->SetBottomMargin(0.15);
    c1->SetRightMargin(0.05);
    c1->SetLeftMargin(0.15);
    if (strcmp(option,"ratio")==0)
        c1->SetGridy();    
    
    TH1D* frame = fin->Get(name);
    
    frame->SetTitle("");
    if (strcmp(option,"ratio")==0)
        frame->SetStats(kFALSE);
    frame->GetYaxis()->SetLabelSize(0.04);
    frame->GetYaxis()->SetLabelOffset(0.01);
    frame->GetYaxis()->SetTitleSize(0.06);
    frame->GetYaxis()->SetTitleOffset(1.0);
    frame->GetXaxis()->SetLabelSize(0.04);
    frame->GetXaxis()->SetLabelOffset(0.01);
    frame->GetXaxis()->SetTitleSize(0.06);
    frame->GetXaxis()->SetTitleOffset(1.05);
    frame->GetXaxis()->SetTitle(xtitle);
    frame->GetYaxis()->SetTitle(ytitle);
        
    if (strcmp(option,"ratio")==0) {
        frame->GetYaxis()->SetRangeUser(0.,2.);
        frame->SetMarkerStyle(21);
        frame->SetLineWidth(2);
        frame->SetMarkerColor(50);
        frame->SetLineColor(50);
        frame->Draw("e");
    }
    if (strcmp(option,"hist")==0) {
        frame->SetLineWidth(1);
        frame->SetFillColor(41);
        frame->SetLineColor(1);
        frame->Draw("");
    }
}

void mkplot(int sel=1)
{
    switch (sel) {
        case 1:
            do_make_plot("histMass1","M(bb) [GeV]","Entries / 2 GeV","hist");
            break;
        case 2:
            do_make_plot("histMass2","Generator M(bb) [GeV]","Entries / 2 GeV","hist");
            break;
        case 3:
            do_make_plot("histMass3","M(bjj) [GeV]","Entries / 2 GeV","hist");
            break;
        case 4:
            do_make_plot("histMass4","Top-matched M(bjj) [GeV]","Entries / 2 GeV","hist");
            break;
        case 5:
            do_make_plot("histMass5","Top-matched generator M(bjj) [GeV]","Entries / 2 GeV","hist");
            break;
        case 6:
            do_make_plot("histR_pt","p_{T}(rec) [GeV]","p_{T}(gen)/p_{T}(rec)");
            break;
        case 7:
            do_make_plot("histR_eta","|#eta(rec)|","p_{T}(gen)/p_{T}(rec)");
            break;
            
    }

}
