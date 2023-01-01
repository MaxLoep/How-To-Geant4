#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "globals.hh"
#include "G4GeneralParticleSource.hh"

class G4Event;
class DetectorConstruction;
class PrimaryGeneratorMessenger;

// Empty classes for some reason...
// class G4ParticleGun;
// class G4GeneralParticleSource;
// class G4Event;
// class G4Box;

//
//PARTICLE GUN
//
/*
class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    PrimaryGeneratorAction(DetectorConstruction*);    
   ~PrimaryGeneratorAction();

  public:
    void SetDefaultKinematic();  
    void SetRndmBeam(G4double value)  {fRndmBeam = value;}
    void SetTimeExposure(G4double value)  {fTimeExposure = value;}

    virtual void GeneratePrimaries(G4Event*);
         
    G4ParticleGun* GetParticleGun() {return fParticleGun;}
    G4double GetTimeExposure() {return fTimeExposure;}

  private:
    G4ParticleGun*             fParticleGun;
    DetectorConstruction*      fDetector;
    G4double                   fRndmBeam;
    G4double                   fTimeExposure;
    PrimaryGeneratorMessenger* fGunMessenger;     
};
*/

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
    //G4double                   fRndmBeam;
    //G4double                   fTimeExposure;
    //PrimaryGeneratorMessenger* fGunMessenger; 
};


#endif
