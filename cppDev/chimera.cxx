// Implementing LLHD for multiple vars (2 for now)

#include <iostream>
#include <iomanip>

#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TMath.h"

using namespace std;

Double_t logLLHD_p1(Int_t n) {

  Double_t p1 = -0.5 * n * TMath::Log( 2.0 * TMath::Pi() );  

  return p1;
}

Double_t logLLHD_p2(Double_t sigmaSqX, Double_t sigmaSqY) {

  Double_t p2 = -0.5 * ( TMath::Log( sigmaSqX ) + TMath::Log( sigmaSqY ) ) ;

  return p2; 
}

Double_t logLLHD_p3(Double_t sigmaSqX, Double_t x, Double_t muX, Double_t sigmaSqY, Double_t y, Double_t muY ) {

  if (sigmaSqX == 0.0)
    cout << "  Error: The code tries to divide by zero! " << endl;

  Double_t p3 = -0.5 * ( ( x - muX )*( x - muX ) / sigmaSqX + ( y - muY )*( y - muY ) / sigmaSqY );

  return p3;
}

int main() {

  // How many parameters are we looking at?
  Int_t numParams = 2;

  // Input "requested" variables
  Double_t inputX = 202.0;
  Double_t inputY = 100.7;
  Double_t desiredSigmaX = 0.001;
  Double_t desiredSigmaY = 100.0;

  Double_t desiredSigmaSqX = desiredSigmaX * desiredSigmaX;
  Double_t desiredSigmaSqY = desiredSigmaY * desiredSigmaY;

  TFile *f1 = new TFile("../data/FinalVertexVariables.root","READ");

  f1->ls();
  TTree *myTree = (TTree *)f1->Get("NuMuVertexVariables");

  // Create the variable(s) that are in the input TTree again
  int _run;
  int _subrun; 
  int _event;
  float _x;
  float _y;
  int _passCuts;
  float _cosmiLL;
  
  myTree->SetBranchAddress("run", &_run);
  myTree->SetBranchAddress("subrun", &_subrun);
  myTree->SetBranchAddress("event", &_event);
  myTree->SetBranchAddress("Xreco", &_x);
  myTree->SetBranchAddress("Yreco", &_y);
  myTree->SetBranchAddress("PassCuts", &_passCuts);
  myTree->SetBranchAddress("CosmicLL", &_cosmiLL);

  Int_t entries = myTree->GetEntries();

  cout << "How many entries are in the input TTree? There are: " << entries << endl;

  cout << "Got TTree... Here's a print out:" << endl;

  myTree->Print();

  //------------------------------
  
  int passCutCount = 0;

  float x;
  float y;

  TFile *outputTree = new TFile("outputTree.root","RECREATE");
  TTree *_tree = new TTree("tree","Just a Tree");
  _tree->Branch("x", &x, "x/F");
  _tree->Branch("y", &x, "y/F");
  
  Double_t smallestLLHD = 99999999999999999999999999999999999999999999999999999999999.99;
  Double_t closestX = 9999.99;
  Double_t correspondingY = 9999.99;
  Int_t chosenRun = -999;
  Int_t chosenSubrun = -999;
  Int_t chosenEvent = -999;

  for (Int_t i = 0; i < entries; i++) { 
    
    // Make sure to get the ith entry
    myTree->GetEntry(i);

    // Check if the entries pass the cuts before getting the x values
    if ( (_passCuts == 1) && (_cosmiLL > -3.0) ) {

      passCutCount = passCutCount + 1; 
      
      cout << "The x value is: " << std::fixed << _x << endl; 
      cout << "The y value is: " << std::fixed << _y << endl; 
      cout << "inputY: " << inputY << endl; 
      cout << "inputY: " << inputY << endl; 
      cout << "The associated track is: Run: " << _run << " Subrun: " << _subrun << " Event: " << _event << endl; 

      // Compute the negative log LLHD here
      Double_t LLHD_i = - ( logLLHD_p1( numParams ) + logLLHD_p2( desiredSigmaSqX, desiredSigmaSqY ) + logLLHD_p3( desiredSigmaSqX, _x, inputX, desiredSigmaSqY, _y, inputY ) );
      cout << "LLHD_i = " << LLHD_i << endl;
    
      if (LLHD_i < smallestLLHD) {

	cout << "Current LLHD is the smallest found so far!" << endl;
	smallestLLHD = LLHD_i;
	closestX = _x;
	correspondingY = _y;
	chosenRun = _run;
	chosenSubrun = _subrun;
	chosenEvent = _event;
	
      }

    }

  }

  cout << "This many events passed the cuts: " << passCutCount << endl;
  cout << "Minimizing the negative log LLHD, we found that the closest X is " << closestX << " with a LLHD value of " << smallestLLHD << "." << endl;
  cout << "CorrespondingY was: " << correspondingY << endl;
  cout << "The input X was: " << inputX << endl;
  cout << "The associated track is: Run: " << chosenRun << " Subrun: " << chosenSubrun << " Event: " << chosenEvent << endl;

  outputTree->Write();
  outputTree->Close();

  return 0;   // Good style

}
