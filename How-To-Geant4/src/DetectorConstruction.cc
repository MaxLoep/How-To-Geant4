/*
This file contains examples on how to define materials and how to create geometries and place them.
There are no external dependencies, you find everything in this file. -> You can copy code snippets and paste them in your simulation.
Remember to include the header-files in your simulation, e.g. if you want to place a Box, you have to put ' #include "G4Box.hh" ' in your file as well.
*/


#include "DetectorConstruction.hh"      //Header file where functions classes and variables may be defined (...)
#include "DetectorMessenger.hh"         //Header file for own macro commands
#include "G4RunManager.hh"              //Necessary. You need this.

#include "G4NistManager.hh"             //for getting material definitions from the NIST database
#include "G4Material.hh"				

#include "G4Box.hh"                     //for cuboid
#include "G4Sphere.hh"                  //for sphere
#include "G4LogicalVolume.hh"           //Necessary. You need this.
#include "G4PVPlacement.hh"             //Necessary. You need this.
#include "G4SystemOfUnits.hh"           //for units
#include "G4UnitsTable.hh"              //for units
#include "G4PhysicalConstants.hh"       //for physial constants like pi

#include "G4VisAttributes.hh"           //for Visualization
#include "G4Color.hh"                   //for Visualization

//Primitive Scorer from example B4d
#include "G4SDManager.hh"
#include "G4SDChargedFilter.hh"
#include "G4MultiFunctionalDetector.hh"
#include "G4VPrimitiveScorer.hh"
#include "G4PSEnergyDeposit.hh"
#include "G4PSTrackLength.hh"
#include "G4PSFlatSurfaceCurrent.hh"

#include "G4SDParticleWithEnergyFilter.hh"
#include "G4SDParticleFilter.hh"
#include "G4SDChargedFilter.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"

#include "SensitiveDetector.hh"         // the SensitiveDetector file
#include "CADMesh.hh"                   // for importing CAD-files (.stl, .obj, ...). Read all about it at: https://github.com/christopherpoole/CADMesh
#include "G4GDMLParser.hh"              // for importing/exporting GDML-files

DetectorConstruction::DetectorConstruction()
:G4VUserDetectorConstruction(),
 fAbsorMaterial(nullptr), fLAbsor(nullptr), world_mat(nullptr), fDetectorMessenger(nullptr),
 fScoringVolume(0)
{
	// for reading and writing GDML
	fLoadFile  ="test.gdml";
	fWriteFile ="wtest.gdml";
	fWritingChoice = 0;
	fLoadingChoice = 0;
	fOnlyLoadChoice = false;


	// World Size
	world_sizeXYZ = 20.*m;

	//set box parameters
	boxX  = 20. *cm;
	boxY  = 20. *cm;
	boxZ  = 20. *cm;

	// set dummy variables
	a = 20.*cm; // used for x- and y-width of Sensitive Detectors
	b = 10.*cm; // unused
	c = 1.*cm;  // unused
	d = 1.*cm;  // unused
	e = 1.*cm;  // unused

	// materials
	DefineMaterials(); // see Materials.cc for defined Materials
	// SetAbsorMaterial("G4_Co");
	//Print all defined materials to console
	G4cout << *(G4Material::GetMaterialTable()) << G4endl;

	// create commands for interactive definition of the geometry
	fDetectorMessenger = new DetectorMessenger(this);
}

DetectorConstruction::~DetectorConstruction()
{ delete fDetectorMessenger; }

G4VPhysicalVolume* DetectorConstruction::Construct()
{
	return ConstructVolumesGDML();
	// return ConstructVolumes();   // before GDML stuff was added this line was used to construct the Geometry
}


