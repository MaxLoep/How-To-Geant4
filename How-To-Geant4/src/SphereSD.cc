#include "platform.h"
#include "SensitiveDetector.hh"
#include "Analysis.hh"

#include "G4VTouchable.hh"
#include "G4Step.hh"
#include "G4ios.hh"
#include "G4SystemOfUnits.hh"
#include "G4VProcess.hh"
#include "G4ParticleTypes.hh"

#include <filesystem>
namespace fs = std::filesystem;

// get folderName from where it is defined (RunAction.cc) - the really dirty way
extern std::string folderName;
// get folderName from where it is defined (Run.cc) - the really dirty way
extern std::string ListFolder;

// Create a map of strings to integers
std::map<std::string, int> SphereSDmap;

// initialize particleCounter outside of anything so this will work with multithreading
// G4int particle_counterSphereSD = 0;

//Get process Main ID
std::thread::id main_idSphereSD = std::this_thread::get_id();

SphereSD::SphereSD(const G4String& name)
 : G4VSensitiveDetector(name)
{
	//  initialize oldTrackID with 0, which will never be a trackID - primaries have TrackID=1, secondaries have TrackID>1
	oldTrackId = 0;
}


SphereSD::~SphereSD()
{
	// Get thread process ID
	thread_id = std::this_thread::get_id();

	//  If returned to main thread (after closing all threads created by multithreading) print secondary counter
	if (main_idSphereSD == thread_id)
	{
		// Get an iterator pointing to the first element in the map
		std::map<std::string, int>::iterator it_console = SphereSDmap.begin();
		std::map<std::string, int>::iterator it_file = SphereSDmap.begin();

		// Iterate through the map and print the elements in console
		G4cout << "PARTICLE COUNT OF SphereSD" << G4endl;
		while (it_console != SphereSDmap.end())
		{
			G4cout << "  " << std::setw(15) << it_console->first << ": " << std::setw(10) << it_console->second << G4endl;
			++it_console;
		}

		//List of generated particles to file

		// create a folder for the files if it does not exists
		fs::create_directory(folderName);
		fs::create_directory(folderName + "/" + ListFolder);

		//Get main process ID
		G4long pid = _getpid();

		// Check if "pid_ListOfGeneratedParticles in SDX.txt" is already existing; if yes, check if "pid+1_ListOfGeneratedParticles in SDX.txt" exists.
		while(std::ifstream(folderName + "/" + ListFolder + "/" + std::to_string(pid) + "_ListOfGeneratedParticles in SphereSD" + ".txt"))
		{
			pid++;
		}
		// Set final file name
		std::string fileName = std::to_string(pid) + "_ListOfGeneratedParticles in SphereSD" + ".txt";

		// flush output to file
		std::ofstream outFile(folderName + "/" + ListFolder + "/" + fileName);

		// Iterate through the map and print the elements in file
		outFile <<  "PARTICLE COUNT OF SphereSD" << G4endl;
		while (it_file != SphereSDmap.end())
		{
			outFile << "  " << std::setw(15) << it_file->first << ": " << std::setw(10) << it_file->second << G4endl;
			++it_file;
		}
	}
}


void SphereSD::Initialize(G4HCofThisEvent* /*hce*/)
{}

G4bool SphereSD::ProcessHits(G4Step* step, G4TouchableHistory* /*history*/)
{
	// Get the Current trackID and the name of the particle
	const G4Track* track = step->GetTrack();
	currentTrackId = track->GetTrackID();
	G4String name   = track->GetDefinition()->GetParticleName();
	// G4double HalfLife = track->GetDefinition()->GetPDGLifeTime() / 1.443; // mean life time divided by 1.443 equals half-life

	// if particle is a secondary (trackID>1) and we have not counted it yet add it to the map
	if ( (currentTrackId > 1) && (currentTrackId != oldTrackId) )
	{
		//  particle_counterSphereSD++;
		 SphereSDmap[name] = SphereSDmap[name]+1;
	}

	// overwrite oldTrackID with currentTrackID
	oldTrackId = currentTrackId;


	// keep only outgoing particle
	const G4ParticleDefinition* particle = track->GetParticleDefinition();

	// code PDG:
	// G4int pdgCode = track->GetDefinition()->GetPDGEncoding();
	//G4String name   = track->GetDefinition()->GetParticleName();

	//G4String name   = particle->GetParticleName();

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
	// G4double time = preStepPoint->GetGlobalTime();

	// Get Analysis Manager
	G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

		// ntuples and histograms are set up in RunAction.cc
		// Store hit in the ntuple
		// if(particle == G4Neutron::Neutron())  analysisManager->FillNtupleDColumn(0, 0, Ekin/MeV);
		if(particle == G4Neutron::Neutron())  analysisManager->FillNtupleDColumn(6, 0, Ekin/MeV);
		if(particle == G4Neutron::Neutron())  analysisManager->FillNtupleDColumn(6, 1, localPosition.theta()/degree);
		if(particle == G4Neutron::Neutron())  analysisManager->FillNtupleDColumn(6, 2, localPosition.phi()/degree);
		if(particle == G4Neutron::Neutron())	analysisManager->AddNtupleRow(6);

		// // Store hit in histogram
		// analysisManager->FillH1(0, ID);
		// analysisManager->FillH1(1, pdgCode);
		// analysisManager->FillH1(2, Ekin/MeV);
		// analysisManager->FillH1(3, localPosition.x()/cm);
		// analysisManager->FillH1(4, localPosition.y()/cm);
		// analysisManager->FillH1(5, time/ns);
	// }
	return true;
}


void SphereSD::EndOfEvent(G4HCofThisEvent* /*hce*/)
{}
