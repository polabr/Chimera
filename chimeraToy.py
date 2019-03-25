# This toy script chooses a muon that is within 30% of the value of all parameters

import ROOT
from ROOT import TFile, gDirectory, TCanvas, TH1D, TColor, TGraphErrors, TMath, TLatex
from ROOT import TH2D
from ROOT import TTree, gROOT, AddressOf
from operator import truediv

# Input "requested" variables
inputX = 161.81
inputY = 31.6943
inputZ = 552.05
inputTheta = 1.69184
inputPhi = -2.27442
inputLength = 86.5161 # cm

f = TFile("data/FinalVertexVariables.root","READ")

print("Now looking inside the file:")
f.ls()

#print("Showing one entry of the tree:")
#f.NuMuVertexVariables.Show(367)

print("Gettin' TTree")
t = f.Get("NuMuVertexVariables")
entries = t.GetEntries()

print("Got TTree...")

#for i in range(0,50):
    #if (t.Muon_PhiReco != (-1)):
    #f.NuMuVertexVariables.Show(i)

passCutCount = 0

# Create new TTree
gROOT.ProcessLine(
"struct MyStruct {\
   Bool_t    fMyBool;\
   Float_t   fMyX;\
   Float_t   fMyY;\
   Float_t   fMyZ;\
   Float_t   fMyPhi;\
   Float_t   fMyTheta;\
   Float_t   fMyLength;\
   Char_t    fMyCode[4];\
};" );

from ROOT import MyStruct
mystruct = MyStruct()

newTree = TFile("exampleTree.root","RECREATE")
tree = TTree("tree", "Just a Tree")
tree.Branch("passedCuts", AddressOf( mystruct, 'fMyBool'), "MyBool/O")
tree.Branch("X", AddressOf( mystruct, 'fMyX'), "MyX/F")
tree.Branch("Y", AddressOf( mystruct, 'fMyY'), "MyY/F")
tree.Branch("Z", AddressOf( mystruct, 'fMyZ'), "MyZ/F")
tree.Branch("Phi", AddressOf( mystruct, 'fMyPhi'), "MyPhi/F")
tree.Branch("Theta", AddressOf( mystruct, 'fMyTheta'), "MyTheta/F")
tree.Branch("Length", AddressOf( mystruct, 'fMyLength'), "MyLength/F")

## setup code ends here -----------------------------------------------------------------------------

for jentry in range(entries):
    
    nb = t.GetEntry(jentry)
    if nb <= 0:
        continue
    #print(nb)
    if (t._passCuts == 1 and t._cosmiLL > -3 
        and t.Xreco >= (0.7*inputX) and t.Xreco < (1.3*inputX) 
        and t.Yreco >= (0.7*inputY) and t.Yreco < (1.3*inputY) 
        and t.Zreco >= (0.7*inputZ) and t.Zreco < (1.3*inputZ) 
        and t.Muon_ThetaReco >= (0.7*inputTheta) and t.Muon_ThetaReco < (1.3*inputTheta) 
        and t.Muon_PhiReco < (0.7*inputPhi) and t.Muon_PhiReco >= (1.3*inputPhi) 
        and t.Muon_TrackLength >= (0.7*inputLength) and t.Muon_TrackLength < (1.3*inputLength)):
        passCutCount = passCutCount+1
        print("This event is close!",jentry)
        mystruct.fMyBool = t._passCuts
        mystruct.fMyX = t.Xreco
        mystruct.fMyY = t.Yreco
        mystruct.fMyZ = t.Zreco
        mystruct.fMyPhi = t.Muon_PhiReco
        mystruct.fMyTheta = t.Muon_ThetaReco
        mystruct.fMyLength = t.Muon_TrackLength

        tree.Fill()

    ## loop code goes here

## wrap up code goes here ---------------------------------------------------------------------------

print("Num of events that pass cuts:", passCutCount)

newTree.Write()
newTree.Close()

print("TTree written!")
