// This code finds both a matching muon and proton given an input neutrino event

#include <iostream>
#include <tuple>
#include "findMuon.h" // *** also include findProton.h

#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"

using namespace std;

int main() {

  //  Int_t numParams = 6; // 6 params each, for muon and proton separately
  int passCutCount = 0;

  // This is the file we open for all input entries
  TFile *f1 = new TFile("../data/FinalVertexVariables.root","READ");
  TTree *inputTree = (TTree *)f1->Get("NuMuVertexVariables");

  // This is the file we open for entries to look through and compare
  TFile *f2 = new TFile("../data/FinalVertexVariables.root","READ");
  TTree *comparisonTree = (TTree *)f2->Get("NuMuVertexVariables");

  // Create variables that are in the input TTree again
  int _run;
  int _subrun;
  int _event;
  int _vtxid;
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

  // Vars for new TTree to fill
  
  // Input ("Truth")
  int run;
  int subrun;
  int event;
  int vtxid;
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
  float chosenPX;
  float chosenPY;
  float chosenPZ;
  float chosenPTheta;
  float chosenPPhi;
  float chosenPLen;
  
  TFile *outputTree = new TFile("test_april24_2019.root","RECREATE");
  TTree *_tree = new TTree("tree","Just a Tree");
  _tree->Branch("run", &run, "run/I");
  _tree->Branch("subrun", &subrun, "subrun/I");
  _tree->Branch("event", &event, "event/I");
  _tree->Branch("vtxid", &vtxid, "vtxid/I");
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
  _tree->Branch("chosenPX", &chosenPX, "chosenPX/F");
  _tree->Branch("chosenPY", &chosenPY, "chosenPY/F");
  _tree->Branch("chosenPZ", &chosenPZ, "chosenPZ/F");
  _tree->Branch("chosenPTheta", &chosenPTheta, "chosenPTheta/F");
  _tree->Branch("chosenPPhi", &chosenPPhi, "chosenPPhi/F");
  _tree->Branch("chosenPLen", &chosenPLen, "chosenPLen/F");

  for (Int_t j = 0; j < 100; j++) {

    // Make sure to get the jth entry
    inputTree->GetEntry(j);

    if ( (_passCuts == 1) && (_cosmiLL > -3.0) ) {

      passCutCount = passCutCount + 1;

      // Set all the "truth" values from the real event
      run = _run;
      subrun = _subrun;
      event = _event;
      vtxid = _vtxid;
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
      tie( chosenMuRun, chosenMuSubrun, chosenMuEvent, chosenMuVtxid, chosenMuX, chosenMuY, chosenMuZ, chosenMuTheta, chosenMuPhi, chosenMuLen ) = findTrack( _x, _y, _z, Muon_ThetaReco, Muon_PhiReco, Muon_TrackLength, j, comparisonTree);

      // Find values for proton
      tie( chosenPRun, chosenPSubrun, chosenPEvent, chosenPVtxid, chosenPX, chosenPY, chosenPZ, chosenPTheta, chosenPPhi, chosenPLen ) = findTrack( _x, _y, _z, Proton_ThetaReco, Proton_PhiReco, Proton_TrackLength, j, comparisonTree);

      _tree->Fill();

    }

  }

  cout << "This is how many events pass cuts: " << passCutCount << endl;

  outputTree->Write();
  outputTree->Close();   

  return 0;
}
