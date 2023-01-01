/*
Test physics Lists. Do they work?
*/
#include "PhysicsList.hh"                     //Necessary. You need this.
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"
#include "G4ProcessManager.hh"                //Necessary. You need this.
#include "G4HadronicInteraction.hh"

#include "G4HadronPhysicsFTF_BIC.hh"            //works!
#include "G4HadronPhysicsQGS_BIC.hh"            //works!

#include "G4HadronPhysicsShielding.hh"          //works!
#include "G4HadronPhysicsShieldingLEND.hh"      //works!

#include "G4HadronPhysicsFTFP_BERT.hh"          //works!
#include "G4HadronPhysicsFTFP_BERT_HP.hh"       //works!
#include "G4HadronPhysicsFTFP_BERT_ATL.hh"      //works!
#include "G4HadronPhysicsFTFP_BERT_TRV.hh"      //works!

#include "G4HadronPhysicsQGSP_BIC.hh"           //works!
#include "G4HadronPhysicsQGSP_BIC_HP.hh"        //works!
#include "G4HadronPhysicsQGSP_BIC_AllHP.hh"     //works!
#include "G4HadronPhysicsQGSP_BERT.hh"          //works!
#include "G4HadronPhysicsQGSP_BERT_HP.hh"       //works!
#include "G4HadronPhysicsQGSP_FTFP_BERT.hh"     //works!
#include "G4HadronPhysicsFTFQGSP_BERT.hh"       //works!

#include "G4HadronPhysicsINCLXX.hh"             //works!
#include "G4HadronPhysicsNuBeam.hh"

#include "G4HadronElasticPhysics.hh"            //works!
#include "G4HadronElasticPhysicsHP.hh"          //works!
#include "G4HadronElasticPhysicsXS.hh"          //works!
#include "G4HadronElasticPhysicsPHP.hh"         //works!
#include "G4HadronElasticPhysicsLEND.hh"        //works!

#include "G4HadronInelasticQBBC.hh"             //works!

#include "G4IonPhysics.hh"                      //works!
#include "G4IonPhysicsXS.hh"                    //works!
#include "G4IonPhysicsPHP.hh"                   //works!
#include "G4IonQMDPhysics.hh"                   //works!
#include "G4IonINCLXXPhysics.hh"                //works!
#include "G4IonElasticPhysics.hh"               //works!
#include "G4IonBinaryCascadePhysics.hh"         //works!

#include "G4EmStandardPhysics.hh"               //works!
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

#include "G4StoppingPhysics.hh"                 //works!
#include "G4VHadronPhysics.hh"
#include "G4OpticalPhysics.hh"
#include "G4SpinDecayPhysics.hh"
#include "G4ChargeExchangePhysics.hh"
#include "G4MuonicAtomDecayPhysics.hh"
#include "G4FastSimulationPhysics.hh"
#include "G4HadronDElasticPhysics.hh"
#include "G4HadronHElasticPhysics.hh"
#include "G4DecayPhysics.hh"                     //works!
#include "G4RadioactiveDecayPhysics.hh"          //works!

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

  //add new units for radioActive decays
  // 
  const G4double minute = 60*second;
  const G4double hour   = 60*minute;
  const G4double day    = 24*hour;
  const G4double year   = 365*day;
  new G4UnitDefinition("minute", "min", "Time", minute);
  new G4UnitDefinition("hour",   "h",   "Time", hour);
  new G4UnitDefinition("day",    "d",   "Time", day);
  new G4UnitDefinition("year",   "y",   "Time", year);

  // Hadron Elastic scattering
 // RegisterPhysics( new HadronElasticPhysicsHP(verb) );
  
  // Hadron Inelastic Physics
 // RegisterPhysics( new G4HadronPhysicsFTFP_BERT_HP(verb));
  ////RegisterPhysics( new G4HadronPhysicsQGSP_BIC_HP(verb));
  ////RegisterPhysics( new G4HadronInelasticQBBC(verb));        
  ////RegisterPhysics( new G4HadronPhysicsINCLXX(verb));

  // Ion Elastic scattering
  //RegisterPhysics( new G4IonElasticPhysics(verb));

  // Ion Inelastic Physics
  //RegisterPhysics( new G4IonPhysicsXS(verb));
  ////RegisterPhysics( new G4IonINCLXXPhysics(verb));
  
  // stopping Particles
  //RegisterPhysics( new G4StoppingPhysics(verb));
      
  // Gamma-Nuclear Physics
  //RegisterPhysics( new GammaNuclearPhysics("gamma"));

  // EM physics
  //RegisterPhysics(new ElectromagneticPhysics());
  ////RegisterPhysics(new G4EmStandardPhysics(verb));
  
  // Decay
  //RegisterPhysics(new G4DecayPhysics());

  // Radioactive decay
  //RegisterPhysics(new G4RadioactiveDecayPhysics());

