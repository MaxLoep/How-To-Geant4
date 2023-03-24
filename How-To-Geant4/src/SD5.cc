#include "SensitiveDetector.hh"
#include "Analysis.hh"

#include "G4VTouchable.hh"
#include "G4Step.hh"
#include "G4ios.hh"
#include "G4SystemOfUnits.hh"

#include "G4VProcess.hh"

#include "G4ParticleTypes.hh"

// initialize particleCounter outside of anything so this will work with multithreading
G4int particle_counter5 = 0;

SD5::SD5(const G4String& name)
 : G4VSensitiveDetector(name)
{
  G4cout << G4endl;
  G4cout << "THIS IS CONSTRUCTOR OF SD5" << G4endl;
  G4cout << "TRACK_ID = 0 AND COUNT = 0" << G4endl;
  G4cout << G4endl;

  //Get process ID
  const std::thread::id id = std::this_thread::get_id();
  G4cout << "\n ID is " << id << G4endl;

  //  initialize oldTrackID with 0, which will never be a trackID - primaries have TrackID=1
  oldTrackId = 0;
}


SD5::~SD5()
{
  G4cout << G4endl;
  G4cout << "THIS IS DESTRUCTOR OF SD5" << G4endl;
  G4cout << "COUNTER IS " << particle_counter5 << G4endl;
  G4cout << G4endl;

  //Get process ID
  const std::thread::id id = std::this_thread::get_id();
  G4cout << "\n ID is " << id << G4endl;
}


void SD5::Initialize(G4HCofThisEvent* /*hce*/)
{}


G4bool SD5::ProcessHits(G4Step* step, G4TouchableHistory* /*history*/)
{
  // Get the Current trackID and the name of the particle
  const G4Track* track = step->GetTrack();
  currentTrackId = track->GetTrackID();
  G4String name   = track->GetDefinition()->GetParticleName();

  // Print for debbuging
  // G4cout <<"TRACK ID IS " << currentTrackId <<  G4endl;
  // G4cout <<"OLD   ID IS " << oldTrackId <<  G4endl;

  // if particle is a secondary (trackID>1) and we have not counted it yet increase the particleCounter
  if ( (currentTrackId > 1) && (currentTrackId != oldTrackId) ) particle_counter5++;

  // if particle is a secondary (trackID>1) print its name in the console
  // if ( (currentTrackId >1) && (currentTrackId != oldTrackId)) G4cout << name << G4endl;

  // overwrite oldTrackID with currentTrackID
  oldTrackId = currentTrackId;


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

    // ntuples and histograms are set up in RunAction.cc
    // Store hit in the ntuple
    if(particle == G4Proton::Proton())  analysisManager->FillNtupleDColumn(5, 0, Ekin/MeV);
    if(particle == G4Proton::Proton())  analysisManager->FillNtupleDColumn(5, 1, localPosition.x()/cm);
    if(particle == G4Proton::Proton())  analysisManager->FillNtupleDColumn(5, 2, time/ns);
    if(particle == G4Proton::Proton())  analysisManager->AddNtupleRow(5);

    // Store hit in one dimensional histogram 
    // analysisManager->FillH1(id, value, G4double weight=1.0)
    // analysisManager->FillH1(0, ID);
    // analysisManager->FillH1(1, pdgCode);
    // analysisManager->FillH1(2, Ekin/MeV);
    // analysisManager->FillH1(3, localPosition.x()/cm);
    // analysisManager->FillH1(4, localPosition.y()/cm);
    // analysisManager->FillH1(5, time/ns);

    // Store hit in two dimensional histogram - heatmap
    // analysisManager->FillH2(id, xvalue, yvalue, G4double weight=1.0)
    if(particle == G4Proton::Proton())  analysisManager->FillH2(0, localPosition.x()/cm, localPosition.y()/cm);


  // }
  return true;
}


void SD5::EndOfEvent(G4HCofThisEvent* /*hce*/)
{
  // G4cout << G4endl;
  // G4cout << "SD5 END OF EVENT!" << G4endl;
  // G4cout <<  "CURRENT COUNTER " << particle_counter5  << G4endl;
  // G4cout << G4endl;

  // reset oldTrackID for the next event
  oldTrackId = 0;
}