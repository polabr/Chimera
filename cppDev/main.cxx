#include <iostream>
#include "findTrack.h"

#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"

using namespace std;

int main() {

  // Loop through each entry in the tree and select the X value (make this the inputX)
  // Find the ChosenX (closest found X) while also skipping this entry
  // Fill a new tree with the found entries
  // Also fill an X variable in the new tree with the FVV.root entries
  // Make a 1D scatter plot showing the correlation (plot all FinalVertexVariables X entries vs. closestX)

  TFile *f1 = new TFile("../data/FinalVertexVariables.root","READ");
  TTree *myTree = (TTree *)f1->Get("NuMuVertexVariables");

  int _run;
  int _subrun;
  int _event;
  int _vtxid;
  float _x;
  int _passCuts;
  float _cosmiLL;

  myTree->SetBranchAddress("run", &_run);
  myTree->SetBranchAddress("subrun", &_subrun);
  myTree->SetBranchAddress("event", &_event);
  myTree->SetBranchAddress("vtxid", &_vtxid);
  myTree->SetBranchAddress("Xreco", &_x);
  myTree->SetBranchAddress("PassCuts", &_passCuts);
  myTree->SetBranchAddress("CosmicLL", &_cosmiLL);

  Int_t entries = myTree->GetEntries();

  // Creating new tree to fill
  float x;
  float chosenX;
  TFile *outputTree = new TFile("comparisonTree_X_2.root","RECREATE");
  TTree *_tree = new TTree("tree","Just a Tree");
  _tree->Branch("x", &x, "x/F");
  _tree->Branch("chosenX", &x, "chosenX/F");

  for (Int_t j = 0; j < 10000; j++) {

    // Make sure to get the jth entry
    myTree->GetEntry(j);

    if ( (_passCuts == 1) && (_cosmiLL > -3.0) ) {

      x = _x;

      cout << "This is the X value: " << _x << endl;
      cout << "The associated track is: Run: " << _run << " Subrun: " << _subrun << " Event: " << _event << " Vtxid: " << _vtxid << endl;

      chosenX = findChosenX( _x, j);

      _tree->Fill();

    }

  }

  outputTree->Write();
  outputTree->Close();   

  return 0;
}
