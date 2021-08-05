#include "PhysicsList.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"
#include "G4ProcessManager.hh"
#include "G4HadronicInteraction.hh"

#include "G4HadronPhysicsFTF_BIC.hh"
#include "G4HadronPhysicsQGS_BIC.hh"

#include "G4HadronPhysicsShielding.hh"
#include "G4HadronPhysicsShieldingLEND.hh"

#include "G4HadronPhysicsFTFP_BERT.hh"
#include "G4HadronPhysicsFTFP_BERT_HP.hh"
#include "G4HadronPhysicsFTFP_BERT_ATL.hh"
#include "G4HadronPhysicsFTFP_BERT_TRV.hh"

#include "G4HadronPhysicsQGSP_BIC.hh"
#include "G4HadronPhysicsQGSP_BIC_HP.hh"
#include "G4HadronPhysicsQGSP_BIC_AllHP.hh"
#include "G4HadronPhysicsQGSP_BERT.hh"
#include "G4HadronPhysicsQGSP_BERT_HP.hh"
#include "G4HadronPhysicsQGSP_FTFP_BERT.hh"
#include "G4HadronPhysicsFTFQGSP_BERT.hh"

#include "G4HadronPhysicsINCLXX.hh"
#include "G4HadronPhysicsNuBeam.hh"

#include "G4HadronElasticPhysics.hh"
#include "G4HadronElasticPhysicsHP.hh"
#include "G4HadronElasticPhysicsXS.hh"
#include "G4HadronElasticPhysicsPHP.hh"
#include "G4HadronElasticPhysicsLEND.hh"

#include "G4HadronInelasticQBBC.hh"

#include "G4IonPhysics.hh"
#include "G4IonPhysicsXS.hh"
#include "G4IonPhysicsPHP.hh"
#include "G4IonQMDPhysics.hh"
#include "G4IonINCLXXPhysics.hh"
#include "G4IonElasticPhysics.hh"
#include "G4IonBinaryCascadePhysics.hh"

#include "G4EmStandardPhysics.hh"
#include "G4EmStandardPhysics_option1.hh"       //_option1 to _option4 available
#include "G4EmStandardPhysicsGS.hh"
#include "G4EmStandardPhysicsSS.hh"
#include "G4EmStandardPhysicsWVI.hh"
#include "G4EmDNAPhysics.hh"
#include "G4EmDNAPhysics_stationary.hh"
#include "G4EmDNAPhysics_stationary_option2.hh" //_option2, _option4 and _option6 are available
#include "G4EmDNAPhysics_option1.hh"            //_option1 to _option8 available
#include "G4EmDNAChemistry.hh"
#include "G4EmDNAChemistry_option1.hh"          //_option1 to _option8 available
#include "G4EmExtraPhysics.hh"
#include "G4EmLEPTSPhysics.hh"
#include "G4EmLowEPPhysics.hh"
#include "G4EmPenelopePhysics.hh"
#include "G4EmLivermorePhysics.hh"
#include "G4EmLivermorePolarizedPhysics.hh"

#include "G4StoppingPhysics.hh"
#include "G4VHadronPhysics.hh"
#include "G4OpticalPhysics.hh"
#include "G4SpinDecayPhysics.hh"
#include "G4ChargeExchangePhysics.hh"
#include "G4MuonicAtomDecayPhysics.hh"
#include "G4FastSimulationPhysics.hh"
#include "G4HadronDElasticPhysics.hh"
#include "G4HadronHElasticPhysics.hh"
#include "G4DecayPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"

#include "G4Neutron.hh"

#include "ElectromagneticPhysics.hh"          //selfwritten in include-folder
#include "HadronElasticPhysicsHP.hh"          //selfwritten in include-folder
#include "GammaNuclearPhysics.hh"             //selfwritten in include-folder

