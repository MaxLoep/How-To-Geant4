/*
WHAT DOES THIS DO?
*/
// #define DEBUG
// #define Collimator
// #define Range
// #define TNY
#define Shielding


#include "DetectorConstruction.hh"      //Header file where functions classes and variables may be defined (...)
#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"

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
fDetectorMessenger(nullptr)
{
	// for reading and writing GDML
	fLoadFile  ="test.gdml";
	fWriteFile ="wtest.gdml";
	fWritingChoice = 0;
	fLoadingChoice = 0;
	fOnlyLoadChoice = false;

	// World Size
	world_sizeXYZ = 20.*m;

	#ifndef Collimator
	// set initial values of dummy variables
	a = 20.*m; 		// used for x- and y-width of Sensitive Detectors
	b = 10.*cm; 	// unused
	c = 1.*cm;  	// unused
	d = 0.*degree;  // unused
	e = 1.*cm;  	// unused
	f = 0.*cm;      // position of the target
	#endif

	#ifdef Collimator
	// set dummy variables
	a = 20.*cm;     //thickness of shielding
	b = 4.*cm;      //Entrance_Diameter of the tungsten colli; MAX 6.14cm
	c = 2.*cm;      //inner diameter (choke) of the tungsten colli; MAX 6.14cm
	d = 4.*cm;      //Exit_Diameter of the tungsten colli; MAX 6.4cm
	e = 0.*degree;  //rotation of the collimator
	f = 0.*cm;      // position of the target; MAX 4.0cm - NEED TO CHECK!
	#endif

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
	// return ConstructVolumes();
}

// Define a Geometry in this file
G4VPhysicalVolume* DetectorConstruction::ConstructVolumes()
{
	// print for DEBUGGING
	G4cout << "\n ----ONLYLOAD is---- " << fOnlyLoadChoice <<  G4endl;

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
													"lWorld");            	//its name

		// G4VPhysicalVolume* fWorldPhysVol =
		fWorldPhysVol =
			new G4PVPlacement(0,                     					//no rotation
												G4ThreeVector(),       	//at (0,0,0)
												lWorld,            		//its logical volume
												"pworld",               //its name
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

	// Different Geometries are constructed depending on defined pre-processor variables at top
	#include "DetectorGeometries.cc"	// see 'Geometries.cc' for defined Geometries

	//Print all defined materials to console
	G4cout << *(G4Material::GetMaterialTable()) << G4endl;

	// save geometry in GDML file
	if(fWritingChoice==1) // default value = 0
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
	auto sd1 = new SD1("SD1");                          //create a new Sensitive Detector
	G4SDManager::GetSDMpointer()->AddNewDetector(sd1);  //add new SD to SDManager
	#if defined DEBUG || defined Shielding
	SetSensitiveDetector("lSD1", sd1);                  //Apply Sensitive Detector 'sdX' to logical Volume 'lSDX'
	#endif

	auto sd2 = new SD2("SD2");                          //create a new Sensitive Detector
	G4SDManager::GetSDMpointer()->AddNewDetector(sd2);  //add new SD to SDManager
	#if defined DEBUG || defined Shielding
	SetSensitiveDetector("lSD2", sd2);                  //Apply Sensitive Detector 'sdX' to logical Volume 'lSDX'
	#endif

	auto sd3 = new SD3("SD3");                          //create a new Sensitive Detector
	G4SDManager::GetSDMpointer()->AddNewDetector(sd3);  //add new SD to SDManager
	#if defined DEBUG
	SetSensitiveDetector("lSD3", sd3);                  //Apply Sensitive Detector 'sdX' to logical Volume 'lSDX'
	#endif

	auto sd4 = new SD4("SD4");                          //create a new Sensitive Detector
	G4SDManager::GetSDMpointer()->AddNewDetector(sd4);  //add new SD to SDManager
	#if defined DEBUG
	SetSensitiveDetector("lSD4", sd4);                  //Apply Sensitive Detector 'sdX' to logical Volume 'lSDX'
	#endif

	auto sd5 = new SD5("SD5");                          //create a new Sensitive Detector
	G4SDManager::GetSDMpointer()->AddNewDetector(sd5);  //add new SD to SDManager
	#if defined DEBUG
	SetSensitiveDetector("lSD5", sd5);                  //Apply Sensitive Detector 'sdX' to logical Volume 'lSDX'
	#endif

	auto sphereSD = new SphereSD("SphereSD");                   //create a new Sensitive Detector
	G4SDManager::GetSDMpointer()->AddNewDetector(sphereSD);     //add new SD to SDManager
	#if defined TNY //|| defined Collimator
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
