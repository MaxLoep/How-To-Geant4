/*
Create output Root file and its structure here
*/
#include "platform.hh"

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

#include "ConfigStructs.cc"

ConfigStructs::RunActionConf global_run_action_conf;

// Standard output folder name
std::string folderName = "Output";
// Standard folder name for the root files
std::string RootFolder = "Root_Files";

//
//Functions for custom GUI and macro commands - see DetectorConstruction.hh, DetectorMessenger.cc, DetectorMessenger.hh
//
void DetectorConstruction::SetOutputFolder(std::string OutFoldName)
{
folderName = OutFoldName;
}

RunAction::RunAction(DetectorConstruction* det, PrimaryGeneratorAction* prim)
	: G4UserRunAction(),
		fDetector(det), fPrimary(prim), fRun(0)
{

	fs::create_directory(folderName);
	fs::create_directory(folderName + "/" + RootFolder);

	// Create analysis manager
	auto analysisManager = G4AnalysisManager::Instance();

	G4cout << "Using " << analysisManager->GetType() << G4endl;
	//analysisManager->SetVerboseLevel(1);
	analysisManager->SetNtupleMerging(true);
	// Note: merging ntuples is available only with Root output

	// How to: Use Ntuples or Histograms
	//
	// Create ntuples
	// CreateNtuple ("name", "title")
	// CreateNtupleDColumn ("name")
	// FinishNtuple ()

	global_run_action_conf.add_analysis(ConfigStructs::NTuple{
		"PS", "Primitive Scorer",
		{"TrackLength"}
	});
	global_run_action_conf.add_analysis(ConfigStructs::NTuple{
		"SD1", "Sensitive Detector",
		{"Ekin", "Xpos", "Ypos", "time"}
	});
	global_run_action_conf.add_analysis(ConfigStructs::NTuple{
		"SD2", "Sensitive Detector",
		{"Ekin", "Xpos", "Ypos", "time"}
	});
	global_run_action_conf.add_analysis(ConfigStructs::NTuple{
		"SD3", "Sensitive Detector",
		{"Ekin", "Xpos", "time"}
	});
	global_run_action_conf.add_analysis(ConfigStructs::NTuple{
		"SD4", "Sensitive Detector",
		{"Ekin", "Xpos", "time"}
	});
	global_run_action_conf.add_analysis(ConfigStructs::NTuple{
		"SD5", "Sensitive Detector",
		{"Ekin", "Xpos", "time"}
	});
	global_run_action_conf.add_analysis(ConfigStructs::NTuple{
		"N_SphereSD", "Sensitive Detector",
		{"N_Ekin", "N_Theta"}
	});
	global_run_action_conf.add_analysis(ConfigStructs::NTuple{
		"G_SphereSD", "Sensitive Detector",
		{"G_Ekin", "G_Theta"}
	});

	global_run_action_conf.add_analysis(ConfigStructs::Histogram{"N_Phi","N_Phi", 100, -180, 180.});

	for (auto tuple : global_run_action_conf.tuples) {
		analysisManager->CreateNtuple(tuple.name, tuple.title);
		for (auto column : tuple.members) analysisManager->CreateNtupleDColumn(column);
		analysisManager->FinishNtuple();
	}


	/*
	// Creating ntuple for Primitive Scorer - ID 0
	analysisManager->CreateNtuple("PS", "Primitive Scorer");
	analysisManager->CreateNtupleDColumn("TrackLength");           // column id = 0
	analysisManager->FinishNtuple();

	// Create ntuple for Sensitive Detector SD1 - ID 1
	analysisManager->CreateNtuple("SD1", "Sensitive Detector");
	analysisManager->CreateNtupleDColumn("Ekin");    // column id = 0
	analysisManager->CreateNtupleDColumn("Xpos");    // column id = 1
	analysisManager->CreateNtupleDColumn("Ypos");    // column id = 2
	analysisManager->CreateNtupleDColumn("time");    // column id = 3
	analysisManager->FinishNtuple();

	// Create ntuple for Sensitive Detector SD2 - ID 2
	analysisManager->CreateNtuple("SD2", "Sensitive Detector");
	analysisManager->CreateNtupleDColumn("Ekin");    // column id = 0
	analysisManager->CreateNtupleDColumn("Xpos");    // column id = 1
	analysisManager->CreateNtupleDColumn("Ypos");    // column id = 2
	analysisManager->CreateNtupleDColumn("time");    // column id = 3
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
	analysisManager->CreateNtuple("N_SphereSD", "Sensitive Detector");
	analysisManager->CreateNtupleDColumn("N_Ekin");    // column id = 0
	analysisManager->CreateNtupleDColumn("N_Theta");    // column id = 1
	analysisManager->FinishNtuple();

	// Create ntuple for Sensitive Detector SphereSD - ID 7
	analysisManager->CreateNtuple("G_SphereSD", "Sensitive Detector");
	analysisManager->CreateNtupleDColumn("G_Ekin");    // column id = 0
	analysisManager->CreateNtupleDColumn("G_Theta");    // column id = 1
	analysisManager->FinishNtuple();

	// Creating one dimensional histograms
	// CreateH1 ("name", "title", nbins, xmin, xmax, unitName="none", fcnName="none")
	analysisManager->CreateH1("N_Phi","N_Phi", 100, -180, 180.);             // column id = 0
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
	// analysisManager->SetNtupleDirectoryName("ntuple");*/
}

