#include <iostream>
#include <tuple>
#include "findTrack.h"


#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"

using namespace std;

int main() {

  Int_t numParams = 6;
  int passCutCount = 0;

  // Loop through each entry in the tree and select the X value (make this the inputX)
  // Find the ChosenX (closest found X) while also skipping this entry
  // Fill a new tree with the found entries
  // Also fill an X variable in the new tree with the FVV.root entries
  // Make a 1D scatter plot showing the correlation (plot all FinalVertexVariables X entries vs. closestX)

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
  inputTree->SetBranchAddress("PassCuts", &_passCuts);
  inputTree->SetBranchAddress("CosmicLL", &_cosmiLL);

  Int_t entries = inputTree->GetEntries();

  // Vars for new TTree to fill
  // Add run, subrun, event, vtxid?
  
  // Input
  float x;
  float y;
  float z;
  float muTheta;
  float muPhi;
  float muLen;
  // Output
  float chosenX;
  float chosenY;
  float chosenZ;
  float chosenMuTheta = 7.9;
  float chosenMuPhi;
  float chosenMuLen;
  
  float diffPhi;
  
  TFile *outputTree = new TFile("comparisonTree_040319_anglesAt001.root","RECREATE");
  TTree *_tree = new TTree("tree","Just a Tree");
  _tree->Branch("x", &x, "x/F");
  _tree->Branch("y", &y, "y/F");
  _tree->Branch("z", &z, "z/F");
  _tree->Branch("muTheta", &muTheta, "muTheta/F");
  _tree->Branch("muPhi", &muPhi, "muPhi/F");
  _tree->Branch("muLen", &muLen, "muLen/F");
  _tree->Branch("chosenX", &chosenX, "chosenX/F");
  _tree->Branch("chosenY", &chosenY, "chosenY/F");
  _tree->Branch("chosenZ", &chosenZ, "chosenZ/F");
  _tree->Branch("chosenMuTheta", &chosenMuTheta, "chosenMuTheta/F");
  _tree->Branch("chosenMuPhi", &chosenMuPhi, "chosenMuPhi/F");
  _tree->Branch("chosenMuLen", &chosenMuLen, "chosenMuLen/F");
  _tree->Branch("diffPhi", &diffPhi, "diffPhi/F");

  for (Int_t j = 0; j < entries; j++) {

    // Make sure to get the jth entry
    inputTree->GetEntry(j);

    if ( (_passCuts == 1) && (_cosmiLL > -3.0) ) {

      passCutCount = passCutCount + 1;

      x = _x;
      y = _y;
      z = _z;
      muTheta = Muon_ThetaReco;
      muPhi = Muon_PhiReco;
      muLen = Muon_TrackLength;

      //      cout << "!!!!! This is the INPUT X value: " << _x << endl;
      cout << "!!!!! This is the INPUT X value: " << _x << endl;
      cout << "The associated track is: Run: " << _run << " Subrun: " << _subrun << " Event: " << _event << " Vtxid: " << _vtxid << endl;

      tie( chosenX, chosenY, chosenZ, chosenMuTheta, chosenMuPhi, chosenMuLen ) = findTrack( _x, _y, _z, Muon_ThetaReco, Muon_PhiReco, Muon_TrackLength, j, comparisonTree);

      cout << "THIS IS WHAT THE CHOSENX WAS FOUND TO BE: " << chosenX << endl;
      cout << "THIS IS WHAT THE CHOSENY WAS FOUND TO BE: " << chosenY << endl;

      diffPhi = (chosenMuPhi - muPhi);

      _tree->Fill();

    }

  }

  cout << "This is how many events pass cuts: " << passCutCount << endl;

  outputTree->Write();
  outputTree->Close();   

  return 0;
}
