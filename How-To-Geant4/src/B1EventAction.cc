/*
Understand what this does and comment it
*/

#include "B1EventAction.hh"
#include "B1RunAction.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"

#include "B4Analysis.hh"


B1EventAction::B1EventAction(B1RunAction* runAction)
: G4UserEventAction(),
  fRunAction(runAction),
  fEdep(0.),
   fEnergyAbs(0.),
   fEnergyGap(0.),
   fTrackLAbs(0.),
   fTrackLGap(0.)
{} 


B1EventAction::~B1EventAction()
{}


void B1EventAction::BeginOfEventAction(const G4Event*)
{   
  //variable initialisation per event 
  fEdep = 0.;

  fEnergyAbs = 0.;
  fEnergyGap = 0.;
  fTrackLAbs = 0.;
  fTrackLGap = 0.;
}


void B1EventAction::EndOfEventAction(const G4Event*)
{   
  // accumulate statistics in run action
  fRunAction->AddEdep(fEdep);
  
  // //from example B4a
  // // Accumulate statistics
  // //

  // // get analysis manager
  // auto analysisManager = G4AnalysisManager::Instance();

  // // fill histograms
  // analysisManager->FillH1(0, fEnergyAbs);
  // analysisManager->FillH1(1, fEnergyGap);
  // analysisManager->FillH1(2, fTrackLAbs);
  // analysisManager->FillH1(3, fTrackLGap);
  
  // // fill ntuple
  // analysisManager->FillNtupleDColumn(0, fEnergyAbs);
  // analysisManager->FillNtupleDColumn(1, fEnergyGap);
  // analysisManager->FillNtupleDColumn(2, fTrackLAbs);
  // analysisManager->FillNtupleDColumn(3, fTrackLGap);
  // analysisManager->AddNtupleRow();  
}

