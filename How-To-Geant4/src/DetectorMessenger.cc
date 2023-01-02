/*
This file contains examples on how to create custom GUI-commands and macro-file-commands.
The actual functions can be found in files where they change variables, e.g. DetectorConstruction.cc
*/

#include "DetectorMessenger.hh"

#include "DetectorConstruction.hh"
#include "G4UIdirectory.hh"               //to create directories to sort your custom commands
#include "G4UIcommand.hh"
#include "G4UIparameter.hh"

#include "G4UIcmdWithAString.hh"          //for commands with a string
#include "G4UIcmdWithABool.hh"            //for commands with a bool
#include "G4UIcmdWithAnInteger.hh"        //for commands with an integer
#include "G4UIcmdWithADouble.hh"          //for commands with a double
#include "G4UIcmdWithADoubleAndUnit.hh"   //for commands with a double and a unit
#include "G4UIcmdWithoutParameter.hh"     //for commands without a parameter

DetectorMessenger::DetectorMessenger(DetectorConstruction * Det)
:G4UImessenger(), 
 fDetector(Det), fTestemDir(nullptr), fDetDir(nullptr), fMaterCmd(nullptr),
//  fThickCmd(nullptr), fSizeYZCmd(nullptr),
  // fIsotopeCmd(nullptr),
 fchange_aCmd(nullptr), fchange_bCmd(nullptr), fchange_cCmd(nullptr), fchange_dCmd(nullptr), fchange_eCmd(nullptr)
{
  //Create a directory for your custom commands
  fTestemDir = new G4UIdirectory("/custom/");
  fTestemDir->SetGuidance("custom commands defined in DetectorMessenger.cc");

  //Create a sub-directory for better sorting
  G4bool broadcast = false;
  fDetDir = new G4UIdirectory("/custom/directory/",broadcast);
  fDetDir->SetGuidance("a directory for custom commands");

  //
  fMaterCmd = new G4UIcmdWithAString("/custom/det/setMat",this);
  fMaterCmd->SetGuidance("Select material of the box.");
  fMaterCmd->SetParameterName("choice",false);
  fMaterCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  // fThickCmd = new G4UIcmdWithADoubleAndUnit("/custom/det/setThickness",this);
  // fThickCmd->SetGuidance("Set thickness of the absor");
  // fThickCmd->SetParameterName("Thickness",false);
  // fThickCmd->SetRange("Thickness>0.");
  // fThickCmd->SetUnitCategory("Length");
  // fThickCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
 
  // fSizeYZCmd = new G4UIcmdWithADoubleAndUnit("/custom/det/setSizeYZ",this);
  // fSizeYZCmd->SetGuidance("Set transverse size of the absor");
  // fSizeYZCmd->SetParameterName("Size",false);
  // fSizeYZCmd->SetRange("Size>0.");
  // fSizeYZCmd->SetUnitCategory("Length");
  // fSizeYZCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  // fIsotopeCmd = new G4UIcommand("/custom/det/setIsotopeMat",this);
  // fIsotopeCmd->SetGuidance("Build and select a material with single isotope");
  // fIsotopeCmd->SetGuidance("  symbol of isotope, Z, A, density of material");
  // //
  // G4UIparameter* symbPrm = new G4UIparameter("isotope",'s',false);
  // symbPrm->SetGuidance("isotope symbol");
  // fIsotopeCmd->SetParameter(symbPrm);
  // //      
  // G4UIparameter* ZPrm = new G4UIparameter("Z",'i',false);
  // ZPrm->SetGuidance("Z");
  // ZPrm->SetParameterRange("Z>0");
  // fIsotopeCmd->SetParameter(ZPrm);
  // //      
  // G4UIparameter* APrm = new G4UIparameter("A",'i',false);
  // APrm->SetGuidance("A");
  // APrm->SetParameterRange("A>0");
  // fIsotopeCmd->SetParameter(APrm);  
  // //    
  // G4UIparameter* densityPrm = new G4UIparameter("density",'d',false);
  // densityPrm->SetGuidance("density of material");
  // densityPrm->SetParameterRange("density>0.");
  // fIsotopeCmd->SetParameter(densityPrm);
  // //
  // G4UIparameter* unitPrm = new G4UIparameter("unit",'s',false);
  // unitPrm->SetGuidance("unit of density");
  // G4String unitList = G4UIcommand::UnitsList(G4UIcommand::CategoryOf("g/cm3"));
  // unitPrm->SetParameterCandidates(unitList);
  // fIsotopeCmd->SetParameter(unitPrm);
  // //
  // fIsotopeCmd->AvailableForStates(G4State_PreInit,G4State_Idle);  

  // Change parameters a,b,c,d,e with Macro commands
  // Change a
  fchange_aCmd = new G4UIcmdWithADoubleAndUnit("/custom/det/change_a",this);
  fchange_aCmd->SetGuidance("Change the value of the variable 'a'");
  fchange_aCmd->SetParameterName("a",false);
  fchange_aCmd->SetRange("a>0.");
  fchange_aCmd->SetUnitCategory("Length");
  fchange_aCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  // Change b
  fchange_bCmd = new G4UIcmdWithADoubleAndUnit("/custom/det/change_b",this);
  fchange_bCmd->SetGuidance("Change the value of the variable 'b'");
  fchange_bCmd->SetParameterName("b",false);
  fchange_bCmd->SetRange("b>0.");
  fchange_bCmd->SetUnitCategory("Length");
  fchange_bCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  // Change c
  fchange_cCmd = new G4UIcmdWithADoubleAndUnit("/custom/det/change_c",this);
  fchange_cCmd->SetGuidance("Change the value of the variable 'c'");
  fchange_cCmd->SetParameterName("c",false);
  fchange_cCmd->SetRange("c>0.");
  fchange_cCmd->SetUnitCategory("Length");
  fchange_cCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  // Change d
  fchange_dCmd = new G4UIcmdWithADoubleAndUnit("/custom/det/change_d",this);
  fchange_dCmd->SetGuidance("Change the value of the variable 'd'");
  fchange_dCmd->SetParameterName("d",false);
  fchange_dCmd->SetRange("d>0.");
  fchange_dCmd->SetUnitCategory("Length");
  fchange_dCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  // Change e
  fchange_eCmd = new G4UIcmdWithADoubleAndUnit("/custom/det/change_e",this);
  fchange_eCmd->SetGuidance("Change the value of the variable 'e'");
  fchange_eCmd->SetParameterName("e",false);
  fchange_eCmd->SetRange("e>0.");
  fchange_eCmd->SetUnitCategory("Length");
  fchange_eCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
}


