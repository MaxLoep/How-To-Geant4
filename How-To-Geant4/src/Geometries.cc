#include "Geometries.hh"


static std::vector<std::tuple<std::string, std::string, std::map<std::string, double>>> record = {};
static fmap placers = {{"cube", geometries::cube}, {"sphere", geometries::sphere}};

namespace Materials {
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

	typedef std::function<G4Material*()> MaterialMaker;

	// NIST Materials
	MaterialMaker Vacuum      = [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic");};
	MaterialMaker Hydrogen    = [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_H");};
	MaterialMaker Lithium		= [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_Li");};
	MaterialMaker Boron       = [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_B");};
	MaterialMaker Carbon      = [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_C");};
	MaterialMaker Silicon		= [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_Si");};
	MaterialMaker Aluminum    = [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_Al");};
	MaterialMaker Scandium	= [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_Sc");};
	MaterialMaker Titanium    = [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_Ti");};
	MaterialMaker Vanadium	= [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_V");};
	MaterialMaker Iron        = [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_Fe");};
	MaterialMaker Nickel      = [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_Ni");};
	MaterialMaker Copper      = [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_Cu");};
	MaterialMaker Cadmium		= [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_Cd");};
	MaterialMaker Tungsten    = [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_W");};
	MaterialMaker Gold		= [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_Au");};

	// NIST Compounds
	MaterialMaker Air			= [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_AIR");};
	MaterialMaker Concrete    = [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_CONCRETE");};
	MaterialMaker Graphite    = [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_GRAPHITE_POROUS");};	// G4_Graphite has the molecular density of 2.2g/cm3, which can not be realized in a solid target
	MaterialMaker Polyethylene= [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_POLYETHYLENE");};
	MaterialMaker Steel       = [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_STAINLESS-STEEL");};
	MaterialMaker Water       = [](){return G4NistManager::Instance()->FindOrBuildMaterial("G4_WATER");};


	// Structure to easily define Custom Materials later on
	// This is just preparation to easily define custom materials later on.
	// Skip this part and go ahead to see how custom materials are defined!
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
	CustomMat BoratedPE = CustomMat("BoratedPE",	// name
				1.0*g/cm3,				//density
				{// components
				{Hydrogen, 14.*perCent},
				{Carbon, 81.*perCent},
				{Boron, 5.*perCent}
				});

	//Define Densimet180 (Manufacturer: Plansee)
	CustomMat Densimet180 = CustomMat("Densimet180",	//name
				18.0*g/cm3,					//density
				{// components
				{Tungsten, 95.*perCent},
				{Iron, 1.6*perCent},
				{Nickel, 3.4*perCent}
				});


	// Initialize dummyMat as Vacuum
	auto dummyMat1	= G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic");
	auto dummyMat2	= G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic");
	auto dummyMat3	= G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic");
	auto dummyMat4	= G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic");

	//Print all defined materials to console - this is currently done in 'DetectorConstruction.cc'
	// G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}


void geometries::register_placement(std::string thing, std::string name, std::map<std::string, double> params) {
	record.push_back({thing, name, params});
}


G4LogicalVolume* geometries::cube(std::string name, std::map<std::string, double>& params) {
	G4Box* sSD1 = new G4Box("s" + name, params["x_size"], params["y_size"], params["z_size"]);

	G4LogicalVolume* lSD1 = new G4LogicalVolume(sSD1, Materials::Iron(), "l" + name);

	//Make (in-)visible and give it a color
	auto lSD1VisAtt = new G4VisAttributes(G4Color(0, 0, 1, 0.8)); //(r, g, b , transparency)
	lSD1VisAtt->SetVisibility(true);
	lSD1->SetVisAttributes(lSD1VisAtt);
	return lSD1;
}


G4LogicalVolume* geometries::sphere(std::string name, std::map<std::string, double>& params) {
	return NULL;
}

void geometries::run_placements(G4LogicalVolume* lWorld) {
	for (auto [thing, name, params] : record) {
		if (placers.count(thing)) {
			auto l_volume = placers[thing](name, params);
			G4ThreeVector pos(params["x_pos"], params["y_pos"], params["z_pos"]);
			new G4PVPlacement(0, pos, l_volume, "p" + name, lWorld, false, 0, true);
			G4cout << name << " has been created and placed!" << G4endl;
		}
	}
}

void geometries::add_placer(std::string name, placer_func func) {
	placers[name] = func;
}
