/*
Comment it more
*/

#include "PrimaryGeneratorAction.hh"    //Header file where functions classes and variables may be defined (...)
     
#include "G4RunManager.hh"                //Necessary. You need this.
#include "G4ParticleTable.hh"             //Necessary. You need this.
#include "G4ParticleDefinition.hh"        //Necessary. You need this.
#include "G4SystemOfUnits.hh"             //Necessary. You need this.
#include "G4Event.hh"                     //Necessary. You need this.
#include "G4ParticleGun.hh"               //Necessary if you want to use the ParticleGun
#include "G4GeneralParticleSource.hh"     //Necessary if you want to use the GeneralParticleSource

#include "DetectorConstruction.hh"
#include "Randomize.hh"

//
//GENERAL PARTICLE SOURCE
//G4GeneralParticleSource (GPS) is used exactly the same way as G4ParticleGun in a Geant4 application.
//In existing applications one can simply change your PrimaryGeneratorAction by globally replacing
//G4ParticleGun with G4GeneralParticleSource --  Geant4 - Book for Application Developers V10.7 
//
PrimaryGeneratorAction::PrimaryGeneratorAction(DetectorConstruction* det)
: G4VUserPrimaryGeneratorAction(),
  fParticleBeam(0),fDetector(det)
{
  fParticleBeam  = new G4GeneralParticleSource();

  // default particle kinematic
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;
  G4ParticleDefinition* particle = particleTable->FindParticle(particleName="gamma");

  fParticleBeam->SetParticleDefinition(particle);

  //GPS doesn't have these three commands; you need to set them in the macro file
  //fParticleBeam->SetParticlePosition(G4ThreeVector(0,0,0));
  //fParticleBeam->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
  //fParticleBeam->SetParticleEnergy(6.*MeV);
  
}


PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete fParticleBeam;
}


void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  fParticleBeam->GeneratePrimaryVertex(anEvent);
}