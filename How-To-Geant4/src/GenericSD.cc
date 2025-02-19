#include "platform.hh"
#include "SensitiveDetector.hh"
#include "Analysis.hh"

#include "G4VTouchable.hh"
#include "G4Step.hh"
#include "G4ios.hh"
#include "G4SystemOfUnits.hh"
#include "G4VProcess.hh"
#include "G4ParticleTypes.hh"
#include "G4IonTable.hh"

#include <filesystem>
namespace fs = std::filesystem;

// get folderName from where it is defined (RunAction.cc) - the really dirty way
extern std::string folderName;
// get folderName from where it is defined (Run.cc) - the really dirty way
extern std::string ListFolder;

//Get process Main ID
std::thread::id main_id = std::this_thread::get_id();

GenericSD::GenericSD(ConfigStructs::SDConfig conf) : G4VSensitiveDetector(conf.name) {
	this->name = conf.name;
	this->log_properties = conf.log_properties;
	// initialize oldTrackID with 0, which will never be a trackID - primaries have TrackID=1, secondaries have TrackID>1
	this->oldTrackId = 0;
	G4cout << "sensitive detector " << this->name << " has been created" << G4endl;
}


GenericSD::~GenericSD() {
	// Get thread process ID
	this->thread_id = std::this_thread::get_id();

	// If returned to main thread (after closing all threads created by multithreading) print secondary counter
	if (main_id == this->thread_id)
	{
		// Get an iterator pointing to the first element in the map
		std::map<std::string, int>::iterator it_console = this->particle_map.begin();
		std::map<std::string, int>::iterator it_file = this->particle_map.begin();

		// Iterate through the map and print the elements in console
		G4cout << "PARTICLE COUNT OF " << this->name << G4endl;
		while (it_console != this->particle_map.end())
		{
			G4cout << "  " << std::setw(15) << it_console->first << ": " << std::setw(10) << it_console->second << G4endl;
			++it_console;
		}

		//List of generated particles to file

		// create a folder for the files if it does not exists
		fs::create_directory(folderName);
		fs::create_directory(folderName + "/" + ListFolder);

		// REMOVE / REWORK because filname is now based on time not on process-ID
		//Get main process ID
		G4long pid = _getpid();

		// Check if "pid_ListOfGeneratedParticles in SDX.txt" is already existing; if yes, check if "pid+1_ListOfGeneratedParticles in SDX.txt" exists.
		while(std::ifstream(folderName + "/" + ListFolder + "/" + std::to_string(pid) + this->name + ".txt"))
		{
			pid++;
		}
		// Set final file name
		std::string fileName = std::to_string(pid) + this->name + ".txt";

		// flush output to file
		std::ofstream outFile(folderName + "/" + ListFolder + "/" + fileName);

		// Iterate through the map and print the elements in file
		outFile <<  this->name << G4endl;
		while (it_file != this->particle_map.end())
		{
			outFile << "  " << std::setw(15) << it_file->first << ": " << std::setw(10) << it_file->second << G4endl;
			++it_file;
		}
	}
}


void GenericSD::Initialize(G4HCofThisEvent* /*hce*/)
{}

G4bool GenericSD::ProcessHits(G4Step* step, G4TouchableHistory* /*history*/)
{
	// Get the Current trackID and the name of the particle
	const G4Track* track = step->GetTrack();
	currentTrackId = track->GetTrackID();
	G4String name   = track->GetDefinition()->GetParticleName();
	// G4double HalfLife = track->GetDefinition()->GetPDGLifeTime() / 1.443; // mean life time divided by 1.443 equals half-life
	// G4cout << "-------------------------------------------------------------------------" << G4endl;
	// G4cout << "-------------------------------------------------------------------------" << G4endl;

	// NOT WORKING!
	// if (currentTrackId == 0)
	// {
	// const G4ParticleDefinition* testparticle = track->GetParticleDefinition();
	// }

	// if particle is a secondary (trackID>1) and we have not counted it yet add it to the map
	if ( (currentTrackId > 0) && (currentTrackId != this->oldTrackId) )
	{
		this->particle_map[name] = this->particle_map[name]+1;
	}

	// overwrite oldTrackID with currentTrackID
	this->oldTrackId = currentTrackId;


	// keep only outgoing particle
	const G4ParticleDefinition* particle = track->GetParticleDefinition();
	// const G4ParticleDefinition* particle = G4IonTable::FindIon(7,14);
	G4String current_name   = track->GetDefinition()->GetParticleName();

	// code PDG:
	// G4int pdgCode = track->GetDefinition()->GetPDGEncoding();

	// Remember preStepPoint:
	G4StepPoint* preStepPoint = step->GetPreStepPoint();

	// Ekin:
	G4double Ekin =  preStepPoint->GetKineticEnergy();

	// Obtain local coordinates:
	const G4VTouchable* touchable = preStepPoint->GetTouchable();
	G4ThreeVector globalPosition = preStepPoint->GetPosition();
	G4ThreeVector localPosition
		= touchable->GetHistory()->GetTopTransform().TransformPoint(globalPosition);
	// // Example for obtaining the local direction:
	// G4ThreeVector globalDirection = preStepPoint->GetMomentumDirection();
	// G4ThreeVector localDirection
	//   = touchable->GetHistory()->GetTopTransform().TransformAxis(localDirection);

	// Time
	G4double time = preStepPoint->GetGlobalTime();

	// Get Analysis Manager
	G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

	G4cout << this->name << " processing a hit from " << current_name << G4endl;

	for (auto det_info : this->log_properties) {
		G4cout << this->name << " is looking for " << det_info.particle_kind << G4endl;
		if (current_name == det_info.particle_kind) {
			for (auto [property, col] : det_info.ntuple_spec) {
				G4cout << this->name << " is writing to col " << col  << " of tuple " << det_info.ntuple << G4endl;
				switch (property){
					case ConfigStructs::ParticleProperty::Ekin:
						analysisManager->FillNtupleDColumn(det_info.ntuple, col, Ekin/MeV);
						break;
					case ConfigStructs::ParticleProperty::local_pos_x:
						analysisManager->FillNtupleDColumn(det_info.ntuple, col, localPosition.x()/cm);
						break;
					case ConfigStructs::ParticleProperty::local_pos_y:
						analysisManager->FillNtupleDColumn(det_info.ntuple, col, localPosition.y()/cm);
						break;
					case ConfigStructs::ParticleProperty::local_pos_z:
						analysisManager->FillNtupleDColumn(det_info.ntuple, col, localPosition.y()/cm);
						break;

					case ConfigStructs::ParticleProperty::time:
						analysisManager->FillNtupleDColumn(det_info.ntuple, col, time/ns);
						break;
				}
			}
			analysisManager->AddNtupleRow(det_info.ntuple);
		}
	}

	return true;
}


void GenericSD::EndOfEvent(G4HCofThisEvent* /*hce*/)
{
	// reset oldTrackID for the next event
	oldTrackId = 0;
}
