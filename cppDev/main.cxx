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

  TFile *f1 = new TFile("../data/FinalVertexVariables.root","READ");
  TTree *myTree = (TTree *)f1->Get("NuMuVertexVariables");

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

  myTree->SetBranchAddress("run", &_run);
  myTree->SetBranchAddress("subrun", &_subrun);
  myTree->SetBranchAddress("event", &_event);
  myTree->SetBranchAddress("vtxid", &_vtxid);
  myTree->SetBranchAddress("Xreco", &_x);
  myTree->SetBranchAddress("Yreco", &_y);
  myTree->SetBranchAddress("Zreco", &_z);
  myTree->SetBranchAddress("Muon_ThetaReco", &Muon_ThetaReco);
  myTree->SetBranchAddress("Muon_PhiReco", &Muon_PhiReco);
  myTree->SetBranchAddress("Muon_TrackLength", &Muon_TrackLength);
  myTree->SetBranchAddress("PassCuts", &_passCuts);
  myTree->SetBranchAddress("CosmicLL", &_cosmiLL);

  Int_t entries = myTree->GetEntries();

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
  
  TFile *outputTree = new TFile("comparisonTree_040319_all1Sigma.root","RECREATE");
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

  for (Int_t j = 0; j < 10000; j++) {

    // Make sure to get the jth entry
    myTree->GetEntry(j);

    if ( (_passCuts == 1) && (_cosmiLL > -3.0) ) {

      x = _x;
      y = _y;
      z = _z;
      muTheta = Muon_ThetaReco;
      muPhi = Muon_PhiReco;
      muLen = Muon_TrackLength;

      //      cout << "!!!!! This is the INPUT X value: " << _x << endl;
      cout << "!!!!! This is the INPUT X value: " << _x << endl;
      cout << "The associated track is: Run: " << _run << " Subrun: " << _subrun << " Event: " << _event << " Vtxid: " << _vtxid << endl;

      tie( chosenX, chosenY, chosenZ, chosenMuTheta, chosenMuPhi, chosenMuLen ) = findTrack( _x, _y, _z, Muon_ThetaReco, Muon_PhiReco, Muon_TrackLength, j );

      cout << "THIS IS WHAT THE CHOSENX WAS FOUND TO BE: " << chosenX << endl;
      cout << "THIS IS WHAT THE CHOSENY WAS FOUND TO BE: " << chosenY << endl;

      _tree->Fill();

    }

  }

  outputTree->Write();
  outputTree->Close();   

  return 0;
}
