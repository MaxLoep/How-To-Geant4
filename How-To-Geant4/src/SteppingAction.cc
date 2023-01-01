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
  
/*
 //longitudinal profile of deposited energy
 //randomize point of energy deposition
 //
 G4ThreeVector prePoint  = aStep->GetPreStepPoint() ->GetPosition();
 G4ThreeVector postPoint = aStep->GetPostStepPoint()->GetPosition();
 G4ThreeVector point = prePoint + G4UniformRand()*(postPoint - prePoint);
 G4double x = point.x();
 G4double xshifted = x + 0.5*fDetector->GetAbsorThickness();
 G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
 analysisManager->FillH1(2, xshifted, edepStep); 
*/

  //B1 SCORING METHOD
  //
  // If fscoringVolume is not defined, call class 'DetectorConstruction' and use function
  // 'GetScoringVolume' to set fScoringVolume to what it is in DetectorConstruction.cc here
  // if (!fScoringVolume) { 
  //   const DetectorConstruction* detectorConstruction
  //     = static_cast<const DetectorConstruction*>
  //       (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
  //   fScoringVolume = detectorConstruction->GetScoringVolume(); 

  //    //G4cout  << "\n I am called! " << G4endl;
  // }

  // // get volume of the current step
  // G4LogicalVolume* volume 
  //   = step->GetPreStepPoint()->GetTouchableHandle()
  //     ->GetVolume()->GetLogicalVolume();
      
  // // check if we are in scoring volume defined in 'DetectorConstruction.cc'. 
  // // If we are not in the scoring volume return (=stop here)
  // if (volume != fScoringVolume) return;

  // // collect energy deposited in this step if we are in fScoringVolume
  // G4double edepStep = step->GetTotalEnergyDeposit();
  // // use function AddEdep(G4double), which is defined in EventAction.hh to sum up energy deposit
  // fEventAction->AddEdep(edepStep); 
    
}

