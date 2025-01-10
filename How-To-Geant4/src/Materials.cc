/*
TO-DO:
-nothing right now
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

	// NIST Materials
	Vacuum      = [&](){return nist->FindOrBuildMaterial("G4_Galactic");};
	Hydrogen    = [&](){return nist->FindOrBuildMaterial("G4_H");};
	Lithium		= [&](){return nist->FindOrBuildMaterial("G4_Li");};
	Boron       = [&](){return nist->FindOrBuildMaterial("G4_B");};
	Carbon      = [&](){return nist->FindOrBuildMaterial("G4_C");};
	Silicon		= [&](){return nist->FindOrBuildMaterial("G4_Si");};
	Aluminum    = [&](){return nist->FindOrBuildMaterial("G4_Al");};
	Scandium	= [&](){return nist->FindOrBuildMaterial("G4_Sc");};
	Titanium    = [&](){return nist->FindOrBuildMaterial("G4_Ti");};
	Vanadium	= [&](){return nist->FindOrBuildMaterial("G4_V");};
	Iron        = [&](){return nist->FindOrBuildMaterial("G4_Fe");};
	Nickel      = [&](){return nist->FindOrBuildMaterial("G4_Ni");};
	Copper      = [&](){return nist->FindOrBuildMaterial("G4_Cu");};
	Cadmium		= [&](){return nist->FindOrBuildMaterial("G4_Cd");};
	Tungsten    = [&](){return nist->FindOrBuildMaterial("G4_W");};
	Gold		= [&](){return nist->FindOrBuildMaterial("G4_Au");};

	// NIST Compounds
	Air			= [&](){return nist->FindOrBuildMaterial("G4_AIR");};
	Concrete    = [&](){return nist->FindOrBuildMaterial("G4_CONCRETE");};
	Graphite    = [&](){return nist->FindOrBuildMaterial("G4_GRAPHITE_POROUS");};	// G4_Graphite has the molecular density of 2.2g/cm3, which can not be realized in a solid target
	Polyethylene= [&](){return nist->FindOrBuildMaterial("G4_POLYETHYLENE");};
	Steel       = [&](){return nist->FindOrBuildMaterial("G4_STAINLESS-STEEL");};
	Water       = [&](){return nist->FindOrBuildMaterial("G4_WATER");};


	// structure to easily define Custom Materials later on
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
			if (this->made) return this->mat_ptr;

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
			this->made = true;
			return this->mat_ptr;
		};

		G4Material* operator () () {
			return this->make();
		}

		private:
		G4Material* mat_ptr;
		bool made = false;
	};

	// Self-defined Materials
	//Define borated PE (Manufacturer: Roechling- Polystone M nuclear with 5% Boron)
	BoratedPE = CustomMat("BoratedPE",	// name
				1.03*g/cm3,				//density
				{// components
				{Hydrogen, 14.*perCent},
				{Carbon, 81.*perCent},
				{Boron, 5.*perCent}
				});

	//Define Densimet180 (Manufacturer: Plansee)
	Densimet180 = CustomMat("Densimet180",	//name
				18.0*g/cm3,					//density
				{// components
				{Tungsten, 95.*perCent},
				{Iron, 1.6*perCent},
				{Nickel, 3.4*perCent}
				});

	// boxMaterial  = nist->FindOrBuildMaterial("G4_Galactic");

	// Initialize dummyMat as Vacuum
	dummyMat     = nist->FindOrBuildMaterial("G4_Galactic");

	//Print all defined materials to console - this is currently done in 'DetectorConstruction.cc'
	// G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}
