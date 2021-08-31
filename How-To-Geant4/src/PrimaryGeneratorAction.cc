/*
Comment it more
*/

#include "PrimaryGeneratorAction.hh"    //Header file where functions classes and variables may be defined (...)
     
#include "G4RunManager.hh"                //Nessesary. You need this.
#include "G4ParticleTable.hh"             //Nessesary. You need this.
#include "G4ParticleDefinition.hh"        //Nessesary. You need this.
#include "G4SystemOfUnits.hh"             //Nessesary. You need this.
#include "G4Event.hh"                     //Nessesary. You need this.
#include "G4ParticleGun.hh"               //Nessesary if you want to use the ParticleGun
#include "G4GeneralParticleSource.hh"     //Nessesary if you want to use the GeneralParticleSource

//old stuff from the original B1 example
//#include "Randomize.hh"
//#include "G4LogicalVolumeStore.hh" 



// //
// //PARTICLE GUN
// //
// PrimaryGeneratorAction::PrimaryGeneratorAction()
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


// PrimaryGeneratorAction::~PrimaryGeneratorAction()
// {
//   delete fParticleGun;

// }


// void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
// {
//   fParticleGun->GeneratePrimaryVertex(anEvent);
// }

//--------------------------------------------------------------------------------------------------------

//
//GENERAL PARTICLE SOURCE
//G4GeneralParticleSource (GPS) is used exactly the same way as G4ParticleGun in a Geant4 application.
//In existing applications one can simply change your PrimaryGeneratorAction by globally replacing
//G4ParticleGun with G4GeneralParticleSource --  Geant4 - Book for Application Developers V10.7 
//
PrimaryGeneratorAction::PrimaryGeneratorAction()
: G4VUserPrimaryGeneratorAction(),
  fParticleBeam(0)
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
