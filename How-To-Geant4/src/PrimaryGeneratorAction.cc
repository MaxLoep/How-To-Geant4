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

#include "DetectorConstruction.hh"
#include "Randomize.hh"
//#include "PrimaryGeneratorMessenger.hh"

//old stuff from the original B1 example
//#include "G4LogicalVolumeStore.hh"

//
//PARTICLE GUN
//
/*
PrimaryGeneratorAction::PrimaryGeneratorAction(DetectorConstruction* det)
:G4VUserPrimaryGeneratorAction(),
 fDetector(det), fRndmBeam(0.), fTimeExposure(0.), fGunMessenger(nullptr)
{
  fParticleGun  = new G4ParticleGun(1);
  SetDefaultKinematic();
    
  //create a messenger for this class
  fGunMessenger = new PrimaryGeneratorMessenger(this);  
}


PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete fParticleGun;
  delete fGunMessenger;  
}


void PrimaryGeneratorAction::SetDefaultKinematic()
{
  G4ParticleDefinition* particle
           = G4ParticleTable::GetParticleTable()->FindParticle("proton");
  fParticleGun->SetParticleDefinition(particle);
  fParticleGun->SetParticleEnergy(10*MeV);  
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(1.,0.,0.));
  G4double position = -0.45*(fDetector->GetWorldSizeX());
  fParticleGun->SetParticlePosition(G4ThreeVector(position,0.*cm,0.*cm));
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  //this function is called at the begining of event

  //randomize the beam, if requested.
  //
  if (fRndmBeam > 0.) 
    {
      G4ThreeVector oldPosition = fParticleGun->GetParticlePosition();
      if (fRndmBeam > fDetector->GetAbsorSizeYZ())
        fRndmBeam = fDetector->GetAbsorSizeYZ(); 
      G4double rbeam = 0.5*fRndmBeam;
      G4double x0 = oldPosition.x();
      G4double y0 = oldPosition.y() + (2*G4UniformRand()-1.)*rbeam;
      G4double z0 = oldPosition.z() + (2*G4UniformRand()-1.)*rbeam;
      fParticleGun->SetParticlePosition(G4ThreeVector(x0,y0,z0));
      fParticleGun->GeneratePrimaryVertex(anEvent);
      fParticleGun->SetParticlePosition(oldPosition);      
    }

  else fParticleGun->GeneratePrimaryVertex(anEvent);

  // randomize time zero of anEvent
  //
  G4double t0 = fTimeExposure*G4UniformRand();
  anEvent->GetPrimaryVertex()->SetT0(t0);

}
*/

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