//GDML-Stuff
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------
#pragma region
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
		//Why did i do this else-case? Isnt it identically to the case before?
		else // if no GDML file is loaded, geometries will be build as defined as in "ConstructVolumes()"
		{
			// print for DEBUGGING 
			G4cout << "\n ----CONSTRUCTING VOLUMES!---- " << G4endl;

			fWorldPhysVol = ConstructVolumes(); // construct volumes as defined in this file
		} 

		// always return the root volume
		return fWorldPhysVol;
	}

	// Define a Geometry in this file
	G4VPhysicalVolume* DetectorConstruction::ConstructVolumes()
	{
			// print for DEBUGGING 
			G4cout << "\n ----ONLYLOAD is---- " << fOnlyLoadChoice <<  G4endl;

		// Cleanup old geometry - this already hapened in ConstructVolumesGDML
		// G4GeometryManager::GetInstance()->OpenGeometry();
		// G4PhysicalVolumeStore::GetInstance()->Clean();
		// G4LogicalVolumeStore::GetInstance()->Clean();
		// G4SolidStore::GetInstance()->Clean();

		//SOLIDS, GEOMETRIES, PLACEMENT, ETC.
		/*
		How to create solids
		It's basically a process with 3 steps:
		1.: Create a Geometry e.g. a Box, Cylinder, Sphere or even a Box minus a Cylinder (-> see boolean operation)
		2.: Make it a Logical Volume by assigning a material to it
		3.: Place it in your simulation
		*/ 

		if(fLoadingChoice==0) //no GDML file is loaded = world Volume needs to be constructed
		{
			// If no GDML file is loaded, a World volume needs to be created - otherwise it should be in the GDML file
			G4Box* solidWorld =    
				new G4Box("sWorld",                       //its name
					0.5*world_sizeXYZ, 0.5*world_sizeXYZ, 0.5*world_sizeXYZ);     //its size
					
			// G4LogicalVolume* lWorld =  
			lWorld =                       
				new G4LogicalVolume(solidWorld,          //its solid
														world_mat,           //its material
														"lWorld");            //its name
																			
			// G4VPhysicalVolume* fWorldPhysVol = 
			fWorldPhysVol = 
				new G4PVPlacement(0,                     //no rotation
													G4ThreeVector(),       //at (0,0,0)
													lWorld,            //its logical volume
													"pWorld",               //its name
													0,                     //its mother  volume
													false,                 //boolean operation?
													0,                     //copy number
													true);                 //overlaps checking?

			//Make world-volume invisible
			auto lWorldVisAtt = new G4VisAttributes(G4Color(1, 1, 1, 0.01)); //(r, g, b , transparency)
			lWorldVisAtt->SetVisibility(false);
			lWorld->SetVisAttributes(lWorldVisAtt);

		}
		else // GDML file is loaded = use world volume from GDML file as global world volume
		{
			lWorld = fWorldPhysVol->GetLogicalVolume();
		}

		// print for DEBUGGING 
		G4cout << lWorld->GetName() << " is the world volume" << G4endl;
#pragma endregion

// 
//Import Standard Geometry (1 box, 1 sphere and 5 SDs)
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------
#pragma region  
	// create a box to be used as Primitive Scorer (PS) and place it in the world volume
	//  
	// Box
	// 
	G4Box* sBox =    
		new G4Box("sBox",                        //its name
				boxX/2, boxY/2, boxZ/2);                   //its size: half x, half y, half z
			
	G4LogicalVolume* lBox =                         
		new G4LogicalVolume(sBox,                //its solid
												// boxMaterial,           //its material
												// Vacuum,
												Aluminum,
												"lBox");              //its name
	
	//G4VPhysicalVolume* physBox=              //you can declare a varibale for placement but it will create a warning if unused   
		new G4PVPlacement(0,                     //no rotation
							G4ThreeVector(0,0,80.*cm),     //position
							lBox,                          //its logical volume
							"pBox",                         //its name
							lWorld,
							// lWorld,                    //its mother  volume
							false,                         //boolean operation?
							0,                             //copy number
							true);                         //overlaps checking?

	//Make (in-)visible and give it a color
	//lBox->SetVisAttributes (G4VisAttributes::GetInvisible());
	auto lBoxVisAtt = new G4VisAttributes(G4Color(1, 0, 0, 0.8)); //(r, g, b , transparency)
	lBoxVisAtt->SetVisibility(true);
	lBox->SetVisAttributes(lBoxVisAtt);

	//create 5 flat boxes to use as Sensitive Detector (SD)
	//     
	// SD1
	// 
	G4Box* sSD1 =    
		new G4Box("sSD1",                        //its name
				a/2, a/2, 0.02*mm /2);                   //its size: half x, half y, half z
			
	G4LogicalVolume* lSD1 =                         
		new G4LogicalVolume(sSD1,                //its solid
												Aluminum,           //its material
												"lSD1");              //its name
		
		new G4PVPlacement(0,                     //no rotation
							G4ThreeVector(0,0,10.*cm),     //position
							lSD1,                          //its logical volume
							"pSD1",                         //its name
							lWorld,
							// lWorld,                    //its mother  volume
							false,                         //boolean operation?
							0,                             //copy number
							true);                         //overlaps checking?

	//Make (in-)visible and give it a color
	auto lSD1VisAtt = new G4VisAttributes(G4Color(0, 0, 1, 0.8)); //(r, g, b , transparency)
	lSD1VisAtt->SetVisibility(true);
	lSD1->SetVisAttributes(lSD1VisAtt);

	//     
	// SD2
	// 
	G4Box* sSD2 =    
		new G4Box("sSD2",                        //its name
				a/2, a/2, 0.02*mm /2);                   //its size: half x, half y, half z
			
	G4LogicalVolume* lSD2 =                         
		new G4LogicalVolume(sSD2,                //its solid
												Titanium,           //its material
												"lSD2");              //its name
		
		new G4PVPlacement(0,                     //no rotation
							G4ThreeVector(0,0,20.*cm),     //position
							lSD2,                          //its logical volume
							"pSD2",                         //its name
							lWorld,
							// lWorld,                    //its mother  volume
							false,                         //boolean operation?
							0,                             //copy number
							true);                         //overlaps checking?

	//Make (in-)visible and give it a color
	auto lSD2VisAtt = new G4VisAttributes(G4Color(0, 0, 1, 0.8)); //(r, g, b , transparency)
	lSD2VisAtt->SetVisibility(true);
	lSD2->SetVisAttributes(lSD2VisAtt);

	//     
	// SD3
	// 
	G4Box* sSD3 =    
		new G4Box("sSD3",                        //its name
				a/2, a/2, 0.02*mm /2);                   //its size: half x, half y, half z
			
	G4LogicalVolume* lSD3 =                         
		new G4LogicalVolume(sSD3,                //its solid
												Copper,           //its material
												"lSD3");              //its name
		
		new G4PVPlacement(0,                     //no rotation
							G4ThreeVector(0,0,30.*cm),     //position
							lSD3,                          //its logical volume
							"pSD3",                         //its name
							lWorld,
							// lWorld,                    //its mother  volume
							false,                         //boolean operation?
							0,                             //copy number
							true);                         //overlaps checking?

	//Make (in-)visible and give it a color
	auto lSD3VisAtt = new G4VisAttributes(G4Color(0, 0, 1, 0.8)); //(r, g, b , transparency)
	lSD3VisAtt->SetVisibility(true);
	lSD3->SetVisAttributes(lSD3VisAtt);

	//     
	// SD4
	// 
	G4Box* sSD4 =    
		new G4Box("sSD4",                        //its name
				a/2, a/2, 0.02*mm /2);                   //its size: half x, half y, half z
			
	G4LogicalVolume* lSD4 =                         
		new G4LogicalVolume(sSD4,                //its solid
												Nickel,           //its material
												"lSD4");              //its name
		
		new G4PVPlacement(0,                     //no rotation
							G4ThreeVector(0,0,40.*cm),     //position
							lSD4,                          //its logical volume
							"pSD4",                         //its name
							lWorld,
							// lWorld,                    //its mother  volume
							false,                         //boolean operation?
							0,                             //copy number
							true);                         //overlaps checking?

	//Make (in-)visible and give it a color
	auto lSD4VisAtt = new G4VisAttributes(G4Color(0, 0, 1, 0.8)); //(r, g, b , transparency)
	lSD4VisAtt->SetVisibility(true);
	lSD4->SetVisAttributes(lSD4VisAtt);

	//     
	// SD5
	// 
	G4Box* sSD5 =    
		new G4Box("sSD5",                        //its name
				a/2, a/2, 0.02*mm /2);                   //its size: half x, half y, half z
				// a/2, a/2, 2.*cm /2);                   //its size: half x, half y, half z
			
	G4LogicalVolume* lSD5 =                         
		new G4LogicalVolume(sSD5,                //its solid
												// Vacuum,           //its material
												Iron,
												"lSD5");              //its name
		
		new G4PVPlacement(0,                     //no rotation
							G4ThreeVector(0,0,50.*cm),     //position
							lSD5,                          //its logical volume
							"pSD5",                         //its name
							lWorld,
							// lWorld,                    //its mother  volume
							false,                         //boolean operation?
							0,                             //copy number
							true);                         //overlaps checking?

	//Make (in-)visible and give it a color
	auto lSD5VisAtt = new G4VisAttributes(G4Color(0, 0, 1, 0.8)); //(r, g, b , transparency)
	lSD5VisAtt->SetVisibility(true);
	lSD5->SetVisAttributes(lSD5VisAtt);

	//
	// Sphere - SD to detect gammas
	//
	G4Sphere* sSphere =
		new G4Sphere("sSphere",                    //name
							2.*cm, 2.1*cm,                  //inner radius, outer radius
							0., twopi,                      //min phi, max phi
							0., pi);                        //min rho, max rho
						
	G4LogicalVolume* lSphere =
		new G4LogicalVolume(sSphere,              //shape
												Vacuum,             //material
												"lSphere");            //name

	new G4PVPlacement(0,                        //no rotation
							G4ThreeVector(0,0,0),           //position
							lSphere,                        //logical volume
							"pSphere",                       //name
							lWorld,
							// lWorld,                     //mother volume
							false,                          //boolean operation?
							0,                              //copy number
							true);                          //overlaps checking?

	//Make (in-)visible and give it a color
	auto lSphereVisAtt = new G4VisAttributes(G4Color(0, 1, 0, 0.8)); //(r, g, b , transparency)
	lSphereVisAtt->SetVisibility(true);
	lSphere->SetVisAttributes(lSphereVisAtt);
	#pragma endregion

	//FROM EXAMPLE B1 - CAN BE REMOVED
	//B1 SCORING METHOD
	//You need also Code for this one to work in:
	//SteppingAction.cc,  RunAction.cc, EventAction.cc          
	// Set logical Box volume as scoring volume (must be a logical volume)
	//This is a public variable defined in the header file to make it accessible from other files
	// fScoringVolume = lSD1;

	// PrintParameters();

	// default value = 0
	if(fWritingChoice==1) // save geometry in GDML file
		{
			SaveGDML( fWriteFile );
		}

	//always return the root volume
	return fWorldPhysVol;
	}

