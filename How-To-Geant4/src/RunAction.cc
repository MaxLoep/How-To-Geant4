/*
Create output Root file and its structure here
*/
#include "platform.h"
#include "RunAction.hh"
#include "Run.hh"
#include "DetectorConstruction.hh"
#include "PrimaryGeneratorAction.hh"
#include "Analysis.hh"

#include "G4Run.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

#include "Randomize.hh"
#include <iomanip>


#include "G4RunManager.hh"
#include "G4AccumulableManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include <filesystem>
namespace fs = std::filesystem;

//Set a 'false' to accumulate runs into one output file or set to 'true' to create one output file per run
G4bool SaveEachRunInSeparateFile = true;

// Standard output folder name
std::string folderName = "Output";
// Standardd folder name for the root files
std::string RootFolder = "Root Files";

//
//Functions for custom GUI and macro commands - see DetectorConstruction.hh, DetectorMessenger.cc, DetectorMessenger.hh
//
// void DetectorConstruction::SetOutputFolder(G4String OutFoldName)
void DetectorConstruction::SetOutputFolder(std::string OutFoldName)
{
folderName = OutFoldName;
}


RunAction::RunAction(DetectorConstruction* det, PrimaryGeneratorAction* prim)
	: G4UserRunAction(),
		fDetector(det), fPrimary(prim), fRun(0), //fHistoManager(0),
	fEdep(0.),
	fEdep2(0.)
{

	//Get process ID
	G4long pid = _getpid();
	// G4cout << "\n PID is " << pid << G4endl;

	// create a folder for the files
	// std::string folderName = "Root Files";
	// std::filesystem::create_directory(folderName);
	// std::filesystem::create_directory(folderName + "/" + RootFolder);
	fs::create_directory(folderName);
	fs::create_directory(folderName + "/" + RootFolder);

	// Get analysis manager
	auto analysisManager = G4AnalysisManager::Instance();

	//use this code to accumulate runs into one output file
	if(SaveEachRunInSeparateFile == false)
	{
		//
		//Open output file at the start of the simulation
		//

		// Check if "pid.root" is already existing; if yes, check if "pid+1.root" exists. Output format as root-file is choosen in Analysis.hh
		while(std::ifstream(folderName + "/" + RootFolder + "/" + "ID_" + std::to_string(pid) + ".root"))
		{
			pid++;
		}
		// Set final file name
		std::string fileName = "ID_" + std::to_string(pid) + ".root";

		// Create the file
		analysisManager->OpenFile(folderName + "/" + RootFolder + "/" + fileName);
	}

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
	analysisManager->SetNtupleMerging(true);
		 // Note: merging ntuples is available only with Root output

	// Set default fileName
	// analysisManager->SetFileName(fFileName);

	// Use Ntuples or Histograms
	//
	// Create ntuples
	// CreateNtuple ("name", "title")
	// CreateNtupleDColumn ("name")
	// FinishNtuple ()

	// Creating ntuple for Primitive Scorer - ID 0
	analysisManager->CreateNtuple("PS", "Primitive Scorer");
	analysisManager->CreateNtupleDColumn("TrackLength");           // column id = 0
	analysisManager->FinishNtuple();

	// Create ntuple for Sensitive Detector SD1 - ID 1
	analysisManager->CreateNtuple("SD1", "Sensitive Detector");
	analysisManager->CreateNtupleDColumn("Ekin");    // column id = 0
	analysisManager->CreateNtupleDColumn("Xpos");    // column id = 1
	analysisManager->CreateNtupleDColumn("time");    // column id = 2
	analysisManager->FinishNtuple();

	// Create ntuple for Sensitive Detector SD2 - ID 2
	analysisManager->CreateNtuple("SD2", "Sensitive Detector");
	analysisManager->CreateNtupleDColumn("Ekin");    // column id = 0
	analysisManager->CreateNtupleDColumn("Xpos");    // column id = 1
	analysisManager->CreateNtupleDColumn("time");    // column id = 2
	analysisManager->FinishNtuple();

	// Create ntuple for Sensitive Detector SD3 - ID 3
	analysisManager->CreateNtuple("SD3", "Sensitive Detector");
	analysisManager->CreateNtupleDColumn("Ekin");    // column id = 0
	analysisManager->CreateNtupleDColumn("Xpos");    // column id = 1
	analysisManager->CreateNtupleDColumn("time");    // column id = 2
	analysisManager->FinishNtuple();

	// Create ntuple for Sensitive Detector SD4 - ID 4
	analysisManager->CreateNtuple("SD4", "Sensitive Detector");
	analysisManager->CreateNtupleDColumn("Ekin");    // column id = 0
	analysisManager->CreateNtupleDColumn("Xpos");    // column id = 1
	analysisManager->CreateNtupleDColumn("time");    // column id = 2
	analysisManager->FinishNtuple();

	// Create ntuple for Sensitive Detector SD5 - ID 5
	analysisManager->CreateNtuple("SD5", "Sensitive Detector");
	analysisManager->CreateNtupleDColumn("Ekin");    // column id = 0
	analysisManager->CreateNtupleDColumn("Xpos");    // column id = 1
	analysisManager->CreateNtupleDColumn("time");    // column id = 2
	analysisManager->FinishNtuple();

	// Create ntuple for Sensitive Detector SphereSD - ID 6
	analysisManager->CreateNtuple("SphereSD", "Sensitive Detector");
	analysisManager->CreateNtupleDColumn("Ekin");    // column id = 0
	analysisManager->FinishNtuple();


	// Creating one dimensional histograms
	// CreateH1 ("name", "title", nbins, xmin, xmax, unitName="none", fcnName="none")
	// analysisManager->CreateH1("ID","Particle ID", 100, 0., 100.);             // column id = 0
	// analysisManager->CreateH1("PDG","PDG Code", 100, 0., 10000);              // column id = 1
	// analysisManager->CreateH1("Ekin","Kinetic Energy", 100, 0., 800*MeV);     // column id = 2
	// analysisManager->CreateH1("Xpos","Hit Position X", 10, -1.*cm, 1.*cm);   // column id = 3
	// analysisManager->CreateH1("Ypos","Hit Position Y", 100, -1.*cm, 1.*cm);   // column id = 4
	// analysisManager->CreateH1("time","Time", 100, 0.*ns, 3.*ns);              // column id = 5

	// Creating two dimensional histograms - heatmaps
	// CreateH2 ("name", "title", nxbins, xmin, xmax, nybins, ymin, ymax, xunitName="none", yunitName="none", xfcnName="none", yfcnName="none")
	// analysisManager->CreateH2 ("X-Y-pos", "X-Y-pos heatmap", 100, -3.*cm, 3.*cm, 100, -3.*cm, 3.*cm);

	// Create directories in the root file - commented out in the original B4d example!
	// analysisManager->SetHistoDirectoryName("histograms");
	// analysisManager->SetNtupleDirectoryName("ntuple");

}


