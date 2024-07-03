/*
TO-DO:
Declared materials load their (crosssection-)data at the start of a run wether they are actually used in the 
simulation or not. This will blow up the needed RAM and slow down the start of a simulation.
->Can this be prevented?
-> stupid solution: comment out the matirials you dont need and re-compile
-> smooth solution: auto check which matirals are used in simulation and only declare them as needed
*/


#include "DetectorConstruction.hh"      //Header file where functions classes and variables may be defined (...)
#include "G4NistManager.hh"             //for getting material definitions from the NIST database
#include "G4Material.hh"
#include "G4SystemOfUnits.hh"           //for units
#include "G4UnitsTable.hh"              //for units

//Define materials and compositions you want to use in the simulation
void DetectorConstruction::DefineMaterials()
{
	//MATERIALS:
	//
	//How to define Materials using the NIST database
	//see https://geant4-userdoc.web.cern.ch/UsersGuides/ForApplicationDeveloper/html/Appendix/materialNames.html for a list of available materials
	//
	// Get nist material manager
	G4NistManager* nist = G4NistManager::Instance();

	// define world material as vacuum (Galactic) and boxMaterial as Copper using the NIST database
	// world_mat    = nist->FindOrBuildMaterial("G4_AIR");
	world_mat   = nist->FindOrBuildMaterial("G4_Galactic");
	boxMaterial = nist->FindOrBuildMaterial("G4_WATER");

	// NIST Materials
	Vacuum      = [&](){return nist->FindOrBuildMaterial("G4_Galactic");};
	Hydrogen    = nist->FindOrBuildMaterial("G4_H");
	Boron       = nist->FindOrBuildMaterial("G4_B");
	Carbon      = nist->FindOrBuildMaterial("G4_C");
	Aluminum    = nist->FindOrBuildMaterial("G4_Al");
	Titanium    = nist->FindOrBuildMaterial("G4_Ti");
	Iron        = nist->FindOrBuildMaterial("G4_Fe");
	Copper      = nist->FindOrBuildMaterial("G4_Cu");
	Nickel      = nist->FindOrBuildMaterial("G4_Ni");
	Tungsten    = nist->FindOrBuildMaterial("G4_W");

	// NIST Compounds
	Concrete    = [&](){return nist->FindOrBuildMaterial("G4_CONCRETE");};
	Graphite    = nist->FindOrBuildMaterial("G4_GRAPHITE");
	Steel       = nist->FindOrBuildMaterial("G4_STAINLESS-STEEL");
	Water       = nist->FindOrBuildMaterial("G4_WATER");

	// Self-defined Materials
	//Define borated PE (Manufacturer: Roechling- Polystone M nuclear with 5% Boron)
	BoratedPE   = new G4Material("BoratedPE",   //name
																1.03*g/cm3,   //density
																3);           //number of elements

	//Add Elements to Material
	BoratedPE->AddMaterial(Hydrogen, 14.*perCent);
	BoratedPE->AddMaterial(Carbon, 81.*perCent);
	BoratedPE->AddMaterial(Boron, 5.*perCent);

	//Define Densimet180 (Manufacturer: Plansee)
	Densimet180 = new G4Material("Densimet180", //name
																18.0*g/cm3,   //density
																3);           //number of elements

	//Add Elements to Material
	Densimet180->AddMaterial(Tungsten, 95.*perCent);
	Densimet180->AddMaterial(Iron, 1.6*perCent);
	Densimet180->AddMaterial(Nickel, 3.4*perCent);


	// boxMaterial  = nist->FindOrBuildMaterial("G4_Galactic");
	dummyMat     = nist->FindOrBuildMaterial("G4_Galactic");

	//Print all defined materials to console
	// G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}