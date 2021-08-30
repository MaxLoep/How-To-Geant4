/*
Understand what this does and comment it
*/

#include "B1EventAction.hh"
#include "B1RunAction.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"

#include "B4Analysis.hh"
#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"
#include <iomanip>

#include "G4Track.hh"
#include "G4StepStatus.hh"
#include "G4ParticleTypes.hh"



B1EventAction::B1EventAction(B1RunAction* runAction)
: G4UserEventAction(),
  fRunAction(runAction),
  fEdep(0.),
   fEnergyAbs(0.),
   fEnergyGap(0.),
   fTrackLAbs(0.),
   fTrackLGap(0.),
   fAbsoEdepHCID(-1),
   fGapEdepHCID(-1),
   fAbsoTrackLengthHCID(-1),
   fGapTrackLengthHCID(-1)
{} 


B1EventAction::~B1EventAction()
{}

//From example B4d
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4THitsMap<G4double>* 
B1EventAction::GetHitsCollection(G4int hcID,
                                  const G4Event* event) const
{
  auto hitsCollection 
    = static_cast<G4THitsMap<G4double>*>(
        event->GetHCofThisEvent()->GetHC(hcID));

  
  if ( ! hitsCollection ) {
    G4ExceptionDescription msg;
    msg << "Cannot access hitsCollection ID " << hcID; 
    G4Exception("B4dEventAction::GetHitsCollection()",
      "MyCode0003", FatalException, msg);
  }         

  return hitsCollection;
}    

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double B1EventAction::GetSum(G4THitsMap<G4double>* hitsMap) const
{
  G4double sumValue = 0.;
  for ( auto it : *hitsMap->GetMap() ) {
    // hitsMap->GetMap() returns the map of std::map<G4int, G4double*>
    sumValue += *(it.second);
  }
  return sumValue;  
}  

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1EventAction::BeginOfEventAction(const G4Event*)
{   
  //variable initialisation per event 
  fEdep = 0.;

  fEnergyAbs = 0.;
  fEnergyGap = 0.;
  fTrackLAbs = 0.;
  fTrackLGap = 0.;
}


void B1EventAction::EndOfEventAction(const G4Event* event)
{   
  // accumulate statistics in run action
  fRunAction->AddEdep(fEdep);

  // //from example B4d
  // Get hist collections IDs
  if ( fAbsoEdepHCID == -1 ) {
    fAbsoEdepHCID 
      = G4SDManager::GetSDMpointer()->GetCollectionID("Absorber/Edep");
    fGapEdepHCID 
      = G4SDManager::GetSDMpointer()->GetCollectionID("Gap/Edep");
    fAbsoTrackLengthHCID 
      = G4SDManager::GetSDMpointer()->GetCollectionID("Absorber/TrackLength");
    fGapTrackLengthHCID 
      = G4SDManager::GetSDMpointer()->GetCollectionID("Gap/TrackLength");
  }
  
  // Get sum values from hits collections
  //
  auto absoEdep = GetSum(GetHitsCollection(fAbsoEdepHCID, event));
  auto gapEdep = GetSum(GetHitsCollection(fGapEdepHCID, event));

  auto absoTrackLength 
    = GetSum(GetHitsCollection(fAbsoTrackLengthHCID, event));
  auto gapTrackLength 
    = GetSum(GetHitsCollection(fGapTrackLengthHCID, event));

  // // get analysis manager
  // auto analysisManager = G4AnalysisManager::Instance();

  // // fill histograms
  // //  
  // if(absoEdep != 0)       analysisManager->FillH1(0, absoEdep);
  // if(gapEdep != 0)        analysisManager->FillH1(1, gapEdep);
  // if(absoTrackLength !=0) analysisManager->FillH1(2, absoTrackLength);
  // if(gapTrackLength != 0) analysisManager->FillH1(3, gapTrackLength);
  
  // // fill ntuple
  // //
  // if(absoEdep != 0)       analysisManager->FillNtupleDColumn(0, absoEdep);
  // if(gapEdep != 0)        analysisManager->FillNtupleDColumn(1, gapEdep);
  // if(absoTrackLength !=0) analysisManager->FillNtupleDColumn(2, absoTrackLength);
  // if(gapTrackLength != 0) analysisManager->FillNtupleDColumn(3, gapTrackLength);
  // analysisManager->AddNtupleRow();  
}