void DetectorConstruction::PrintParameters()
{
	G4cout << "\n The Absorber is " << G4BestUnit(boxX,"Length")
				 << " of " << boxMaterial->GetName() 
				 << "\n \n" << boxMaterial << G4endl;
}


//
//Functions for custom GUI and macro commands - see DetectorConstruction.hh, DetectorMessenger.cc, DetectorMessenger.hh
//
void DetectorConstruction::SetAbsorMaterial(G4String materialChoice)
{
	// search the material by its name
	G4Material* pttoMaterial = G4NistManager::Instance()->FindOrBuildMaterial(materialChoice);   
	
	if (pttoMaterial) { 
		dummyMat = pttoMaterial;
		G4RunManager::GetRunManager()->ReinitializeGeometry();
		G4cout << "\n The dummyMat is now "
					 << dummyMat->GetName() 
					 << "\n \n" << dummyMat << G4endl;

		if(fLAbsor) { fLAbsor->SetMaterial(fAbsorMaterial); }
		G4RunManager::GetRunManager()->PhysicsHasBeenModified();
		G4cout << "\n Weird if-case happened..." << G4endl;
	} else {
		G4cout << "\n--> warning from DetectorConstruction::SetMaterial : "
					 << materialChoice << " not found" << G4endl;
	}              
}

