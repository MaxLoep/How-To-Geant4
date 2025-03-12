#include "Geometries.hh"

static std::vector<std::tuple<std::string, std::string, std::map<std::string, double>, Materials::MaterialMaker>> record = {};
static std::mutex record_mutex;

static fmap placers = {{"cube", geometries::cube}, {"sphere", geometries::sphere}};


void geometries::register_placement(std::string thing, std::string name, std::map<std::string, double> params, Materials::MaterialMaker material) {
	record.push_back({thing, name, params, material});
}


G4LogicalVolume* geometries::cube(std::string name, std::map<std::string, double>& params, Materials::MaterialMaker mat) {
	G4Box* sCube = new G4Box("s" + name, params["x_size"], params["y_size"], params["z_size"]);

	G4LogicalVolume* lCube = new G4LogicalVolume(sCube, mat(), "l" + name);

	//Make (in-)visible and give it a color
	auto color = G4Color(
		params.count("red")? params["red"]: 0.,
		params.count("green")? params["green"]: 0.,
		params.count("blue")? params["blue"]: 1.,
		params.count("transparency")? params["transparency"]: 0.8
	);
	auto lCubeVisAtt = new G4VisAttributes(color); //(r, g, b , transparency)
	lCubeVisAtt->SetVisibility(true);
	lCube->SetVisAttributes(lCubeVisAtt);
	return lCube;
}


G4LogicalVolume* geometries::sphere(std::string name, std::map<std::string, double>& params, Materials::MaterialMaker mat) {
	G4Sphere* sSphere = new G4Sphere(name,
		params.count("inner_radius")? params["inner_radius"]: 0.,
		params["radius"],
		params.count("min_phi")? params["min_phi"]: 0.,
		params.count("max_phi")? params["max_phi"]: twopi,
		params.count("min_phi")? params["min_theta"]: 0.,
		params.count("max_phi")? params["max_theta"]: pi
	);
	G4LogicalVolume* lSphere = new G4LogicalVolume(sSphere, mat(), "l" + name);

	//Make (in-)visible and give it a color
	auto color = G4Color(
		params.count("red")? params["red"]: 0.,
		params.count("green")? params["green"]: 1.,
		params.count("blue")? params["blue"]: 0.,
		params.count("transparency")? params["transparency"]: 0.8
	);
	auto lSphereVisAtt = new G4VisAttributes(color); //(r, g, b , transparency)
	lSphereVisAtt->SetVisibility(true);
	lSphere->SetVisAttributes(lSphereVisAtt);
	return lSphere;
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
