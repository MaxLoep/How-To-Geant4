#include "SteppingAction.hh"
#include "DetectorConstruction.hh"
#include "Run.hh"
#include "EventAction.hh"
#include "Analysis.hh"

#include "G4RunManager.hh"
                           
#include "G4Step.hh"
#include "G4Event.hh"
#include "G4LogicalVolume.hh"


SteppingAction::SteppingAction(DetectorConstruction* det, EventAction* event)
: G4UserSteppingAction(), fDetector(det), fEventAction(event)
{ }


SteppingAction::~SteppingAction()
{ }


void SteppingAction::UserSteppingAction(const G4Step* aStep)
{
  // count processes
  // 
  const G4StepPoint* endPoint = aStep->GetPostStepPoint();
  const G4VProcess* process   = endPoint->GetProcessDefinedStep();
  Run* run = static_cast<Run*>(
        G4RunManager::GetRunManager()->GetNonConstCurrentRun());
  run->CountProcesses(process);
  
  // energy deposit
  //
  G4double edepStep = aStep->GetTotalEnergyDeposit();
  if (edepStep <= 0.) return; 
  fEventAction->AddEdep(edepStep);   
}

