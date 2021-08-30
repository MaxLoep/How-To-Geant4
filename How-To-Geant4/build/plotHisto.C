// ROOT macro file for plotting example B4 histograms 
// 
// Can be run from ROOT session:
// root[0] .x plotHisto.C

{
  gROOT->Reset();
  gROOT->SetStyle("Plain");
  
  // Draw histos filled by Geant4 simulation 
  //   

  // Open file filled by Geant4 simulation 
  TFile f("B4.root");

  // Create a canvas and divide it into 2x2 pads
  TCanvas* c1 = new TCanvas("c1", "", 20, 20, 1000, 1000);
  c1->Divide(2,2);
  
  // Draw Eabs histogram in the pad 1
  c1->cd(1);
  TH1D* hist1 = (TH1D*)f.Get("Eabs");
  hist1->Draw("HIST");
  
  // Draw Labs histogram in the pad 2
  c1->cd(2);
  TH1D* hist2 = (TH1D*)f.Get("Labs");
  hist2->Draw("HIST");
  
  // Draw Egap histogram in the pad 3
  // with logaritmic scale for y
  TH1D* hist3 = (TH1D*)f.Get("Egap");
  c1->cd(3);
  //gPad->SetLogy(1);
  hist3->Draw("HIST");
  
  // Draw Lgap histogram in the pad 4
  // with logaritmic scale for y
  c1->cd(4);
  //gPad->SetLogy(1);
  TH1D* hist4 = (TH1D*)f.Get("Lgap");
  hist4->Draw("HIST");

//_____________________________________________________________________

    // Create a canvas and divide it into 2x2 pads
  TCanvas* c2 = new TCanvas("c2", "", 20, 20, 1000, 1000);
  c2->Divide(2,2);
  
  // Draw Eabs histogram in the pad 1
  c2->cd(1);
  TH1D* hist5 = (TH1D*)f.Get("Eabs");
  hist5->Draw("HIST");
  
  // Draw Labs histogram in the pad 2
  c2->cd(2);
  TH1D* hist6 = (TH1D*)f.Get("Labs");
  hist6->Draw("HIST");
  
  // Draw Egap histogram in the pad 3
  // with logaritmic scale for y
  TH1D* hist7= (TH1D*)f.Get("Egap");
  c2->cd(3);
  gPad->SetLogy(1);
  hist7->Draw("HIST");
  
  // Draw Lgap histogram in the pad 4
  // with logaritmic scale for y
  c2->cd(4);
  gPad->SetLogy(1);
  TH1D* hist8 = (TH1D*)f.Get("Lgap");
  hist8->Draw("HIST");
}  