PhysicsList::PhysicsList()
:G4VModularPhysicsList(),
 fHadronElastic(nullptr), fHadronInelastic(nullptr),
 fIonElastic(nullptr), fIonInelastic(nullptr),
 fGammaNuclear(nullptr), fElectromagnetic(nullptr),
 fDecay(nullptr), fRadioactiveDecay(nullptr), fStopping(nullptr)
{
  G4int verb = 0;
  SetVerboseLevel(verb);
  
  //add new units
  //
  new G4UnitDefinition( "millielectronVolt", "meV", "Energy", 1.e-3*eV);
  new G4UnitDefinition( "mm2/g",  "mm2/g", "Surface/Mass", mm2/g);
  new G4UnitDefinition( "um2/mg", "um2/mg","Surface/Mass", um*um/mg);
    
  // Hadron Elastic scattering
  fHadronElastic = new HadronElasticPhysicsHP(verb);
  RegisterPhysics(fHadronElastic);
  
  // Hadron Inelastic Physics
  //fHadronInelastic = new G4HadronPhysicsFTFP_BERT_HP(verb);
  //fHadronInelastic = new G4HadronPhysicsQGSP_BIC(verb);
  //fHadronInelastic = new G4HadronPhysicsQGSP_BIC_HP(verb);
  //fHadronInelastic = new G4HadronPhysicsQGSP_BIC_AllHP(verb);
  fHadronInelastic = new G4HadronInelasticQBBC(verb);
  //fHadronInelastic = new G4HadronPhysicsINCLXX(verb);
  //fHadronInelastic = new FTFP_INCLXX_HP(verb);
  //fHadronInelastic = new QGSP_INCLXX_HP(verb);
  RegisterPhysics(fHadronInelastic);

  // Ion Elastic Physics
  fIonElastic = new G4IonElasticPhysics(verb);
  RegisterPhysics(fIonElastic);

  // Ion Inelastic Physics
  fIonInelastic = new G4IonPhysicsXS(verb);
  //fIonInelastic = new G4IonPhysicsPHP(verb);
  //fIonInelastic = new G4IonINCLXXPhysics(verb);
  RegisterPhysics(fIonInelastic);

  // stopping Particles
  fStopping = new G4StoppingPhysics(verb);
  RegisterPhysics(fStopping);

  // Gamma-Nuclear Physics
  fGammaNuclear = new GammaNuclearPhysics("gamma");
  RegisterPhysics(fGammaNuclear);

  // EM physics
  //fElectromagnetic = new ElectromagneticPhysics();
  fElectromagnetic = new G4EmStandardPhysics();
  RegisterPhysics(fElectromagnetic);

  // Decay
  fDecay = new G4DecayPhysics();
  RegisterPhysics(fDecay);

  // Radioactive decay
  fRadioactiveDecay = new G4RadioactiveDecayPhysics();
  RegisterPhysics(fRadioactiveDecay);
}



PhysicsList::~PhysicsList()
{ }



void PhysicsList::ConstructProcess()
{
  // Transportation first (mandatory)
  //
  AddTransportation();

  // Physics constructors
  //
  fHadronElastic->ConstructProcess();
  fHadronInelastic->ConstructProcess();
  fIonElastic->ConstructProcess();
  fIonInelastic->ConstructProcess();
  fGammaNuclear->ConstructProcess();
  fElectromagnetic->ConstructProcess();
  fDecay->ConstructProcess();
  fRadioactiveDecay->ConstructProcess();
  fStopping->ConstructProcess();


  // example of GetHadronicModel (due to bug in QGSP_BIC_AllHP)
  //
//  G4ProcessManager* pManager = G4Neutron::Neutron()->GetProcessManager();
//  G4HadronicProcess* process
//       = dynamic_cast<G4HadronicProcess*>(pManager->GetProcess("nCapture"));
//  G4HadronicInteraction* model = process->GetHadronicModel("nRadCapture");
//  if(model) model->SetMinEnergy(19.9*MeV);
}



void PhysicsList::SetCuts()
{
  SetCutValue(0*mm, "proton");
  SetCutValue(10*km, "e-");
  SetCutValue(10*km, "e+");
  SetCutValue(10*km, "gamma");
}


