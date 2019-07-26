// This code finds both a matching muon and proton given an input neutrino event

#include <iostream>
#include <tuple>
#include "findMuon.h" 
#include "findProton.h"

#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"

using namespace std;

int main( int nargs, char** argv ) {

  std::string first_FVV_file  = argv[1];
  std::string second_FVV_file  = argv[2];
  std::string output_llhd_file  = argv[3];

  //  Int_t numParams = 6; // 6 params each, for muon and proton separately
  int passCutCount = 0;

  // This is the file we open for all input entries
  TFile *f1 = new TFile(first_FVV_file.c_str(),"READ");
  TTree *inputTree = (TTree *)f1->Get("NuMuVertexVariables");

  // This is the file we open for entries to look through and compare
  TFile *f2 = new TFile(second_FVV_file.c_str(),"READ");
  TTree *comparisonTree = (TTree *)f2->Get("NuMuVertexVariables");

  // Create variables that are in the input TTree again
  int _run;
  int _subrun;
  int _event;
  int _vtxid;
  int Muon_id;
  int Proton_id;
  float _x;
  float _y;
  float _z;
  float Muon_ThetaReco;
  float Muon_PhiReco;
  float Muon_TrackLength;
  float Proton_ThetaReco;
  float Proton_PhiReco;
  float Proton_TrackLength;
  int _passCuts;
  float _cosmiLL;

  inputTree->SetBranchAddress("run", &_run);
  inputTree->SetBranchAddress("subrun", &_subrun);
  inputTree->SetBranchAddress("event", &_event);
  inputTree->SetBranchAddress("vtxid", &_vtxid);
  inputTree->SetBranchAddress("Muon_id", &Muon_id);
  inputTree->SetBranchAddress("Proton_id", &Proton_id);
  inputTree->SetBranchAddress("Xreco", &_x);
  inputTree->SetBranchAddress("Yreco", &_y);
  inputTree->SetBranchAddress("Zreco", &_z);
  inputTree->SetBranchAddress("Muon_ThetaReco", &Muon_ThetaReco);
  inputTree->SetBranchAddress("Muon_PhiReco", &Muon_PhiReco);
  inputTree->SetBranchAddress("Muon_TrackLength", &Muon_TrackLength);
  inputTree->SetBranchAddress("Proton_ThetaReco", &Proton_ThetaReco);
  inputTree->SetBranchAddress("Proton_PhiReco", &Proton_PhiReco);
  inputTree->SetBranchAddress("Proton_TrackLength", &Proton_TrackLength);
  inputTree->SetBranchAddress("PassCuts", &_passCuts);
  inputTree->SetBranchAddress("CosmicLL", &_cosmiLL);

  Int_t entries = inputTree->GetEntries();

  std::cout << "There are this many entries: " << entries << std::endl;

  // Vars for new TTree to fill
  
  // Input ("Truth")
  int run;
  int subrun;
  int event;
  int vtxid;
  int muID;
  int protID;
  float x;
  float y;
  float z;
  float muTheta;
  float muPhi;
  float muLen;
  float pTheta;
  float pPhi;
  float pLen;

  // Output
  int chosenMuRun;
  int chosenMuSubrun;
  int chosenMuEvent;
  int chosenMuVtxid;
  int chosenMuMuID;
  int chosenMuProtID;
  float chosenMuX;
  float chosenMuY;
  float chosenMuZ;
  float chosenMuTheta;
  float chosenMuPhi;
  float chosenMuLen;

  int chosenPRun;
  int chosenPSubrun;
  int chosenPEvent;
  int chosenPVtxid;
  int chosenPMuID;
  int chosenPProtID;
  float chosenPX;
  float chosenPY;
  float chosenPZ;
  float chosenPTheta;
  float chosenPPhi;
  float chosenPLen;
  
  TFile *outputTree = new TFile(output_llhd_file.c_str(),"RECREATE");
  TTree *_tree = new TTree("tree","Just a Tree");
  _tree->Branch("run", &run, "run/I");
  _tree->Branch("subrun", &subrun, "subrun/I");
  _tree->Branch("event", &event, "event/I");
  _tree->Branch("vtxid", &vtxid, "vtxid/I");
  _tree->Branch("muID", &muID, "muID/I");
  _tree->Branch("protID", &protID, "protID/I");
  _tree->Branch("x", &x, "x/F");
  _tree->Branch("y", &y, "y/F");
  _tree->Branch("z", &z, "z/F");
  _tree->Branch("muTheta", &muTheta, "muTheta/F");
  _tree->Branch("muPhi", &muPhi, "muPhi/F");
  _tree->Branch("muLen", &muLen, "muLen/F");
  _tree->Branch("pTheta", &pTheta, "pTheta/F");
  _tree->Branch("pPhi", &pPhi, "pPhi/F");
  _tree->Branch("pLen", &pLen, "pLen/F");
  _tree->Branch("chosenMuRun", &chosenMuRun, "chosenMuRun/I");
  _tree->Branch("chosenMuSubrun", &chosenMuSubrun, "chosenMuSubrun/I");
  _tree->Branch("chosenMuEvent", &chosenMuEvent, "chosenMuEvent/I");
  _tree->Branch("chosenMuVtxid", &chosenMuVtxid, "chosenMuVtxid/I");
  _tree->Branch("chosenMuMuID", &chosenMuMuID, "chosenMuMuID/I");
  _tree->Branch("chosenMuProtID", &chosenMuProtID, "chosenMuProtID/I");
  _tree->Branch("chosenMuX", &chosenMuX, "chosenMuX/F");
  _tree->Branch("chosenMuY", &chosenMuY, "chosenMuY/F");
  _tree->Branch("chosenMuZ", &chosenMuZ, "chosenMuZ/F");
  _tree->Branch("chosenMuTheta", &chosenMuTheta, "chosenMuTheta/F");
  _tree->Branch("chosenMuPhi", &chosenMuPhi, "chosenMuPhi/F");
  _tree->Branch("chosenMuLen", &chosenMuLen, "chosenMuLen/F");
  _tree->Branch("chosenPRun", &chosenPRun, "chosenPRun/I");
  _tree->Branch("chosenPSubrun", &chosenPSubrun, "chosenPSubrun/I");
  _tree->Branch("chosenPEvent", &chosenPEvent, "chosenPEvent/I");
  _tree->Branch("chosenPVtxid", &chosenPVtxid, "chosenPVtxid/I");
  _tree->Branch("chosenPMuID", &chosenPMuID, "chosenPMuID/I");
  _tree->Branch("chosenPProtID", &chosenPProtID, "chosenPProtID/I");
  _tree->Branch("chosenPX", &chosenPX, "chosenPX/F");
  _tree->Branch("chosenPY", &chosenPY, "chosenPY/F");
  _tree->Branch("chosenPZ", &chosenPZ, "chosenPZ/F");
  _tree->Branch("chosenPTheta", &chosenPTheta, "chosenPTheta/F");
  _tree->Branch("chosenPPhi", &chosenPPhi, "chosenPPhi/F");
  _tree->Branch("chosenPLen", &chosenPLen, "chosenPLen/F");

  for (Int_t j = 0; j < entries; j++) {

    // Make sure to get the jth entry
    inputTree->GetEntry(j);

    if ( (_passCuts == 1) && (_cosmiLL > -3.0) ) {

      passCutCount = passCutCount + 1;

      // Set all the "truth" values from the real event
      run = _run;
      subrun = _subrun;
      event = _event;
      vtxid = _vtxid;
      muID = Muon_id;
      protID = Proton_id;
      x = _x;
      y = _y;
      z = _z;
      muTheta = Muon_ThetaReco;
      muPhi = Muon_PhiReco;
      muLen = Muon_TrackLength;
      pTheta = Proton_ThetaReco;
      pPhi = Proton_PhiReco;
      pLen = Proton_TrackLength;

      // Find values for muon
      tie( chosenMuRun, chosenMuSubrun, chosenMuEvent, chosenMuVtxid, chosenMuMuID, chosenMuProtID, chosenMuX, chosenMuY, chosenMuZ, chosenMuTheta, chosenMuPhi, chosenMuLen ) = findMuonTrack( _x, _y, _z, Muon_ThetaReco, Muon_PhiReco, Muon_TrackLength, j, comparisonTree);

      // Find values for proton
      tie( chosenPRun, chosenPSubrun, chosenPEvent, chosenPVtxid, chosenPMuID, chosenPProtID, chosenPX, chosenPY, chosenPZ, chosenPTheta, chosenPPhi, chosenPLen ) = findProtonTrack( _x, _y, _z, Proton_ThetaReco, Proton_PhiReco, Proton_TrackLength, j, comparisonTree);

      _tree->Fill();

    }

  }

  cout << "This is how many events pass cuts: " << passCutCount << endl;

  outputTree->Write();
  outputTree->Close();   

  return 0;
}
