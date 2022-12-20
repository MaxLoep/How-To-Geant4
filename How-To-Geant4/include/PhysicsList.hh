#ifndef PhysicsList_h
#define PhysicsList_h 1

#include "G4VModularPhysicsList.hh"
#include "globals.hh"


class PhysicsList: public G4VModularPhysicsList
{
public:
  PhysicsList();
 ~PhysicsList();

public:
  virtual void ConstructProcess();
  virtual void SetCuts();

  private:
    G4VPhysicsConstructor* fHadronElastic;
    G4VPhysicsConstructor* fHadronInelastic;
    G4VPhysicsConstructor* fIonElastic;
    G4VPhysicsConstructor* fIonInelastic;
    G4VPhysicsConstructor* fGammaNuclear;
    G4VPhysicsConstructor* fElectromagnetic;
    G4VPhysicsConstructor* fDecay;
    G4VPhysicsConstructor* fRadioactiveDecay;
    G4VPhysicsConstructor* fStopping;
};


#endif
