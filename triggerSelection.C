#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TGraphAsymmErrors.h"
//#include "TTreeReader.h"
//#include "TTreeReaderValue.h"
#include "TMath.h"
#include "TVector2.h"
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
#include <vector>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <bitset>
#include <bits/stdc++.h>

/*struct(IsEmpty)
{
  bool operator()(const std::string& s)
  {
    return s.find("", 0) !=  std::string::nops;
  }
  }*/


void Run(TString address){

  gStyle->SetOptStat(0);

  //SetAtlasStyle();
  TString filename = address(address.Last('/')+1,address.Last('.')-address.Last('/')-1);
  TFile* f;
  f = TFile::Open(Form("/project/projectdirs/alice/NTuples/%s",address.Data()),"READ");

  cout << Form("/project/projectdirs/alice/NTuples/%s",address.Data()) << endl;
  if(!f){
      printf("Error: cannot open ntuple.root");
      return;
  }
  TTree* tree;
  tree = (TTree*)f->Get("AliAnalysisTaskNTGJ/_tree_event");
  if(!tree){
    tree = (TTree*)f->Get("_tree_event");
    if(!tree){ printf("Error: cannot find tree"); }
  }
  const Int_t kMax = 5000;
  
  //Cuts/selections
  int run_number;
  int new_run_number = 0;
  ULong64_t trigger_mask[2];
  std::vector<string> * trigger_class = {};
  
  tree->SetBranchAddress("run_number", &run_number);
  tree->SetBranchAddress("trigger_mask", trigger_mask);
  tree->SetBranchAddress("trigger_class", &trigger_class);

  //cout << __LINE__ << endl;
  
  TH1F* trigBit1 = new TH1F("trigBit1", "trigBit1", 63, -0.5, 62.5);
  TH1F* trigBit2 = new TH1F("trigBit2", "trigBit2", 63, -0.5, 62.5);
  const ULong64_t one1 = 1;

  //cout << triggerMask_13bc << "\t" << trigMask << endl;
  int nevent = 0;
  Long64_t totEvents = tree->GetEntries();
  Long64_t restrictEvents = 100000;
  Long64_t numEntries = TMath::Min(totEvents,restrictEvents);
  std::cout << numEntries << std::endl;
  
  for (Long64_t ievent=0;ievent< numEntries ;ievent++) {
    tree->GetEntry(ievent);
    nevent += 1;
    if(nevent%10000==0)
      {
	std::cout << nevent << std::endl;
	cout << run_number << endl;
      }
    
    int cntr = 0;
    if(new_run_number != run_number){
      cout << "Run Number: " << run_number << endl;
      new_run_number = run_number;
      for(std::vector<string>::iterator it = trigger_class->begin(); it != trigger_class->end(); ++it)
	{
	  int index = (it - trigger_class->begin());
	  string trigString = (string)*it;
	  cout << index << " ; " << trigString << endl;
	   
	}
      cout << '\n';
      cout << trigger_class->size() << "\t" <<__FILE__ << endl;
    }
    
  }//loop over events
  std::cout << " END LOOP  " << std::endl;
  
  //trigBit1->Draw();

  //auto fout = new TFile(Form("OutputData/trigHistos_%s.root", filename.Data()), "RECREATE");
  //trigBit1->Write("trigBit1");
  //trigBit2->Write("trigBit2");
  //fout->Close();
}

    
void triggerSelection(){  
  
  //Input to Run is as follow: Run(TString address)

  Run("pp/17q/17q.root");



  return;
}




