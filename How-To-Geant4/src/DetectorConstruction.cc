/*
WHAT DOES THIS DO?
*/
// #define DEBUG
// #define Collimator
#define Range
// #define TNY

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
#include "G4SubtractionSolid.hh"        //for the boolean subtraction operation
#include "G4UnionSolid.hh"              //for the boolean combination operation

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

#include "SensitiveDetector.hh"         // the SensitiveDetector file
#include "CADMesh.hh"                   // for importing CAD-files (.stl, .obj, ...). Read all about it at: https://github.com/christopherpoole/CADMesh
#include "G4GDMLParser.hh"              // for importing/exporting GDML-files

DetectorConstruction::DetectorConstruction()
:G4VUserDetectorConstruction(),
 world_mat(nullptr), fDetectorMessenger(nullptr)
{
	// for reading and writing GDML
	fLoadFile  ="test.gdml";
	fWriteFile ="wtest.gdml";
	fWritingChoice = 0;
	fLoadingChoice = 0;
	fOnlyLoadChoice = false;

	// World Size
	world_sizeXYZ = 20.*m;

	// set dummy variables
	a = 20.*m; // used for x- and y-width of Sensitive Detectors
	b = 10.*cm; // unused
	c = 1.*cm;  // unused
	d = 0.*degree;  // unused
	e = 1.*cm;  // unused
	f = 0.*cm;      // position of the target

	// Define Materials
	DefineMaterials(); // see 'Materials.cc' for defined Materials

	// create commands for interactive definition of the geometry via macro file
	fDetectorMessenger = new DetectorMessenger(this);
}

DetectorConstruction::~DetectorConstruction()
{ delete fDetectorMessenger; }

// Construct geometry
G4VPhysicalVolume* DetectorConstruction::Construct()
{
	// see 'GDML.cc' - check if any GDML related stuff should be done and then execute ConstructVolume() in most cases
	return ConstructVolumesGDML();
}

