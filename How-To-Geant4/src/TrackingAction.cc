/*
do stuff every track
TO-DO'S:
-More if-statements to filter between generations of secondaries
-use variable 'fTimeBirth' to built a filter to look at the isotopes at a specific moment in time
*/
#include "Run.hh"
#include "EventAction.hh"
#include "TrackingAction.hh"

#include "Analysis.hh"
#include "G4RunManager.hh"
#include "G4Track.hh"
#include "G4StepStatus.hh"
#include "G4ParticleTypes.hh"
#include "G4IonTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

TrackingAction::TrackingAction(EventAction* event)
:G4UserTrackingAction(), fEventAction(event)
{
	 fTimeBirth = fTimeEnd = 0.;
}

TrackingAction::~TrackingAction()
{ }

void TrackingAction::PreUserTrackingAction(const G4Track* track)
{
	Run* run = static_cast<Run*>(
		 G4RunManager::GetRunManager()->GetNonConstCurrentRun());

	// get infos about the particle of the current track
	G4ParticleDefinition* particle 	= track->GetDefinition();
	G4String name     				= particle->GetParticleName();
	G4double meanLife 				= particle->GetPDGLifeTime() / 1.443; 	// mean life time divided by 1.443 equals half-life
	fTimeBirth       				= track->GetGlobalTime();				// can this be used to filter for isotopes at Time = x?

	// HERE COULD BE MORE IF-STATEMENTS TO FILTER BETWEEN 1st, 2nd, 3rd, etc. GENERATION OF SECONDARIES
	// count secondary particles: track ID > 1
	if (track->GetTrackID() > 1)  run->ParticleCount(name,meanLife);
}

void TrackingAction::PostUserTrackingAction(const G4Track* track)
{
	Run* run = static_cast<Run*>(
		 G4RunManager::GetRunManager()->GetNonConstCurrentRun());
	
	// get infos about the particle of the current track
	const G4ParticleDefinition* particle 	= track->GetParticleDefinition();
	G4String name     						= particle->GetParticleName();
	G4double ekin     						= track->GetKineticEnergy();
	fTimeEnd         						= track->GetGlobalTime();
	if ((particle->GetPDGStable())&&(ekin == 0.)) fTimeEnd = DBL_MAX;

	// keep only emerging particles for the List of Particles emerging from volume
	// get Step Status - inbuilt variable that gives information about Step: status = 0 means particle is out of the world volume  (see G4StepStatus.hh for others)
	G4StepStatus status = track->GetStep()->GetPostStepPoint()->GetStepStatus();
	// if particle has not left the world volume yet, return from this function (= do nothing)
	if (status != fWorldBoundary) return; 

	//count particles leaving the world volume (status = 0)
	run->ParticleFlux(name);
}