/*
TO-DO:
-implement Self-defined materials in a way like NIST materials (only get loaded when actually used)
*/


#include "DetectorConstruction.hh"      //Header file where functions classes and variables may be defined (...)
#include "G4NistManager.hh"             //for getting material definitions from the NIST database
#include "G4Material.hh"
#include "G4SystemOfUnits.hh"           //for units
#include "G4UnitsTable.hh"              //for units

#include <functional>
#include <string>
#include <tuple>
#include <type_traits>
#include <vector>
using std::string;



//Define materials and compositions you want to use in the simulation
void DetectorConstruction::DefineMaterials()
{
	//MATERIALS:
	//
	//How to define Materials using the NIST database
	//see https://geant4-userdoc.web.cern.ch/UsersGuides/ForApplicationDeveloper/html/Appendix/materialNames.html for a list of available materials
	//
	// Initiate the NIST Material Manager
	nist = G4NistManager::Instance();

	// define world material as vacuum (Galactic) and boxMaterial as Copper using the NIST database
	// world_mat    = nist->FindOrBuildMaterial("G4_AIR");
	world_mat   = nist->FindOrBuildMaterial("G4_Galactic");
	// boxMaterial = nist->FindOrBuildMaterial("G4_WATER");

	// NIST Materials
	Vacuum      = [&](){return nist->FindOrBuildMaterial("G4_Galactic");};
	Hydrogen    = [&](){return nist->FindOrBuildMaterial("G4_H");};
	Boron       = [&](){return nist->FindOrBuildMaterial("G4_B");};
	Carbon      = [&](){return nist->FindOrBuildMaterial("G4_C");};
	Aluminum    = [&](){return nist->FindOrBuildMaterial("G4_Al");};
	Titanium    = [&](){return nist->FindOrBuildMaterial("G4_Ti");};
	Iron        = [&](){return nist->FindOrBuildMaterial("G4_Fe");};
	Copper      = [&](){return nist->FindOrBuildMaterial("G4_Cu");};
	Nickel      = [&](){return nist->FindOrBuildMaterial("G4_Ni");};
	Tungsten    = [&](){return nist->FindOrBuildMaterial("G4_W");};

	// NIST Compounds
	Concrete    = [&](){return nist->FindOrBuildMaterial("G4_CONCRETE");};
	Graphite    = [&](){return nist->FindOrBuildMaterial("G4_GRAPHITE_POROUS");};	// G4_Graphite has the molecular density of 2.2g/cm3, which can not be realized in a solid target
	Steel       = [&](){return nist->FindOrBuildMaterial("G4_STAINLESS-STEEL");};
	Water       = [&](){return nist->FindOrBuildMaterial("G4_WATER");};


	typedef std::function<G4Material*()> MaterialMaker;

	struct CustomMat{
		string name;
		double density;
		std::vector<std::tuple<MaterialMaker, double>> parts;
		CustomMat(
			string name = "",
			double density = 0.0,
			std::vector<std::tuple<MaterialMaker, double>> parts = {}
		) {
			this -> name = name;
			this -> density = density;
			this -> parts = parts;
		};

		G4Material* make() {
			if (this->mat_ptr) return this->mat_ptr;

			auto material = new G4Material(
				this->name,
				this->density,
				this->parts.size()
			);
			for (auto part : this->parts){
				auto [mat, amount] = part;
				material->AddMaterial(mat(), amount);
			}
			this->mat_ptr = material;
			return this->mat_ptr;
		};

		private:
		G4Material* mat_ptr;
	};

	std::vector<CustomMat> known_materials = {
		#include "custom_materials.h"
	};

	std::function<G4Material*(string)> find_or_build_custom = [&](string name){
		for (auto custom_mat :  known_materials) {
			if (custom_mat.name == name) return custom_mat.make();
		}
		return known_materials[0].make();
	};

	BoratedPE = find_or_build_custom("BoratedPE");
	// Self-defined Materials
	//Define borated PE (Manufacturer: Roechling- Polystone M nuclear with 5% Boron)
	// BoratedPE   = new G4Material("B
	//
	// return *material;oratedPE",   //name
	// 															1.03*g/cm3,   //density
	// 															3);           //number of elements

	//Add Elements to Material
	// BoratedPE->AddMaterial(Hydrogen(), 14.*perCent);
	// BoratedPE->AddMaterial(Carbon(), 81.*perCent);
	// BoratedPE->AddMaterial(Boron(), 5.*perCent);

	// //Define Densimet180 (Manufacturer: Plansee)
	// Densimet180 = new G4Material("Densimet180", //name
	// 															18.0*g/cm3,   //density
	// 															3);           //number of elements

	// //Add Elements to Material
	// Densimet180->AddMaterial(Tungsten, 95.*perCent);
	// Densimet180->AddMaterial(Iron, 1.6*perCent);
	// Densimet180->AddMaterial(Nickel, 3.4*perCent);


	// boxMaterial  = nist->FindOrBuildMaterial("G4_Galactic");

	// Initialize dummyMat as Vacuum
	dummyMat     = nist->FindOrBuildMaterial("G4_Galactic");

	//Print all defined materials to console
	// G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}
