#include "B1PrimaryGeneratorAction.hh"

#include "G4LogicalVolumeStore.hh"
#include "G4RunManager.hh"
#include "G4ParticleGun.hh"
#include "G4GeneralParticleSource.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include "G4Event.hh"



// //
// //PARTICLE GUN
// //
// B1PrimaryGeneratorAction::B1PrimaryGeneratorAction()
// : G4VUserPrimaryGeneratorAction(),
//   fParticleGun(0)
// {
//   G4int n_particle = 1;
//   fParticleGun  = new G4ParticleGun(n_particle);

//   // default particle kinematic
//   G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
//   G4String particleName;
//   G4ParticleDefinition* particle = particleTable->FindParticle(particleName="gamma");

//   fParticleGun->SetParticleDefinition(particle);
//   fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
//   fParticleGun->SetParticleEnergy(6.*MeV);
    
//   fParticleGun->SetParticlePosition(G4ThreeVector(0,0,0));

// }


// B1PrimaryGeneratorAction::~B1PrimaryGeneratorAction()
// {
//   delete fParticleGun;

// }


// void B1PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
// {
//   fParticleGun->GeneratePrimaryVertex(anEvent);
// }


//
//GENERAL PARTICLE SOURCE
//G4GeneralParticleSource (GPS) is used exactly the same way as G4ParticleGun in a Geant4 application.
//In existing applications one can simply change your PrimaryGeneratorAction by globally replacing
//G4ParticleGun with G4GeneralParticleSource --  Geant4 - Book for Application Developers V10.7 
//
B1PrimaryGeneratorAction::B1PrimaryGeneratorAction()
: G4VUserPrimaryGeneratorAction(),
  fParticleBeam(0)
{
  fParticleBeam  = new G4GeneralParticleSource();//n_particle);

  // default particle kinematic
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;
  G4ParticleDefinition* particle = particleTable->FindParticle(particleName="gamma");

  fParticleBeam->SetParticleDefinition(particle);

  //GPS doesn't have these two commands; you need to set them in the macro file
  //fParticleBeam->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
  //fParticleBeam->SetParticleEnergy(6.*MeV);
    
  fParticleBeam->SetParticlePosition(G4ThreeVector(0,0,0));

}


B1PrimaryGeneratorAction::~B1PrimaryGeneratorAction()
{
  delete fParticleBeam;
}


void B1PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  fParticleBeam->GeneratePrimaryVertex(anEvent);
}
