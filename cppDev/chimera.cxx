// Implementing LLHD for 1 Variable

#include <iostream>
#include <iomanip>

#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TMath.h"

using namespace std;

Double_t logLLHD(Double_t sigmaSq, Double_t x, Double_t mu) {

  Double_t logLLHD = 0.0;

  if (sigmaSq == 0.0)
    cout << " Error : The code tries to divide by zero ! " << endl;

  logLLHD = -0.5 * TMath::Log( 2.0 * TMath::Pi() ) - 0.5 * TMath::Log( sigmaSq ) - 0.5 *  ( x - mu )*( x - mu ) / sigmaSq;

  //  cout << "part3: " << 0.5 * ( x - mu )*( x - mu ) / sigmaSq << endl;
  //    cout << "x - mu = " << x - mu << endl;

  return logLLHD;

}

int main() {

  // Input "requested" variables
  Double_t inputX = 202.0;
  Double_t desiredSigma = 0.001;

  Double_t desiredSigmaSq = desiredSigma * desiredSigma;

  TFile *f1 = new TFile("../data/FinalVertexVariables.root","READ");

  f1->ls();
  TTree *myTree = (TTree *)f1->Get("NuMuVertexVariables");

  // Create the variable(s) that are in the input TTree again
  float _x;
  int _passCuts;
  float _cosmiLL;
  
  myTree->SetBranchAddress("Xreco", &_x);
  myTree->SetBranchAddress("PassCuts", &_passCuts);
  myTree->SetBranchAddress("CosmicLL", &_cosmiLL);

  Int_t entries = myTree->GetEntries();

  cout << "How many entries are in the input TTree? There are: " << entries << endl;

  cout << "Got TTree... Here's a print out:" << endl;

  myTree->Print();

  //------------------------------
  
  int passCutCount = 0;

  float x;

  TFile *outputTree = new TFile("outputTree.root","RECREATE");
  TTree *_tree = new TTree("tree","Just a Tree");
  _tree->Branch("x", &x, "x/F");
  
  Double_t smallestLLHD = 9999.99;
  Double_t closestX = 9999.99;

  for (Int_t i = 0; i < entries; i++) { 
    
    // Make sure to get the ith entry
    myTree->GetEntry(i);

    // Check if the entries pass the cuts before getting the x values
    if ( (_passCuts == 1) && (_cosmiLL > -3.0) ) {

      passCutCount = passCutCount + 1; 
      
      cout << "The x value is: " << std::fixed << _x << endl; 
      cout << "inputX: " << inputX << endl; 

      // Compute the LLHD here
      Double_t LLHD_i = -logLLHD( desiredSigmaSq, _x, inputX );
      //      cout << "LLHD_i = " << LLHD_i << endl;
    
      if (LLHD_i < smallestLLHD) {

	cout << "Current LLHD is the smallest found so far!" << endl;
	smallestLLHD = LLHD_i;
	closestX = _x;
	
      }

    }

  }

  cout << "This many events passed the cuts: " << passCutCount << endl;
  cout << "Minimizing the negative log LLHD, we found that the closest X is " << closestX << " with a LLHD value of " << smallestLLHD << "." << endl;
  cout << "The input X was: " << inputX << endl;

  outputTree->Write();
  outputTree->Close();

  return 0;   // Good style

}
