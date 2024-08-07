/*
do stuff every track
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

	G4ParticleDefinition* particle = track->GetDefinition();
	G4String name     = particle->GetParticleName();
	G4double meanLife = particle->GetPDGLifeTime() / 1.443; // mean life time divided by 1.443 equals half-life
	fTimeBirth       = track->GetGlobalTime();

	//count secondary particles
	if (track->GetTrackID() > 1)  run->ParticleCount(name,meanLife);
}

void TrackingAction::PostUserTrackingAction(const G4Track* track)
{
	Run* run = static_cast<Run*>(
		 G4RunManager::GetRunManager()->GetNonConstCurrentRun());
	
	const G4ParticleDefinition* particle = track->GetParticleDefinition();
	G4String name     = particle->GetParticleName();
	G4double ekin     = track->GetKineticEnergy();
	fTimeEnd         = track->GetGlobalTime();
	if ((particle->GetPDGStable())&&(ekin == 0.)) fTimeEnd = DBL_MAX;

	// keep only emerging particles
	G4StepStatus status = track->GetStep()->GetPostStepPoint()->GetStepStatus();
	if (status != fWorldBoundary) return; 

	//count particles leaving the world volume
	run->ParticleFlux(name);
}