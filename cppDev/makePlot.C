// Comparison plots of params

void makePlot() {
 
  //TFile* f1 = new TFile("comparisonTree_040319_anglesAt001.root");
  TFile* f1 = new TFile("comparisonTree_040319_anglesAt001.root");

  TTree *mytree = (TTree *)f1->Get("tree");

  TCanvas *c1 = new TCanvas("c1", "plot", 600, 400);
  c1->DrawFrame(0,0,10,60);

  c1->cd(1);
  //  gStyle->SetOptStat(1);

  //mytree->Draw("chosenX:x");
  //mytree->Draw("chosenY:y");
        //mytree->Draw("chosenZ:z");
        //mytree->Draw("chosenMuTheta:muTheta");
  	mytree->Draw("chosenMuPhi:muPhi");
	//	mytree->Draw("chosenMuLen:muLen");

  //mytree->Draw("(chosenX-x)/x:x");
  //  mytree->Draw("(chosenY-y)/y:y");
  //mytree->Draw("(chosenZ-z)/z:z");
  //  mytree->Draw("(chosenMuTheta-muTheta)/muTheta:muTheta");
  //    mytree->Draw("(chosenMuPhi-muPhi)/muPhi:muPhi");
	/// mytree->Draw("(chosenMuLen-muLen)/muLen:muLen");
  
  //mytree->Draw("(chosenX-x):x");
    //mytree->Draw("(chosenY-y):y");
    //mytree->Draw("(chosenZ-z):z");
    //mytree->Draw("(chosenMuTheta-muTheta):muTheta");
  //    mytree->Draw("(chosenMuPhi-muPhi):muPhi");
    //mytree->Draw("(chosenMuLen-muLen):muLen");

  TGraph *g = (TGraph*)gPad->FindObject("Graph");
  g->SetMarkerColor(kRed);
  g->SetMarkerStyle(20);
  //auto axis = g->GetYaxis();
  //  g->GetYaxis()->SetRangeUser(0.5, 0.6);
  //  g->GetYaxis()->SetLimits(-0.1, 1.0);
  g->GetYaxis()->SetRangeUser(-0.1, 1.0);
  //axis->SetLimits(-0.5,0.5);
  //g->GetHistogram()->SetMaximum(20.);

  g->Draw("P");
  //  g->GetYaxis()->SetRange(22,23);
}
