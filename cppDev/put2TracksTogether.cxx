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
#include "/Users/nduser/dllee_unified/LArCV/core/DataFormat/EventImage2D.h"

using namespace std;

int main() {

  // This is the likelihood output tree containing matching pairs of muons & protons
  TFile *f1 = new TFile("output_likelihood_smoltest.root","READ");
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

  // Use IOManager to read input image2d tree
  larcv::IOManager* ioman = new larcv::IOManager(larcv::IOManager::kREAD);
  ioman->add_in_file("hadded_output.root");
  ioman->initialize();

  larcv::IOManager* out_iom = new larcv::IOManager(larcv::IOManager::kWRITE);
  out_iom->set_out_file("happy4thofjuly.root");
  out_iom->initialize();

  // For each track chosen by the likelihood, find the corresponding image2ds
  Int_t llhdEntries = llhdOutputTree->GetEntries();
  ULong_t image2dEntries = ioman->get_n_entries();

  Int_t _run;
  ULong_t _subrun;
  ULong_t _event;  

  for (Int_t i = 0; i < llhdEntries; i++) {

    llhdOutputTree->GetEntry(i);

    std::vector<larcv::Image2D> out_img_muon(3);
    std::vector<larcv::Image2D> out_img_proton(3);
    std::vector<larcv::Image2D> out_img_combined(3);
    std::vector<larcv::ImageMeta> meta(3);

    bool foundMuon = false;
    bool foundProton = false;

    Int_t _muonRun; 
    Int_t _muonSubrun;
    ULong_t _muonEvent;
    Int_t _protonRun; 
    Int_t _protonSubrun;
    ULong_t _protonEvent;
    Int_t _combinedRun; 
    Int_t _combinedSubrun;
    unsigned long long _combinedEvent;
    //    uint64_t _combinedEvent;
    
    for (Int_t j = 0; j < image2dEntries; j++) {

      ioman->read_entry(j);
      //      out_iom->read_entry(j);
      
      // Include this line every time using IOManager, after getting the entry
      const auto ev_img = (larcv::EventImage2D*)ioman->get_data( larcv::kProductImage2D, "test" );
      larcv::EventImage2D* ev_img_out = (larcv::EventImage2D*)out_iom->get_data( larcv::kProductImage2D, "testCombined" );
      larcv::EventImage2D* ev_img_out_muon = (larcv::EventImage2D*)out_iom->get_data( larcv::kProductImage2D, "testMuon" );
      larcv::EventImage2D* ev_img_out_proton = (larcv::EventImage2D*)out_iom->get_data( larcv::kProductImage2D, "testProton" );

      _run =  ioman->event_id().run();
      _subrun =  ioman->event_id().subrun();
      _event =  ioman->event_id().event();

      if (_run != _chosenMuRun && _run != _chosenPRun) continue;
      if (_subrun != _chosenMuSubrun && _subrun != _chosenPSubrun) continue;
      if (_event != (_chosenMuEvent*100 + _chosenMuVtxid*10 + _chosenMuMuID) && _event != (_chosenPEvent*100 + _chosenPVtxid*10 + _chosenPProtID)) continue;

      std::cout << "Run: " << _run << " ChosenMuRun: " << _chosenMuRun << " ChosenPRun: " << _chosenPRun << std::endl;
      std::cout << "Subrun: " << _subrun << " ChosenMuSubrun: " << _chosenMuSubrun << " ChosenPSubrun: " << _chosenPSubrun << std::endl;
      std::cout << "Event: " << _event << " ChosenMuEvent: " << _chosenMuEvent << " ChosenPEvent: " << _chosenPEvent << std::endl;

      if (_run == _chosenMuRun && _subrun == _chosenMuSubrun && _event == ( _chosenMuEvent*100 + _chosenMuVtxid*10 + _chosenMuMuID) ) {

	std::cout << "FOUND A MUON MATCH!!" << std::endl;
	foundMuon = true;
	
	out_img_muon = ev_img->Image2DArray();
	
	std::cout << "Event #: " << _event << std::endl;
	std::cout << "Chosen Mu Event #: " <<  _chosenMuEvent*100 + _chosenMuVtxid*10 + _chosenMuMuID << std::endl;

	_muonRun = _run;
	_muonSubrun = _subrun;
	_muonEvent = _event;

      }
      
      if (_run == _chosenPRun && _subrun == _chosenPSubrun && _event == ( _chosenPEvent*100 + _chosenPVtxid*10 + _chosenPProtID)) {

	std::cout << "FOUND A PROTON MATCH!!" << std::endl;
	foundProton = true;

	out_img_proton = ev_img->Image2DArray();

	std::cout << "Event #: " << _event << std::endl;
	std::cout << "Chosen P Event #: " <<  _chosenPEvent*100 + _chosenPVtxid*10 + _chosenPProtID << std::endl;

	_protonRun = _run;
	_protonSubrun = _subrun;
	_protonEvent = _event;

      }      
     
      if (foundMuon && foundProton) {
	
	_combinedRun = _muonRun * 100000 + _protonRun;
	_combinedSubrun = _muonSubrun * 10000 + _protonSubrun;
	_combinedEvent = _muonEvent * 100000000 + _protonEvent;
	
	for (size_t iPlane= 0; iPlane < 3; iPlane++ ) {
	  meta[iPlane] = out_img_muon[iPlane].meta();
	  out_img_combined[iPlane] = out_img_muon[iPlane];
	  out_img_combined[iPlane].paint(0.0);
	  for (size_t icol = 0; icol < out_img_muon[iPlane].meta().cols(); icol++) {
	    for(size_t irow = 0; irow < out_img_muon[iPlane].meta().rows(); irow++) {
	      out_img_combined[iPlane].set_pixel(irow,icol,out_img_muon[iPlane].pixel(irow, icol) + out_img_proton[iPlane].pixel(irow, icol));
	      
	    }
	  }
	  
	  ev_img_out->Emplace(std::move(out_img_combined[iPlane]));
	  ev_img_out_muon->Emplace(std::move(out_img_muon[iPlane]));
	  ev_img_out_proton->Emplace(std::move(out_img_proton[iPlane]));
	  
	}

	out_iom->set_id(_combinedRun, _combinedSubrun, _combinedEvent);
	out_iom->save_entry();
	break;
	
	//	for (size_t iPlane= 0; iPlane < 3; iPlane++ ) {

	  

	//	}
	
      }
    
    }
  }
 

  ioman->finalize();
  out_iom->finalize();
  
  return 0;

}


// Same meta: single loop, rows and columns
// Set the pixel after running over both to create a new image2d with both sets of pixels in it

// GetProjectedImagePixel (in UBWireTool)

// Add run, subrun, event 
