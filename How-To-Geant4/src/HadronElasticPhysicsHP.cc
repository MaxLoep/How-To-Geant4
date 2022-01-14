// HP models for neutron < 20 MeV

#include "HadronElasticPhysicsHP.hh"

#include "NeutronHPMessenger.hh"

#include "G4HadronicProcess.hh"
#include "G4ParticleHPElastic.hh"
#include "G4ParticleHPElasticData.hh"
#include "G4ParticleHPThermalScattering.hh"
#include "G4ParticleHPThermalScatteringData.hh"

#include "G4SystemOfUnits.hh"


HadronElasticPhysicsHP::HadronElasticPhysicsHP(G4int ver)
: G4HadronElasticPhysics(ver),
  fThermal(false), fNeutronMessenger(0)  
{
  fNeutronMessenger   = new NeutronHPMessenger(this);  
}


HadronElasticPhysicsHP::~HadronElasticPhysicsHP()
{
  delete fNeutronMessenger;  
}


void HadronElasticPhysicsHP::ConstructProcess()
{
  G4HadronElasticPhysics::ConstructProcess();
  GetNeutronModel()->SetMinEnergy(19.5*MeV);

  G4HadronicProcess* process = GetNeutronProcess();
  G4ParticleHPElastic* model1 = new G4ParticleHPElastic();
  process->RegisterMe(model1);
  process->AddDataSet(new G4ParticleHPElasticData());

  if (fThermal) {
    model1->SetMinEnergy(4*eV);
    G4ParticleHPThermalScattering* model2 = new G4ParticleHPThermalScattering();
    process->RegisterMe(model2);
    process->AddDataSet(new G4ParticleHPThermalScatteringData());  
  }
}

