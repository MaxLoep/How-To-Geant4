#ifndef GEOMETRYBUILDER
#define GEOMETRYBUILDER

#include <cwchar>
#include <string>
#include <vector>
#include <map>
#include <functional>

#include "G4LogicalVolumeStore.hh"

#include "G4NistManager.hh"             //for getting material definitions from the NIST database

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"
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


namespace Materials {
	typedef std::function<G4Material*()> MaterialMaker;

	//MATERIALS:
	//
	//How to define Materials using the NIST database
	//see https://geant4-userdoc.web.cern.ch/UsersGuides/ForApplicationDeveloper/html/Appendix/materialNames.html for a list of available materials
	//
	// Initiate the NIST Material Manager

	// ### DEFINE A NIST MATERIAL
	// ###-----------------------------------------------------------------------------------------------------------------------------------------
	// Create a Material from the NIST database with
	// and use the function call 'name()' to assign it to a geometry.
	// This way, it gets created (=loaded into memory) only when it is actually used

	struct CustomMat{
		std::string name;
		double density;
		std::vector<std::tuple<MaterialMaker, double>> parts;
		CustomMat(
			std::string name = "",
			double density = 0.0,
			std::vector<std::tuple<MaterialMaker, double>> parts = {}
		) {
			this -> name = name;
			this -> density = density;
			this -> parts = parts;
		};

		void to_molecule() {
			for (auto& element : this->parts) {
				// convert atom count to mass fraction

			}
		}

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

	// NIST Materials
	static MaterialMaker Vacuum      = [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic");};
	static MaterialMaker Hydrogen    = [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_H");};
	static MaterialMaker Lithium		= [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_Li");};
	static MaterialMaker Boron       = [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_B");};
	static MaterialMaker Carbon      = [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_C");};
	static MaterialMaker Silicon		= [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_Si");};
	static MaterialMaker Aluminum    = [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_Al");};
	static MaterialMaker Scandium	= [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_Sc");};
	static MaterialMaker Titanium    = [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_Ti");};
	static MaterialMaker Vanadium	= [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_V");};
	static MaterialMaker Iron        = [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_Fe");};
	static MaterialMaker Nickel      = [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_Ni");};
	static MaterialMaker Copper      = [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_Cu");};
	static MaterialMaker Cadmium		= [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_Cd");};
	static MaterialMaker Tungsten    = [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_W");};
	static MaterialMaker Gold		= [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_Au");};
	static MaterialMaker Yttrium		= [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_Y");};
	static MaterialMaker Barium		= [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_Ba");};

	// NIST Compounds
	static MaterialMaker Air			= [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_AIR");};
	static MaterialMaker Concrete    = [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_CONCRETE");};
	static MaterialMaker Graphite    = [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_GRAPHITE_POROUS");};	// G4_Graphite has the molecular density of 2.2g/cm3, which can not be realized in a solid target
	static MaterialMaker Polyethylene= [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_POLYETHYLENE");};
	static MaterialMaker Steel       = [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_STAINLESS-STEEL");};
	static MaterialMaker Water       = [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_WATER");};

	// Structure to easily define Custom Materials later on
	// This is just preparation to easily define custom materials later on.
	// Skip this part and go ahead to see how custom materials are defined!

	// ### DEFINE A CUSTOM MATERIAL
	// ###-----------------------------------------------------------------------------------------------------------------------------------------
	// Create a Custom Material with defined NIST Materials with
	// name = CustomMat("BoratedPE",	// name
	// 				1.03*g/cm3,				//density
	// 				{// components and abundances
	// 				{Hydrogen, 14.*perCent},
	// 				{Carbon, 81.*perCent},
	// 				{Boron, 5.*perCent}
	// 				});
	// and use the function call 'name()' to assign it to a geometry.
	// This way, it gets created (=loaded into memory) only when it is actually used


	// Self-defined Materials
	//Define borated PE (Manufacturer: Roechling- Polystone M nuclear with 5% Boron)
	static CustomMat BoratedPE = CustomMat("BoratedPE",	// name
				1.0*g/cm3,				//density
				{// components
				{Hydrogen, 14.*perCent},
				{Carbon, 81.*perCent},
				{Boron, 5.*perCent}
				});

	//Define Densimet180 (Manufacturer: Plansee)
	static CustomMat Densimet180 = CustomMat("Densimet180",	//name
				18.0*g/cm3,					//density
				{// components
				{Tungsten, 95.*perCent},
				{Iron, 1.6*perCent},
				{Nickel, 3.4*perCent}
				});


	// Initialize dummyMat as Vacuum
	static auto dummyMat1	= G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic");
	static auto dummyMat2	= G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic");
	static auto dummyMat3	= G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic");
	static auto dummyMat4	= G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic");

	//Print all defined materials to console - this is currently done in 'DetectorConstruction.cc'
	// G4cout << *(G4Material::GetMaterialTable()) << G4endl;

	static std::map<std::string, MaterialMaker> known_materials = {
		{"vacuum", Vacuum},
		{"hydrogen", Hydrogen},
		{"lithium", Lithium},
		{"boron", Boron},
		{"carbon", Carbon},
		{"silicon", Silicon},
		{"aluminum", Aluminum},
		{"scandium", Scandium},
		{"titanium", Titanium},
		{"vanadium", Vanadium},
		{"yttrium", Yttrium},
		{"barium", Barium},
		{"iron", Iron},
		{"nickel", Nickel},
		{"copper", Copper},
		{"cadmium", Cadmium},
		{"tungsten", Tungsten},
		{"gold", Gold},
		{"air", Air},
		{"concrete", Concrete},
		{"graphite", Graphite},
		{"polyethylene", Polyethylene},
		{"steel", Steel},
		{"water", Water},
		{"boratedPE", BoratedPE},
		{"densimet180", Densimet180}
	};

	void add_custom_mat(std::string mat_name, std::vector<std::tuple<std::string, double>> composition, double density);
	MaterialMaker get_mat(std::string name);
}

using placer_func = std::function<G4LogicalVolume*(std::string, std::map<std::string, double>&, Materials::MaterialMaker)>;
using fmap = std::map<std::string, placer_func>;

namespace geometries {
	void register_placement(std::string thing, std::string name, std::map<std::string, double> params, Materials::MaterialMaker material);
	void run_placements(G4LogicalVolume* lWorld);
	G4LogicalVolume* cube(std::string name, std::map<std::string, double>& params, Materials::MaterialMaker mat);
	G4LogicalVolume* sphere(std::string name, std::map<std::string, double>& params, Materials::MaterialMaker mat);
	void add_placer(std::string name, placer_func func);
	void change_param(std::string name, std::string param, double value);
}


#endif