//--------------------------------------------------------------------------

  // Hadron Elastic scattering
  //fHadronElastic = new G4HadronElasticPhysics(verb);
  fHadronElastic = new G4HadronElasticPhysicsHP(verb);
  //fHadronElastic = new G4HadronElasticPhysicsXS(verb);
  //fHadronElastic = new G4HadronElasticPhysicsPHP(verb);
  //fHadronElastic = new G4HadronElasticPhysicsLEND(verb);

  //fHadronElastic = new HadronElasticPhysicsHP(verb);
  RegisterPhysics(fHadronElastic);
  
  // Hadron Inelastic Physics
  //fHadronInelastic = new G4HadronPhysicsFTF_BIC(verb);
  //fHadronInelastic = new G4HadronPhysicsQGS_BIC(verb);
  //fHadronInelastic = new G4HadronPhysicsShielding(verb);
  //fHadronInelastic = new G4HadronPhysicsShieldingLEND(verb);
  //fHadronInelastic = new G4HadronPhysicsFTFP_BERT(verb);
  fHadronInelastic = new G4HadronPhysicsFTFP_BERT_HP(verb);
  //fHadronInelastic = new G4HadronPhysicsFTFP_BERT_ATL(verb);
  //fHadronInelastic = new G4HadronPhysicsFTFP_BERT_TRV(verb);
  //fHadronInelastic = new G4HadronPhysicsQGSP_BIC(verb);
  //fHadronInelastic = new G4HadronPhysicsQGSP_BIC_HP(verb);
  //fHadronInelastic = new G4HadronPhysicsQGSP_BIC_AllHP(verb);
  //fHadronInelastic = new G4HadronPhysicsQGSP_BERT(verb);
  //fHadronInelastic = new G4HadronPhysicsQGSP_BERT_HP(verb);
  //fHadronInelastic = new G4HadronPhysicsQGSP_FTFP_BERT(verb);
  //fHadronInelastic = new G4HadronPhysicsFTFQGSP_BERT(verb);

  //fHadronInelastic = new G4HadronInelasticQBBC(verb);
  //fHadronInelastic = new G4HadronPhysicsINCLXX(verb);
  RegisterPhysics(fHadronInelastic);

  // Ion Elastic Physics
  fIonElastic = new G4IonElasticPhysics(verb);
  RegisterPhysics(fIonElastic);

  // Ion Inelastic Physics
  //fIonInelastic = new G4IonPhysics(verb);
  fIonInelastic = new G4IonPhysicsXS(verb);
  //fIonInelastic = new G4IonPhysicsPHP(verb);
  //fIonInelastic = new G4IonINCLXXPhysics(verb);
  //fIonInelastic = new G4IonQMDPhysics(verb);
  //fIonInelastic = new G4IonBinaryCascadePhysics(verb);
  
  RegisterPhysics(fIonInelastic);

  // stopping Particles
  fStopping = new G4StoppingPhysics(verb);
  RegisterPhysics(fStopping);

  // Gamma-Nuclear Physics
  fGammaNuclear = new GammaNuclearPhysics("gamma");
  RegisterPhysics(fGammaNuclear);

  // EM physics
  fElectromagnetic = new ElectromagneticPhysics();
  //fElectromagnetic = new G4EmStandardPhysics(verb);
  RegisterPhysics(fElectromagnetic);

  // Decay
  fDecay = new G4DecayPhysics(verb);
  RegisterPhysics(fDecay);

  // Radioactive decay
  fRadioactiveDecay = new G4RadioactiveDecayPhysics(verb);
  //RegisterPhysics(fRadioactiveDecay);
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
  //fRadioactiveDecay->ConstructProcess();
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
