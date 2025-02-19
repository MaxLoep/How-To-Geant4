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

#include "ConfigStructs.hh"

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

	// G4cout << "Using " << analysisManager->GetType() << G4endl; //Old line of code that doesn't do anything anymore since RunManager got changed from Geant v10 to v11
	//analysisManager->SetVerboseLevel(1);
	analysisManager->SetNtupleMerging(true);
	// Note: merging ntuples is available only with Root output

	// How to: Use Ntuples or Histograms
	//
	// Create ntuples
	// CreateNtuple ("name", "title")
	// CreateNtupleDColumn ("name")
	// FinishNtuple ()

	global_run_action_conf.add_analysis(ConfigStructs::Histogram{"N_Phi","N_Phi", 100, -180, 180.});
	extern ConfigStructs::GlobalConf global_conf;


	for (auto tuple : global_conf.ra_conf.tuples) {
		analysisManager->CreateNtuple(tuple.name, tuple.title);
		for (auto column : tuple.members) analysisManager->CreateNtupleDColumn(column);
		analysisManager->FinishNtuple();
	}

	for (auto hist : global_conf.ra_conf.histograms) {
		analysisManager->CreateH1(hist.name, hist.title, hist.nbins, hist.xmin, hist.xmax, hist.unit_name, hist.fcn_name);
	}
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
