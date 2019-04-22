#ifndef FINDPROTON_H
#define FINDPROTON_H

#include "TROOT.h"
#include "TTree.h"
#include <tuple>

Double_t logLLHD_p1(Int_t n);

Double_t logLLHD_p2(Double_t sigmaSqX, Double_t sigmaSqY, Double_t sigmaSqZ, Double_t sigmaSqTheta, Double_t sigmaSqPhi, Double_t sigmaSqLen);

Double_t logLLHD_p3(Double_t sigmaSqX, Double_t x, Double_t pX, Double_t sigmaSqY, Double_t y, Double_t pY, 
		    Double_t sigmaSqZ, Double_t z, Double_t pZ, Double_t sigmaSqTheta, Double_t theta, Double_t pTheta, 
		    Double_t sigmaSqPhi, Double_t phi, Double_t pPhi, Double_t sigmaSqLen, Double_t len, Double_t pLen);

double findChosenX( Double_t inX, Double_t inY, Double_t inZ, Double_t inPTheta, Double_t inPPhi, Double_t inPLen, Int_t j);

std::tuple<Double_t, Double_t, Double_t, Double_t, Double_t, Double_t> findTrack( Double_t inX, Double_t inY, Double_t inZ, Double_t inPTheta, Double_t inPPhi, Double_t inPLen, Int_t j, TTree *myTree);

#endif
