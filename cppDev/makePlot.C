// Comparison plots of params

void makePlot() {
 
  TFile* f1 = new TFile("comparisonTree_X.root");

  TTree *mytree = (TTree *)f1->Get("tree");

  TCanvas *c1 = new TCanvas("c1", "c1", 600, 400);

  c1->cd(1);
  gStyle->SetOptStat(1);

  mytree->Draw("chosenX:x");

  TGraph *g = (TGraph*)gPad->FindObject("Graph");
  g->SetMarkerColor(kRed);
  g->SetMarkerStyle(20);

  g->Draw("p");

}