RunAction::~RunAction()
{
 //delete fHistoManager;

	//use this code to accumulate runs into one output file
	if(SaveEachRunInSeparateFile == false)
	{
		//close file at end of simulation
		auto analysisManager = G4AnalysisManager::Instance();
		analysisManager->Write();
		analysisManager->CloseFile();
	}
}


G4Run* RunAction::GenerateRun()
{
	fRun = new Run(fDetector);
	return fRun;
}


void RunAction::BeginOfRunAction(const G4Run*)
{
	//Get process ID
	G4long pid = _getpid();

	// create a folder for the files
	// std::string folderName = "Root Files";
	// std::filesystem::create_directory(folderName);
	// std::filesystem::create_directory(folderName + "/" + RootFolder);
	fs::create_directory(folderName);
	fs::create_directory(folderName + "/" + RootFolder);

	//G4RunManager::GetRunManager()->GeometryHasBeenModified();
	//G4RunManager::GetRunManager()->ReinitializeGeometry();

	//use this code to create one file per run
	if(SaveEachRunInSeparateFile == true)
	{
		//
		//Create a new File with each Run
		//
		// Get analysis manager
		auto analysisManager = G4AnalysisManager::Instance();

		// Create an output file which increases in number if the simulation is run again
		//

		// Check if "pid.root" is already existing; if yes, check if "pid+1.root" exists. Output format as root-file is choosen in Analysis.hh
		while(std::ifstream(folderName + "/" + RootFolder + "/" + "ID_" + std::to_string(pid) + ".root"))
		{
			pid++;
		}
		// Set final file name
		std::string fileName = "ID_" + std::to_string(pid) + ".root";

		// Create the file
		// analysisManager->OpenFile("Folder2/" + fileName);
		analysisManager->OpenFile(folderName + "/" + RootFolder + "/" + fileName);
	}

	//
	//change Random Seed
	//
	// inform the runManager to save random number seed
	G4RunManager::GetRunManager()->SetRandomNumberStore(false);

	// Create seed array
	G4long seed[2];

	seed[0] = (G4long) pid;
	seed[1] = (G4long) pid;
	G4Random::setTheSeeds(seed);

	// reset accumulables to their initial values
	G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
	accumulableManager->Reset();

	//From example B4d
	//inform the runManager to save random number seed
	//G4RunManager::GetRunManager()->SetRandomNumberStore(true);

	// // Get analysis manager
	// auto analysisManager = G4AnalysisManager::Instance();

	// show Rndm status
	if (isMaster) G4Random::showEngineStatus();

	// keep run condition
	if (fPrimary) {
		G4ParticleDefinition* particle
			= fPrimary->GetParticleGun()->GetParticleDefinition();
		G4double energy = fPrimary->GetParticleGun()->GetParticleEnergy();
		fRun->SetPrimary(particle, energy);
	}
}


void RunAction::EndOfRunAction(const G4Run* run)
{
	//use this code to create one file per run
	if(SaveEachRunInSeparateFile == true)
	{
		//
		//Close the file at the end of a run
		//
		auto analysisManager = G4AnalysisManager::Instance();
		analysisManager->Write();
		analysisManager->CloseFile();
	}

	G4int nofEvents = run->GetNumberOfEvent();
	if (nofEvents == 0) return;


	// CAN BE REMOVED?
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
		<< " The run has finished! "
		<< G4endl

		// REMOVE
		//  << " Cumulated dose per run, in scoring volume : "
		//  << G4BestUnit(dose,"Dose") << " rms = " << G4BestUnit(rmsDose,"Dose")
		//  << G4endl
		//  << G4BestUnit(edep,"Energy") << " rms = " << G4BestUnit(rms,"Energy")
		//  << G4endl
		//  << "------------------------------------------------------------"
		//  << G4endl
		//  << G4endl
	 ;


	if (isMaster) fRun->EndOfRun();

	/*
	//save histograms
	G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
	if ( analysisManager->IsActive() ) {
		analysisManager->Write();
		analysisManager->CloseFile();
	}
	*/

	// show Rndm status
	if (isMaster) G4Random::showEngineStatus();
}


//CAN BE REMOVED?
//B1 SCORING METHOD
//define function AddEdep(G4double) to sum up the total energy
void RunAction::AddEdep(G4double edep)
{
	fEdep  += edep;
	fEdep2 += edep*edep;
}