RunAction::~RunAction()
{ }

G4Run* RunAction::GenerateRun()
{
	fRun = new Run(fDetector);
	return fRun;
}

void RunAction::BeginOfRunAction(const G4Run*)
{
	// create a folder for the files
	fs::create_directory(folderName);
	fs::create_directory(folderName + "/" + RootFolder);

	// Get analysis manager
	auto analysisManager = G4AnalysisManager::Instance();

	// get epoch time and system clock nanosecond value that were used as seeds in main() to create file name
	G4long time 	= G4Random::getTheSeeds()[0];
	G4long time_ns 	= G4Random::getTheSeeds()[1];
	// set file name
	std::string fileName = std::to_string(time) + "_" + std::to_string(time_ns) + ".root";

	// Create the file
	// analysisManager->OpenFile("Folder2/" + fileName);
	analysisManager->OpenFile(folderName + "/" + RootFolder + "/" + fileName);

	// inform the runManager to save random number seed
	G4RunManager::GetRunManager()->SetRandomNumberStore(false);

	// reset accumulables to their initial values
	G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
	accumulableManager->Reset();

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
	//Close the file at the end of a run
	auto analysisManager = G4AnalysisManager::Instance();
	analysisManager->Write();
	analysisManager->CloseFile();

	G4int nofEvents = run->GetNumberOfEvent();
	if (nofEvents == 0) return;

	const DetectorConstruction* detectorConstruction
	 = static_cast<const DetectorConstruction*>
		 (G4RunManager::GetRunManager()->GetUserDetectorConstruction());




	//REMOVE ! : WHAT IS THIS GOOD FOR? IS THIS A LEFT OVER OF SOMETHING I COPIED FROM SOMEWHERE?
	// // Run conditions
	// //  note: There is no primary generator action object for "master"
	// //        run manager for multi-threaded mode.
	// const PrimaryGeneratorAction* generatorAction
	//  = static_cast<const PrimaryGeneratorAction*>
	// 	 (G4RunManager::GetRunManager()->GetUserPrimaryGeneratorAction());
	// G4String runCondition;

	// //GPS
	// 	if (generatorAction)
	// {
	// 	const G4GeneralParticleSource* fParticleBeam = generatorAction->GetParticleGun();
	// 	runCondition += fParticleBeam->GetParticleDefinition()->GetParticleName();
	// 	runCondition += " of ";
	// 	G4double particleEnergy = fParticleBeam->GetParticleEnergy();
	// 	runCondition += G4BestUnit(particleEnergy,"Energy");
	// 	//run condition is a string and at this point it contains something like
	// 	// "proton of 100 MeV"
	//  // THIS STRING NEVER GETS PRINTED OR USED SOMEWHERE!
	// }





	// Print End of Run messages
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
		<< G4endl;


	if (isMaster) fRun->EndOfRun();

	// show Rndm status
	if (isMaster) G4Random::showEngineStatus();
}
