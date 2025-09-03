#include "G4Types.hh"

class rbs_physics : public G4VPhysicsConstructor
{
public:

  explicit rbs_physics(G4int ver=1, const G4String& name="");

  virtual ~rbs_physics();

  virtual void ConstructParticle();
  virtual void ConstructProcess();

private:
  G4int  verbose;
};
