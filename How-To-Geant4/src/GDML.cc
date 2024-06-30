/*
For all functions containing GDML stuff



*/

// #include "DetectorConstruction.hh"      //Header file where functions classes and variables may be defined (...)

// // LoadGDML - function that loads a GDML file into the geometry
// void DetectorConstruction::LoadGDML( const G4String& File )
// {
// 	// **** LOOK HERE*** FOR READING GDML FILES
// 	// ACTIVATING OVERLAP CHECK when read volumes are placed.
// 	// Can take long time in case of complex geometries
// 	fParser.SetOverlapCheck(true);

// 	fParser.Read(File);

// 	// READING GDML FILES OPTION: 2nd Boolean argument "Validate".
// 	// Flag to "false" disables check with the Schema when reading GDML file.
// 	// See the GDML Documentation for more information.
// 	// fParser.Read(fLoadFile,false);
		 
// 	// Prints the material information
// 	// G4cout << *(G4Material::GetMaterialTable() ) << G4endl;
				 
// 	// Giving World Physical Volume from GDML Parser 
// 	fWorldPhysVol = fParser.GetWorldVolume(); 
// }


// // SaveGDML - function that saves the geometry into a GDML file
// void DetectorConstruction::SaveGDML( const G4String& File )
// {
// 	// **** LOOK HERE*** FOR WRITING GDML FILES
// 	// If everything is constructed, then you can save the Geometry in a GDML file 

// 	// OPTION: TO ADD MODULE AT DEPTH LEVEL ...
// 	// Can be a integer or a pointer to the top Physical Volume:
// 	// G4int depth=1;
// 	// fParser.AddModule(depth);
			
// 	// OPTION: SETTING ADDITION OF POINTER TO NAME TO FALSE
// 	// By default, written names in GDML consist of the given name with
// 	// appended the pointer reference to it, in order to make it unique.
// 	// Naming policy can be changed by using the following method, or
// 	// calling Write with additional Boolean argument to "false".
// 	// NOTE: you have to be sure not to have duplication of names in your Geometry Setup.
// 	// fParser.SetAddPointerToName(false); //<- does not work?
// 	// or
// 	// Writing Geometry to GDML File
// 	fParser.Write(fWriteFile, fWorldPhysVol, false);
// 	// fParser.Write(fWriteFile, fWorldPhysVol);
			
// 	// OPTION: SET MAXIMUM LEVEL TO EXPORT (REDUCED TREE)...
// 	// Can be a integer greater than zero:
// 	// G4int maxlevel=3;
// 	// fParser.SetMaxExportLevel(maxlevel);
// }



// // SetGDMLLoadFile - Function for changing the name of the File that gets loaded via Macro Command
// void DetectorConstruction::SetLoadGDMLFile( const G4String& File )
// {
// 	G4cout  << "GDML load file is now " << File << G4endl;
// 	fLoadFile=File;
// 	fLoadingChoice=1;
// }

// // SetGDMLWriteFile - Function for changing the name of the File that gets saved as GDML via Macro Command
// void DetectorConstruction::SetWriteGDMLFile( const G4String& File )
// {
// 	G4cout  << "GDML write file is now " << File << G4endl;
// 	fWriteFile=File;
// 	fWritingChoice=1;
// }

// void DetectorConstruction::SetOnlyLoadGDML( G4bool value )
// {
// 	G4cout  << "Only-GDML choice is now " << value << G4endl;
// 	fOnlyLoadChoice = value;
// }