// Define a Geometry in this file
G4VPhysicalVolume* DetectorConstruction::ConstructVolumes()
{
	// print for DEBUGGING
	G4cout << "\n ----ONLYLOAD is---- " << fOnlyLoadChoice <<  G4endl;

	// SOLIDS, GEOMETRIES, PLACEMENT, ETC.
	// How to create solids
	// It's basically a process with 3 steps:
	// 1.: Create a Geometry e.g. a Box, Cylinder, Sphere or even a Box minus a Cylinder (-> see boolean operation)
	// 2.: Make it a Logical Volume by assigning a material to it
	// 3.: Place it in your simulation

	if(fLoadingChoice==0) //no GDML file is loaded = world Volume needs to be constructed
	{
		// If no GDML file is loaded, a World volume needs to be created - otherwise it should be in the GDML file
		G4Box* solidWorld =
			new G4Box("sWorld",                       							//its name
				0.5*world_sizeXYZ, 0.5*world_sizeXYZ, 0.5*world_sizeXYZ);     	//its size

		// G4LogicalVolume* lWorld =
		lWorld =
			new G4LogicalVolume(solidWorld,          						//its solid
													Vacuum(),				//its material
													// world_mat,           //its material
													"lWorld");            	//its name

		// G4VPhysicalVolume* fWorldPhysVol =
		fWorldPhysVol =
			new G4PVPlacement(0,                     					//no rotation
												G4ThreeVector(),       	//at (0,0,0)
												lWorld,            		//its logical volume
												"pWorld",               //its name
												0,                     	//its mother  volume
												false,                	//boolean operation?
												0,                     	//copy number
												true);                 	//overlaps checking?

		// Make world-volume invisible
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

	#include "DetectorGeometries.cc"	// see 'Geometries.cc' for defined Geometries

	//Import Standard Geometry (1 sphere and 5 SDs)
	//----------------------------------------------------------------------------------------------------------------------------------------------------------------------

	//create 5 flat boxes to use as Sensitive Detector (SD)
	//
	// SD1
	G4Box* sSD1 =
		new G4Box("sSD1",                        				//its name
				a/2, a/2, 0.02*mm /2);                   		//its size: half x, half y, half z

	G4LogicalVolume* lSD1 =
		new G4LogicalVolume(sSD1,                				//its solid
												// Aluminum(),		//its material
												Vacuum(),		//its material
												"lSD1");        //its name

		new G4PVPlacement(0,                     				//no rotation
							G4ThreeVector(0,0,10.*cm),     		//position
							lSD1,                          		//its logical volume
							"pSD1",                         	//its name
							lWorld,								//its mother  volume
							false,                         		//any boolean operation?
							0,                             		//copy number
							true);                         		//overlaps checking?

	//Make (in-)visible and give it a color
	auto lSD1VisAtt = new G4VisAttributes(G4Color(0, 0, 1, 0.8)); //(r, g, b , transparency)
	lSD1VisAtt->SetVisibility(true);
	lSD1->SetVisAttributes(lSD1VisAtt);

	// SD2
	G4Box* sSD2 =
		new G4Box("sSD2",                        				//its name
				a/2, a/2, 0.02*mm /2);                   		//its size: half x, half y, half z

	G4LogicalVolume* lSD2 =
		new G4LogicalVolume(sSD2,                				//its solid
												Vacuum(),		//its material
												"lSD2");		//its name

		new G4PVPlacement(0,                     				//no rotation
							G4ThreeVector(0,0,20.*cm),     		//position
							lSD2,                          		//its logical volume
							"pSD2",                         	//its name
							lWorld,								//its mother  volume
							false,                         		//any boolean operation?
							0,                             		//copy number
							true);                         		//overlaps checking?

	//Make (in-)visible and give it a color
	auto lSD2VisAtt = new G4VisAttributes(G4Color(0, 0, 1, 0.8)); //(r, g, b , transparency)
	lSD2VisAtt->SetVisibility(true);
	lSD2->SetVisAttributes(lSD2VisAtt);

	// SD3
	G4Box* sSD3 =
		new G4Box("sSD3",                        				//its name
				a/2, a/2, 0.02*mm /2);                   		//its size: half x, half y, half z

	G4LogicalVolume* lSD3 =
		new G4LogicalVolume(sSD3,                				//its solid
												Vacuum(),		//its material
												"lSD3");		//its name

		new G4PVPlacement(0,                     				//no rotation
							G4ThreeVector(0,0,30.*cm),     		//position
							lSD3,                          		//its logical volume
							"pSD3",                         	//its name
							lWorld,								//its mother  volume
							false,                         		//any boolean operation?
							0,                             		//copy number
							true);                         		//overlaps checking?

	//Make (in-)visible and give it a color
	auto lSD3VisAtt = new G4VisAttributes(G4Color(0, 0, 1, 0.8)); //(r, g, b , transparency)
	lSD3VisAtt->SetVisibility(true);
	lSD3->SetVisAttributes(lSD3VisAtt);

	// SD4
	G4Box* sSD4 =
		new G4Box("sSD4",                        				//its name
				a/2, a/2, 0.02*mm /2);                   		//its size: half x, half y, half z

	G4LogicalVolume* lSD4 =
		new G4LogicalVolume(sSD4,                				//its solid
												Vacuum(),		//its material
												"lSD4");		//its name

		new G4PVPlacement(0,                     				//no rotation
							G4ThreeVector(0,0,40.*cm),     		//position
							lSD4,                          		//its logical volume
							"pSD4",                         	//its name
							lWorld,								//its mother  volume
							false,                         		//any boolean operation?
							0,                             		//copy number
							true);                         		//overlaps checking?

	//Make (in-)visible and give it a color
	auto lSD4VisAtt = new G4VisAttributes(G4Color(0, 0, 1, 0.8)); //(r, g, b , transparency)
	lSD4VisAtt->SetVisibility(true);
	lSD4->SetVisAttributes(lSD4VisAtt);

	// SD5
	G4Box* sSD5 =
		new G4Box("sSD5",                        				//its name
				a/2, a/2, 0.02*mm /2);                   		//its size: half x, half y, half z

	G4LogicalVolume* lSD5 =
		new G4LogicalVolume(sSD5,                				//its solid
												Vacuum(),		//its material
												"lSD5");		//its name

		new G4PVPlacement(0,                     				//no rotation
							G4ThreeVector(0,0,50.*cm),     		//position
							lSD5,                          		//its logical volume
							"pSD5",                         	//its name
							lWorld,								//its mother  volume
							false,                         		//any boolean operation?
							0,                             		//copy number
							true);                         		//overlaps checking?

	//Make (in-)visible and give it a color
	auto lSD5VisAtt = new G4VisAttributes(G4Color(0, 0, 1, 0.8)); //(r, g, b , transparency)
	lSD5VisAtt->SetVisibility(true);
	lSD5->SetVisAttributes(lSD5VisAtt);

	//Print all defined materials to console
	G4cout << *(G4Material::GetMaterialTable()) << G4endl;

	// default value = 0
	if(fWritingChoice==1) // save geometry in GDML file
		{
			SaveGDML( fWriteFile );
		}

	//always return the root volume
	return fWorldPhysVol;
}

//
//Functions for custom GUI and macro commands - see DetectorConstruction.hh, DetectorMessenger.cc, DetectorMessenger.hh
//
void DetectorConstruction::SetAbsorMaterial(G4String materialChoice)	//change "dummyMat" via macro file command
{
	// search the material by its name
	G4Material* NewMaterial = G4NistManager::Instance()->FindOrBuildMaterial(materialChoice);

	if (NewMaterial) {
		dummyMat = NewMaterial;
		G4RunManager::GetRunManager()->ReinitializeGeometry();
		G4cout << "\n The dummyMat is now "
					 << dummyMat->GetName()
					 << G4endl;
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

// Change f
void DetectorConstruction::change_f(G4double value)
{
  f = value;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
  G4cout  << "\n f is now " << G4BestUnit(f,"Length") << G4endl;
}

// Assign Detectors and Scorers to Volume
void DetectorConstruction::ConstructSDandField()
{
	G4SDManager::GetSDMpointer()->SetVerboseLevel(1);

	// SENSITIVE DETECTORS
	// You need also Code for this one to work in:
	// SDX.cc to specify what to quantity to track (Energy, position, etc.)
	// RunAction.cc to open a file and declare ntuple or histograms to save data in
	// Make a Volume a Sensitive Detector (SD); SD are able to access Track/Step information of Particles going through e.g. :
	// Kinetic energy, Momentum


	// Declare a Sensitive Detector
	// auto sd1 = new SD1("SD1");                          //create a new Sensitive Detector
	// G4SDManager::GetSDMpointer()->AddNewDetector(sd1);  //add new SD to SDManager
	// SetSensitiveDetector("lSD1", sd1);                  //Apply Sensitive Detector 'sdX' to logical Volume 'lSDX'

	// auto sd2 = new SD2("SD2");                          //create a new Sensitive Detector
	// G4SDManager::GetSDMpointer()->AddNewDetector(sd2);  //add new SD to SDManager
	// SetSensitiveDetector("lSD2", sd2);                  //Apply Sensitive Detector 'sdX' to logical Volume 'lSDX'

	// auto sd3 = new SD3("SD3");                          //create a new Sensitive Detector
	// G4SDManager::GetSDMpointer()->AddNewDetector(sd3);  //add new SD to SDManager
	// SetSensitiveDetector("lSD3", sd3);                  //Apply Sensitive Detector 'sdX' to logical Volume 'lSDX'

	// auto sd4 = new SD4("SD4");                          //create a new Sensitive Detector
	// G4SDManager::GetSDMpointer()->AddNewDetector(sd4);  //add new SD to SDManager
	// SetSensitiveDetector("lSD4", sd4);                  //Apply Sensitive Detector 'sdX' to logical Volume 'lSDX'

	// auto sd5 = new SD5("SD5");                          //create a new Sensitive Detector
	// G4SDManager::GetSDMpointer()->AddNewDetector(sd5);  //add new SD to SDManager
	// SetSensitiveDetector("lSD5", sd5);                  //Apply Sensitive Detector 'sdX' to logical Volume 'lSDX'

	auto sphereSD = new SphereSD("SphereSD");                   //create a new Sensitive Detector
	G4SDManager::GetSDMpointer()->AddNewDetector(sphereSD);     //add new SD to SDManager
	#ifdef TNY
	SetSensitiveDetector("lSphere", sphereSD);                  //Apply Sensitive Detector 'SphereSD' to logical Volume 'lSphere'
	#endif

	// PRIMITIVE SCORERS
	// You need also Code for this one to work in:
	// RunAction.cc, EventAction.cc
	// Make a Volume a Primitive Scorer (PS); PS are able to save information on events related to inside the volume e.g. :
	// energy deposit, track length, current, flux


	// FILTERS FOR PRIMITVIE SCORERS
	// Declare filters on Particles, Charge, Energy
	G4String fltName,particleName;

	// charged particle filter
	// auto charged = new G4SDChargedFilter("chargedFilter");
	// G4SDChargedFilter* chargedFilter = new G4SDChargedFilter(fltName="chargedFilter");

  	// Proton filter
	G4SDParticleFilter* protonFilter =
	new G4SDParticleFilter(fltName="protonFilter", particleName="proton");

	// Deuteron filter
	// G4SDParticleFilter* deuteronFilter =
	// new G4SDParticleFilter(fltName="deuteronFilter", particleName="deuteron");

  	// Alpha filter
	// G4SDParticleFilter* alphaFilter =
	// new G4SDParticleFilter(fltName="alphaFilter", particleName="alpha");

  	// Neutron filter
	// G4SDParticleFilter* neutronFilter =
	// new G4SDParticleFilter(fltName="neutronFilter", particleName="neutron");

	// Gamma filter
	// G4SDParticleFilter* gammaFilter =
	// new G4SDParticleFilter("gammaFilter", "gamma");


	// Create a MultiFunctionalDetector (MFD) and name it "Scorer"
	auto MFD_Scorer = new G4MultiFunctionalDetector("Scorer");
	G4SDManager::GetSDMpointer()->AddNewDetector(MFD_Scorer );

	// Declare what quantity should be scored and apply filters
	G4VPrimitiveScorer* PS_TrackLength;						//create a scorer called "PS_TrackLength"
	PS_TrackLength = new G4PSTrackLength("TrackLength");	//give "PS_TrackLength" the ability to track G4PSTrackLength and save in data "TrackLength" 
	PS_TrackLength ->SetFilter(protonFilter);				//apply a filter; score only protons
	// PS_TrackLength ->SetFilter(deuteronFilter);			//apply a filter; score only deuterons
	// PS_TrackLength ->SetFilter(alphaFilter);				//apply a filter; score only alphas
	// PS_TrackLength ->SetFilter(neutronFilter);			//apply a filter; score only neutrons

	// Register Scorer to MultiFunctionalDetector
	MFD_Scorer ->RegisterPrimitive(PS_TrackLength);

	#ifdef Range
	// Apply MFD to Volume
	SetSensitiveDetector("lBox",MFD_Scorer );
	#endif

	//
	// other Scorers
	// PS_EnergyDeposit = new G4PSEnergyDeposit("Edep");
}
