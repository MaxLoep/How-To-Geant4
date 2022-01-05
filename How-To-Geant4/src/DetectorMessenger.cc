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
 fThickCmd(nullptr), fSizeYZCmd(nullptr), fIsotopeCmd(nullptr)
{
  //Create a directory for your custom commands
  fTestemDir = new G4UIdirectory("/custom/");
  fTestemDir->SetGuidance("custom commands defined in DetectorMessenger.cc");

  //Create a sub-directory for better sorting
  G4bool broadcast = false;
  fDetDir = new G4UIdirectory("/custom/directory/",broadcast);
  fDetDir->SetGuidance("a directory for custom commands");

  //
  fMaterCmd = new G4UIcmdWithAString("/custom/directory/setMat",this);
  fMaterCmd->SetGuidance("Select material of the box.");
  fMaterCmd->SetParameterName("choice",false);
  fMaterCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  fThickCmd = new G4UIcmdWithADoubleAndUnit("/custom/directory/setThickness",this);
  fThickCmd->SetGuidance("Set thickness of the absor");
  fThickCmd->SetParameterName("Thickness",false);
  fThickCmd->SetRange("Thickness>0.");
  fThickCmd->SetUnitCategory("Length");
  fThickCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
 
  fSizeYZCmd = new G4UIcmdWithADoubleAndUnit("/custom/directory/setSizeYZ",this);
  fSizeYZCmd->SetGuidance("Set transverse size of the absor");
  fSizeYZCmd->SetParameterName("Size",false);
  fSizeYZCmd->SetRange("Size>0.");
  fSizeYZCmd->SetUnitCategory("Length");
  fSizeYZCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  fIsotopeCmd = new G4UIcommand("/custom/directory/setIsotopeMat",this);
  fIsotopeCmd->SetGuidance("Build and select a material with single isotope");
  fIsotopeCmd->SetGuidance("  symbol of isotope, Z, A, density of material");
  //
  G4UIparameter* symbPrm = new G4UIparameter("isotope",'s',false);
  symbPrm->SetGuidance("isotope symbol");
  fIsotopeCmd->SetParameter(symbPrm);
  //      
  G4UIparameter* ZPrm = new G4UIparameter("Z",'i',false);
  ZPrm->SetGuidance("Z");
  ZPrm->SetParameterRange("Z>0");
  fIsotopeCmd->SetParameter(ZPrm);
  //      
  G4UIparameter* APrm = new G4UIparameter("A",'i',false);
  APrm->SetGuidance("A");
  APrm->SetParameterRange("A>0");
  fIsotopeCmd->SetParameter(APrm);  
  //    
  G4UIparameter* densityPrm = new G4UIparameter("density",'d',false);
  densityPrm->SetGuidance("density of material");
  densityPrm->SetParameterRange("density>0.");
  fIsotopeCmd->SetParameter(densityPrm);
  //
  G4UIparameter* unitPrm = new G4UIparameter("unit",'s',false);
  unitPrm->SetGuidance("unit of density");
  G4String unitList = G4UIcommand::UnitsList(G4UIcommand::CategoryOf("g/cm3"));
  unitPrm->SetParameterCandidates(unitList);
  fIsotopeCmd->SetParameter(unitPrm);
  //
  fIsotopeCmd->AvailableForStates(G4State_PreInit,G4State_Idle);  
}


DetectorMessenger::~DetectorMessenger()
{
  delete fMaterCmd;
  delete fThickCmd;
  delete fSizeYZCmd;
  delete fIsotopeCmd;
  delete fDetDir;
  delete fTestemDir;
}


void DetectorMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{ 
  if( command == fMaterCmd )
   { 
     // fDetector->SetAbsorMaterial(newValue);
   }

  if( command == fThickCmd )
   { fDetector->SetAbsorThickness(fThickCmd->GetNewDoubleValue(newValue));}

  if( command == fSizeYZCmd )
   { fDetector->SetAbsorSizeYZ(fSizeYZCmd->GetNewDoubleValue(newValue));}

  if (command == fIsotopeCmd)
   {
     G4int Z; G4int A; G4double dens;
     G4String name, unt;
     std::istringstream is(newValue);
     is >> name >> Z >> A >> dens >> unt;
     dens *= G4UIcommand::ValueOf(unt);
    //  fDetector->MaterialWithSingleIsotope (name,name,dens,Z,A);
    //  fDetector->SetAbsorMaterial(name);    
   }   
}

