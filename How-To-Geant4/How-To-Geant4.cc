#include "B1DetectorConstruction.hh"      //This is where you define your Geometry
#include "PhysicsList.hh"                 //This is where you define waht physics processes should be used
#include "B1ActionInitialization.hh"      //
#include "G4RunManagerFactory.hh"         //Nessesary. You need this.
#include "G4UImanager.hh"                 //Nessesary. You need this.
#include "G4VisExecutive.hh"              //Nessesary. You need this.
#include "G4UIExecutive.hh"               //Nessesary. You need this.
#include "Randomize.hh"

#include "G4ParticleHPManager.hh"


//Old code Snippet from the original B1 example. Can be removed i guess...
//#include "QBBC.hh"
// Physics list
// G4VModularPhysicsList* physicsList = new QBBC;
// G4VModularPhysicsList* physicsList = new PhysicsList;
// physicsList->SetVerboseLevel(1);
// runManager->SetUserInitialization(physicsList);

// The main function
int main(int argc,char** argv)
{
  // Detect interactive mode (if no arguments) and define UI session
  //
  G4UIExecutive* ui = 0;
  if ( argc == 1 ) {
    ui = new G4UIExecutive(argc, argv);
  }

  // Optionally: choose a different Random engine...
  // G4Random::setTheEngine(new CLHEP::MTwistEngine);
  
  // Construct the default run manager
  // Auto detect if singlethreaded mode or multithreaded mode is used
  auto* runManager =
    G4RunManagerFactory::CreateRunManager(G4RunManagerType::Default);

  // Set mandatory initialization classes
  //
  // Detector construction
  runManager->SetUserInitialization(new B1DetectorConstruction());

  // Physics list
  PhysicsList* phys = new PhysicsList;
  phys->SetVerboseLevel(0);                 //<- This does nothing. Why?
  runManager->SetUserInitialization(phys);
    
  // User action initialization
  runManager->SetUserInitialization(new B1ActionInitialization());
  
  // Replaced HP (high-precision) environmental variables with C++ calls
 G4ParticleHPManager::GetInstance()->SetSkipMissingIsotopes( false );
 G4ParticleHPManager::GetInstance()->SetDoNotAdjustFinalState( false );
 G4ParticleHPManager::GetInstance()->SetUseOnlyPhotoEvaporation( false );
 G4ParticleHPManager::GetInstance()->SetNeglectDoppler( false );
 G4ParticleHPManager::GetInstance()->SetProduceFissionFragments( false );
 G4ParticleHPManager::GetInstance()->SetUseWendtFissionModel( false );
 G4ParticleHPManager::GetInstance()->SetUseNRESP71Model( false );

//  G4ParticleHPManager::GetInstance()->SetSkipMissingIsotopes( true );
//  G4ParticleHPManager::GetInstance()->SetDoNotAdjustFinalState( true );
//  G4ParticleHPManager::GetInstance()->SetUseOnlyPhotoEvaporation( true );
//  G4ParticleHPManager::GetInstance()->SetNeglectDoppler( true );
//  G4ParticleHPManager::GetInstance()->SetProduceFissionFragments( true );
//  G4ParticleHPManager::GetInstance()->SetUseWendtFissionModel( true );
//  G4ParticleHPManager::GetInstance()->SetUseNRESP71Model( true );

  // Initialize visualization
  //
  G4VisManager* visManager = new G4VisExecutive;
  // G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
  // G4VisManager* visManager = new G4VisExecutive("Quiet");
  visManager->Initialize();

  // Get the pointer to the User Interface manager
  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  // Process macro or start UI session
  // A UI session is started if the program is execute without a macro file.
  if ( ! ui ) { 
    // batch mode
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    UImanager->ApplyCommand(command+fileName);
  }
  else { 
    // interactive mode
    UImanager->ApplyCommand("/control/execute vis.mac");
    ui->SessionStart();
    delete ui;
  }

  // Job termination
  // Free the store: user actions, physics_list and detector_description are
  // owned and deleted by the run manager, so they should not be deleted 
  // in the main() program !
  
  delete visManager;
  delete runManager;
}
