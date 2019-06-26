// This code takes Image2D files of 2 selected tracks and puts them together for each event

#include <iostream>
#include <tuple>
#include <stdlib.h>
#include "findMuon.h" 
#include "findProton.h"

#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "/Users/nduser/dllee_unified/LArCV/core/DataFormat/IOManager.h"

using namespace std;

int main() {

  // This is the likelihood output tree containing matching pairs of muons & protons
  TFile *f1 = new TFile("output_likelihood_aab.root","READ");
  TTree *llhdOutputTree = (TTree *)f1->Get("tree");

  // Create variables that are in the LLHD output TTree again
  int _chosenMuRun;
  int _chosenMuSubrun;
  int _chosenMuEvent;
  int _chosenMuVtxid;
  int _chosenMuMuID;
  int _chosenMuProtID;
  int _chosenPRun;
  int _chosenPSubrun;
  int _chosenPEvent;
  int _chosenPVtxid;
  int _chosenPMuID;
  int _chosenPProtID;

  llhdOutputTree->SetBranchAddress("chosenMuRun", &_chosenMuRun);
  llhdOutputTree->SetBranchAddress("chosenMuSubrun", &_chosenMuSubrun);
  llhdOutputTree->SetBranchAddress("chosenMuEvent", &_chosenMuEvent);
  llhdOutputTree->SetBranchAddress("chosenMuVtxid", &_chosenMuVtxid);
  llhdOutputTree->SetBranchAddress("chosenMuMuID", &_chosenMuMuID);
  llhdOutputTree->SetBranchAddress("chosenMuProtID", &_chosenMuProtID);
  llhdOutputTree->SetBranchAddress("chosenPRun", &_chosenPRun);
  llhdOutputTree->SetBranchAddress("chosenPSubrun", &_chosenPSubrun);
  llhdOutputTree->SetBranchAddress("chosenPEvent", &_chosenPEvent);
  llhdOutputTree->SetBranchAddress("chosenPVtxid", &_chosenPVtxid);
  llhdOutputTree->SetBranchAddress("chosenPMuID", &_chosenPMuID);
  llhdOutputTree->SetBranchAddress("chosenPProtID", &_chosenPProtID);

  // Use IOManager to deal with image2d file
  larcv::IOManager* ioman = new larcv::IOManager(larcv::IOManager::kREAD);

  ioman->add_in_file("myCoolTest.root");
  ioman->initialize();

  // For each track chosen by the likelihood, find the corresponding image2d
  Int_t llhdEntries = llhdOutputTree->GetEntries();
  ULong_t image2dEntries = ioman->get_n_entries();

  Int_t _run;
  ULong_t _subrun;
  ULong_t _event;  

  for (Int_t i = 0; i < llhdEntries; i++) {

    llhdOutputTree->GetEntry(i);
    
    for (Int_t j = 0; j < image2dEntries; j++) {

      _run = -999;
      
      std::cout << _run << std::endl;

      ioman->read_entry(j);

      _run =  ioman->event_id().run();

      std::cout << "Run: " << _run << " ChosenMuRun: " << _chosenMuRun << std::endl;

      if (_run == _chosenMuRun) {

	std::cout << "Found a matching run!" << _run << "=" << _chosenMuRun << std::endl;	

      }


    }

  }

  return 0;
}
