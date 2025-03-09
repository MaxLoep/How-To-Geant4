#include "Geometries.hh"


static std::vector<std::tuple<std::string, std::string, std::map<std::string, double>, Materials::MaterialMaker>> record = {};
static fmap placers = {{"cube", geometries::cube}, {"sphere", geometries::sphere}};


void geometries::register_placement(std::string thing, std::string name, std::map<std::string, double> params, Materials::MaterialMaker material) {
	record.push_back({thing, name, params, material});
}


G4LogicalVolume* geometries::cube(std::string name, std::map<std::string, double>& params, Materials::MaterialMaker mat) {
	G4Box* sSD1 = new G4Box("s" + name, params["x_size"], params["y_size"], params["z_size"]);

	G4LogicalVolume* lSD1 = new G4LogicalVolume(sSD1, mat(), "l" + name);

	//Make (in-)visible and give it a color
	auto lSD1VisAtt = new G4VisAttributes(G4Color(0, 0, 1, 0.8)); //(r, g, b , transparency)
	lSD1VisAtt->SetVisibility(true);
	lSD1->SetVisAttributes(lSD1VisAtt);
	return lSD1;
}


G4LogicalVolume* geometries::sphere(std::string name, std::map<std::string, double>& params, Materials::MaterialMaker mat) {
	return NULL;
}

void geometries::run_placements(G4LogicalVolume* lWorld) {
	for (auto [thing, name, params, material] : record) {
		if (placers.count(thing)) {
			auto l_volume = placers[thing](name, params, material);
			G4ThreeVector pos(params["x_pos"], params["y_pos"], params["z_pos"]);
			new G4PVPlacement(0, pos, l_volume, "p" + name, lWorld, false, 0, true);
			G4cout << name << " has been created and placed!" << G4endl;
		}
	}
}

void geometries::add_placer(std::string name, placer_func func) {
	placers[name] = func;
}
