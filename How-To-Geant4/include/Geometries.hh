#ifndef GEOMETRYBUILDER
#define GEOMETRYBUILDER

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

	// NIST Materials from the periodic table in natural abundances
	static MaterialMaker Vacuum     = [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic");};	// basically low density Hydrogen
	// static MaterialMaker H, Hydrogen = H   = [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_H");};			// Z =  1 				//rewritte everything like this so Element Name and Symbol can be used?
	static MaterialMaker Hydrogen	= [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_H");};			// Z =  1
	static MaterialMaker Helium		= [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_He");};		// Z =  2
	static MaterialMaker Lithium	= [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_Li");};		// Z =  3
	static MaterialMaker Beryllium	= [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_Be");};		// Z =  4
	static MaterialMaker Boron      = [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_B");};			// Z =  5
	static MaterialMaker Carbon     = [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_C");};			// Z =  6
	static MaterialMaker Nitrogen	= [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_N");};			// Z =  7
	static MaterialMaker Oxygen		= [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_O");};			// Z =  8
	static MaterialMaker Fluorine	= [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_F");};			// Z =  9
	static MaterialMaker Neon		= [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_Ne");};		// Z = 10
	static MaterialMaker Sodium		= [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_Na");};		// Z = 11
	static MaterialMaker Magnesium	= [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_Mg");};		// Z = 12
	static MaterialMaker Aluminum   = [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_Al");};		// Z = 13
	static MaterialMaker Silicon	= [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_Si");};		// Z = 14
	static MaterialMaker Phosphorus	= [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_P");};			// Z = 15
	static MaterialMaker Sulfur		= [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_S");};			// Z = 16
	static MaterialMaker Chlorine	= [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_Cl");};		// Z = 17
	static MaterialMaker Argon		= [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_Ar");};		// Z = 18
	static MaterialMaker Potassium	= [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_K");};			// Z = 19
	static MaterialMaker Calcium	= [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_Ca");};		// Z = 20
	static MaterialMaker Scandium	= [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_Sc");};		// Z = 21
	static MaterialMaker Titanium   = [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_Ti");};		// Z = 22
	static MaterialMaker Vanadium	= [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_V");};			// Z = 23
	static MaterialMaker Chromium	= [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_Cr");};		// Z = 24
	static MaterialMaker Manganese	= [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_Mn");};		// Z = 25
	static MaterialMaker Iron       = [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_Fe");};		// Z = 26
	static MaterialMaker Cobalt		= [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_Co");};		// Z = 27
	static MaterialMaker Nickel     = [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_Ni");};		// Z = 28
	static MaterialMaker Copper     = [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_Cu");};		// Z = 29
	static MaterialMaker Zinc		= [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_Zn");};		// Z = 30
	// static MaterialMaker = [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_Ga");};		// Z = 31
	// static MaterialMaker = [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_Ge");};		// Z = 32
	// static MaterialMaker = [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_As");};		// Z = 33
	// static MaterialMaker = [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_Se");};		// Z = 34
	// static MaterialMaker = [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_Br");};		// Z = 35
	// static MaterialMaker = [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_Kr");};		// Z = 36
	// static MaterialMaker = [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_Rb");};		// Z = 37
	// static MaterialMaker = [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_Sr");};		// Z = 38
	// static MaterialMaker = [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_Y");};			// Z = 39
	// static MaterialMaker = [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_Zr");};		// Z = 40
	// static MaterialMaker = [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_Nb");};		// Z = 41
	// static MaterialMaker = [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_Mo");};		// Z = 42
	// static MaterialMaker = [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_Tc");};		// Z = 43
	// static MaterialMaker = [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_Ru");};		// Z = 44
	// static MaterialMaker = [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_Rh");};		// Z = 45
	// static MaterialMaker = [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_Pd");};		// Z = 46
	// static MaterialMaker = [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_Ag");};		// Z = 47
	static MaterialMaker Cadmium	= [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_Cd");};		// Z = 48
	// static MaterialMaker = [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_In");};		// Z = 49
	// static MaterialMaker = [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_Sn");};		// Z = 50
	// static MaterialMaker = [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_Sb");};		// Z = 51
	// static MaterialMaker = [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_Te");};		// Z = 52
	// static MaterialMaker = [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_I");};			// Z = 53
	// static MaterialMaker = [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_Xe");};		// Z = 54
	// static MaterialMaker = [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_Cs");};		// Z = 55
	// static MaterialMaker = [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_Ba");};		// Z = 56
	// static MaterialMaker = [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_La");};		// Z = 57
	// static MaterialMaker = [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_Ce");};		// Z = 58
	// static MaterialMaker = [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_Pm");};		// Z = 59
	// static MaterialMaker = [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_Nd");};		// Z = 60
	// static MaterialMaker = [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_Pm");};		// Z = 61
	// static MaterialMaker = [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_Sm");};		// Z = 62
	// static MaterialMaker = [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_Eu");};		// Z = 63
	// static MaterialMaker = [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_Gd");};		// Z = 64
	// static MaterialMaker = [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_Tb");};		// Z = 65
	// static MaterialMaker = [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_Dy");};		// Z = 66
	// static MaterialMaker = [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_Ho");};		// Z = 67
	// static MaterialMaker = [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_Er");};		// Z = 68
	// static MaterialMaker = [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_Tm");};		// Z = 69
	// static MaterialMaker = [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_Yb");};		// Z = 70
	// static MaterialMaker = [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_Lu");};		// Z = 71
	// static MaterialMaker = [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_Hf");};		// Z = 72
	// static MaterialMaker = [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_Ta");};		// Z = 73
	static MaterialMaker Tungsten   = [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_W");};			// Z = 74
	static MaterialMaker Rhenium	= [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_Re");};		// Z = 75
	static MaterialMaker Osmium		= [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_Os");};		// Z = 76
	static MaterialMaker Iridium	= [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_Ir");};		// Z = 77
	static MaterialMaker Platinum	= [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_Pt");};		// Z = 78
	static MaterialMaker Gold		= [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_Au");};		// Z = 79
	static MaterialMaker Mercury	= [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_Hg");};		// Z = 80
	static MaterialMaker Thallium	= [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_Tl");};		// Z = 81
	static MaterialMaker Lead		= [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_Pb");};		// Z = 82
	// static MaterialMaker = [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_Bi");};		// Z = 83
	// static MaterialMaker = [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_Po");};		// Z = 84
	// static MaterialMaker = [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_At");};		// Z = 85
	// static MaterialMaker = [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_Rn");};		// Z = 86
	// static MaterialMaker = [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_Fr");};		// Z = 87
	// static MaterialMaker = [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_Ra");};		// Z = 88
	// static MaterialMaker = [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_Ac");};		// Z = 89
	// static MaterialMaker = [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_Th");};		// Z = 90
	// static MaterialMaker = [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_Pa");};		// Z = 91
	// static MaterialMaker = [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_U");};			// Z = 92
	// static MaterialMaker = [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_Np");};		// Z = 93
	// static MaterialMaker = [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_Pu");};		// Z = 94
	// static MaterialMaker = [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_Am");};		// Z = 95
	// static MaterialMaker = [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_Cm");};		// Z = 96
	// static MaterialMaker = [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_Bk");};		// Z = 97
	// static MaterialMaker = [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_Cf");};		// Z = 98

	// NIST Compounds of commonly used materials
	static MaterialMaker Air			= [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_AIR");};
	static MaterialMaker Concrete		= [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_CONCRETE");};
	static MaterialMaker Graphite		= [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_GRAPHITE_POROUS");};	// G4_Graphite has the molecular density of 2.2g/cm3, which can not be realized in a solid target
	static MaterialMaker Kapton			= [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_KAPTON");};
	static MaterialMaker Plexiglass		= [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_PLEXIGLASS");};		// also known as acrylic glass or PMMA
	static MaterialMaker Polyethylene	= [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_POLYETHYLENE");};
	static MaterialMaker Steel			= [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_STAINLESS-STEEL");};
	static MaterialMaker Viton			= [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_VITON");};
	static MaterialMaker Water			= [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_WATER");};
	


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
