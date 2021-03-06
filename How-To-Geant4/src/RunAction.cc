/*
Understand what this does and comment it
*/

#include "RunAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "DetectorConstruction.hh"
// #include "B1Run.hh"                //Relict form old version of example B1?

#include "G4RunManager.hh"
#include "G4Run.hh"
#include "G4AccumulableManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include <filesystem>

#include "Analysis.hh"              //For Output file format


RunAction::RunAction()
: G4UserRunAction(),
  fEdep(0.),
  fEdep2(0.)
{ 
  // Get analysis manager
  auto analysisManager = G4AnalysisManager::Instance();

  // Create an output file which increases in number if the simulation is run again
  //
  // File number count starts with 0
  G4int filenumber = 0;

  // File name variable
  std::string fileName = "RunData";

  // Check if "RunData_x.root" already existing; if yes, check if "RunData_x+1.root" exists. Output format as root-file is choosen in Analysis.hh 
  while(std::ifstream(fileName + "_" + std::to_string(filenumber) + ".root"))
  {
    filenumber++;
  }

  // Set final file name 
  fileName = "RunData_" + std::to_string(filenumber);

  // Create the file
  analysisManager->OpenFile(fileName);

  // Creating histograms
  analysisManager->CreateH1("ID","Particle ID", 100, 0., 100.);             // column id = 0
  analysisManager->CreateH1("PDG","PDG Code", 100, 0., 10000);              // column id = 1
  analysisManager->CreateH1("Ekin","Kinetic Energy", 100, 0., 800*MeV);     // column id = 2
  analysisManager->CreateH1("Xpos","Hit Position X", 100, -1.*cm, 1.*cm);   // column id = 3
  analysisManager->CreateH1("Ypos","Hit Position Y", 100, -1.*cm, 1.*cm);   // column id = 4
  analysisManager->CreateH1("time","Time", 100, 0.*ns, 3.*ns);              // column id = 5

  // add new units for dose
  // 
  const G4double milligray = 1.e-3*gray;
  const G4double microgray = 1.e-6*gray;
  const G4double nanogray  = 1.e-9*gray;  
  const G4double picogray  = 1.e-12*gray;

  //Introducing new Units:
  //new G4UnitDefinition("name", "symbol" , "category", value);
  // Already available catogiers are: Length, Surface, Volume, Angle, Time, Frequency, Electric Charge, Energy,
  // Mass, Volumic Mass, Power, Force, Pressure, Electric Current, Electric Potential, Magnetic Flux, Magnetic Flux Density,
  // Temperature, Amount of Substance, Activity, Dose
  new G4UnitDefinition("milligray", "milliGy" , "Dose", milligray);
  new G4UnitDefinition("microgray", "microGy" , "Dose", microgray);
  new G4UnitDefinition("nanogray" , "nanoGy"  , "Dose", nanogray);
  new G4UnitDefinition("picogray" , "picoGy"  , "Dose", picogray); 


  //B1 SCORING METHOD
  // Register accumulable to the accumulable manager
  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  accumulableManager->RegisterAccumulable(fEdep);
  accumulableManager->RegisterAccumulable(fEdep2); 


  //SENSITIVE DETECTOR
  // From example B4d and extended/physicslist/genericPL

  // Create analysis manager
  // The choice of analysis technology is done via selectin of a namespace in Analysis.hh
  //auto analysisManager = G4AnalysisManager::Instance();
  G4cout << "Using " << analysisManager->GetType() << G4endl;
  //analysisManager->SetVerboseLevel(1);
  //analysisManager->SetNtupleMerging(true);
     // Note: merging ntuples is available only with Root output


  // From example extended/physicslist/genericPL
  // Set default fileName
  // analysisManager->SetFileName(fFileName);

  // Use Ntuples or Histograms
  //
  // Create ntuple
  // analysisManager->CreateNtuple("Screen", "Screen hits");
  // analysisManager->CreateNtupleIColumn("ID");      // column id = 0
  // analysisManager->CreateNtupleIColumn("PDG");     // column id = 1
  // analysisManager->CreateNtupleDColumn("Ekin");    // column id = 2
  // analysisManager->CreateNtupleDColumn("Xpos");    // column id = 3
  // analysisManager->CreateNtupleDColumn("Ypos");    // column id = 4
  // analysisManager->CreateNtupleDColumn("time");    // column id = 5
  // analysisManager->FinishNtuple();


  // // Creating histograms
  // analysisManager->CreateH1("ID","Particle ID", 100, 0., 100.);             // column id = 0
  // analysisManager->CreateH1("PDG","PDG Code", 100, 0., 10000);              // column id = 1
  // analysisManager->CreateH1("Ekin","Kinetic Energy", 100, 0., 800*MeV);     // column id = 2
  // analysisManager->CreateH1("Xpos","Hit Position X", 10, -1.*cm, 1.*cm);   // column id = 3
  // analysisManager->CreateH1("Ypos","Hit Position Y", 100, -1.*cm, 1.*cm);   // column id = 4
  // analysisManager->CreateH1("time","Time", 100, 0.*ns, 3.*ns);              // column id = 5

  //PRIMITIVE SCORERS
  //From example B4d
  // Create analysis manager
  // The choice of analysis technology is done via selectin of a namespace in Analysis.hh
  //auto analysisManager = G4AnalysisManager::Instance();
  //G4cout << "Using " << analysisManager->GetType() << G4endl;
  //analysisManager->SetVerboseLevel(1);
  //analysisManager->SetNtupleMerging(true);
  // Note: merging ntuples is available only with Root output


  // Create directories in the root file - commented out in the original B4d example!
  // analysisManager->SetHistoDirectoryName("histograms");
  // analysisManager->SetNtupleDirectoryName("ntuple");

  // Book histograms, ntuple
  //
  
  // // Creating histograms
  // analysisManager->CreateH1("Eabs","Edep in absorber", 100, 0., 800*MeV);
  // analysisManager->CreateH1("Egap","Edep in gap", 100, 0., 800*MeV);
  // //analysisManager->CreateH1("Egap","Edep in Surface", 100, 0., 800*MeV);
  // analysisManager->CreateH1("Labs","trackL in absorber", 100, 0., 1*m);
  // analysisManager->CreateH1("Lgap","trackL in gap", 100, 0., 1*m);
  // //analysisManager->CreateH1("Surface","trackL in Surface", 100, 0., 100000);

  // // Creating ntuple
  // //
  // analysisManager->CreateNtuple("B4", "Edep and TrackL");
  // analysisManager->CreateNtupleDColumn("Eabs");
  // analysisManager->CreateNtupleDColumn("Egap");
  // analysisManager->CreateNtupleDColumn("Labs");
  // analysisManager->CreateNtupleDColumn("Lgap");
  // //analysisManager->CreateNtupleDColumn("Surface1");
  // //analysisManager->CreateNtupleDColumn("Surface2");
  // analysisManager->FinishNtuple();

}