// Change Parameters via Macro file with these
// Change a
void DetectorConstruction::change_a(G4double value)
{
	a = value;
	G4RunManager::GetRunManager()->ReinitializeGeometry();
	G4cout  << "\n a is now " << G4BestUnit(a,"Length") << G4endl;
}

// Change b
void DetectorConstruction::change_b(G4double value)
{
	b = value;
	G4RunManager::GetRunManager()->ReinitializeGeometry();
	G4cout  << "\n b is now " << G4BestUnit(b,"Length") << G4endl;
}

// Change c
void DetectorConstruction::change_c(G4double value)
{
	c = value;
	G4RunManager::GetRunManager()->ReinitializeGeometry();
	G4cout  << "\n c is now " << G4BestUnit(c,"Length") << G4endl;
}

// Change d
void DetectorConstruction::change_d(G4double value)
{
	d = value;
	G4RunManager::GetRunManager()->ReinitializeGeometry();
	G4cout  << "\n d is now " << G4BestUnit(d,"Length") << G4endl;
}

// Change e
void DetectorConstruction::change_e(G4double value)
{
	e = value;
	G4RunManager::GetRunManager()->ReinitializeGeometry();
	G4cout  << "\n e is now " << G4BestUnit(e,"Length") << G4endl;
}

//This stuff gets moved to GDML.cc
//------------------------------------------------------------------------------------------------------------------
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
//---------------------------------------------------------------------------------------------------------------

