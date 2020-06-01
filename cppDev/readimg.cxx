#include <iostream>

// ROOT
#include "TFile.h"
#include "TH2D.h"
#include "TCanvas.h"
#include "TStyle.h"

// larlite
#include "DataFormat/storage_manager.h"
#include "LArUtil/LArProperties.h"

// larcv
#include "DataFormat/IOManager.h"
#include "DataFormat/EventImage2D.h"
#include "DataFormat/EventChStatus.h"

/**
 * simple macro to read an Image2D and plot it in TH2D
 *
 */
int main( int nargs, char** argv ) {

  gStyle->SetOptStat(0);

  std::string input_larcv  = argv[1]; // full file path of input larcv file 
  std::string output_larcv  = argv[2]; // full file path of output larcv file 
  
  int run, subrun;
  ULong_t event;
  
  // input larcv
  larcv::IOManager iolarcv( larcv::IOManager::kREAD );
  iolarcv.add_in_file( input_larcv );
  iolarcv.initialize();

  /* code below shows how I would save the output
  // larcv:
  larcv::IOManager outlarcv( larcv::IOManager::kWRITE, "", larcv::IOManager::kTickBackward );
  outlarcv.set_out_file( output_larcv );
  outlarcv.initialize();
  //larlite:
  larlite::storage_manager outlarlite( larlite::storage_manager::kWRITE );
  outlarlite.set_out_filename( output_larlite );
  outlarlite.open();
  */
  //  std::string output_hist = "cooloutput_8thofjuly.root";
  TFile* fout = new TFile( output_larcv.c_str(), "recreate" ); // do not rewrite
  TH2D* hadc_muon[3];
  TH2D* hadc_proton[3];
  TH2D* hadc_combined[3];

  int nentries = iolarcv.get_n_entries();
  //  nentries=1;
  for (int i=0; i<nentries; i++) {
    iolarcv.read_entry(i);

    std::cout << "entry " << i << std::endl;

    // in
    auto ev_img_muon = (larcv::EventImage2D*)iolarcv.get_data( larcv::kProductImage2D, "testMuon" );
    auto ev_img_proton = (larcv::EventImage2D*)iolarcv.get_data( larcv::kProductImage2D, "testProton" );
    auto ev_img_combined = (larcv::EventImage2D*)iolarcv.get_data( larcv::kProductImage2D, "testCombined" );
        
    // out: if I am saving Image2Ds
    //auto evout_wire            = (larcv::EventImage2D*)outlarcv.get_data( larcv::kProductImage2D, "testout");
    
    run    = iolarcv.event_id().run();
    subrun = iolarcv.event_id().subrun();
    event  = iolarcv.event_id().event();

    std::cout << ev_img_muon->Image2DArray().size() << " " << ev_img_combined->Image2DArray().size() << std::endl;

    //std::cout <<"run "<< run <<" subrun " << subrun <<" event " << event << std::endl;

    for ( int planeid=0; planeid<3; planeid++ ) {
      auto const& img_muon = ev_img_muon->Image2DArray().at( planeid );
      auto const& img_proton = ev_img_proton->Image2DArray().at( planeid );
      auto const& img_combined = ev_img_combined->Image2DArray().at( planeid );
      auto const& meta = img_muon.meta();
      char histname_event[100];
      sprintf(histname_event,"hadc_muon_run%d_event%lu_plane%d",run,event,planeid);
      hadc_muon[planeid] = new TH2D(histname_event,"",meta.cols(), 0, meta.cols(), meta.rows(),0,meta.rows());
      sprintf(histname_event,"hadc_proton_run%d_event%lu_plane%d",run,event,planeid);
      hadc_proton[planeid] = new TH2D(histname_event,"",meta.cols(), 0, meta.cols(), meta.rows(),0,meta.rows());
      sprintf(histname_event,"hadc_combined_run%d_event%lu_plane%d",run,event,planeid);
      hadc_combined[planeid] = new TH2D(histname_event,"",meta.cols(), 0, meta.cols(), meta.rows(),0,meta.rows());

      for(int row = 0; row<meta.rows(); row++){
        for(int col=0; col<meta.cols(); col++){
          float pixamp = img_muon.pixel( row, col );
          hadc_muon[planeid]->SetBinContent(col+1, row+1, pixamp);
          pixamp = img_proton.pixel( row, col );
          hadc_proton[planeid]->SetBinContent(col+1, row+1, pixamp);
	  pixamp = img_combined.pixel(row, col);
          hadc_combined[planeid]->SetBinContent(col+1, row+1, pixamp);
        }
      }
    }
    // if I am savinf Image2D output: transfer image and chstatus data to output file
    //for ( auto const& img : ev_img->Image2DArray() ) {
    //  evout_wire->Append( img );
    //}

    //for larlite output
    //outlarlite.set_id( run, subrun, event );
    //outlarlite.next_event();

    // for larcv output
    //outlarcv.set_id( run, subrun, event );
    //outlarcv.save_entry();

    fout->cd();
    for ( int p=0; p<3; p++ ) {    
      hadc_muon[p]->Write();
      hadc_proton[p]->Write();
      hadc_combined[p]->Write();
    }

  }//end of entry loop
  
  iolarcv.finalize();
  //outlarcv.finalize();
  //outlarlite.close();
  fout->Close();

  
  return 0;
}