RunAction::~RunAction()
{
  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->Write();
  analysisManager->CloseFile();
}


void RunAction::BeginOfRunAction(const G4Run*)
{ 
  // inform the runManager to save random number seed
  G4RunManager::GetRunManager()->SetRandomNumberStore(false);

  // File number count starts with 0
  G4int filenumber = 0;

  // File name variable
  std::string fileName = "RunData";

  // File number count starts with 0
  G4int seednumber = 0;

  long seed[2];
  seed[0] = (long) seednumber;
  seed[1] = (long) seednumber;

    // Check if "RunData_x.root" already existing; if yes, check if "RunData_x+1.root" exists. Output format as root-file is choosen in Analysis.hh 
  while(std::ifstream(fileName + "_" + std::to_string(filenumber) + ".root"))
  {
    filenumber++;
    seednumber++;
    seed[0] = (long) seednumber;
    seed[1] = (long) seednumber;
  }

  G4Random::setTheSeeds(seed);

  // reset accumulables to their initial values
  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  accumulableManager->Reset();

  //From example B4d
  //inform the runManager to save random number seed
  //G4RunManager::GetRunManager()->SetRandomNumberStore(true);
  
  // // Get analysis manager
  // auto analysisManager = G4AnalysisManager::Instance();

  // // Open an output file
  // //
  // G4String fileName = "RunData";
  // analysisManager->OpenFile(fileName);

}