//
//Assign Detectors and Scorers to Volume
//
void DetectorConstruction::ConstructSDandField()
{
	G4SDManager::GetSDMpointer()->SetVerboseLevel(1);

	//
	//SENSITIVE DETECTORS
	//You need also Code for this one to work in:
	//SDX.cc to specify what to quantity to track (Energy, position, etc.)
	//RunAction.cc to open a file and declare ntuple or histograms to save data in
	//Make a Volume a Sensitive Detector (SD); SD are able to access Track/Step information of Particles going through e.g. :
	//Kinetic energy, Momentum


	//Declare a Sensitive Detector
	auto sd1 = new SD1("SD1");                          //create a new Sensitive Detector
	G4SDManager::GetSDMpointer()->AddNewDetector(sd1);  //add new SD to SDManager
	SetSensitiveDetector("lSD1", sd1);                   //Apply Sensitive Detector 'sdX' to logical Volume 'SDX'

	auto sd2 = new SD2("SD2");                          //create a new Sensitive Detector
	G4SDManager::GetSDMpointer()->AddNewDetector(sd2);  //add new SD to SDManager
	SetSensitiveDetector("lSD2", sd2);                   //Apply Sensitive Detector 'sdX' to logical Volume 'SDX'

	auto sd3 = new SD3("SD3");                          //create a new Sensitive Detector
	G4SDManager::GetSDMpointer()->AddNewDetector(sd3);  //add new SD to SDManager
	SetSensitiveDetector("lSD3", sd3);                   //Apply Sensitive Detector 'sdX' to logical Volume 'SDX'

	auto sd4 = new SD4("SD4");                          //create a new Sensitive Detector
	G4SDManager::GetSDMpointer()->AddNewDetector(sd4);  //add new SD to SDManager
	SetSensitiveDetector("lSD4", sd4);                   //Apply Sensitive Detector 'sdX' to logical Volume 'SDX'

	auto sd5 = new SD5("SD5");                          //create a new Sensitive Detector
	G4SDManager::GetSDMpointer()->AddNewDetector(sd5);  //add new SD to SDManager
	SetSensitiveDetector("lSD5", sd5);                   //Apply Sensitive Detector 'sdX' to logical Volume 'SDX'

	auto sphereSD = new SphereSD("SphereSD");                   //create a new Sensitive Detector
	G4SDManager::GetSDMpointer()->AddNewDetector(sphereSD);     //add new SD to SDManager
	SetSensitiveDetector("lSphere", sphereSD);                   //Apply Sensitive Detector 'SphereSD' to logical Volume 'Sphere'


	// // 
	// //PRIMITIVE SCORERS
	// //You need also Code for this one to work in:
	// //RunAction.cc, EventAction.cc
	// //Make a Volume a Primitive Scorer (PS); PS are able to save information on events related to inside the volume e.g. :
	// //energy deposit, track length, current, flux


	// //Filters
	//Declare filters on Particles, Charge, Energy
	G4String fltName,particleName;

	// //charged particle filter
	// auto charged = new G4SDChargedFilter("chargedFilter");
	// G4SDChargedFilter* chargedFilter = new G4SDChargedFilter(fltName="chargedFilter");

	// //Proton filter
	G4SDParticleFilter* protonFilter =
	new G4SDParticleFilter(fltName="protonFilter", particleName="proton");

	//Neutron filter
	// G4SDParticleFilter* neutronFilter =
	// new G4SDParticleFilter(fltName="neutronFilter", particleName="neutron");
	
	// //Electron filter
	// G4SDParticleFilter* electronFilter =
	// new G4SDParticleFilter(fltName="electronFilter");
	// electronFilter->add(particleName="e+");   // accept electrons.
	// electronFilter->add(particleName="e-");   // accept positrons.
	
	// //Gamma filter
	// G4SDParticleFilter* gammaFilter =
	// new G4SDParticleFilter("gammaFilter", "gamma");

	// //Proton energy filter (Protons in energy range)
	// G4SDParticleWithEnergyFilter* protonEnergy=
	// new G4SDParticleWithEnergyFilter(fltName="protonEnergy");
	// protonEnergy->add("proton");
	// protonEnergy->SetKineticEnergy(200*MeV, 300*MeV); //Only particles with an energy between these values are counted as long as they are between these values

	// //Declare a volume as a MultiFunctionalDetector scorer 
	auto boxPS = new G4MultiFunctionalDetector("Scorer");
	G4SDManager::GetSDMpointer()->AddNewDetector(boxPS);

	// //Declare what quantity should be scored and apply filters
	// //
	// //Score Deposited Energy (of protons)
	G4VPrimitiveScorer* primitive;

	// //Score TrackLength (of protons)
	primitive = new G4PSTrackLength("TrackLength");
	primitive ->SetFilter(protonFilter);

	// //Register Filters to Scorer
	boxPS->RegisterPrimitive(primitive);  

	//Apply Scorer to Volume
	SetSensitiveDetector("lBox",boxPS);

	// //
	// //Score Deposited Energy
	// primitive = new G4PSEnergyDeposit("Edep");


}