DetectorMessenger::~DetectorMessenger()
{
  delete fMaterCmd;
  // delete fThickCmd;
  // delete fSizeYZCmd;
  // delete fIsotopeCmd;
  delete fDetDir;
  delete fTestemDir;

  // Change to parameters a,b,c,d,e 
  delete fchange_aCmd;
  delete fchange_bCmd;
  delete fchange_cCmd;
  delete fchange_dCmd;
  delete fchange_eCmd;
}

void DetectorMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{ 
  if( command == fMaterCmd )
   { fDetector->SetAbsorMaterial(newValue);}

  // if( command == fThickCmd )
  //  { fDetector->SetAbsorThickness(fThickCmd->GetNewDoubleValue(newValue));}

  // if( command == fSizeYZCmd )
  //  { fDetector->SetAbsorSizeYZ(fSizeYZCmd->GetNewDoubleValue(newValue));}

  // if (command == fIsotopeCmd)
  //  {
  //    G4int Z; G4int A; G4double dens;
  //    G4String name, unt;
  //    std::istringstream is(newValue);
  //    is >> name >> Z >> A >> dens >> unt;
  //    dens *= G4UIcommand::ValueOf(unt);
  //    fDetector->MaterialWithSingleIsotope (name,name,dens,Z,A);
  //    fDetector->SetAbsorMaterial(name);    
  //  } 

  // Change to parameters a,b,c,d,e 
  if( command == fchange_aCmd )
   { fDetector->change_a(fchange_aCmd->GetNewDoubleValue(newValue));} 
  
  if( command == fchange_bCmd )
   { fDetector->change_b(fchange_bCmd->GetNewDoubleValue(newValue));} 

  if( command == fchange_cCmd )
   { fDetector->change_c(fchange_cCmd->GetNewDoubleValue(newValue));} 
  
  if( command == fchange_dCmd )
   { fDetector->change_d(fchange_dCmd->GetNewDoubleValue(newValue));} 

  if( command == fchange_eCmd )
   { fDetector->change_e(fchange_eCmd->GetNewDoubleValue(newValue));} 
}
