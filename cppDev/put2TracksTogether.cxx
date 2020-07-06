// This code takes Image2D files of 2 selected tracks and puts them together for each event

#include <iostream>
//#include <tuple>
#include <stdlib.h>
#include "findMuon.h" 
#include "findProton.h"

#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "../../dllee_unified/LArCV/core/DataFormat/IOManager.h"
#include "../../dllee_unified/LArCV/core/DataFormat/EventImage2D.h"

using namespace std;

int main( int nargs, char** argv) {

  std::string input_llhd_file  = argv[1];
  std::string input_vtx_file  = argv[2];
  std::string input_image2d_file  = argv[3];
  std::string output_chimera_file  = argv[4];
  std::string output_vtx_file  = argv[5];

  // This is the likelihood output tree containing matching pairs of muons & protons
  TFile *f1 = new TFile(input_llhd_file.c_str(),"READ");
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

  // This is the vertex tree, containing vertex point information for each track
  TFile *f2 = new TFile(input_vtx_file.c_str(),"READ");
  TTree *vtxTree = (TTree *)f2->Get("tree");

  // Create vars in the vertex tree again and set branch address
  int run;
  int subrun;
  int event;
  double vtxPt_plane0_x;
  double vtxPt_plane0_y;
  double vtxPt_plane1_x;
  double vtxPt_plane1_y;
  double vtxPt_plane2_x;
  double vtxPt_plane2_y;
  std::vector<std::vector<double>> *treeVtxVector = 0;

  gROOT->ProcessLine("#include <vector>");

  vtxTree->SetBranchAddress("run", &run);
  vtxTree->SetBranchAddress("subrun", &subrun);
  vtxTree->SetBranchAddress("event", &event);
  vtxTree->SetBranchAddress("vtxPt_plane0_x", &vtxPt_plane0_x);
  vtxTree->SetBranchAddress("vtxPt_plane0_y", &vtxPt_plane0_y);
  vtxTree->SetBranchAddress("vtxPt_plane1_x", &vtxPt_plane1_x);
  vtxTree->SetBranchAddress("vtxPt_plane1_y", &vtxPt_plane1_y);
  vtxTree->SetBranchAddress("vtxPt_plane2_x", &vtxPt_plane2_x);
  vtxTree->SetBranchAddress("vtxPt_plane2_y", &vtxPt_plane2_y);
  vtxTree->SetBranchAddress("treeVtxVector", &treeVtxVector);

  // Use IOManager to read input image2d tree
  larcv::IOManager* ioman = new larcv::IOManager(larcv::IOManager::kREAD);
  ioman->add_in_file(input_image2d_file.c_str());
  ioman->initialize();
  
  // This will be the output chimera image2d
  larcv::IOManager* out_iom = new larcv::IOManager(larcv::IOManager::kWRITE);
  out_iom->set_out_file(output_chimera_file.c_str());
  out_iom->initialize();

  // Create a new tree to hold vertex information for Tracker
  int trkrRun;
  int trkrSubrun;
  int trkrEvent;
  std::vector<double> temp;
  std::vector<std::vector<double>> *vtxVector = 0;

  TFile *trackerVtx = new TFile(output_vtx_file.c_str(),"RECREATE");
  TTree *trackerTree = new TTree("tree","vtxTree");
  trackerTree->Branch("trkrRun", &trkrRun, "trkrRun/I");
  trackerTree->Branch("trkrSubrun", &trkrSubrun, "trkrSubrun/I");
  trackerTree->Branch("trkrEvent", &trkrEvent, "trkrEvent/I");
  trackerTree->Branch("vtxVector", &vtxVector);

  // For each track chosen by the likelihood, find the corresponding image2ds
  Int_t llhdEntries = llhdOutputTree->GetEntries();
  ULong_t image2dEntries = ioman->get_n_entries();

  // These will be the RSE for every image2d entry
  Int_t _run;
  ULong_t _subrun;
  ULong_t _event;  
  
  // First loop thru all llhd entries, which contain info for both muon & proton
  //  for (Int_t i = 0; i < llhdEntries; i++) {
  for (Int_t i = 0; i < 4; i++) {
    vtxVector->clear();

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
    
    //    std::pair<double, double> muonXY;
    //std::pair<double, double> protonXY;
    //    std::vector<std::pair<double, double>> muonVtx;
    std::vector<std::vector<double>> muonVtx( 3, std::vector<double> (2,-1) );
    std::vector<std::vector<double>> protonVtx( 3, std::vector<double> (2,-1) );
											
    /*
    double muonPt_plane0_x;
    double muonPt_plane0_y;
    double muonPt_plane1_x;
    double muonPt_plane1_y;
    double muonPt_plane2_x;
    double muonPt_plane2_y;
    double protonPt_plane0_x;
    double protonPt_plane0_y;
    double protonPt_plane1_x;
    double protonPt_plane1_y;
    double protonPt_plane2_x;
    double protonPt_plane2_y;
    */    

    larcv::EventImage2D* ev_img_out = (larcv::EventImage2D*)out_iom->get_data( larcv::kProductImage2D, "testCombined" );
    larcv::EventImage2D* ev_img_out_muon = (larcv::EventImage2D*)out_iom->get_data( larcv::kProductImage2D, "testMuon" );
    larcv::EventImage2D* ev_img_out_proton = (larcv::EventImage2D*)out_iom->get_data( larcv::kProductImage2D, "testProton" );
    
    // Find the corresponding track image2ds for the current muon and proton
    for (Int_t j = 0; j < image2dEntries; j++) {
      
      ioman->read_entry(j);
      //      out_iom->read_entry(j);
      
      // Grab entry from vertex tree
      vtxTree->GetEntry(j);

      // Include this line every time using IOManager, after getting the entry
      const auto ev_img = (larcv::EventImage2D*)ioman->get_data( larcv::kProductImage2D, "test" );

      _run =  ioman->event_id().run();
      _subrun =  ioman->event_id().subrun();
      _event =  ioman->event_id().event();

      /*
      std::cout << "_event: " << _event << std::endl;
      std::cout << "_chosenMuEvent: " << _chosenMuEvent << std::endl;
      std::cout << "_chosenMuEvent*100 + _chosenMuVtxid*10 + _chosenMuMuID: " << _chosenMuEvent*100 + _chosenMuVtxid*10 + _chosenMuMuID << std::endl;
      std::cout << "_chosenPEvent: " << _chosenPEvent << std::endl;
      std::cout << "_chosenPEvent*100 + _chosenPVtxid*10 + _chosenPProtID: " << _chosenPEvent*100 + _chosenPVtxid*10 + _chosenPProtID << std::endl;
      */
      if (_run != _chosenMuRun && _run != _chosenPRun) continue;
      if (_subrun != _chosenMuSubrun && _subrun != _chosenPSubrun) continue;
      if (_event != (_chosenMuEvent*100 + _chosenMuVtxid*10 + _chosenMuMuID) && _event != (_chosenPEvent*100 + _chosenPVtxid*10 + _chosenPProtID)) continue;
      /*
      std::cout << "Run: " << _run << " ChosenMuRun: " << _chosenMuRun << " ChosenPRun: " << _chosenPRun << std::endl;
      std::cout << "Subrun: " << _subrun << " ChosenMuSubrun: " << _chosenMuSubrun << " ChosenPSubrun: " << _chosenPSubrun << std::endl;
      std::cout << "Event: " << _event << " ChosenMuEvent: " << _chosenMuEvent << " ChosenPEvent: " << _chosenPEvent << std::endl;

      std::cout << "Vtx Run: " << run << std::endl;
      std::cout << "Vtx Event: " << event << std::endl;
      */

      // Found a match!
      // Now, is it the muon or proton?
      
      if (_run == _chosenMuRun && _subrun == _chosenMuSubrun && _event == ( _chosenMuEvent*100 + _chosenMuVtxid*10 + _chosenMuMuID) ) {
	
	//std::cout << "FOUND A MUON MATCH!!" << std::endl;
	foundMuon = true;
	
	out_img_muon = ev_img->Image2DArray();
	/*
	std::cout << "Event #: " << _event << std::endl;
	std::cout << "Chosen Mu Event #: " <<  _chosenMuEvent*100 + _chosenMuVtxid*10 + _chosenMuMuID << std::endl;
	*/
	_muonRun = _run;
	_muonSubrun = _subrun;
	_muonEvent = _event;

	muonVtx[0][0] = vtxPt_plane0_x; 
	muonVtx[0][1] = vtxPt_plane0_y; 
	muonVtx[1][0] = vtxPt_plane1_x; 
	muonVtx[1][1] = vtxPt_plane1_y; 
	muonVtx[2][0] = vtxPt_plane2_x; 
	muonVtx[2][1] = vtxPt_plane2_y; 
	
	vtxVector = treeVtxVector;
	
	//	std::cout << "Muon Plane 0, x_pixel: " << vtxPt_plane0_x << " y_pixel: " << vtxPt_plane0_y << std::endl;

	/*
	muonPt_plane0_x = vtxPt_plane0_x;
	muonPt_plane0_y = vtxPt_plane0_y;
	muonPt_plane1_x = vtxPt_plane1_x;
	muonPt_plane1_y = vtxPt_plane1_y;
	muonPt_plane2_x = vtxPt_plane2_x;
	muonPt_plane2_y = vtxPt_plane2_y;
	*/
      }
      
      if (_run == _chosenPRun && _subrun == _chosenPSubrun && _event == ( _chosenPEvent*100 + _chosenPVtxid*10 + _chosenPProtID)) {

	//std::cout << "FOUND A PROTON MATCH!!" << std::endl;
	foundProton = true;

	out_img_proton = ev_img->Image2DArray();

	//std::cout << "Event #: " << _event << std::endl;
	//std::cout << "Chosen P Event #: " <<  _chosenPEvent*100 + _chosenPVtxid*10 + _chosenPProtID << std::endl;

	_protonRun = _run;
	_protonSubrun = _subrun;
	_protonEvent = _event;
	
	protonVtx[0][0] = vtxPt_plane0_x; 
	protonVtx[0][1] = vtxPt_plane0_y; 
	protonVtx[1][0] = vtxPt_plane1_x; 
	protonVtx[1][1] = vtxPt_plane1_y; 
	protonVtx[2][0] = vtxPt_plane2_x; 
	protonVtx[2][1] = vtxPt_plane2_y; 
	
	///	std::cout << "Proton Plane 0, x_pixel: " << vtxPt_plane0_x << " y_pixel: " << vtxPt_plane0_y << std::endl;

	/*
	protonPt_plane0_x = vtxPt_plane0_x;
	protonPt_plane0_y = vtxPt_plane0_y;
	protonPt_plane1_x = vtxPt_plane1_x;
	protonPt_plane1_y = vtxPt_plane1_y;
	protonPt_plane2_x = vtxPt_plane2_x;
	protonPt_plane2_y = vtxPt_plane2_y;
	*/
      }      
     
      if (foundMuon && foundProton) {

	trkrRun = _muonRun;
	trkrSubrun = _muonSubrun;
	trkrEvent = _muonEvent;

	_combinedRun = _muonRun * 100000 + _protonRun;
	_combinedSubrun = _muonSubrun * 10000 + _protonSubrun;
	_combinedEvent = _muonEvent * 100000000 + _protonEvent;
	
	for (size_t iPlane= 0; iPlane < 3; iPlane++ ) {
	  meta[iPlane] = out_img_muon[iPlane].meta();
	  //	  out_img_combined[iPlane] = out_img_muon[iPlane];
	  //	  out_img_combined[iPlane].paint(0.0);
	  
	  //	  double deltaX = muonVtx[iPlane][0] - protonVtx[iPlane][0];
	  //double deltaY = muonVtx[iPlane][1] - protonVtx[iPlane][1];

	  // double deltaX = 1043-697;
	  //double deltaY = 758-600;

	  double deltaX_d = protonVtx[iPlane][0] - muonVtx[iPlane][0];
	  double deltaY_d = protonVtx[iPlane][1] - muonVtx[iPlane][1];

	  int deltaX = (int) deltaX_d;
	  int deltaY = (int) deltaY_d;
	  
	  /*
	  std::cout << "deltaX: " << deltaX << std::endl;
	  std::cout << "deltaY: " << deltaY << std::endl;

	  std::cout << "Here are the meta dumps: muon, proton, combined" << std::endl;
	  std::cout << out_img_muon[iPlane].meta().dump() << std::endl;
	  std::cout << out_img_proton[iPlane].meta().dump() << std::endl;
	  std::cout << out_img_combined[iPlane].meta().dump() << std::endl;
	  */
	  
	  /*
	  std::cout << "protonVtx[iPlane][0] = " << protonVtx[iPlane][0] << std::endl;
	  std::cout << "protonVtx[iPlane][1] = " << protonVtx[iPlane][1] << std::endl;
	  std::cout << "muonVtx[iPlane][0] = " << muonVtx[iPlane][0] << std::endl;
	  std::cout << "muonVtx[iPlane][1] = " << muonVtx[iPlane][1] << std::endl;

	  std::cout << "deltaX = " << deltaX << std::endl;
	  std::cout << "What deltaX should be: " << 1043-697 << std::endl;
	  std::cout << "deltaY = " << deltaY << std::endl;
	  std::cout << "What deltaY should be: " << 758-600 << std::endl;

	  std::cout << deltaX << " " << deltaY << std::endl;
	  */

	  // Loop for zeroing out dead regions in the muon, proton images
	  for (int icol = 0; icol < out_img_proton[iPlane].meta().cols(); icol++) {
	    for(int irow = 0; irow < out_img_proton[iPlane].meta().rows(); irow++) {

	      if (out_img_muon[iPlane].pixel(irow, icol) <= 11) out_img_muon[iPlane].set_pixel(irow,icol,0);
	      if (out_img_proton[iPlane].pixel(irow, icol) <= 11) out_img_proton[iPlane].set_pixel(irow,icol,0);

	      // DEBUG: set muon, proton tracks to different colors
	      if (out_img_muon[iPlane].pixel(irow, icol) > 11) out_img_muon[iPlane].set_pixel(irow,icol,300);
	      if (out_img_proton[iPlane].pixel(irow, icol) > 11) out_img_proton[iPlane].set_pixel(irow,icol,600);
	      
	    }
	  }
	 
	  // Copy the muon event with removed dead regions to become the combined event
	  out_img_combined[iPlane] = out_img_muon[iPlane];
	  /*
	  // DEBUG: Draw where we expect the vertex to be:
	  out_img_muon[iPlane].set_pixel(muonVtx[iPlane][1],muonVtx[iPlane][0],4000);
	  out_img_proton[iPlane].set_pixel(protonVtx[iPlane][1],protonVtx[iPlane][0],4000);
	  out_img_combined[iPlane].set_pixel(muonVtx[iPlane][1],muonVtx[iPlane][0],4000);
	  */
	  // Loop for copying pixels to create the chimera
	  // "For a given proton pixel:"
	  for (int icol = 0; icol < out_img_proton[iPlane].meta().cols(); icol++) {
	    for(int irow = 0; irow < out_img_proton[iPlane].meta().rows(); irow++) {

	      if (out_img_proton[iPlane].pixel(irow, icol) > 11) {
	      
	      // "If pixel is within bounds:"
		if ( ( icol+deltaX >= 0 ) 
		     && ( icol+deltaX < meta[iPlane].cols() )
		     && ( irow+deltaY >= 0 )
		     && ( irow+deltaY < meta[iPlane].rows() ) 
		     ) {
				
		//if (out_img_proton[iPlane].pixel(irow+deltaY, icol+deltaX)>0) std::cout << "This passed" << std::endl;

		//		if (out_img_muon[iPlane].pixel(irow, icol) > 11 || out_img_proton[iPlane].pixel(irow, icol) > 11) {


		  // Flip image upside down
		  //out_img_combined[iPlane].set_pixel(meta[iPlane].rows()-irow-1,icol,out_img_muon[iPlane].pixel(irow, icol) + out_img_proton[iPlane].pixel(irow + deltaY, icol + deltaX));

		  // Right side up image
		  float pixelValue = out_img_combined[iPlane].pixel(irow-deltaY, icol-deltaX);
		  pixelValue+=out_img_proton[iPlane].pixel(irow, icol);
		  //		  out_img_combined[iPlane].set_pixel(irow,icol,out_img_muon[iPlane].pixel(irow, icol) + out_img_proton[iPlane].pixel(irow + deltaY, icol + deltaX));
		  out_img_combined[iPlane].set_pixel(irow - deltaY, icol - deltaX, pixelValue);
		  
		}
		//		if (out_img_combined[iPlane].pixel(irow, icol) <= 11) out_img_combined[iPlane].set_pixel(irow,icol,0);

	      }

	      /*else {

		// Remove dead wire zones on original isolated muon and proton
		if (out_img_muon[iPlane].pixel(irow, icol) <= 11) out_img_muon[iPlane].set_pixel(irow,icol,0);
		if (out_img_proton[iPlane].pixel(irow, icol) <= 11) out_img_proton[iPlane].set_pixel(irow,icol,0);

		//		out_img_combined[iPlane].set_pixel(irow,icol,out_img_muon[iPlane].pixel(irow, icol));
		
	      }
	      */
	    }

	    // row col loop
	    
	  }

	  
	
	  ev_img_out->Emplace(std::move(out_img_combined[iPlane]));
	  ev_img_out_muon->Emplace(std::move(out_img_muon[iPlane]));
	  ev_img_out_proton->Emplace(std::move(out_img_proton[iPlane]));
	  
	}
	
	trackerTree->Fill();	  	
	out_iom->set_id(_combinedRun, _combinedSubrun, _combinedEvent);
	out_iom->save_entry();
	break;
	
      }
      
    }
    
  }  

  ioman->finalize();
  out_iom->finalize();  
  trackerVtx->cd();
  trackerVtx->Write();
  trackerVtx->Close();
  

  return 0;
  
}
      
// Same meta: single loop, rows and columns
// Set the pixel after running over both to create a new image2d with both sets of pixels in it

// GetProjectedImagePixel (in UBWireTool)

// Add run, subrun, event 
