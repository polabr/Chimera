import ROOT
from ROOT import TFile, gDirectory, TCanvas, TH2D, TH1D, TColor
from operator import truediv

f = TFile("comparisonTree_040319_anglesAt001.root","READ")
t = f.Get("tree")
canvas = TCanvas("canvas", "canvas", 720, 152, 682, 505)
canvas.cd()
corr = TH2D("corr","Chosen muPhi vs. Input muPhi",100,-4,4,100,-4,4)
f.tree.Draw("chosenMuPhi:muPhi>>corr", "", "COLZ")
corr.GetXaxis().SetTitle("Input Muon Phi Angle [radians]")
corr.GetYaxis().SetTitle("Chosen Muon Phi Angle [radians]")

input = raw_input("Press enter to continue...")
