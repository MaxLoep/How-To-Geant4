#include "platform.hh"
#include "seed.hh"

#include "G4Types.hh"
// #include "SteppingVerbose.hh"

#include "DetectorConstruction.hh"        //This is where you define your Geometry and Scorers
#include "PhysicsList.hh"                 //This is where you define what physics processes should be used, alternatively you can choose a complete physics list in this file
#include "ActionInitialization.hh"        //This is where you define what the simulation does (...)

#include "G4Version.hh"                   //for checking which Geant4 version is installed
#if G4VERSION_NUMBER>=1070
#include "G4RunManagerFactory.hh"         //Necessary. You need this.
#else
#include "G4RunManager.hh"                //for Geant4 Version < 10.7.0 single threaded
#include "G4MTRunManager.hh"              //for Geant4 Version < 10.7.0 multi threaded
#endif

#include "G4UImanager.hh"                 //Nessesary. You need this.
#include "G4VisExecutive.hh"              //Nessesary. You need this.
#include "G4UIExecutive.hh"               //Nessesary. You need this.
#include "Randomize.hh"                   //To use the randomize-function

#include "G4ScoringManager.hh"            //Necessary to use the built-in scoring mesh funtionality in the macro file

#include "G4ParticleHPManager.hh"
#include "G4HadronicProcessStore.hh"

//Physics Lists
#include "QBBC.hh"                        //works!

#include "FTF_BIC.hh"                     //works!
#include "FTFP_INCLXX.hh"                 //works!
#include "FTFP_INCLXX_HP.hh"              //works!
#include "FTFP_BERT.hh"                   //works!
#include "FTFP_BERT_HP.hh"                //works!
#include "FTFP_BERT_ATL.hh"               //works!
#include "FTFP_BERT_TRV.hh"               //works!
#include "FTFQGSP_BERT.hh"                //works!

#include "QGSP_BIC.hh"                    //works!
#include "QGSP_BIC_HP.hh"                 //works!
#include "QGSP_BIC_AllHP.hh"              //works! but system environmental variable 'G4PARTICLEHPDATA' needs to be set to path to data library e.g. TENDL
#include "QGSP_INCLXX.hh"                 //works!
#include "QGSP_INCLXX_HP.hh"              //works!
#include "QGSP_BERT.hh"                   //works!
#include "QGSP_BERT_HP.hh"                //works!
#include "QGSP_FTFP_BERT.hh"              //works!
#include "Shielding.hh"                   //works!

// long TheSeed = time(NULL);
// long TheSeed = G4Random::getTheSeed();

