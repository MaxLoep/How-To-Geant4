//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
/// \file BoxSD.cc
/// \brief Implementation of the BoxSD class
//

#include "BoxSD.hh"
#include "B4Analysis.hh"

#include "G4VTouchable.hh"
#include "G4Step.hh"
#include "G4ios.hh"
#include "G4SystemOfUnits.hh"

#include "G4VProcess.hh"

#include "G4ParticleTypes.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

BoxSD::BoxSD(const G4String& name)
 : G4VSensitiveDetector(name)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

BoxSD::~BoxSD()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void BoxSD::Initialize(G4HCofThisEvent* /*hce*/)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool BoxSD::ProcessHits(G4Step* step, G4TouchableHistory* /*history*/)
{
  // Current track:
  const G4Track* track = step->GetTrack();

 // keep only outgoing particle
 const G4ParticleDefinition* particle = track->GetParticleDefinition();
 
 G4StepStatus status1 = track->GetStep()->GetPreStepPoint()->GetStepStatus();
 G4StepStatus status2 = track->GetStep()->GetPostStepPoint()->GetStepStatus();

 auto status3 = track->GetStep()->GetPreStepPoint()->GetPhysicalVolume()->GetName();
 auto status4 = track->GetStep()->GetPostStepPoint()->GetPhysicalVolume()->GetName();

 G4int status5 = track->GetStep()->IsFirstStepInVolume();
 G4int status6 = track->GetStep()->IsLastStepInVolume();


 //if (status3 != "Box" && status4 == "Box")
 //if (status5 == true) 
 //if(status2 != fGeomBoundary && particle == G4Proton::Proton()){
 //if(particle == G4Neutron::Neutron()){

    // Track ID:
    G4int ID = track->GetTrackID();

    // code PDG:
    G4int pdgCode = track->GetDefinition()->GetPDGEncoding();
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

    // Store hit in the ntuple
      G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
      analysisManager->FillNtupleIColumn(0, ID);
      analysisManager->FillNtupleIColumn(1, pdgCode);
      analysisManager->FillNtupleDColumn(2, Ekin/MeV);
      analysisManager->FillNtupleDColumn(3, localPosition.x()/cm);
      analysisManager->FillNtupleDColumn(4, localPosition.y()/cm);
      analysisManager->FillNtupleDColumn(5, time/ns);
      analysisManager->AddNtupleRow();
  //}
  return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void BoxSD::EndOfEvent(G4HCofThisEvent* /*hce*/)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