void RunAction::EndOfRunAction(const G4Run* run)
{
  G4int nofEvents = run->GetNumberOfEvent();
  if (nofEvents == 0) return;

  //B1 SCORING METHOD
  // Merge accumulables 
  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  accumulableManager->Merge();

  // Compute dose = total energy deposit in a run and its variance
  G4double edep  = fEdep.GetValue();
  G4double edep2 = fEdep2.GetValue();
  
  G4double rms = edep2 - edep*edep/nofEvents;
  if (rms > 0.) rms = std::sqrt(rms); else rms = 0.;  

  const DetectorConstruction* detectorConstruction
   = static_cast<const DetectorConstruction*>
     (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
  //G4double mass = detectorConstruction->GetScoringVolume()->GetMass();
  //G4double dose = edep/mass;
  //G4double rmsDose = rms/mass;

  // Run conditions
  //  note: There is no primary generator action object for "master"
  //        run manager for multi-threaded mode.
  const PrimaryGeneratorAction* generatorAction
   = static_cast<const PrimaryGeneratorAction*>
     (G4RunManager::GetRunManager()->GetUserPrimaryGeneratorAction());
  G4String runCondition;

  //PARTICLE GUN
  // if (generatorAction)
  // {
  //   const G4ParticleGun* particleGun = generatorAction->GetParticleGun();
  //   runCondition += particleGun->GetParticleDefinition()->GetParticleName();
  //   runCondition += " of ";
  //   G4double particleEnergy = particleGun->GetParticleEnergy();
  //   runCondition += G4BestUnit(particleEnergy,"Energy");
  // }


  //GPS
    if (generatorAction)
  {
    const G4GeneralParticleSource* fParticleBeam = generatorAction->GetParticleGun();
    runCondition += fParticleBeam->GetParticleDefinition()->GetParticleName();
    runCondition += " of ";
    G4double particleEnergy = fParticleBeam->GetParticleEnergy();
    runCondition += G4BestUnit(particleEnergy,"Energy");
    //run condition is a string and at this point it contains something like
    // "proton of 100 MeV"
  }
        
  // Print End of Run messages
  //  from example B1
  if (IsMaster()) {
    G4cout
     << G4endl
     << "--------------------End of Global Run-----------------------";
  }
  else {
    G4cout
     << G4endl
     << "--------------------End of Local Run------------------------";
  }
  
  G4cout
     << G4endl
     << " The run consists of " << nofEvents << " "<< runCondition
     << G4endl
    //  << " Cumulated dose per run, in scoring volume : " 
    //  << G4BestUnit(dose,"Dose") << " rms = " << G4BestUnit(rmsDose,"Dose")
    //  << G4endl
    //  << G4BestUnit(edep,"Energy") << " rms = " << G4BestUnit(rms,"Energy")
    //  << G4endl
    //  << "------------------------------------------------------------"
    //  << G4endl
    //  << G4endl
    ;

  //from example B4d
  // print histogram statistics
  //
  //auto analysisManager = G4AnalysisManager::Instance();
  // if ( analysisManager->GetH1(1) ) {
  //   G4cout << G4endl << " ----> print histograms statistic ";
  //   if(isMaster) {
  //     G4cout << "for the entire run " << G4endl << G4endl; 
  //   }
  //   else {
  //     G4cout << "for the local thread " << G4endl << G4endl; 
  //   }
    
  //   G4cout << " EAbs : mean = " 
  //      << G4BestUnit(analysisManager->GetH1(0)->mean(), "Energy") 
  //      << " rms = " 
  //      << G4BestUnit(analysisManager->GetH1(0)->rms(),  "Energy") << G4endl;
    
  //   G4cout << " EGap : mean = " 
  //      << G4BestUnit(analysisManager->GetH1(1)->mean(), "Energy") 
  //      << " rms = " 
  //      << G4BestUnit(analysisManager->GetH1(1)->rms(),  "Energy") << G4endl;
    
  //   G4cout << " LAbs : mean = " 
  //     << G4BestUnit(analysisManager->GetH1(2)->mean(), "Length") 
  //     << " rms = " 
  //     << G4BestUnit(analysisManager->GetH1(2)->rms(),  "Length") << G4endl;

  //   G4cout << " LGap : mean = " 
  //     << G4BestUnit(analysisManager->GetH1(3)->mean(), "Length") 
  //     << " rms = " 
  //     << G4BestUnit(analysisManager->GetH1(3)->rms(),  "Length") << G4endl;
  // }

}

//B1 SCORING METHOD
//define function AddEdep(G4double) to sum up the total energy
void RunAction::AddEdep(G4double edep)
{
  fEdep  += edep;
  fEdep2 += edep*edep;
}



