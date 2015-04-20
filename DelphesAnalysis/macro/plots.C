void drawObservable( TFile* f, std::string output=".", std::string histName="Evt_O", std::string unit="O/M_{top}^{3}", std::string CHname="LepJets", bool legX=1 ){
   bool isLepJets=false;
   bool isMultiJets=false;
   if( CHname.compare("LepJets") == 0 ) isLepJets=true;
   if( CHname.compare("MultiJets") == 0 ) isMultiJets=true;

   //TCanvas *c1 = new TCanvas("c1", "c1",113,93,1099,750);
   //c1->Range(-0.25,1247.471,2.25,1397.296);
   TCanvas *c1 = new TCanvas("c1", "c1",57,97,1099,752);
   gStyle->SetOptStat(0);
   c1->Range(-2.530337,-153.6535,2.391011,1015.73);
   c1->SetFillColor(0);
   c1->SetBorderMode(0);
   c1->SetBorderSize(2);
   c1->SetRightMargin(0.07945205);
   c1->SetTopMargin(0.0691563);
   c1->SetBottomMargin(0.131397);
   c1->SetFrameBorderMode(0);
   c1->SetFrameBorderMode(0);
	
   TH1D* Oh0 = (TH1D*)f->Get(histName.c_str());
   TH1D* Oh = (TH1D*)Oh0->Clone("CopyOriginal");
   TH1D* Oh_mu, *Oh_el;
   Oh->SetLineColor(4);
   Oh->SetLineWidth(3);
   Oh->GetXaxis()->SetTitle(unit.c_str());
   Oh->GetXaxis()->SetTitleSize(0.05);
   Oh->GetXaxis()->SetTitleFont(62);
   Oh->GetXaxis()->SetLabelSize(0.05);
   Oh->GetXaxis()->SetLabelFont(62);
   Oh->GetYaxis()->SetLabelFont(62);
   Oh->GetYaxis()->SetTitle("Events");
   Oh->GetYaxis()->SetTitleSize(0.06);
   Oh->GetYaxis()->SetTitleOffset(0.77);
   Oh->GetYaxis()->SetTitleFont(42);
   Oh->GetZaxis()->SetLabelFont(62);
   Oh->GetZaxis()->SetLabelSize(0.035);
   Oh->GetZaxis()->SetTitleSize(0.035);
   Oh->GetZaxis()->SetTitleFont(62);
   Oh->Draw("HISTE");
   if( isLepJets )
    Oh_mu = (TH1D*)f->Get((histName+"_Mu").c_str());
    Oh_el = (TH1D*)f->Get((histName+"_El").c_str());
    Oh_mu->SetLineColor(kOrange+4);
    Oh_el->SetLineColor(kGreen+3);
    Oh_mu->SetLineWidth(3);
    Oh_el->SetLineWidth(3);
    Oh_mu->Draw("SAMEHISTE");
    Oh_el->Draw("SAMEHISTE");
	
   TLegend *leg;
   if( legX == 0 ) //Left 
   	leg = new TLegend(0.173516,0.6726768,0.4310502,0.8363384,NULL,"brNDC");
   else //right
    leg = new TLegend(0.6283105,0.7026279,0.9324201,0.8990318,NULL,"brNDC");
	
   leg->SetBorderSize(0);
   leg->SetLineStyle(0);
   leg->SetLineWidth(0);
   leg->SetFillColor(0);
   leg->SetFillStyle(0);
   if( isMultiJets )
    leg->AddEntry(Oh,"Multi-Jets channel","l");
   if( isLepJets )
    leg->AddEntry(Oh,"Combined channel","l");
    leg->AddEntry(Oh_mu,"Muon+Jets channel","l");
    leg->AddEntry(Oh_el,"Electron+Jets channel","l");
   leg->Draw();
   c1->SaveAs((output+"/"+histName+".pdf").c_str());
}
void drawACP( TFile* f, std::string output=".", std::string histName, std::string Oname="O", std::string CHname="", bool legX=0 ){
   TCanvas *c1 = new TCanvas("c1", "c1",113,93,1099,750);
   gStyle->SetOptStat(0);
   c1->Range(-0.25,1247.471,2.25,1397.296);
   c1->SetFillColor(0);
   c1->SetBorderMode(0);
   c1->SetBorderSize(2);
   c1->SetFrameBorderMode(0);
   c1->SetFrameBorderMode(0);
	
   TH1D* ACP1Sigma0 = (TH1D*)f->Get(histName.c_str());
   TH1D* ACP1Sigma = (TH1D*)ACP1Sigma0->Clone("CopyOriginal");
   TH1D *ACP = (TH1D*)ACP1Sigma->Clone("ACP");
   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#9999ff");
   ACP1Sigma->SetFillColor(ci);
   ACP1Sigma->GetXaxis()->SetLabelSize(0.08);
   ACP1Sigma->GetXaxis()->SetLabelFont(62);
   ACP1Sigma->GetXaxis()->SetTitleSize(0.035);
   ACP1Sigma->GetXaxis()->SetTitleFont(62);
   ACP1Sigma->GetYaxis()->SetLabelFont(62);
   ACP1Sigma->GetYaxis()->SetTitle("Events");
   ACP1Sigma->GetYaxis()->SetTitleSize(0.06);
   ACP1Sigma->GetYaxis()->SetTitleOffset(0.77);
   ACP1Sigma->GetYaxis()->SetTitleFont(42);
   ACP1Sigma->GetZaxis()->SetLabelFont(62);
   ACP1Sigma->GetZaxis()->SetLabelSize(0.035);
   ACP1Sigma->GetZaxis()->SetTitleSize(0.035);
   ACP1Sigma->GetZaxis()->SetTitleFont(62);
   ACP1Sigma->Draw("E2");
 
   ci = TColor::GetColor("#000099");
   ACP->SetLineColor(ci);
   ACP->SetLineWidth(3);
   ACP->GetXaxis()->SetBinLabel(1,(Oname+"<0").c_str());
   ACP->GetXaxis()->SetBinLabel(2,(Oname+">0").c_str());
   ACP->GetXaxis()->SetLabelFont(42);
   ACP->GetXaxis()->SetLabelSize(0.035);
   ACP->GetXaxis()->SetTitleSize(0.035);
   ACP->GetXaxis()->SetTitleFont(42);
   ACP->GetYaxis()->SetLabelFont(42);
   ACP->GetYaxis()->SetLabelSize(0.035);
   ACP->GetYaxis()->SetTitleSize(0.035);
   ACP->GetYaxis()->SetTitleFont(42);
   ACP->GetZaxis()->SetLabelFont(42);
   ACP->GetZaxis()->SetLabelSize(0.035);
   ACP->GetZaxis()->SetTitleSize(0.035);
   ACP->GetZaxis()->SetTitleFont(42);
   ACP->Draw("histsametext0");

   TLegend *leg;
   if( legX == 0 ) //Left 
   	leg = new TLegend(0.173516,0.6726768,0.4310502,0.8363384,NULL,"brNDC");
   else //right
    leg = new TLegend(0.5780822,0.7040111,0.8356164,0.8672199,NULL,"brNDC");
	
   leg->SetBorderSize(0);
   leg->SetLineStyle(0);
   leg->SetLineWidth(0);
   leg->SetFillColor(0);
   leg->SetFillStyle(0);
   leg->AddEntry(ACP1Sigma,"1#sigma stat.","f");
   if( CHname.size() == 0 ){
   	leg->AddEntry(ACP1,Oname.c_str(),"l");
   }else{
	std::string legs=Oname+" in "+CHname+" channel";
   	leg->AddEntry(ACP,legs.c_str(),"l");
   }
   leg->Draw();
   c1->SaveAs((output+"/"+histName+".pdf").c_str());
}
