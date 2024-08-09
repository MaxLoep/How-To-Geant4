/*
For all functions containing GDML stuff



*/

#include "DetectorConstruction.hh"      //Header file where functions classes and variables may be defined (...)

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

void DetectorConstruction::SetOnlyLoadGDML( G4bool value )
{
	G4cout  << "Only-GDML choice is now " << value << G4endl;
	fOnlyLoadChoice = value;
}










// // GDML-Stuff
// // ----------------------------------------------------------------------------------------------------------------------------------------------------------------------
// #pragma region
// 	G4VPhysicalVolume* DetectorConstruction::ConstructVolumesGDML()
// 	{

// 		// Cleanup old geometry - needed if you want to change parameters via macro commands
// 		G4GeometryManager::GetInstance()->OpenGeometry();
// 		G4PhysicalVolumeStore::GetInstance()->Clean();
// 		G4LogicalVolumeStore::GetInstance()->Clean();
// 		G4SolidStore::GetInstance()->Clean();

// 		// LOAD GDML FILE
// 		// default value = 0
// 		// you need to set it to 1 by using the macro command 'SetLoadGDMLFile' to read a GDML file
// 		if(fLoadingChoice==1 && fOnlyLoadChoice==1)	//Load only a GDML file
// 		{
// 			// print for DEBUGGING
// 			G4cout << "\n ----READING GDML!---- " << G4endl;

// 			LoadGDML(fLoadFile); // load a Geometry from GDML file
// 		}
// 		else if(fLoadingChoice==1 && fOnlyLoadChoice==0)	//Load a GDML file and construct other volumes in this file
// 		{
// 			// print for DEBUGGING
// 			G4cout << "\n ----READING GDML AND CONSTRUCTING VOLUMES!---- " << G4endl;

// 			LoadGDML(fLoadFile);                  // load a Geometry from GDML file
// 			fWorldPhysVol = ConstructVolumes();   // construct volumes as defined in this file
// 		}
// 		else if( fLoadingChoice!=1 && fOnlyLoadChoice==1 )	//dont load a GDML file but construct volumes in this file
// 		{
// 			// print for DEBUGGING
// 			G4cout << "\n ----You did not load any GDML file---- " << G4endl;
// 			G4cout << "\n ----CONSTRUCTING VOLUMES instead!---- " << G4endl;

// 			fWorldPhysVol = ConstructVolumes(); // construct volumes as defined in this file
// 		}
// 		//Why did i do this else-case? Isnt it identically to the case before?
// 		else // if no GDML file is loaded, geometries will be build as defined as in "ConstructVolumes()"
// 		{
// 			// print for DEBUGGING
// 			G4cout << "\n ----CONSTRUCTING VOLUMES!---- " << G4endl;

// 			fWorldPhysVol = ConstructVolumes(); // construct volumes as defined in this file
// 		}

// 		// always return the root volume
// 		return fWorldPhysVol;
// 	}

// 	// Define a Geometry in this file
// 	G4VPhysicalVolume* DetectorConstruction::ConstructVolumes()
// 	{
// 			// print for DEBUGGING
// 			G4cout << "\n ----ONLYLOAD is---- " << fOnlyLoadChoice <<  G4endl;

// 		// Cleanup old geometry - this already hapened in ConstructVolumesGDML
// 		// G4GeometryManager::GetInstance()->OpenGeometry();
// 		// G4PhysicalVolumeStore::GetInstance()->Clean();
// 		// G4LogicalVolumeStore::GetInstance()->Clean();
// 		// G4SolidStore::GetInstance()->Clean();

// 		//SOLIDS, GEOMETRIES, PLACEMENT, ETC.
// 		/*
// 		How to create solids
// 		It's basically a process with 3 steps:
// 		1.: Create a Geometry e.g. a Box, Cylinder, Sphere or even a Box minus a Cylinder (-> see boolean operation)
// 		2.: Make it a Logical Volume by assigning a material to it
// 		3.: Place it in your simulation
// 		*/

// 		if(fLoadingChoice==0) //no GDML file is loaded = world Volume needs to be constructed
// 		{
// 			// If no GDML file is loaded, a World volume needs to be created - otherwise it should be in the GDML file
// 			G4Box* solidWorld =
// 				new G4Box("sWorld",                       //its name
// 					0.5*world_sizeXYZ, 0.5*world_sizeXYZ, 0.5*world_sizeXYZ);     //its size

// 			// G4LogicalVolume* lWorld =
// 			lWorld =
// 				new G4LogicalVolume(solidWorld,          //its solid
// 														Vacuum(),
// 														// world_mat,           //its material
// 														// BoratedPE(),
// 														"lWorld");            //its name

// 			// G4VPhysicalVolume* fWorldPhysVol =
// 			fWorldPhysVol =
// 				new G4PVPlacement(0,                     //no rotation
// 													G4ThreeVector(),       //at (0,0,0)
// 													lWorld,            //its logical volume
// 													"pWorld",               //its name
// 													0,                     //its mother  volume
// 													false,                 //boolean operation?
// 													0,                     //copy number
// 													true);                 //overlaps checking?

// 			//Make world-volume invisible
// 			auto lWorldVisAtt = new G4VisAttributes(G4Color(1, 1, 1, 0.01)); //(r, g, b , transparency)
// 			lWorldVisAtt->SetVisibility(false);
// 			lWorld->SetVisAttributes(lWorldVisAtt);

// 		}
// 		else // GDML file is loaded = use world volume from GDML file as global world volume
// 		{
// 			lWorld = fWorldPhysVol->GetLogicalVolume();
// 		}

// 		// print for DEBUGGING
// 		G4cout << lWorld->GetName() << " is the world volume" << G4endl;
// #pragma endregion