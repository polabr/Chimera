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
  TTree *inputTree = (TTree *)f1->Get("FinalVertexVariables");
  
  // This is the file we open for entries to look through and compare
  TFile *f2 = new TFile(second_FVV_file.c_str(),"READ");
  TTree *comparisonTree = (TTree *)f2->Get("FinalVertexVariables");

  // Create variables that are in the input TTree again
  int run;
  int subrun;
  int event;
  int vtxid;
  int Lepton_ID;
  float Proton_ID;
  float Xreco;
  float Yreco;
  float Zreco;
  float Lepton_ThetaReco;
  float Lepton_PhiReco;
  float Lepton_TrackLength;
  float Proton_ThetaReco;
  float Proton_PhiReco;
  float Proton_TrackLength;
  int PassSimpleCuts;
  //  float _cosmiLL; // BDT score, would get by training the BDT

  inputTree->SetBranchAddress("run", &run);

  std::cout << "set first branch address" << std::endl;

  inputTree->SetBranchAddress("subrun", &subrun);
  inputTree->SetBranchAddress("event", &event);
  inputTree->SetBranchAddress("vtxid", &vtxid);
  inputTree->SetBranchAddress("Lepton_ID", &Lepton_ID);
  inputTree->SetBranchAddress("Proton_ID", &Proton_ID);
  inputTree->SetBranchAddress("Xreco", &Xreco);
  inputTree->SetBranchAddress("Yreco", &Yreco);
  inputTree->SetBranchAddress("Zreco", &Zreco);
  inputTree->SetBranchAddress("Lepton_ThetaReco", &Lepton_ThetaReco);
  inputTree->SetBranchAddress("Lepton_PhiReco", &Lepton_PhiReco);
  inputTree->SetBranchAddress("Lepton_TrackLength", &Lepton_TrackLength);
  inputTree->SetBranchAddress("Proton_ThetaReco", &Proton_ThetaReco);
  inputTree->SetBranchAddress("Proton_PhiReco", &Proton_PhiReco);
  inputTree->SetBranchAddress("Proton_TrackLength", &Proton_TrackLength);
  inputTree->SetBranchAddress("PassSimpleCuts", &PassSimpleCuts);
  //  inputTree->SetBranchAddress("CosmicLL", &_cosmiLL);

  Int_t entries = inputTree->GetEntries();

  std::cout << "There are this many entries: " << entries << std::endl;

  // Vars for new TTree to fill
  
  // Input ("Truth")
  int _run;
  int _subrun;
  int _event;
  int _vtxid;
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
  _tree->Branch("_run", &_run, "_run/I");
  _tree->Branch("_subrun", &_subrun, "_subrun/I");
  _tree->Branch("_event", &_event, "_event/I");
  _tree->Branch("_vtxid", &_vtxid, "_vtxid/I");
  _tree->Branch("muID", &muID, "muID/I");
  _tree->Branch("protID", &protID, "protID/F");
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
  _tree->Branch("chosenMuProtID", &chosenMuProtID, "chosenMuProtID/F");
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
  _tree->Branch("chosenPProtID", &chosenPProtID, "chosenPProtID/F");
  _tree->Branch("chosenPX", &chosenPX, "chosenPX/F");
  _tree->Branch("chosenPY", &chosenPY, "chosenPY/F");
  _tree->Branch("chosenPZ", &chosenPZ, "chosenPZ/F");
  _tree->Branch("chosenPTheta", &chosenPTheta, "chosenPTheta/F");
  _tree->Branch("chosenPPhi", &chosenPPhi, "chosenPPhi/F");
  _tree->Branch("chosenPLen", &chosenPLen, "chosenPLen/F");

  for (Int_t j = 0; j < entries; j++) {

    // Make sure to get the jth entry
    inputTree->GetEntry(j);

    //    if ( (PassSimpleCuts == 1) && (_cosmiLL > -3.0) ) {
    if (PassSimpleCuts == 1) {

      passCutCount = passCutCount + 1;

      // Set all the "truth" values from the real event
      _run = run;
      _subrun = subrun;
      _event = event;
      _vtxid = vtxid;
      muID = Lepton_ID;
      protID = Proton_ID;
      x = Xreco;
      y = Yreco;
      z = Zreco;
      muTheta = Lepton_ThetaReco;
      muPhi = Lepton_PhiReco;
      muLen = Lepton_TrackLength;
      pTheta = Proton_ThetaReco;
      pPhi = Proton_PhiReco;
      pLen = Proton_TrackLength;

      // Find values for muon
      tie( chosenMuRun, chosenMuSubrun, chosenMuEvent, chosenMuVtxid, chosenMuMuID, chosenMuProtID, chosenMuX, chosenMuY, chosenMuZ, chosenMuTheta, chosenMuPhi, chosenMuLen ) = findMuonTrack( Xreco, Yreco, Zreco, Lepton_ThetaReco, Lepton_PhiReco, Lepton_TrackLength, j, comparisonTree);

      // Find values for proton
      tie( chosenPRun, chosenPSubrun, chosenPEvent, chosenPVtxid, chosenPMuID, chosenPProtID, chosenPX, chosenPY, chosenPZ, chosenPTheta, chosenPPhi, chosenPLen ) = findProtonTrack( Xreco, Yreco, Zreco, Proton_ThetaReco, Proton_PhiReco, Proton_TrackLength, j, comparisonTree);

      _tree->Fill();

    }

  }

  cout << "This is how many events pass cuts: " << passCutCount << endl;

  outputTree->Write();
  outputTree->Close();   

  return 0;
}
