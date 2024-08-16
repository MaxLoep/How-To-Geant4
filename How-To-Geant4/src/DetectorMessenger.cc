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
 fDetector(Det), fTestemDir(nullptr), fDetDir(nullptr), fGDMLDir(nullptr), 
 fOutFoldCmd(nullptr),
 fMaterCmd(nullptr),
 fTheLoadCommand(0),fTheWriteCommand(0), fTheOnlyLoadCommand(0),
 fchange_aCmd(nullptr), fchange_bCmd(nullptr), fchange_cCmd(nullptr), fchange_dCmd(nullptr), fchange_eCmd(nullptr)
{
	//Create a directory for your custom commands
	fTestemDir = new G4UIdirectory("/custom/");
	fTestemDir->SetGuidance("custom commands defined in DetectorMessenger.cc");

	//Create a sub-directory for better sorting - geometry changing variables
	G4bool broadcast = false;
	fDetDir = new G4UIdirectory("/custom/geo/",broadcast);
	fDetDir->SetGuidance("Custom commands that change the geometry.");

	//Create a sub-directory for better sorting - analysis changing variables
	fDetDir = new G4UIdirectory("/custom/ana/",broadcast);
	fDetDir->SetGuidance("Custom commands that change the analysis.");

	//Create a sub-directory for better sorting - GDML related variables
	fGDMLDir = new G4UIdirectory("/custom/GDML/",broadcast);
	fGDMLDir->SetGuidance("Custom commands related to GDML.");

	// Change output folder name
	fOutFoldCmd = new G4UIcmdWithAString("/custom/ana/setOutFolder",this);
	fOutFoldCmd->SetGuidance("Set the name of the output folder");
	fOutFoldCmd->SetParameterName("choice",false);
	fOutFoldCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

	// Change Material dummyMat
	fMaterCmd = new G4UIcmdWithAString("/custom/geo/setMat",this);
	fMaterCmd->SetGuidance("Select material of the box.");
	fMaterCmd->SetParameterName("choice",false);
	fMaterCmd->AvailableForStates(G4State_PreInit,G4State_Idle); 

	// Change GDML-load-file name
	fTheLoadCommand = new G4UIcmdWithAString("/custom/GDML/loadFile", this);
	fTheLoadCommand ->SetGuidance("READ GDML file with given name");
	fTheLoadCommand ->SetParameterName("FileRead", false);
	fTheLoadCommand ->SetDefaultValue("test.gdml");
	fTheLoadCommand ->AvailableForStates(G4State_PreInit);
	
	// Change GDML-write-file name
	fTheWriteCommand = new G4UIcmdWithAString("/custom/GDML/writeFile", this);
	fTheWriteCommand ->SetGuidance("WRITE geometry to GDML file with given name");
	fTheWriteCommand ->SetParameterName("FileWrite", false);
	fTheWriteCommand ->SetDefaultValue("wtest.gdml");
	fTheWriteCommand ->AvailableForStates(G4State_PreInit);

	// Change if only a GDML-file should be loaded
	fTheOnlyLoadCommand = new G4UIcmdWithABool("/custom/GDML/LoadOnly", this);
	fTheOnlyLoadCommand ->SetGuidance("Decide if you only want to load a GDML file (true) or if you want to load a GDML file and add Geometries via Detector Construction (false)");
	fTheOnlyLoadCommand ->SetParameterName("LoadOnly", false);
	fTheOnlyLoadCommand ->SetDefaultValue( false );
	fTheOnlyLoadCommand ->AvailableForStates(G4State_PreInit);

	// Change parameters a,b,c,d,e with Macro commands
	// Change a
	fchange_aCmd = new G4UIcmdWithADoubleAndUnit("/custom/geo/change_a",this);
	fchange_aCmd->SetGuidance("Change the value of the variable 'a'");
	fchange_aCmd->SetParameterName("a",false);
	fchange_aCmd->SetRange("a>0.");
	fchange_aCmd->SetUnitCategory("Length");
	fchange_aCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

	// Change b
	fchange_bCmd = new G4UIcmdWithADoubleAndUnit("/custom/geo/change_b",this);
	fchange_bCmd->SetGuidance("Change the value of the variable 'b'");
	fchange_bCmd->SetParameterName("b",false);
	fchange_bCmd->SetRange("b>0.");
	fchange_bCmd->SetUnitCategory("Length");
	fchange_bCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

	// Change c
	fchange_cCmd = new G4UIcmdWithADoubleAndUnit("/custom/geo/change_c",this);
	fchange_cCmd->SetGuidance("Change the value of the variable 'c'");
	fchange_cCmd->SetParameterName("c",false);
	fchange_cCmd->SetRange("c>0.");
	fchange_cCmd->SetUnitCategory("Length");
	fchange_cCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

	// Change d
	fchange_dCmd = new G4UIcmdWithADoubleAndUnit("/custom/geo/change_d",this);
	fchange_dCmd->SetGuidance("Change the value of the variable 'd'");
	fchange_dCmd->SetParameterName("d",false);
	fchange_dCmd->SetRange("d>0.");
	fchange_dCmd->SetUnitCategory("Length");
	fchange_dCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

	// Change e
	fchange_eCmd = new G4UIcmdWithADoubleAndUnit("/custom/geo/change_e",this);
	fchange_eCmd->SetGuidance("Change the value of the variable 'e'");
	fchange_eCmd->SetParameterName("e",false);
	fchange_eCmd->SetRange("e>0.");
	fchange_eCmd->SetUnitCategory("Length");
	fchange_eCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
}


DetectorMessenger::~DetectorMessenger()
{
	delete fDetDir;
	delete fTestemDir;

	// Change output folder name
	delete fOutFoldCmd;

	// Change Material dummyMat
	delete fMaterCmd;

	// Change if GDML-file should be loaded or saved
	delete fTheLoadCommand;
	delete fTheWriteCommand;
	delete fTheOnlyLoadCommand;

	// Change to parameters a,b,c,d,e 
	delete fchange_aCmd;
	delete fchange_bCmd;
	delete fchange_cCmd;
	delete fchange_dCmd;
	delete fchange_eCmd;
}

void DetectorMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{ 
	// Change output folder name
	if( command == fOutFoldCmd )
	 { fDetector->SetOutputFolder(newValue);}

	// Change Material dummyMat
	if( command == fMaterCmd )
	 { fDetector->SetAbsorMaterial(newValue);}

	// Change GDML load-file-name
	if( command == fTheLoadCommand )
	 { fDetector->SetLoadGDMLFile(newValue);}

	// Change GDML write-file-name
	if( command == fTheWriteCommand )
	 { fDetector->SetWriteGDMLFile(newValue);}

	// Change Only load GDML-file
	if( command == fTheOnlyLoadCommand )
	 { fDetector->SetOnlyLoadGDML(newValue);}

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
