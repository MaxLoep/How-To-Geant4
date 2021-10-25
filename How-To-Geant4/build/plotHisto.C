// ROOT macro file for plotting histograms 
// 
// run with -b to run in batch-mode otherwise every plot is shown on screen
// root -b plotHist.C

{
  gROOT->Reset();
  gROOT->SetStyle("Plain");

  // Draw histos filled by Geant4 simulation 
  //  
  // Open file Zero
  TFile f("RunData_0.root");

  // Create a canvas and divide it into 2x2 pads
  TCanvas* c1 = new TCanvas("c1", "", 20, 20, 1000, 1000);
  c1->Divide(2,2);
  
  // Draw Ekin histogram in the pad 1
  c1->cd(1);
  TH1D* hist1 = (TH1D*)f.Get("Ekin");
  //Set Labels
  hist1->GetXaxis()->SetTitle("Energy (MeV)");
  hist1->GetYaxis()->SetTitle("Number");
  hist1->GetYaxis()->SetMaxDigits(3);
  hist1->GetYaxis()->SetTitleOffset(1.2);
  //Set logscale
  //gPad->SetLogy(1);

  // // Draw Xpos histogram in the pad 2
  c1->cd(2);
  TH1D* hist2 = (TH1D*)f.Get("Xpos");

  // Draw Ypos histogram in the pad 3
  c1->cd(3);
  TH1D* hist3 = (TH1D*)f.Get("Ypos");

  // Draw time histogram in the pad 4
  c1->cd(4);
  TH1D* hist4 = (TH1D*)f.Get("time");


  //Iterate over other files and add to existing histograms
  for (Int_t i=1; i<=99; i++)
  {
    //Reset filepath in each iteration
    TString filepath;
    filepath = "RunData_";
    filepath += i;
    filepath += ".root";


    //Check if file exists otherwise stop loop
    if (FILE *file = fopen(filepath, "r"))
    {

      cout << filepath << " is found" << endl;
      //Open file filled by Geant4 simulation 
      TFile f_i(filepath);
      //Grab data and add to existing
      c1->cd(1);
      TH1D* hist1_i = (TH1D*)f_i.Get("Ekin");
      hist1->Add(hist1_i);

      c1->cd(2);
      TH1D* hist2_i = (TH1D*)f_i.Get("Xpos");
      hist2->Add(hist2_i);

      c1->cd(3);
      TH1D* hist3_i = (TH1D*)f_i.Get("Ypos");
      hist3->Add(hist3_i);

      c1->cd(4);
      TH1D* hist4_i = (TH1D*)f_i.Get("time");
      hist4->Add(hist4_i);
    }
    else
    { 
      cout << filepath << " is not found" << endl;
      //Draw all in canvas
      c1->cd(1);
      hist1->Draw("H");
      hist1->Draw("HIST");

      c1->cd(2); 
      hist2->Draw("HIST");

      c1->cd(3);
      hist3->Draw("HIST");
      
      c1->cd(4);
      hist4->Draw("HIST");

      //Save as pdf file
      c1->Print("./results/Output.pdf");

      return 0;
    }

  }

  //Draw all in canvas
  c1->cd(1);
  hist1->Draw("H");
  hist1->Draw("HIST");

  c1->cd(2); 
  hist2->Draw("HIST");

  c1->cd(3);
  hist3->Draw("HIST");
  
  c1->cd(4);
  hist4->Draw("HIST");

  //Save as pdf file
  c1->Print("./results/Output.pdf");

//_____________________________________________________________________

  //   // Create a canvas and divide it into 2x2 pads
  // TCanvas* c2 = new TCanvas("c2", "", 20, 20, 1000, 1000);
  // c2->Divide(2,2);
  
  // // Draw Eabs histogram in the pad 1
  // c2->cd(1);
  // TH1D* hist5 = (TH1D*)f.Get("Eabs");
  // hist5->Draw("HIST");
  
  // // Draw Labs histogram in the pad 2
  // c2->cd(2);
  // TH1D* hist6 = (TH1D*)f.Get("Labs");
  // hist6->Draw("HIST");
  
  // // Draw Egap histogram in the pad 3
  // // with logaritmic scale for y
  // TH1D* hist7= (TH1D*)f.Get("Egap");
  // c2->cd(3);
  // gPad->SetLogy(1);
  // hist7->Draw("HIST");
  
  // // Draw Lgap histogram in the pad 4
  // // with logaritmic scale for y
  // c2->cd(4);
  // gPad->SetLogy(1);
  // TH1D* hist8 = (TH1D*)f.Get("Lgap");
  // hist8->Draw("HIST");
}  
