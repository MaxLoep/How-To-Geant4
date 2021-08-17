/*
Understand what this does and comment it
*/

#include "B1SteppingAction.hh"
#include "B1EventAction.hh"
#include "DetectorConstruction.hh"

#include "G4Step.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"


B1SteppingAction::B1SteppingAction(B1EventAction* eventAction)
: G4UserSteppingAction(),
  fEventAction(eventAction),
  fScoringVolume(0)
{}


B1SteppingAction::~B1SteppingAction()
{}


void B1SteppingAction::UserSteppingAction(const G4Step* step)
{
  // If fscoringVolume is not defined, call class 'DetectorConstruction' and use function
  // 'GetScoringVolume' to set fScoringVolume to what it is in DetectorConstruction.cc here
  if (!fScoringVolume) { 
    const DetectorConstruction* detectorConstruction
      = static_cast<const DetectorConstruction*>
        (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
    fScoringVolume = detectorConstruction->GetScoringVolume(); 

     //G4cout  << "\n I am called! " << G4endl;
  }

  // get volume of the current step
  G4LogicalVolume* volume 
    = step->GetPreStepPoint()->GetTouchableHandle()
      ->GetVolume()->GetLogicalVolume();
      
  // check if we are in scoring volume defined in 'DetectorConstruction.cc'. 
  // If we are not in the scoring volume return (=stop here)
  if (volume != fScoringVolume) return;

  // collect energy deposited in this step if we are in fScoringVolume
  G4double edepStep = step->GetTotalEnergyDeposit();
  // use function AddEdep(G4double), which is defined in EventAction.hh to sum up energy deposit
  fEventAction->AddEdep(edepStep);  
}


