#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "G4GeneralParticleSource.hh"
#include "globals.hh"

// Empty classes for some reason...
// class G4ParticleGun;
// class G4GeneralParticleSource;
// class G4Event;
// class G4Box;

// //
// //PARTICLE GUN
// //
// class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
// {
//   public:
//     PrimaryGeneratorAction();    
//     virtual ~PrimaryGeneratorAction();

//     // method from the base class
//     virtual void GeneratePrimaries(G4Event*);         
  
//     // method to access particle gun
//     const G4ParticleGun* GetParticleGun() const { return fParticleGun; }
  
//   private:
//     G4ParticleGun*  fParticleGun; // pointer a to G4 gun class
// };

// #endif

//
// GENERAL PARTICLE SOURCE
//
class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    PrimaryGeneratorAction();    
    virtual ~PrimaryGeneratorAction();

    // method from the base class
    virtual void GeneratePrimaries(G4Event*);         
  
    // method to access particle gun
    const G4GeneralParticleSource* GetParticleGun() const {return fParticleBeam;}
  
  private:
    G4GeneralParticleSource*  fParticleBeam;
};

#endif
