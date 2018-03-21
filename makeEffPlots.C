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
  
  /*TGraphAsymmErrors* eff_TPC = (TGraphAsymmErrors*)fTPC->Get("Efficiency");
    TGraphAsymmErrors* eff_ITS = (TGraphAsymmErrors*)fITS->Get("Efficiency");
    eff_TPC->SetLineColor(kRed);
    eff_ITS->SetLineColor(kBlack);
    eff_TPC->GetXaxis()->SetRangeUser(0.15,10);
    eff_ITS->GetXaxis()->SetRangeUser(0.15,10);
    eff_TPC->GetYaxis()->SetRangeUser(0,1);
    eff_ITS->GetYaxis()->SetRangeUser(0,1);*/
  
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
  hEff_TPC->GetYaxis()->SetRangeUser(0,1);
  hEff_ITS->GetYaxis()->SetRangeUser(0,1);
  
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
  
  TLegend* leg4 = new TLegend(0.15,0.15,0.45,0.35);
  leg4->AddEntry(hEff_TPC,"JJ+DPMJET, TPC+ITS N_{reco,embed}/N_{gen}");
  leg4->AddEntry(hEff_ITS,"JJ+DPMJET, ITS Only N_{reco,embed}/N_{gen}");
  leg4->AddEntry(hResoEff_TPC,"JJ+DPMJET, TPC+ITS N_{reco}/N_{gen}");
  leg4->AddEntry(hResoEff_ITS,"JJ+DPMJET, ITS Only N_{reco}/N_{gen}");
  
  
  TCanvas* c4 = new TCanvas("c4", "c4", 800, 600);
  c4->SetLogx();
  hEff_TPC->Draw("P");
  hEff_ITS->Draw("P SAME");
  hResoEff_TPC->Draw("P HIST SAME");
  hResoEff_ITS->Draw("P HIST SAME");
  
  leg4->Draw("SAME" );
  
  
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// //////////////////////////////////////////DATA////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
  
  TFile* fTPCd = new TFile("/global/homes/d/ddixit/trackEff/OutputData/fout_3_13c_pass4_v2.root","READ");
  TFile* fITSd = new TFile("/global/homes/d/ddixit/trackEff/OutputData/fout_16_13c_pass4_v2.root","READ");
  
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
  hTrack_TPC->SetMarkerStyle(8);
  hTrack_ITS->SetMarkerStyle(8);
  
  vector <double> x_cor;
  vector <double> y_cor;
  
  double x_n, y_n;
  ifstream inFile;
  inFile.open("pt_spectra.csv");
  if(!inFile.is_open()){cout << "csv file not open. Try again." << endl; return;}
  while(!inFile.eof())
    {
      inFile >> x_n >> y_n;
      x_cor.push_back(x_n);
      y_cor.push_back(y_n);
    }
  inFile.close();
  
  const Double_t bins_track[47] = {
    0.2,   0.25,   0.3,   0.35,   0.4,    0.45,   0.5,    0.55,   0.6,   0.65,   
    0.7,   0.75,   0.8,   0.85,   0.9,    0.95,   1.01,   1.1,   1.2,    1.3,   
    1.4,   1.5,    1.6,   1.7,    1.8,    1.9,    2.0,    2.2,   2.4,    2.6,   
    2.8,   3.0,    3.2,   3.4,    3.6,    3.8,    4.07,   4.5,   5.0,    5.5,
    6.0,   6.5,    7.12,  8.0,    9.0,    10.0,   11.0};//*/
  
  
  TH1F* hTrack_data = new TH1F("hTrack_data", "", 46, bins_track);
  //hTrack_data->Sumw2();
  hTrack_data->SetTitle("p_{T} Spectra;p_{T} (GeV/c);1/N_{evn} dn^{2}/dp_{T}d#eta");
  hTrack_data->SetLineColor(kBlue);
  hTrack_data->SetMarkerColor(kBlue);
  hTrack_data->SetMarkerStyle(8);
  for(int i = 0; i < hTrack_data->GetNbinsX();i++)
    {
      //hTrack_data->Fill(x_cor[i], y_cor[i]);
      hTrack_data->SetBinContent(i, y_cor[i]);
    }
  
  TLegend* leg3 = new TLegend(0.15,0.2,0.5,0.35);
  leg3->AddEntry(hTrack_TPC,  "13c,  #sqrt{s} = 5.02 TeV, p-Pb, TPC+ITS");
  leg3->AddEntry(hTrack_ITS,  "13c,  #sqrt{s} = 5.02 TeV, p-Pb, ITS only"); 
  leg3->AddEntry(hTrack_data, "2013, #sqrt{s} = 5.02 TeV, p-Pb, Published Data");
  
  TCanvas* c3 = new TCanvas("c3", "c3", 800, 600);
  c3->SetLogy();
  c3->SetLogx();
  hTrack_ITS->Draw("P E");
  hTrack_data->Draw("P HIST SAME");
  hTrack_TPC->Draw("P E SAME");
  leg3->Draw("same");
  
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
  hTrack_TPC2->GetYaxis()->SetRangeUser(0,1);
  hTrack_ITS2->GetYaxis()->SetRangeUser(0,1);
  
  TLegend* leg2 = new TLegend(0.15,0.15,0.45,0.35);
  leg2->AddEntry(hEff_TPC,"JJ+DPMJET, TPC+ITS N_{reco,embed}/N_{gen}");
  leg2->AddEntry(hEff_ITS,"JJ+DPMJET, ITS Only N_{reco,embed}/N_{gen}");
  leg2->AddEntry(hTrack_TPC2,"TPC+ITS p_{T} spectra/Published Data p_{T} spectra");
  leg2->AddEntry(hTrack_ITS2,"ITS Only p_{T} spectra/Published Data p_{T} spectra");
  
  TCanvas* c2 = new TCanvas("c2", "c2", 800, 600);
  c2->SetLogx();
  hEff_TPC->Draw("P");
  hEff_ITS->Draw("P SAME");
  hTrack_ITS2->Draw("P E HIST SAME");
  hTrack_TPC2->Draw("P E HIST SAME");
  leg2->Draw("SAME");
  



}
