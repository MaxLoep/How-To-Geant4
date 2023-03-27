#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "globals.hh"
#include "G4GeneralParticleSource.hh"

class G4Event;
class DetectorConstruction;
class PrimaryGeneratorMessenger;

//
// GENERAL PARTICLE SOURCE
//
class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    PrimaryGeneratorAction(DetectorConstruction*);    
    virtual ~PrimaryGeneratorAction();

    // method from the base class
    virtual void GeneratePrimaries(G4Event*);         
  
    // method to access particle gun
    const G4GeneralParticleSource* GetParticleGun() const {return fParticleBeam;}
  
  private:
    G4GeneralParticleSource*  fParticleBeam;

  private:
    G4ParticleGun*             fParticleGun;
    DetectorConstruction*      fDetector;
};

#endif
