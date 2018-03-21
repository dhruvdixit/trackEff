#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TGraphAsymmErrors.h"
//#include "TTreeReader.h"
//#include "TTreeReaderValue.h"
#include "TMath.h"
#include "TVector2.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <stdio.h>
//#include "/root/atlasstyle-00-03-05/AtlasStyle.h"
//#include "/root/atlasstyle-00-03-05/AtlasStyle.C"
//#include "/root/atlasstyle-00-03-05/AtlasUtils.h"
//#include "/root/atlasstyle-00-03-05/AtlasUtils.C"
//#include "/root/atlasstyle-00-03-05/AtlasLabels.h"
//#include "/root/atlasstyle-00-03-05/AtlasLabels.C"
#include "TDatabasePDG.h"
#include "TEfficiency.h"

#include <algorithm>
#include <iostream>

void Run(const int TrackBit, char* filename){
  //SetAtlasStyle();
  
  auto f = TFile::Open(Form("/project/projectdirs/alice/NTuples/pPb/13c/%s.root",filename),"READ");
  
  //auto f = TFile::Open(Form("/project/projectdirs/alice/NTuples/MC/%s.root",filename),"READ");
  cout << filename << endl;
  if(!f){
      printf("Error: cannot open ntuple.root");
      return;
  }
  //TTree* tree = (TTree*)f->Get("_tree_event");
  TTree* tree = (TTree*)f->Get("AliAnalysisTaskNTGJ/_tree_event");
  if(!tree){ printf("Error: cannot find tree"); }
  const Int_t kMax = 5000;
  unsigned int ntrack;
  unsigned int nmc_truth;
  float mc_truth_pt[kMax];
  float mc_truth_eta[kMax];
  float mc_truth_phi[kMax];
  short mc_truth_pdg_code[kMax];
  char mc_truth_charge[kMax];
  float track_pt[kMax];
  float track_eta[kMax];
  float track_phi[kMax];
  float track_dca_xy[kMax];
  float track_dca_z[kMax];
  float track_its_chi_square[kMax];
  
  unsigned int ncluster;
  float cluster_iso_its_04[kMax];
  float cluster_iso_tpc_04[kMax];
  float cluster_iso_04_truth[kMax];
  float cluster_pt[kMax];

  char track_charge[kMax];
  unsigned short track_mc_truth_index[kMax];
  UChar_t track_its_ncluster[kMax];
  unsigned char track_quality[kMax];

  tree->SetBranchAddress("ncluster",&ncluster);
  tree->SetBranchAddress("nmc_truth",&nmc_truth);
  tree->SetBranchAddress("mc_truth_pt",mc_truth_pt);
  tree->SetBranchAddress("mc_truth_eta",mc_truth_eta);
  tree->SetBranchAddress("mc_truth_phi",mc_truth_phi);
  tree->SetBranchAddress("mc_truth_charge",mc_truth_charge);
  tree->SetBranchAddress("mc_truth_pdg_code",mc_truth_pdg_code);

  tree->SetBranchAddress("track_its_ncluster", track_its_ncluster);
  tree->SetBranchAddress("ntrack", &ntrack);    
  tree->SetBranchAddress("track_pt",track_pt);
  tree->SetBranchAddress("track_eta",track_eta);
  tree->SetBranchAddress("track_phi",track_phi);
  tree->SetBranchAddress("track_mc_truth_index", track_mc_truth_index);
  tree->SetBranchAddress("track_quality", track_quality);
  tree->SetBranchAddress("track_dca_xy", track_dca_xy);
  tree->SetBranchAddress("track_dca_z", track_dca_z);
  tree->SetBranchAddress("track_its_chi_square", track_its_chi_square);

  tree->SetBranchAddress("cluster_iso_its_04",cluster_iso_its_04);
  tree->SetBranchAddress("cluster_iso_tpc_04",cluster_iso_tpc_04);
  tree->SetBranchAddress("cluster_iso_04_truth", cluster_iso_04_truth);
  tree->SetBranchAddress("cluster_pt", cluster_pt);
  
  const Double_t bins[10] = {  0.1,          0.16681005,   0.27825594,   0.46415888 ,  0.77426368, 1.29154967,   2.15443469,   3.59381366,   5.9948425,   10.0};

  /*const Double_t bins_track[50] = {
    0.15,    0.169,   0.19,    0.214,   0.241,   0.271,   0.306,   0.344,   0.387,   0.436,   
    0.491,   0.553,   0.622,   0.701,   0.789,   0.888,   1,       1.126,   1.267,   1.427,
    1.606,   1.808,   2.036,   2.292,   2.581,   2.906,   3.271,   3.683,   4.147,   4.669,   
    5.256,   5.918,   6.663,   7.501,   8.445,   9.508,   10.705,  12.052,  13.569,  15.277,
    17.2,    19.365,  21.802,  24.546,  27.636,  31.114,  35.03,   39.439,  44.403,  49.992};//*/

  const Double_t bins_track[47] = {
    0.2,   0.25,   0.3,   0.35,   0.4,    0.45,   0.5,    0.55,   0.6,   0.65,   
    0.7,   0.75,   0.8,   0.85,   0.9,    0.95,   1.01,   1.1,   1.2,    1.3,   
    1.4,   1.5,    1.6,   1.7,    1.8,    1.9,    2.0,    2.2,   2.4,    2.6,   
    2.8,   3.0,    3.2,   3.4,    3.6,    3.8,    4.07,   4.5,   5.0,    5.5,
    6.0,   6.5,    7.12,  8.0,    9.0,    10.0,   11.0};//*/

  const int nbinseta = 10;
  Double_t etabins[nbinseta+1] = {};
  double etamin = -0.9;
  double etamax = 0.9;
  double etastep = (etamax-etamin)/nbinseta;
  for(int i=0; i<nbinseta+1; i++){
    etabins[i] = etamin + i*etastep;
  }

  const int nbinsphi = 40;
  Double_t phibins[nbinsphi+1] = {};
  double phimin = -1.0*TMath::Pi();
  double phimax = 1.0*TMath::Pi();
  double phistep = (phimax-phimin)/nbinsphi;
  for(int i=0; i<nbinsphi+1; i++){
    phibins[i] = phimin + i*phistep;
  }

  auto hCorrelation   = new TH2F("hCorrelation", "", 80, 0, 10.0, 80, 0, 10.0);
  auto hRes_Pt   = new TH2F("hRes_Pt", "", 200, 0, 10.0, 80, -50, 50);
  auto hDen  = new TH1F("hDen", "", 46, bins_track);
  auto hNum  = new TH1F("hNum","", 46, bins_track);
  auto hNum2  = new TH1F("hNum2","", 46, bins_track);
  auto hFake = new TH1F("hFake", "", 46, bins_track);
  auto hReco = new TH1F("hReco","", 46,bins_track);
  auto hNum2Deta = new TH2F("hNum2Deta","", nbinseta, etabins, 9, bins);
  auto hDen2Deta = new TH2F("hDen2Deta","", nbinseta, etabins, 9, bins);
  auto hNum2Dphi = new TH2F("hNum2Dphi","", nbinsphi, phibins, 9, bins);
  auto hDen2Dphi = new TH2F("hDen2Dphi","", nbinsphi, phibins, 9, bins);

  auto hNum2D = new TH2F("hNum2D","", nbinsphi, phibins, nbinseta, etabins);
  auto hDen2D = new TH2F("hDen2D","", nbinsphi, phibins, nbinseta, etabins);


  auto hIsoCorrelation = new TH2F("hIsoCorrelation", "", 80, 0, 10.0, 80, 0, 10.0); 
  auto hIsoReco        = new TH1F("hIsoReco", "", 200, 0, 100);
  auto hIsoTrue        = new TH1F("hIsoTrue", "", 200, 0, 100);
  auto hCorrelation_dEtapT = new TH2F("hCorrelation_dEtapT", "" , 100, -0.1, 0.1, 100, 0.1, 10.0);
  auto hCorrelation_dPhipT = new TH2F("hCorrelation_dPhipT", "" , 100, -0.1, 0.1, 100, 0.1, 10.0);

  auto hDCA_xy = new TH1F("hDCA_xy", "", 500, -1.0, 1.0);
  auto hDCA_z = new TH1F("hDCA_z", "", 500, -1.0, 1.0);
  auto hDCA_xy_fake = new TH1F("hDCA_xy_fake", "", 500, -1.0, 1.0);
  auto hDCA_z_fake = new TH1F("hDCA_z_fake", "", 500, -1.0, 1.0);   
  
  auto hChi2 = new TH1F("hChi2","", 50, 0, 10.1);
  auto hChi2_fake = new TH1F("hChi2_fake","", 50, 0, 10.1);


  auto hITSclus = new TH1F("hITSclus", "", 7, -0.5, 6.5);
  auto hITSclus_fake = new TH1F("hITSclus_fake", "", 7, -0.5, 6.5);

  auto hIso_ITS = new TH1F("hIso_ITS","", 25, -10, 40);
  auto hIso_TPC = new TH1F("hIso_TPC","", 25, -10, 40);  
  auto hIso_Truth = new TH1F("hIso_Truth","", 25, -10, 40);

  auto hTrue_eta  = new TH1F("hTrue_eta", "", 20, -1.0, 1.0);
  auto hReco_eta  = new TH1F("hReco_eta","", 20, -1.0, 1.0);

  auto hTrack_pt = new TH1F("hTrack_pt","", 46, bins_track);
  hTrack_pt->SetTitle(";p_{T} GeV/c;dn/dp_{T}");

  //auto hEventSelection = new TH1F("hEventSelection", "", 10, -0.5, 9.5);

   
  hFake->Sumw2();  
  hDen->Sumw2();
  hTrue_eta->Sumw2();
  hTrack_pt->Sumw2();
  int nevent = 0; 
  int numEvents = 0;
  //const int TrackBit = 16; //ITSONLY==16; ITS--TPC with full-jet cuts

  const double maxEta = 0.8;
  const bool doCutDCA = false; 
  const bool doCutChi2 = false;

  Long64_t numEntries = tree->GetEntries ();
  std::cout << numEntries << std::endl;
  for (Long64_t ievent=0;ievent< numEntries ;ievent++) {
    tree->GetEntry(ievent);
    nevent += 1;
    if(nevent%1000==0) std::cout << nevent << std::endl;
    bool eventChange = true;
    //Loop over all the tracks
    for (int n = 0;  n< ntrack; n++){
      
      if((track_quality[n]&TrackBit)==0) continue;
      if(TMath::Abs(track_eta[n])> maxEta) continue;
      if(track_its_chi_square[n]>10.0) continue;
      if(TrackBit == 16)
	if(track_its_ncluster[n] < 5) continue;
      double DCAcut = 0.0231+0.0315/TMath::Power(track_pt[n],1.3);
      if(TMath::Abs(track_dca_xy[n]) > DCAcut) continue;
      hTrack_pt->Fill(track_pt[n]);
      if (eventChange) {numEvents++; eventChange = false;}
      

      unsigned short index = track_mc_truth_index[n];
      if(index>65534) continue; //particles not associated with MC particle (i.e, secondaries or fakes)
      if(TMath::Abs(mc_truth_pdg_code[index])!=211) continue;
      
      hCorrelation->Fill(mc_truth_pt[index], track_pt[n]);
      hRes_Pt->Fill(mc_truth_pt[index], 100*(track_pt[n]-mc_truth_pt[index])/(mc_truth_pt[index]));
      
      hNum->Fill(mc_truth_pt[index]);
      hNum2->Fill(track_pt[n]);
      //cout << track_pt[n] << "\t" << mc_truth_pt[index]
      if(mc_truth_pt[index]>1.0)       
	  hReco_eta->Fill(mc_truth_eta[index]);
      hNum2Deta->Fill(mc_truth_eta[index], mc_truth_pt[index]);
      hNum2Dphi->Fill(mc_truth_phi[index], mc_truth_pt[index]);
      hNum2D->Fill(mc_truth_phi[index], mc_truth_eta[index]);

      hCorrelation_dEtapT->Fill(track_eta[n]-mc_truth_eta[index], mc_truth_pt[index]);
      hCorrelation_dPhipT->Fill(track_phi[n]-mc_truth_phi[index], mc_truth_pt[index]);

    }//end loop over tracks

    //Loop over MC particles (all are primaries), pick charged ones with |eta|<0.8
    for (int n = 0;  n< nmc_truth; n++){
        int pdgcode = mc_truth_pdg_code[n];
        if(TMath::Abs(mc_truth_pdg_code[n])!=211) continue;
        if(int(mc_truth_charge[n])==0) continue;
        if(TMath::Abs(mc_truth_eta[n])> maxEta) continue; //skip particles with |eta|<0.8
        hDen->Fill(mc_truth_pt[n]);
	if(mc_truth_pt[n] > 1.0)
	  hTrue_eta->Fill(mc_truth_eta[n]);
        hDen2Deta->Fill(mc_truth_eta[n], mc_truth_pt[n]);
        hDen2Dphi->Fill(mc_truth_phi[n], mc_truth_pt[n]);
        hNum2D->Fill(mc_truth_phi[n], mc_truth_eta[n]);
     }
    
     for (int n=0; n< ntrack; n++){ 
         if((track_quality[n]&TrackBit)==0) continue;
	 if(doCutChi2 && track_its_chi_square[n]>10) continue;
         if(TMath::Abs(track_eta[n])> maxEta) continue;
	 double DCAcut = 0.0231+0.0315/TMath::Power(track_pt[n],1.3);
	 if(doCutDCA && TMath::Abs(track_dca_xy[n]) > DCAcut) continue;
         double chi2 = track_its_chi_square[n];
	 if(chi2>10.0) chi2=10.0;
         hReco->Fill(track_pt[n]);
         
         if(track_pt[n]>1.0){
             hChi2->Fill(chi2);
             hDCA_xy->Fill(track_dca_xy[n]);
             hDCA_z->Fill(track_dca_z[n]);
             hITSclus->Fill(int(track_its_ncluster[n]));
	 }
	 if(track_mc_truth_index[n]>65534){
             hFake->Fill(track_pt[n]);   
             if(track_pt[n]>1.0){
	        hChi2_fake->Fill(chi2);
                hDCA_xy_fake->Fill(track_dca_xy[n]);
                hDCA_z_fake->Fill(track_dca_z[n]);
                hITSclus_fake->Fill(int(track_its_ncluster[n]));
	     }
	  }
      }
  
      for(int n=0; n< ncluster; n++){
        if(cluster_pt[n]<6) continue;
	hIso_ITS->Fill(cluster_iso_its_04[n]);
        hIso_TPC->Fill(cluster_iso_tpc_04[n]);
        hIso_Truth->Fill(cluster_iso_04_truth[n]);
      }

  }//loop over events
  std::cout << " END LOOP  " << std::endl;

   auto gausfit = new TF1("gaus","gaus", -25,25);
   gausfit->SetLineColor(kRed);
   auto g_mean = new TGraphErrors();
   auto g_sigma = new TGraphErrors();
 
   auto c1 = new TCanvas();
   
   //Study of the ITS-only track pT resolution
   int nbins = 9;
   for(int i=0; i<nbins; i++){
     double minpt = bins[i];
     double maxpt = bins[i+1];
     double binwidth = maxpt-minpt;
     int minbin =  hRes_Pt->GetXaxis()->FindBin(minpt);
     int maxbin =  hRes_Pt->GetXaxis()->FindBin(maxpt);
     auto h1 = hRes_Pt->ProjectionY("h", minbin, maxbin);
     
     h1->SetTitle("; (p_{T}^{reco}-p_{T}^{true})/p_{T}^{true} [%] ; counts");
     h1->Draw();  
     h1->GetYaxis()->SetNdivisions(5);
     h1->GetXaxis()->SetNdivisions(5);
     h1->GetYaxis()->SetTitle("counts");
     h1->Fit(gausfit,"R");
     h1->SetTitle("; (p_{T}^{reco}-p_{T}^{true})/p_{T}^{true} [%] ; counts");
     gausfit->Draw("same");
     //myText(0.18, 0.8, kBlack, Form("%2.1f < p_{T}^{truth} < %2.1f GeV", minpt, maxpt));
     //myText(0.18, 0.74, kRed, Form("#mu = %2.1f [%]", gausfit->GetParameter(1)));
     //myText(0.18, 0.68, kRed, Form("#sigma = %2.1f [%]", gausfit->GetParameter(2))); 
     g_sigma->SetPoint(g_sigma->GetN(), (maxpt+minpt)/2.0, gausfit->GetParameter(2));     
     g_sigma->SetPointError(g_sigma->GetN()-1, binwidth/2.0, gausfit->GetParError(2));
     g_mean->SetPoint(g_mean->GetN(), (maxpt+minpt)/2.0, gausfit->GetParameter(1));
     g_mean->SetPointError(g_mean->GetN()-1, binwidth/2.0, gausfit->GetParError(1));
     
     c1->SaveAs(Form("PDFOUTPUT/projecting%i_TrackBit%i_%s.pdf", i, TrackBit,filename));
   }

   cout << numEvents << endl;
   const double tot_eta = 1.6;
   for(int i = 0; i < hTrack_pt->GetNbinsX(); i++)
     {
       double dpt = hTrack_pt->GetBinWidth(i);

       double content = hTrack_pt->GetBinContent(i);
       double temp = content/((double)numEvents*dpt*tot_eta);
       hTrack_pt->SetBinContent(i, temp);

       double error = hTrack_pt->GetBinError(i);
       double tempErr = error/((double)numEvents*dpt*tot_eta);
       hTrack_pt->SetBinError(i, tempErr);
     }
   
   auto p1 = new TF1("p1","[0] + [1]*x", 0.50, 10.0);
   p1->SetLineColor(kRed);   
   g_sigma->SetTitle("Relative resolution vs p_{T} ; p_{T}^{true} [GeV]; #sigma(p_{T})/p_{T} [%]"); 
   g_mean->SetTitle("; p_{T}^{true} [GeV]; Relative bias [%]");
   hCorrelation->SetTitle("; True p_{T} [GeV]; Reconstructed p_{T} [GeV]");
   hCorrelation_dEtapT->SetTitle("; #eta^{truth}-#eta^{reco}; p_{T}^{true} [GeV]");
   hCorrelation_dPhipT->SetTitle("; #phi^{truth}-#phi^{reco}; p_{T}^{true} [GeV]");

   auto c = new TCanvas();   
  
   auto fout = new TFile(Form("OutputData/fout_%i_%s.root",TrackBit, filename), "RECREATE");
   //   
   hDen->SetTitle("; p_{T}^{reco} [GeV]; entries");
   hDen->SetLineColor(1);
   hFake->SetLineColor(kRed);
   hDen->Draw("hist");
   hFake->Draw("histsame");
   gPad->SetLogy(1);
   gPad->SetLogx(1);
   //hDen->SetMinimum(hDen->GetMinimum()/100.0);
   //myText(0.18, 0.96, kBlack, "All tracks");
   //myText(0.18, 0.92, kRed, "Unmatched tracks");
   c->SaveAs(Form("PDFOUTPUT/FakeRateDistributions_%i_%s.pdf",TrackBit,filename));
   gPad->SetLogy(0);
   gPad->SetLogx(0);
   c->Clear();

   hChi2->SetTitle("; ITS #chi^{2} ; counts"); 
   hChi2->Draw();
   //myText(0.18, 0.96, kBlack, "All tracks");
   //myText(0.18, 0.92, kRed, "Unmatched tracks");
   //myText(0.18, 0.92, kBlack, "p_{T}>1 GeV");
   hChi2_fake->SetLineColor(2);
   hChi2_fake->Draw("same");
   gPad->SetLogy(kTRUE);
   //gPad->SetLogx(kTRUE);
   c->SaveAs(Form("PDFOUTPUT/Chi2_%i_%s.pdf",TrackBit,filename));
   c->Clear();
   gPad->SetLogx(kFALSE);
      
   hFake->SetTitle("; p_{T}^{reco} [GeV]; Fake Rate");
   hFake->Write("hFake");
   hFake->Divide(hReco);
   hFake->Write("FakeRate");
   //   hReco->Write("hReco");
   g_sigma->Write("g_sigma");
   g_mean->Write("g_mean");

   hDen->SetTitle("; p_{T}^{true} [GeV/c]; entries");
   hNum->SetTitle("; p_{T}^{Reco,Embed} [GeV/c]; entries");
   hNum2->SetTitle("; p_{T}^{Reco} [GeV/c]; entries");
   hDen->Write("hTruth");
   hNum->Write("hRecoEmbed");
   hNum2->Write("hReco");


   //Writing efficiency
   hNum2Deta->Write("hNum2Deta");
   hDen2Deta->Write("hDen2Deta");
   hNum2Deta->SetMaximum(1.0);
   hNum2Deta->Divide(hDen2Deta);
   hNum2Deta->SetMinimum(0.0);
   hNum2Deta->Write("hEff2Deta");

   hNum2Dphi->Write("hNum2Dphi");
   hDen2Dphi->Write("hDen2Dphi");
   hNum2Dphi->Divide(hDen2Dphi);
   hNum2Dphi->SetMaximum(1.0);
   hNum2Dphi->SetMinimum(0.0);
   hNum2Dphi->Write("hEff2Dphi");

     
   hCorrelation_dEtapT->Write("dEta_pt");
   hCorrelation_dPhipT->Write("dPhi_pt");
   hCorrelation->Write();

   hTrue_eta->Write("hTrue_eta");
   hReco_eta->Write("hReco_eta");

   hTrack_pt->Write("track_pt");
   //TH1 eta_eff= new TGraphAsymmErrors(hReco_eta, hTrue_eta);
   //eta_eff->SetTitle("; #eta^{true} ; #epsilon");
   //eta_eff->Write("Efficiency_eta");

   TGraphAsymmErrors* eff = new TGraphAsymmErrors(hNum, hDen);
   eff->SetTitle("; p_{T}^{true} ; #epsilon");
   eff->Write("Efficiency");
   fout->Close();

   hCorrelation_dEtapT->Draw("colz");
   gPad->SetLogz();
   hCorrelation_dEtapT->GetYaxis()->SetNdivisions(8);
   hCorrelation_dEtapT->GetXaxis()->SetNdivisions(10);
   c->SaveAs(Form("PDFOUTPUT/Correlation_deta_pt%i_%s.pdf",TrackBit,filename));

   c->Clear();
   hITSclus->Draw("hist");
   hITSclus_fake->Draw("histsame");
   hITSclus_fake->SetLineColor(2);
   //myText(0.18, 0.96, kRed, "All tracks");
   //myText(0.18, 0.92, kBlack, "Unmatched tracks");
   //myText(0.18, 0.82, kBlack, "p_{T}>1 GeV");
 

   gPad->SetLogy(kTRUE);
   c->SaveAs(Form("PDFOUTPUT/ITSnCluster_%i_%s.pdf",TrackBit,filename));
   c->Clear();

   
   hDCA_xy->SetTitle("; d_{0} [mm]; counts");
   hDCA_xy->Draw();
   hDCA_xy_fake->SetLineColor(kRed);
   hDCA_xy_fake->Draw("same");
   gPad->SetLogy();
   //myText(0.18, 0.96, kBlack, "All tracks");
   //myText(0.18, 0.92, kRed, "Unmatched tracks");
   //myText(0.18, 0.82, kBlack, "p_{T}>1 GeV");
   c->SaveAs(Form("PDFOUTPUT/DCA_xy%i_%s.pdf",TrackBit,filename));
   gPad->SetLogy(0);
   c->Clear();

   hIso_ITS->SetTitle("; #Sigma_{R<0.4} p_{T} [GeV]; counts ");
   hIso_ITS->Draw("hist");
   //myText(0.18, 0.96, kRed, "ITS-only");
   //myText(0.18, 0.92, kBlack, "ITS+TPC");
   //myText(0.68, 0.85, kBlack, "p_{T}^{cluster} > 6 GeV");
   hIso_TPC->SetLineColorAlpha(2,0.5);
   hIso_TPC->Draw("histsame");
   //hIso_Truth->SetLineColorAlpha(4,0.5);
   // hIso_Truth->Draw("histsame");
   gPad->SetLogy(0); 
   c->SaveAs(Form("PDFOUTPUT/Iso%s.pdf",filename));
   c->Clear();

   hIso_Truth->Draw("hist");
   c->SaveAs(Form("PDFOUTPUT/IsoTrue%s.pdf",filename));
   c->Clear();


   hDCA_z->SetTitle("; z_{0} [cm] ; counts");
   hDCA_z->Draw();
   hDCA_z_fake->SetLineColor(kRed);
   hDCA_z_fake->Draw("same");
   gPad->SetLogy(kTRUE);
   //myText(0.18, 0.96, kBlack, "All tracks");
   //myText(0.18, 0.92, kRed, "Unmatched tracks");
   //myText(0.18, 0.82, kBlack, "p_{T}>1 GeV");
   c->SaveAs(Form("PDFOUTPUT/DCA_z%i_%s.pdf",TrackBit,filename));
   gPad->SetLogy(kFALSE);


   hCorrelation_dPhipT->Draw("colz");
   gPad->SetLogz();
   hCorrelation_dPhipT->GetYaxis()->SetNdivisions(8);
   hCorrelation_dPhipT->GetXaxis()->SetNdivisions(10);
   c->SaveAs(Form("PDFOUTPUT/Correlation_dphi_pt%i_%s.pdf",TrackBit,filename));
   
   c->Clear();
   hCorrelation->Draw("colz");
   gPad->SetLogz();
   hCorrelation->GetYaxis()->SetNdivisions(5);
   hCorrelation->GetXaxis()->SetNdivisions(5);
   c->SaveAs(Form("PDFOUTPUT/MomentumCorrelation_dphi_pt%i_%s.pdf",TrackBit,filename));

   c->Clear();
   hNum2Deta->Draw("colz");
   hNum2Deta->SetTitle("; #eta^{true} ; p_{T}^{true} [GeV]");
   gPad->SetLogz(0);
   gPad->SetLogy(1);
   c->SaveAs(Form("PDFOUTPUT/Eff_Eta_%i_%s.pdf",TrackBit,filename));
   c->Clear();
   hNum2Dphi->Draw("colz");
   hNum2Dphi->SetTitle("; #phi^{true} ; p_{T}^{true} [GeV]");
   gPad->SetLogz(0);
   gPad->SetLogy(1);
   c->SaveAs(Form("PDFOUTPUT/Eff_phi_%i_%s.pdf",TrackBit,filename));

   c->Clear();
   gPad->SetLogy(0);
   g_sigma->Draw("AP");
   c->SaveAs(Form("PDFOUTPUT/ResolutionPT_%i_%s.pdf",TrackBit,filename));

   c->Clear();
   eff->Draw("AP");
   eff->SetMaximum(1.0);
   eff->SetMinimum(0.0);
   gPad->SetLogx(1);
   c->SaveAs(Form("PDFOUTPUT/Efficiency_%i_%s.pdf",TrackBit,filename));

   c->Clear();
   hFake->Draw();
   hFake->SetTitle("; p_{T}^{reco} [GeV]; Fake Rate");
   c->SaveAs(Form("PDFOUTPUT/FakeRate_%i_%s.pdf",TrackBit,filename));
   
   c->Clear();
   c->Close();
   c1->Close();
  
}


void efficiencyCalculation(){
 
 
  //Run(16, "16c2");
  // Run(3, "16c2");
  
  //Run(16, "16c3b_small");
  //Run(3, "16c3b/16c3b_small");
  //Run(16, "17f4b");
  //Run(3, "17f4b");
  //Run(16, "17f4b_woSDD");
  //Run(3, "17f4b_woSDD");
  
  //Run(3, "17g8a_woSDD");
  //Run(16, "17g8a_woSDD");
  
  //Run(3, "17g6a3_pthat2_clusterv2_small");
  //Run(16, "17g6a3_pthat2_clusterv2_small");
  
  Run(3, "13c_pass4_v2");
  Run(16, "13c_pass4_v2");
  return;
}




