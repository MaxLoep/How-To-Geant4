/*
Understand what this does and comment it
Primitive Scorer data are saved here
*/

#include "EventAction.hh"

#include "Run.hh"
#include "Analysis.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"

#include "RunAction.hh"

#include "Analysis.hh"
#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"
#include <iomanip>

#include "G4Track.hh"
#include "G4StepStatus.hh"
#include "G4ParticleTypes.hh"


EventAction::EventAction()
:G4UserEventAction(),
 fTotalEnergyDeposit(0.), fTotalEnergyFlow(0.),
 fAbsoEdepHCID(-1)
//  ,fRunAction(runAction),
//   fEdep(0.),
//    fEnergyAbs(0.),
//    fEnergyGap(0.),
//    fTrackLAbs(0.),
//    fTrackLGap(0.),
//   //  fAbsoEdepHCID(-1),
//    fGapEdepHCID(-1),
//    fAbsoTrackLengthHCID(-1),
//    fGapTrackLengthHCID(-1)
  
{ }


EventAction::~EventAction()
{ }

//PRIMITIVE SCORERS
//From example B4d

G4THitsMap<G4double>* 
EventAction::GetHitsCollection(G4int hcID,
                                  const G4Event* event) const
{
  auto hitsCollection = static_cast<G4THitsMap<G4double>*>(event->GetHCofThisEvent()->GetHC(hcID));
  
  if ( ! hitsCollection ) {
    G4ExceptionDescription msg;
    msg << "Cannot access hitsCollection ID " << hcID; 
    G4Exception("B4dEventAction::GetHitsCollection()",
      "MyCode0003", FatalException, msg);
  }         

  return hitsCollection;
}    


G4double EventAction::GetSum(G4THitsMap<G4double>* hitsMap) const
{
  G4double sumValue = 0.;
  for ( auto it : *hitsMap->GetMap() ) {
    // hitsMap->GetMap() returns the map of std::map<G4int, G4double*>
    sumValue += *(it.second);
  }
  return sumValue;  
} 

void EventAction::BeginOfEventAction(const G4Event*)
{
  fTotalEnergyDeposit = 0.;
  fTotalEnergyFlow = 0.; 

/*
  //variable initialisation per event 
  //from B1
  fEdep = 0.;

  //from B4d
  fEnergyAbs = 0.;
  fEnergyGap = 0.;
  fTrackLAbs = 0.;
  fTrackLGap = 0.;
*/
}


void EventAction::AddEdep(G4double Edep)
{
  fTotalEnergyDeposit += Edep;
}


void EventAction::AddEflow(G4double Eflow)
{
  fTotalEnergyFlow += Eflow;
}


void EventAction::EndOfEventAction(const G4Event* event)
{
  Run* run = static_cast<Run*>(G4RunManager::GetRunManager()->GetNonConstCurrentRun());
             
  run->AddEdep (fTotalEnergyDeposit);             
  run->AddEflow(fTotalEnergyFlow);
               
  //G4AnalysisManager::Instance()->FillH1(1,fTotalEnergyDeposit);
  //G4AnalysisManager::Instance()->FillH1(3,fTotalEnergyFlow);  

  // REMOVE
  //B1 SCORING METHOD
  // accumulate statistics in run action
  //fRunAction->AddEdep(fEdep);

  // PRIMITVE SCORERS
  // from example B4d
  // Get hist collections IDs
  if ( fAbsoEdepHCID == -1 ) {
    fAbsoEdepHCID           = G4SDManager::GetSDMpointer()->GetCollectionID("Scorer/TrackLength");
    // fAbsoEdepHCID        = G4SDManager::GetSDMpointer()->GetCollectionID("Scorer/Edep");
    // fGapEdepHCID         = G4SDManager::GetSDMpointer()->GetCollectionID("Gap/Edep");
    // fAbsoTrackLengthHCID = G4SDManager::GetSDMpointer()->GetCollectionID("Absorber/TrackLength");
    // fGapTrackLengthHCID  = G4SDManager::GetSDMpointer()->GetCollectionID("Gap/TrackLength");
  }
  
  // Get sum values from hits collections
  //
  auto TrackLength        = GetSum(GetHitsCollection(fAbsoEdepHCID, event));
  // auto absoEdep        = GetSum(GetHitsCollection(fAbsoEdepHCID, event));
  // auto gapEdep         = GetSum(GetHitsCollection(fGapEdepHCID, event));

  // auto absoTrackLength = GetSum(GetHitsCollection(fAbsoTrackLengthHCID, event));
  // auto gapTrackLength  = GetSum(GetHitsCollection(fGapTrackLengthHCID, event));

  // get analysis manager
  auto analysisManager = G4AnalysisManager::Instance();

  // // fill histograms
  // //  
  // if(absoEdep != 0)       analysisManager->FillH1(0, absoEdep);
  // if(gapEdep != 0)        analysisManager->FillH1(1, gapEdep);
  // if(absoTrackLength !=0) analysisManager->FillH1(2, absoTrackLength);
  // if(gapTrackLength != 0) analysisManager->FillH1(3, gapTrackLength);
  
  // fill ntuple
  //
  if(TrackLength != 0)       analysisManager->FillNtupleDColumn(0, 0, TrackLength);
  // if(absoEdep != 0)       analysisManager->FillNtupleDColumn(0, 0, absoEdep);
  // if(gapEdep != 0)        analysisManager->FillNtupleDColumn(1, gapEdep);
  // if(absoTrackLength !=0) analysisManager->FillNtupleDColumn(2, absoTrackLength);
  // if(gapTrackLength != 0) analysisManager->FillNtupleDColumn(3, gapTrackLength);
  if(TrackLength != 0)       analysisManager->AddNtupleRow(0); 

}

