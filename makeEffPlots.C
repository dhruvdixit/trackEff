#include <fstream>
#include <vector>
void makeEffPlots()
{
  gStyle->SetOptStat(0);
  gStyle->SetLegendBorderSize(0);
  
  
  
  TFile* fTPC = new TFile("/global/homes/d/ddixit/trackEff/OutputData/fout_3_17g6a3_pthat2_clusterv2_small.root","READ");
  TFile* fITS = new TFile("/global/homes/d/ddixit/trackEff/OutputData/fout_16_17g6a3_pthat2_clusterv2_small.root","READ");
  
  if(!fTPC->IsOpen())
    {
      cout << "cannot find the TPC+ITS file" << endl;
      return;
    }
  if(!fITS->IsOpen())
    {
      cout << "cannot find the ITS only file" << endl;
      return;
    }
  //////////////////////////Eta Efficiency//////////////////////////
  TH1F* hTrueEta_TPC = (TH1F*)fTPC->Get("hTrue_eta");
  TH1F* hRecoEta_TPC = (TH1F*)fTPC->Get("hReco_eta");
  TH1F* hTrueEta_ITS = (TH1F*)fITS->Get("hTrue_eta");
  TH1F* hRecoEta_ITS = (TH1F*)fITS->Get("hReco_eta");
  
  
  TH1F* hEtaEff_TPC = (TH1F*)hRecoEta_TPC->Clone("hEtaEff_TPC");
  hEtaEff_TPC->Divide(hTrueEta_TPC);
  
  TH1F* hEtaEff_ITS = (TH1F*)hRecoEta_ITS->Clone("hEtaEff_ITS");
  hEtaEff_ITS->Divide(hTrueEta_ITS);
  
  hEtaEff_TPC->SetTitle(";#eta^{true};#epsilon");
  hEtaEff_ITS->SetTitle(";#eta^{true};#epsilon");
  hEtaEff_TPC->SetLineColor(kRed);
  hEtaEff_ITS->SetLineColor(kBlack);
  
  TLegend* leg = new TLegend(0.7,0.15,0.85,0.3);
  leg->SetHeader("p_{T} > 1.0 GeV/c");
  leg->AddEntry(hEtaEff_TPC,"TPC+ITS");
  leg->AddEntry(hEtaEff_ITS,"ITS only"); 
  
  TCanvas* c1 = new TCanvas("c1","c1",800,600);
  hEtaEff_ITS->Draw();
  hEtaEff_TPC->Draw("same");
  leg->Draw("same");
  
  //////////////////////Track Efficiency///////////////////////////////
  
  TGraphAsymmErrors* eff_TPC = (TGraphAsymmErrors*)fTPC->Get("Efficiency");
  TGraphAsymmErrors* eff_ITS = (TGraphAsymmErrors*)fITS->Get("Efficiency");
  eff_TPC->SetLineColor(kRed);
  eff_ITS->SetLineColor(kBlack);
  eff_TPC->GetXaxis()->SetRangeUser(0.15,10);
  eff_ITS->GetXaxis()->SetRangeUser(0.15,10);
  eff_TPC->GetYaxis()->SetRangeUser(0,1);
  eff_ITS->GetYaxis()->SetRangeUser(0,1);

  TLegend* leg2 = new TLegend(0.7,0.15,0.85,0.3);
  leg2->AddEntry(eff_TPC,"TPC+ITS");
  leg2->AddEntry(eff_ITS,"ITS only"); 
  
  TCanvas* c2 = new TCanvas("c2","c2",800,600);
  eff_ITS->Draw();
  eff_TPC->Draw("same");
  leg2->Draw("same");
    
  TH1F* hTrue_TPC = (TH1F*)fTPC->Get("hTruth");
  TH1F* hRecoEmbed_TPC = (TH1F*)fTPC->Get("hRecoEmbed");
  TH1F* hTrue_ITS = (TH1F*)fITS->Get("hTruth");
  TH1F* hRecoEmbed_ITS = (TH1F*)fITS->Get("hRecoEmbed");
  
  
  TH1F* hEff_TPC = (TH1F*)hRecoEmbed_TPC->Clone("hEff_TPC");
  hEff_TPC->Divide(hTrue_TPC);
  TH1F* hEff_ITS = (TH1F*)hRecoEmbed_ITS->Clone("hEff_ITS");
  hEff_ITS->Divide(hTrue_ITS);
  hEff_TPC->SetMarkerColor(kRed);
  hEff_ITS->SetMarkerColor(kBlack);
  hEff_TPC->SetMarkerStyle(24);
  hEff_ITS->SetMarkerStyle(24);


  hEff_TPC->SetTitle(";p_{T}^{true} (GeV/c);#epsilon");
  hEff_ITS->SetTitle(";p_{T}^{true} (GeV/c);#epsilon");
  hEff_TPC->SetLineColor(kRed);
  hEff_ITS->SetLineColor(kBlack);
  hEff_TPC->GetXaxis()->SetRangeUser(0.15,10);
  hEff_ITS->GetXaxis()->SetRangeUser(0.15,10);
  hEff_TPC->GetYaxis()->SetRangeUser(0,2);
  hEff_ITS->GetYaxis()->SetRangeUser(0,2);
  
  ///////////////////////////Resolution Efficiency/////////////////////////
  TH1F* hReco_TPC = (TH1F*)fTPC->Get("hReco");
  TH1F* hReco_ITS = (TH1F*)fITS->Get("hReco");
  
  
  TH1F* hResoEff_TPC = (TH1F*)hRecoEmbed_TPC->Clone("hResoEff_TPC");
  hResoEff_TPC->Divide(hTrue_TPC);
  
  TH1F* hResoEff_ITS = (TH1F*)hRecoEmbed_ITS->Clone("hResoEff_ITS");
  hResoEff_ITS->Divide(hTrue_ITS);
  
  hResoEff_TPC->SetTitle(";p_{T}^{reco} (GeV/c);#epsilon'");
  hResoEff_ITS->SetTitle(";p_{T}^{reco} (GeV/c);#epsilon'");
  hResoEff_TPC->SetLineColor(kRed);
  hResoEff_ITS->SetLineColor(kBlack);
  hResoEff_TPC->GetXaxis()->SetRangeUser(0.15,10);
  hResoEff_ITS->GetXaxis()->SetRangeUser(0.15,10);
  hResoEff_TPC->GetYaxis()->SetRangeUser(0,1);
  hResoEff_ITS->GetYaxis()->SetRangeUser(0,1);
  hResoEff_TPC->SetMarkerColor(kRed);
  hResoEff_ITS->SetMarkerColor(kBlack);
  hResoEff_TPC->SetMarkerStyle(25);
  hResoEff_ITS->SetMarkerStyle(25);
  
  TLegend* leg3 = new TLegend(0.15,0.15,0.45,0.35);
  leg3->AddEntry(hEff_TPC,"JJ+DPMJET, TPC+ITS N_{prim,rec}^{MC}/N_{prim,gen}^{MC}");
  leg3->AddEntry(hEff_ITS,"JJ+DPMJET, ITS Only N_{prim,rec}^{MC}/N_{prim,gen}^{MC}");
  leg3->AddEntry(hResoEff_TPC,"JJ+DPMJET, TPC+ITS N_{reco}/N_{prim,gen}^{MC}");
  leg3->AddEntry(hResoEff_ITS,"JJ+DPMJET, ITS Only N_{reco}/N_{prim,gen}^{MC}");
  
  
  TCanvas* c3 = new TCanvas("c3", "c3", 800, 600);
  c3->SetLogx();
  hEff_TPC->Draw("P");
  hEff_ITS->Draw("P SAME");
  hResoEff_TPC->Draw("P HIST SAME");
  hResoEff_ITS->Draw("P HIST SAME");
  
  leg3->Draw("SAME" );
  
  
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// //////////////////////////////////////////DATA////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
  
  //TFile* fTPCd = new TFile("/global/homes/d/ddixit/trackEff/OutputData/fout_3_13c_pass4_v2.root","READ");
  //TFile* fITSd = new TFile("/global/homes/d/ddixit/trackEff/OutputData/fout_16_13c_pass4_v2.root","READ");
  //TFile* fTPCd = new TFile("/global/homes/d/ddixit/trackEff/OutputData/fout_3_13b_pass4_v2_1run.root","READ");
  //TFile* fITSd = new TFile("/global/homes/d/ddixit/trackEff/OutputData/fout_16_13b_pass4_v2_1run.root","READ");
  //TFile* fTPCd = new TFile("/global/homes/d/ddixit/trackEff/OutputData/fout_3_13b_pass4_v2_3run.root","READ");
  //TFile* fITSd = new TFile("/global/homes/d/ddixit/trackEff/OutputData/fout_16_13b_pass4_v2_3run.root","READ");
  TFile* fTPCd = new TFile("/global/homes/d/ddixit/trackEff/OutputData/fout_3_13f_pass4_v2_minbias_1run.root","READ");
  TFile* fITSd = new TFile("/global/homes/d/ddixit/trackEff/OutputData/fout_16_13f_pass4_v2_minbias_1run.root","READ");
  
  if(!fTPCd->IsOpen())
    {
      cout << "cannot find the TPC+ITS file" << endl;
      return;
    }
  if(!fITSd->IsOpen())
    {
      cout << "cannot find the ITS only file" << endl;
      return;
    }
  


  ///////////////////pT Spectra from Data//////////////////////////////////
  TH1F* hTrack_TPC = (TH1F*)fTPCd->Get("track_pt");
  TH1F* hTrack_ITS = (TH1F*)fITSd->Get("track_pt");
  hTrack_TPC->SetTitle(";p_{T} (GeV/c);1/N_{evn} dn^{2}/dp_{T}d#eta");
  hTrack_ITS->SetTitle(";p_{T} (GeV/c);1/N_{evn} dn^{2}/dp_{T}d#eta");
  hTrack_TPC->SetMarkerColor(kRed);
  hTrack_ITS->SetMarkerColor(kBlack);
  hTrack_TPC->GetXaxis()->SetRangeUser(0.15,10);
  hTrack_ITS->GetXaxis()->SetRangeUser(0.15,10);
  hTrack_TPC->SetMarkerStyle(8);
  hTrack_ITS->SetMarkerStyle(8);
  
  /*const Double_t bins_track[47] = {
    0.2,   0.25,   0.3,   0.35,   0.4,    0.45,   0.5,    0.55,   0.6,   0.65,   
    0.7,   0.75,   0.8,   0.85,   0.9,    0.95,   1.01,   1.1,   1.2,    1.3,   
    1.4,   1.5,    1.6,   1.7,    1.8,    1.9,    2.0,    2.2,   2.4,    2.6,   
    2.8,   3.0,    3.2,   3.4,    3.6,    3.8,    4.07,   4.5,   5.0,    5.5,
    6.0,   6.5,    7.12,  8.0,    9.0,    10.0,   11.0};//my binning*/ 

 const Double_t bins_track[49] = {
    0.15, 0.20, 0.25, 0.30, 0.35, 0.40, 0.45, 0.50, 0.55, 0.60, 
    0.65, 0.70, 0.75, 0.80, 0.85, 0.90, 0.95, 1.00, 1.10, 1.20,
    1.30, 1.40, 1.50, 1.60, 1.70, 1.80, 1.90, 2.00, 2.20, 2.40,
    2.60, 2.80, 3.00, 3.20, 3.40, 3.60, 3.80, 4.00, 4.50, 5.00,
    5.50, 6.00, 6.50, 7.00, 8.00, 9.00, 10.00,11.00,12.00};//*/
  
 const Double_t published_Data[48] = {

   2.55942e+01, 2.56715e+01, 2.45478e+01, 2.26218e+01, 2.05202e+01,
   1.85302e+01, 1.66830e+01, 1.50036e+01, 1.34819e+01, 1.21208e+01,
   1.09316e+01, 9.86578e+00, 8.92289e+00, 8.06027e+00, 7.30633e+00,
   6.61428e+00, 6.00343e+00, 5.19552e+00, 4.30899e+00, 3.57940e+00,
   2.98222e+00, 2.49543e+00, 2.09140e+00, 1.75490e+00, 1.47859e+00, 
   1.24485e+00, 1.05267e+00, 8.27018e-01, 5.99409e-01, 4.37646e-01,
   3.21463e-01, 2.39181e-01, 1.78957e-01, 1.34288e-01, 1.03206e-01, 
   7.91170e-02, 6.15749e-02, 4.08424e-02, 2.29648e-02, 1.36942e-02, 
   8.38155e-03, 5.43098e-03, 3.61685e-03, 2.08163e-03, 1.06300e-03, 
   5.87051e-04, 3.46767e-04, 2.16321e-04};

 const Double_t published_Data_errors[48]= {
   2.8625e-03, 2.0712e-03, 1.9698e-03, 1.8561e-03, 1.7417e-03, 
   1.6420e-03, 1.5811e-03, 1.5418e-03, 1.4896e-03, 1.4433e-03, 
   1.4047e-03, 1.3555e-03, 1.3089e-03, 1.2608e-03, 1.2166e-03, 
   1.1701e-03, 1.1165e-03, 7.2896e-04, 6.5577e-04, 5.9199e-04, 
   5.3632e-04, 4.8865e-04, 4.4479e-04, 4.0557e-04, 3.7095e-04, 
   3.3902e-04, 3.1093e-04, 1.9454e-04, 1.6497e-04, 1.4045e-04, 
   1.1979e-04, 1.0304e-04, 8.8769e-05, 7.6415e-05, 6.7031e-05, 
   5.8425e-05, 5.1516e-05, 2.6412e-05, 1.9621e-05, 1.5113e-05, 
   1.1691e-05, 9.4211e-06, 7.6719e-06, 4.0881e-06, 2.8973e-06, 
   2.1455e-06, 1.6446e-06, 1.2972e-06};


  
  TH1F* hTrack_data = new TH1F("hTrack_data", "", 48, bins_track);
  //hTrack_data->Sumw2();
  for(int i = 1; i < hTrack_data->GetNbinsX();i++)
    {
      hTrack_data->SetBinContent(i, published_Data[i-1]);
      hTrack_data->SetBinError(i, published_Data_errors[i-1]); 
    }
  hTrack_data->SetTitle("p_{T} Spectra;p_{T} (GeV/c);1/N_{evn} dn^{2}/dp_{T}d#eta");
  hTrack_data->GetXaxis()->SetRangeUser(0.15,10);
  hTrack_data->SetLineColor(kBlue);
  hTrack_data->SetMarkerColor(kBlue);
  hTrack_data->SetMarkerStyle(8);
  
  TLegend* leg4 = new TLegend(0.15,0.2,0.5,0.35);
  leg4->AddEntry(hTrack_TPC,  "13b,  #sqrt{s} = 5.02 TeV, p-Pb, TPC+ITS");
  leg4->AddEntry(hTrack_ITS,  "13b,  #sqrt{s} = 5.02 TeV, p-Pb, ITS only"); 
  leg4->AddEntry(hTrack_data, "2013, #sqrt{s} = 5.02 TeV, p-Pb, Published Data");
  
  TCanvas* c4 = new TCanvas("c4", "c4", 800, 600);
  c4->SetLogy();
  c4->SetLogx();
  hTrack_ITS->Draw("P E");
  hTrack_data->Draw("P E HIST SAME");
  hTrack_TPC->Draw("P E SAME");
  leg4->Draw("same");
  
  ////////////////////////Published Data Ratio////////////////////////////
  TH1F* hTrack_TPC2 = (TH1F*)hTrack_TPC->Clone("hTrack_TPC2");
  TH1F* hTrack_ITS2 = (TH1F*)hTrack_ITS->Clone("hTrack_ITS2");
  hTrack_TPC2->SetMarkerColor(kRed);
  hTrack_ITS2->SetMarkerColor(kBlack);
  hTrack_TPC2->SetMarkerStyle(8);
  hTrack_ITS2->SetMarkerStyle(8);
  for(int i = 0; i < hTrack_TPC2->GetNbinsX(); i++)
    {
      double y = hTrack_TPC2->GetBinContent(i);
      double yerr = hTrack_TPC2->GetBinError(i);
      double yref = hTrack_data->GetBinContent(i);
      hTrack_TPC2->SetBinContent(i, y/yref);
      hTrack_TPC2->SetBinError(i, yerr/yref);
    }
  //hTrack_ITS2->Divide(hTrack_data);
  for(int i = 0; i < hTrack_ITS2->GetNbinsX(); i++)
    {
      double y = hTrack_ITS2->GetBinContent(i);
      double yerr = hTrack_ITS2->GetBinError(i);
      double yref = hTrack_data->GetBinContent(i);
      hTrack_ITS2->SetBinContent(i, y/yref);
      hTrack_ITS2->SetBinError(i, yerr/yref);
    }
  hTrack_TPC2->SetMaximum(2);
  hTrack_ITS2->SetMaximum(2);
  hTrack_TPC2->SetTitle(";p_{T} (GeV/c);Ratio");
  hTrack_ITS2->SetTitle(";p_{T} (GeV/c);Ratio");
  hTrack_TPC2->GetXaxis()->SetRangeUser(0.15,10);
  hTrack_ITS2->GetXaxis()->SetRangeUser(0.15,10);
  hTrack_TPC2->GetYaxis()->SetRangeUser(0,2);
  hTrack_ITS2->GetYaxis()->SetRangeUser(0,2);
  
  TLegend* leg5_TPC = new TLegend(0.15,0.15,0.45,0.35);
  leg5_TPC->AddEntry(hEff_TPC,"Efficiency from MC");
  leg5_TPC->AddEntry(hTrack_TPC2,"Efficiency from Data");
  
  TCanvas* c5_TPC = new TCanvas("c5_TPC", "c5_TPC", 800, 600);
  //c5_TPC->SetLogx();
  hEff_TPC->Draw("P");
  hTrack_TPC2->Draw("P E HIST SAME");
  leg5_TPC->Draw("SAME");
  
  TLegend* leg5_ITS = new TLegend(0.15,0.15,0.45,0.35);
  leg5_ITS->AddEntry(hEff_ITS,"Efficiency from MC");
  leg5_ITS->AddEntry(hTrack_ITS2,"Efficiency from Data");
  
  TCanvas* c5_ITS = new TCanvas("c5_ITS", "c5_ITS", 800, 600);
  //c5_ITS->SetLogx();
  hEff_ITS->Draw("P");
  hTrack_ITS2->Draw("P E HIST SAME");
  leg5_ITS->Draw("SAME");



}
