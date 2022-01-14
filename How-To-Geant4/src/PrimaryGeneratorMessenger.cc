/*
#include "PrimaryGeneratorMessenger.hh"

#include "PrimaryGeneratorAction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"


PrimaryGeneratorMessenger::PrimaryGeneratorMessenger(
                                                   PrimaryGeneratorAction* Gun)
:G4UImessenger(),
  fAction(Gun),fGunDir(nullptr),fDefaultCmd(nullptr),
  fRndmCmd(nullptr),fTimeCmd(nullptr)
{ 
  fGunDir = new G4UIdirectory("/custom/gun/");
  fGunDir->SetGuidance("gun control");

  fDefaultCmd = new G4UIcmdWithoutParameter("/custom/gun/setDefault",this);
  fDefaultCmd->SetGuidance("set/reset kinematic defined in PrimaryGenerator");
  fDefaultCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  fRndmCmd = new G4UIcmdWithADoubleAndUnit("/custom/gun/beamSize",this);
  fRndmCmd->SetGuidance("random lateral extension on the beam");
  fRndmCmd->SetParameterName("rBeam",false);
  fRndmCmd->SetRange("rBeam>=0.");
  fRndmCmd->SetUnitCategory("Length");
  fRndmCmd->AvailableForStates(G4State_PreInit,G4State_Idle);  

  fTimeCmd = new G4UIcmdWithADoubleAndUnit("/custom/gun/beamTime",this);
  fTimeCmd->SetGuidance("set time of exposure");
  fTimeCmd->SetParameterName("tBeam",false);
  fTimeCmd->SetRange("tBeam>=0.");
  fTimeCmd->SetUnitCategory("Time");
  fTimeCmd->AvailableForStates(G4State_PreInit,G4State_Idle);  
}


PrimaryGeneratorMessenger::~PrimaryGeneratorMessenger()
{
  delete fDefaultCmd;
  delete fRndmCmd;
  delete fTimeCmd;
  delete fGunDir;
}


void PrimaryGeneratorMessenger::SetNewValue(G4UIcommand* command,
                                               G4String newValue)
{ 
  if (command == fDefaultCmd)
   {fAction->SetDefaultKinematic();}

  if (command == fRndmCmd)
   {fAction->SetRndmBeam(fRndmCmd->GetNewDoubleValue(newValue));}

  if (command == fTimeCmd)
   {fAction->SetTimeExposure(fTimeCmd->GetNewDoubleValue(newValue));}
}
*/