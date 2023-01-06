#include "SensitiveDetector.hh"
#include "Analysis.hh"

#include "G4VTouchable.hh"
#include "G4Step.hh"
#include "G4ios.hh"
#include "G4SystemOfUnits.hh"

#include "G4VProcess.hh"

#include "G4ParticleTypes.hh"


SD4::SD4(const G4String& name)
 : G4VSensitiveDetector(name)
{}


SD4::~SD4()
{}


void SD4::Initialize(G4HCofThisEvent* /*hce*/)
{}


G4bool SD4::ProcessHits(G4Step* step, G4TouchableHistory* /*history*/)
{
  // Current track:
  const G4Track* track = step->GetTrack();

 // keep only outgoing particle
 const G4ParticleDefinition* particle = track->GetParticleDefinition();
 
//  G4StepStatus status1 = track->GetStep()->GetPreStepPoint()->GetStepStatus();
//  G4StepStatus status2 = track->GetStep()->GetPostStepPoint()->GetStepStatus();

//  auto status3 = track->GetStep()->GetPreStepPoint()->GetPhysicalVolume()->GetName();
//  auto status4 = track->GetStep()->GetPostStepPoint()->GetPhysicalVolume()->GetName();

//  G4int status5 = track->GetStep()->IsFirstStepInVolume();
//  G4int status6 = track->GetStep()->IsLastStepInVolume();


 //if (status3 != "Box" && status4 == "Box")
 //if (status5 == true) 
 //if(status2 != fGeomBoundary && particle == G4Proton::Proton()){
 //if(particle == G4Neutron::Neutron()){
// if(particle == G4Proton::Proton()){

    // Track ID:
    // G4int ID = track->GetTrackID();

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
    G4double time = preStepPoint->GetGlobalTime();

    // Get Analysis Manager
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

    // Store hit in the ntuple
    if(particle == G4Proton::Proton())  analysisManager->FillNtupleDColumn(4, 0, Ekin/MeV);
    if(particle == G4Proton::Proton())  analysisManager->FillNtupleDColumn(4, 1, localPosition.x()/cm);
    if(particle == G4Proton::Proton())  analysisManager->FillNtupleDColumn(4, 2, time/ns);
    if(particle == G4Proton::Proton())  analysisManager->AddNtupleRow(4);

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


void SD4::EndOfEvent(G4HCofThisEvent* /*hce*/)
{}

