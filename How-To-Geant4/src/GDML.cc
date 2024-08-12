/*
For all functions containing GDML stuff
*/

#include "DetectorConstruction.hh"      //Header file where functions classes and variables may be defined (...)
#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"

// LoadGDML - function that loads a GDML file into the geometry
void DetectorConstruction::LoadGDML( const G4String& File )
{
	// **** LOOK HERE*** FOR READING GDML FILES
	// ACTIVATING OVERLAP CHECK when read volumes are placed.
	// Can take long time in case of complex geometries
	fParser.SetOverlapCheck(true);

	fParser.Read(File);

	// READING GDML FILES OPTION: 2nd Boolean argument "Validate".
	// Flag to "false" disables check with the Schema when reading GDML file.
	// See the GDML Documentation for more information.
	// fParser.Read(fLoadFile,false);
		 
	// Prints the material information
	// G4cout << *(G4Material::GetMaterialTable() ) << G4endl;
				 
	// Giving World Physical Volume from GDML Parser 
	fWorldPhysVol = fParser.GetWorldVolume(); 
}

// SaveGDML - function that saves the geometry into a GDML file
void DetectorConstruction::SaveGDML( const G4String& File )
{
	// **** LOOK HERE*** FOR WRITING GDML FILES
	// If everything is constructed, then you can save the Geometry in a GDML file 

	// OPTION: TO ADD MODULE AT DEPTH LEVEL ...
	// Can be a integer or a pointer to the top Physical Volume:
	// G4int depth=1;
	// fParser.AddModule(depth);
			
	// OPTION: SETTING ADDITION OF POINTER TO NAME TO FALSE
	// By default, written names in GDML consist of the given name with
	// appended the pointer reference to it, in order to make it unique.
	// Naming policy can be changed by using the following method, or
	// calling Write with additional Boolean argument to "false".
	// NOTE: you have to be sure not to have duplication of names in your Geometry Setup.
	// fParser.SetAddPointerToName(false); //<- does not work?
	// or
	// Writing Geometry to GDML File
	fParser.Write(fWriteFile, fWorldPhysVol, false);
	// fParser.Write(fWriteFile, fWorldPhysVol);
			
	// OPTION: SET MAXIMUM LEVEL TO EXPORT (REDUCED TREE)...
	// Can be a integer greater than zero:
	// G4int maxlevel=3;
	// fParser.SetMaxExportLevel(maxlevel);
}

// SetGDMLLoadFile - Function for changing the name of the File that gets loaded via Macro Command
void DetectorConstruction::SetLoadGDMLFile( const G4String& File )
{
	G4cout  << "GDML load file is now " << File << G4endl;
	fLoadFile=File;
	fLoadingChoice=1;
}

// SetGDMLWriteFile - Function for changing the name of the File that gets saved as GDML via Macro Command
void DetectorConstruction::SetWriteGDMLFile( const G4String& File )
{
	G4cout  << "GDML write file is now " << File << G4endl;
	fWriteFile=File;
	fWritingChoice=1;
}

// SetOnlyLoadGDML - Function to so the bool variable 'fOnlyLoadChoice' to TRUE or FALSE
// -> decide if want want to only load a GDML (fOnlyLoadChoice=TRUE) or 
// 	if you want to load a GDML and construct additional stuff around it (fOnlyLoadChoice=FALSE)
void DetectorConstruction::SetOnlyLoadGDML( G4bool value )
{
	G4cout  << "Only-GDML choice is now " << value << G4endl;
	fOnlyLoadChoice = value;
}

// ConstructVolumesGDML - Function that gets ALWAYS executed in 'DetectorConstriction.cc' 
// 1. - clean up old geometry; must be done for changing dimensions,materials,etc. via macrofile
// 2. - check if a GDML-file should be loaded and check following cases:
// 2.a- only load a GDML-file: LoadGDML()
// 2.b- load a GDML-file and add geometry via 'DetectorConstruction.cc': LoadGDML() + ConstructVolumes()
// 2.c- don't load a GDML-file but construct geometry in 'DetectorConstruction.cc': ConstructVolumes()
// 2.d- error handling: but construct geometry in 'DetectorConstruction.cc': ConstructVolumes()
// 3. - always return the root volume: return fWorldPhysVol
G4VPhysicalVolume* DetectorConstruction::ConstructVolumesGDML()
{
	// Cleanup old geometry - needed if you want to change parameters via macro commands
	G4GeometryManager::GetInstance()->OpenGeometry();
	G4PhysicalVolumeStore::GetInstance()->Clean();
	G4LogicalVolumeStore::GetInstance()->Clean();
	G4SolidStore::GetInstance()->Clean();

	// LOAD GDML FILE
	// default value = 0
	// you need to set it to 1 by using the macro command 'SetLoadGDMLFile' to read a GDML file
	if(fLoadingChoice==1 && fOnlyLoadChoice==1)	//Load only a GDML file
	{
		// print for DEBUGGING
		G4cout << "\n ----READING GDML!---- " << G4endl;

		LoadGDML(fLoadFile); // load a Geometry from GDML file
	}
	else if(fLoadingChoice==1 && fOnlyLoadChoice==0)	//Load a GDML file and construct other volumes in this file
	{
		// print for DEBUGGING
		G4cout << "\n ----READING GDML AND CONSTRUCTING VOLUMES!---- " << G4endl;

		LoadGDML(fLoadFile);                  // load a Geometry from GDML file
		fWorldPhysVol = ConstructVolumes();   // construct volumes as defined in this file
	}
	else if( fLoadingChoice!=1 && fOnlyLoadChoice==1 )	//dont load a GDML file but construct volumes in this file
	{
		// print for DEBUGGING
		G4cout << "\n ----You did not load any GDML file---- " << G4endl;
		G4cout << "\n ----CONSTRUCTING VOLUMES instead!---- " << G4endl;

		fWorldPhysVol = ConstructVolumes(); // construct volumes as defined in this file
	}
	//Why did i do this else-case? Isn't it identically to the case before?
	else // if no GDML file is loaded, geometries will be build as defined as in "ConstructVolumes()"
	{
		// print for DEBUGGING
		G4cout << "\n ----CONSTRUCTING VOLUMES!---- " << G4endl;

		fWorldPhysVol = ConstructVolumes(); // construct volumes as defined in this file
	}

	// always return the root volume
	return fWorldPhysVol;
}