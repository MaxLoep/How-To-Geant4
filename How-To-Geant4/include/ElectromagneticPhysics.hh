#ifndef ElectromagneticPhysics_h
#define ElectromagneticPhysics_h 1

#include "G4VPhysicsConstructor.hh"
#include "globals.hh"


class ElectromagneticPhysics : public G4VPhysicsConstructor
{
  public: 
    ElectromagneticPhysics(const G4String& name = "standard");
   ~ElectromagneticPhysics();

  public: 
    // This method is dummy for physics
    virtual void ConstructParticle() {};
 
    // This method will be invoked in the Construct() method.
    // each physics process will be instantiated and
    // registered to the process manager of each particle type 
    virtual void ConstructProcess();
};


#endif

