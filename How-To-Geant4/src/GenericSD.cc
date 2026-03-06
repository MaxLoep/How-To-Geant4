#include "platform.hh"
#include "config.hh"
#include "SensitiveDetector.hh"
#include "Analysis.hh"

#include "G4VTouchable.hh"
#include "G4Step.hh"
#include "G4ios.hh"
#include "G4SystemOfUnits.hh"
#include "G4VProcess.hh"
#include "G4ParticleTypes.hh"
#include "G4IonTable.hh"

#include "ConfigStructs.hh"
extern ConfigStructs::GlobalConf global_conf;

#include <filesystem>
namespace fs = std::filesystem;

// get folderName from where it is defined (RunAction.cc) - the really dirty way
extern std::string folderName;
extern std::string run_name;
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
	auto _ = global_conf.lock();
	for (auto [particle, particle_data] : this->particle_map) {
		global_conf.sd_counts[this->name][particle] = global_conf.sd_counts[this->name][particle] + particle_data;
	}
	if (main_id == this->thread_id) {
		G4cout << "PARTICLE COUNT OF " << this->name << G4endl;
		for (auto [particle, count] : global_conf.sd_counts[this->name]) {
			G4cout << "  " << std::setw(15) << particle << ": " << std::setw(10) << count.fCount << G4endl;
		}

		//List of generated particles to file

		// create a folder for the files if it does not exists
		fs::create_directory(folderName);
		fs::create_directory(folderName + "/" + ListFolder);

		// name generation via process-ID does not work on cluster!
		// G4long pid = _getpid();

		// // Check if "pid_ListOfGeneratedParticles in SDX.txt" is already existing; if yes, check if "pid+1_ListOfGeneratedParticles in SDX.txt" exists.
		// while(std::ifstream(folderName + "/" + ListFolder + "/" + run_name + "_" + std::to_string(pid) + "_" + this->name + ".txt")) {
		// 	pid++;
		// }
		// // Set final file name
		// std::string fileName = run_name + "_" + std::to_string(pid) + "_" + this->name + ".txt";

		// get epoch time and system clock nanosecond value that were used as seeds in main() to create file name
		G4long time 	= G4Random::getTheSeeds()[0];
		G4long time_ns 	= G4Random::getTheSeeds()[1];
		// set file name
		std::string fileName = run_name + "_" + std::to_string(time) + "_" + std::to_string(time_ns) + "_" + this->name + ".txt";

		// flush output to file
		std::ofstream outFile(folderName + "/" + ListFolder + "/" + fileName);

		pmap_writer::write_pmap_to_stream(global_conf.sd_counts[this->name], outFile);

		// Iterate through the map and print the elements in file
		// outFile <<  this->name << G4endl;
		// for (auto [particle, count] : global_conf.sd_counts[this->name]) {
			// outFile << "  " << std::setw(15) << particle << ": " << std::setw(10) << count.fCount << G4endl;
		// }
	}
}


void GenericSD::Initialize(G4HCofThisEvent* /*hce*/) {}

G4bool GenericSD::ProcessHits(G4Step* step, G4TouchableHistory* /*history*/) {
	// Get the Current trackID and the name of the particle
	const G4Track* track = step->GetTrack();
	currentTrackId = track->GetTrackID();
	G4String particle_name   = track->GetDefinition()->GetParticleName();
	// G4double HalfLife = track->GetDefinition()->GetPDGLifeTime() / 1.443; // mean life time divided by 1.443 equals half-life
	// G4cout << "-------------------------------------------------------------------------" << G4endl;
	// G4cout << "-------------------------------------------------------------------------" << G4endl;

	// NOT WORKING!
	// if (currentTrackId == 0)
	// {
	// const G4ParticleDefinition* testparticle = track->GetParticleDefinition();
	// }

	const G4ParticleDefinition* particle = track->GetParticleDefinition();
	G4double life_time = particle->GetPDGLifeTime();
	// if particle is a secondary (trackID>1) and we have not counted it yet add it to the map
	if ( (currentTrackId > 1) && (currentTrackId != this->oldTrackId) ) {
		//G4cout << this->name <<  " detected: " << particle_name << this->particle_map[particle_name] + 1  << "times" << G4endl;
		if (this->particle_map.count(particle_name)) {
			this->particle_map[particle_name] = this->particle_map[particle_name] + 1;
		} else {
			this->particle_map[particle_name] = ParticleData(1, life_time);
			std::cout << this->particle_map[particle_name].fTmean << std::endl;
		}
	}

	// overwrite oldTrackID with currentTrackID
	this->oldTrackId = currentTrackId;


	// keep only outgoing particle
	// const G4ParticleDefinition* particle = G4IonTable::FindIon(7,14);

	// code PDG:
	// G4int pdgCode = track->GetDefinition()->GetPDGEncoding();

	// Remember preStepPoint:
	G4StepPoint* preStepPoint = step->GetPreStepPoint();

	// Ekin:
	G4double Ekin =  preStepPoint->GetKineticEnergy();

	// Obtain local coordinates:
	const G4VTouchable* touchable = preStepPoint->GetTouchable();
	G4ThreeVector globalPosition = preStepPoint->GetPosition();
	G4ThreeVector localPosition = touchable->GetHistory()->GetTopTransform().TransformPoint(globalPosition);
	// // Example for obtaining the local direction:
	// G4ThreeVector globalDirection = preStepPoint->GetMomentumDirection();
	// G4ThreeVector localDirection
	//   = touchable->GetHistory()->GetTopTransform().TransformAxis(localDirection);

	// Time
	G4double time = preStepPoint->GetGlobalTime();

	// Get Analysis Manager
	G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

	//G4cout << this->name << " processing a hit from " << particle_name << G4endl;

	#ifdef MOREDEBUG
	std::cout << "GENERIC SD: " << this->name << " got a hit from: " << particle_name << std::endl;
	#endif

	for (auto det_info : this->log_properties) {
		//G4cout << this->name << " is looking for " << det_info.particle_kind << G4endl;
		if (
			(particle_name == det_info.particle_kind)
			or (det_info.particle_kind == "all")
			or (det_info.particle_kind == "primary" and currentTrackId == 1)
		) {
			for (auto [property, col] : det_info.ntuple_spec) {
				//G4cout << this->name << " is writing to col " << col  << " of tuple " << det_info.ntuple << G4endl;
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
					case ConfigStructs::ParticleProperty::theta:
						analysisManager->FillNtupleDColumn(det_info.ntuple, col, localPosition.theta());
						break;
					case ConfigStructs::ParticleProperty::phi:
						analysisManager->FillNtupleDColumn(det_info.ntuple, col, localPosition.phi());
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


void GenericSD::EndOfEvent(G4HCofThisEvent* /*hce*/) {
	// reset oldTrackID for the next event
	oldTrackId = 0;
}
