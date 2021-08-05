#include "GammaNuclearPhysics.hh"

#include "G4ParticleDefinition.hh"
#include "G4ProcessManager.hh"

// Processes

#include "G4PhotoNuclearProcess.hh"
#include "G4LowEGammaNuclearModel.hh"
#include "G4CascadeInterface.hh"

#include "G4SystemOfUnits.hh"


GammaNuclearPhysics::GammaNuclearPhysics(const G4String& name)
:  G4VPhysicsConstructor(name)
{ }


GammaNuclearPhysics::~GammaNuclearPhysics()
{ }


void GammaNuclearPhysics::ConstructProcess()
{
   G4PhotoNuclearProcess* process = new G4PhotoNuclearProcess();

   // to not register a model, set Emax=0; eg. Emax1 = 0.
   const G4double Emax1 = 200*MeV, Emax2 = 10*GeV;

   if (Emax1 > 0.) {  // model 1
     G4LowEGammaNuclearModel* model1 = new G4LowEGammaNuclearModel();
     model1->SetMaxEnergy(Emax1);
     process->RegisterMe(model1);
   }
   
   if (Emax2 > 0.) {  // model 2   
     G4CascadeInterface* model2 = new G4CascadeInterface();
     G4double Emin2 = std::max(Emax1-1*MeV, 0.);
     model2->SetMinEnergy(Emin2);   
     model2->SetMaxEnergy(Emax2);
     process->RegisterMe(model2);
   }
   
   G4ProcessManager* pManager = G4Gamma::Gamma()->GetProcessManager();
   pManager->AddDiscreteProcess(process);
}