int main(int argc,char** argv) {

  // Detect interactive mode (if no arguments) and define UI session
  G4UIExecutive* ui = 0;
  if ( argc == 1 ) {
    ui = new G4UIExecutive(argc, argv);
  }

  // choose the Random engine
  // G4Random::setTheEngine(new CLHEP::DualRand);             //works!
  // G4Random::setTheEngine(new CLHEP::HepJamesRandom);       //works!
  G4Random::setTheEngine(new CLHEP::MixMaxRng);            //works! Default and recommended!
  // G4Random::setTheEngine(new CLHEP::MTwistEngine);         //works! Default?
  // G4Random::setTheEngine(new CLHEP::RanecuEngine);         //works! uses Tables. obsolete?
  // G4Random::setTheEngine(new CLHEP::Ranlux64Engine);       //works!
  // G4Random::setTheEngine(new CLHEP::RanluxEngine);         //works!
  // G4Random::setTheEngine(new CLHEP::RanshiEngine);         //works!

  // error C2039: 'the following': is not a member of 'CLHEP' -> can this be deleteted?
  // G4Random::setTheEngine(new CLHEP::DRand48Engine);    -not working!
  // G4Random::setTheEngine(new CLHEP::Hurd160Engine);    -not working!
  // G4Random::setTheEngine(new CLHEP::Hurd288Engine);    -not working!
  // G4Random::setTheEngine(new CLHEP::NonRandomEngine);  -not working!
  // G4Random::setTheEngine(new CLHEP::RandEngine);       -not working!
  // G4Random::setTheEngine(new CLHEP::TripleRand);       -not working!

  // set a initial random seed based on process id
  // G4long pid = _getpid(); //make this a global variable because the process ID is used to name output files in Run.cc, RunAction.cc and SDX.cc
  // G4long seed = pid;
  // G4Random::setTheSeed(seed);
  // srand(time(NULL));
  // int seed=std::rand();
  std::timespec ts;
  std::timespec_get(&ts, TIME_UTC);
  int time=ts.tv_sec;
  int time_ns=ts.tv_nsec;
  G4Random::setTheSeed(time_ns);
  
  G4cout
		<< G4endl
		<< " The random Seed in main() is:" << G4endl
    << time << G4endl
    << time_ns << G4endl
	  << G4endl;
	
	// Create seed array
	G4long seed[2];
  // Use epoch time and nanosecond clock time as seeds
  seed[0] = (G4long) time;
	seed[1] = (G4long) time_ns;
  G4Random::setTheSeeds(seed);

  #if G4VERSION_NUMBER>=1070
    // Construct the default run manager in Geant4 Version > 10.7.0
    // Auto detect if singlethreaded mode or multithreaded mode is used
    auto* runManager = G4RunManagerFactory::CreateRunManager(G4RunManagerType::Default);
  #else
    //for Geant4 Versions < 10.7 use this!
    //check if Geant4 is built with multithread option
    #ifdef G4MULTITHREADED
      G4MTRunManager* runManager = new G4MTRunManager;
    #else
      //my Verbose output class
      G4VSteppingVerbose::SetInstance(new SteppingVerbose);
      G4RunManager* runManager = new G4RunManager;
    #endif
  #endif

 // Activate UI-command base scorer
 G4ScoringManager * scManager = G4ScoringManager::GetScoringManager();
 scManager->SetVerboseLevel(1);

  //set mandatory initialization classes
  DetectorConstruction* det= new DetectorConstruction;
  runManager->SetUserInitialization(det);

  // Physics list -> choose between selfmade Physics List in PhysicsList.cc or choose one of Geant4 default Physics Lists
  // runManager->SetUserInitialization(new PhysicsList);

  // G4VModularPhysicsList* physicsList = new QBBC;
  // G4VModularPhysicsList* physicsList = new FTF_BIC;
  // G4VModularPhysicsList* physicsList = new FTFP_INCLXX;
  // G4VModularPhysicsList* physicsList = new FTFP_INCLXX_HP;

  // G4VModularPhysicsList* physicsList = new FTFP_BERT;
  // G4VModularPhysicsList* physicsList = new FTFP_BERT_HP;
  // G4VModularPhysicsList* physicsList = new FTFP_BERT_ATL;
  // G4VModularPhysicsList* physicsList = new FTFP_BERT_TRV;
  // G4VModularPhysicsList* physicsList = new FTFQGSP_BERT;

  // G4VModularPhysicsList* physicsList = new QGSP_BIC;
  // G4VModularPhysicsList* physicsList = new QGSP_BIC_HP;
  G4VModularPhysicsList* physicsList = new QGSP_BIC_AllHP;  //system environmental variable 'G4PARTICLEHPDATA' needs to be set to path to data library e.g. TENDL
  //G4VModularPhysicsList* physicsList = new QGSP_INCLXX;
  //G4VModularPhysicsList* physicsList = new QGSP_INCLXX_HP;
  // G4VModularPhysicsList* physicsList = new QGSP_BERT;
  // G4VModularPhysicsList* physicsList = new QGSP_BERT_HP;
  // G4VModularPhysicsList* physicsList = new QGSP_FTFP_BERT;

  // G4VModularPhysicsList* physicsList = new Shielding;

  runManager->SetUserInitialization(physicsList);
  G4HadronicProcessStore::Instance()->SetVerbose(0);

  runManager->SetUserInitialization(new ActionInitialization(det));

  // Replaced HP (high-precision) environmental variables with C++ calls
  //
  //SkipMissingIsotopes: It sets to zero the cross section of the isotopes which are not present in the neutron library. If GEANT4 doesn’t find an isotope,
  //then it looks for the natural composition data of that element. Only if the element is not found then the cross section is set to zero.
  //On the contrary, if this variable is not defined, GEANT4 looks then for the neutron data of another isotope close in Z and A, which will
  //have completely different nuclear properties and lead to incorrect results (highly recommended).
  G4ParticleHPManager::GetInstance()->SetSkipMissingIsotopes( true );

  //DoNotAdjustFinalState: If this variable is not defined, a GEANT4 model that attempts to satisfy the energy and momentum conservation in some nuclear
  //reactions, by generating artificial gamma rays. By setting such a variable one avoids the correction and leads to the result obtained with the
  //ENDF-6 libraries. Even though energy and momentum conservation are desirable, the ENDF-6 libraries do not provide the necessary correlations
  //between secondary particles for satisfying them in all cases. On the contrary, ENDF-6 libraries intrinsically violate energy and momentum
  //conservation for several processes and have been built for preserving the overall average quantities such as average energy releases, average number of
  //secondaries. . . (highly recommended).
  G4ParticleHPManager::GetInstance()->SetDoNotAdjustFinalState( true );

  G4ParticleHPManager::GetInstance()->SetUseOnlyPhotoEvaporation( false );

  //Doppler broadening of the resonances, due to target thermal motion, is calculated on-the-fly (from T = 0 K values)
  //Very CPU intense: for those applications that do not need it, it can be switched off by setting the environmental variable
  G4ParticleHPManager::GetInstance()->SetNeglectDoppler( true );

  G4ParticleHPManager::GetInstance()->SetProduceFissionFragments( false );
  //G4ParticleHPManager::GetInstance()->SetUseWendtFissionModel( false );   //not working in Geant4 Versions < 10.7
  G4ParticleHPManager::GetInstance()->SetUseNRESP71Model( false );

  // Initialize visualization
  // G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
  G4VisManager* visManager = new G4VisExecutive("Quiet");
  visManager->Initialize();

  // Get the pointer to the User Interface manager
  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  // set verbosity off
  UImanager->ApplyCommand(G4String("/process/verbose       0"));
  UImanager->ApplyCommand(G4String("/process/em/verbose    0"));
  UImanager->ApplyCommand(G4String("/process/had/verbose   0"));
  UImanager->ApplyCommand(G4String("/process/eLoss/verbose 0"));

  UImanager->ApplyCommand(G4String("/control/verbose  0"));
  UImanager->ApplyCommand(G4String("/run/verbose      0"));
  UImanager->ApplyCommand(G4String("/event/verbose    0"));
  UImanager->ApplyCommand(G4String("/hits/verbose     0"));
  UImanager->ApplyCommand(G4String("/tracking/verbose 0"));
  UImanager->ApplyCommand(G4String("/stepping/verbose 0"));

  // Process macro or start UI session
  // A UI session is started if the program is execute without a macro file. -> if you execute without macro then the macro ../vis.mac will be executed
  if ( ! ui ) {
    // batch mode
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    UImanager->ApplyCommand(command+fileName);
  }
  else {
    // interactive mode
    UImanager->ApplyCommand("/control/execute ../vis.mac");
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
