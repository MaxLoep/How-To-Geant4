#include "Geometries.hh"

static std::vector<std::tuple<std::string, std::string, std::map<std::string, double>, Materials::MaterialMaker>> record = {};
static std::mutex record_mutex;

static fmap placers = {{"cube", geometries::cube}, {"sphere", geometries::sphere}, {"cylinder", geometries::cylinder}};



Materials::MaterialMaker Materials::get_mat(std::string name) {
	if (known_materials.count(name)) {
		return known_materials[name];
	}

	return Vacuum; // just return vacuum as default, idk, error handling seems over the top for now
}

void Materials::add_custom_mat(std::string mat_name, std::vector<std::tuple<std::string, double>> composition, double density) {
	std::cout << ">> making: " << mat_name << " with composition: " << std::endl;

	std::vector<std::tuple<MaterialMaker, double>> make_up = {};
	for (auto [name, amount] : composition) {
		std::cout << name << " " << amount << std::endl;
		make_up.push_back({get_mat(name), amount});
	}

	known_materials[mat_name] = CustomMat(mat_name, density * g / cm3, make_up);
}

void geometries::register_placement(std::string thing, std::string name, std::map<std::string, double> params, Materials::MaterialMaker material) {
	auto _ = std::lock_guard<std::mutex>(record_mutex);
	record.push_back({thing, name, params, material});
}

void geometries::change_param(std::string name, std::string param, double value) {
	auto _ = std::lock_guard<std::mutex>(record_mutex);
	int id = 0;
	bool found = false;
	for (auto [thing, obj_name, params, material] : record) {
		if (obj_name == name) {
			found = true;
			break;
		} else {
			++ id;
		}
	}
	G4cout << name << " " << param << " " << found << G4endl;
	if (not found) return;
	std::get<2>(record[id])[param] = value;
}


G4LogicalVolume* geometries::cube(std::string name, std::map<std::string, double>& params, Materials::MaterialMaker mat) {
	G4Box* sCube = new G4Box("s" + name, params["size_x"], params["size_y"], params["size_z"]);

	G4LogicalVolume* lCube = new G4LogicalVolume(sCube, mat(), "l" + name);

	//Make (in-)visible and give it a color
	auto color = G4Color(
		params.count("red")? params["red"]: 0.,
		params.count("green")? params["green"]: 0.,
		params.count("blue")? params["blue"]: 1.,
		params.count("alpha")? params["alpha"]: 0.8
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
		params.count("min_phi")? params["phi_min"]: 0.,
		params.count("max_phi")? params["phi_max"]: twopi,
		params.count("min_phi")? params["theta_min"]: 0.,	// shouldnt this be: params.count("min_theta")? params["theta_min"]: 0., 	?
		params.count("max_phi")? params["theta_max"]: pi	// shouldnt this be: params.count("max_theta")? params["theta_max"]: pi  	?
	);
	G4LogicalVolume* lSphere = new G4LogicalVolume(sSphere, mat(), "l" + name);

	//Make (in-)visible and give it a color
	auto color = G4Color(
		params.count("red")? params["red"]: 0.,
		params.count("green")? params["green"]: 1.,
		params.count("blue")? params["blue"]: 0.,
		params.count("alpha")? params["alpha"]: 0.8
	);
	auto lSphereVisAtt = new G4VisAttributes(color); //(r, g, b , transparency)
	lSphereVisAtt->SetVisibility(true);
	lSphere->SetVisAttributes(lSphereVisAtt);
	return lSphere;
}

G4LogicalVolume* geometries::cylinder(std::string name, std::map<std::string, double>& params, Materials::MaterialMaker mat) {
	G4Tubs* sCylinder = new G4Tubs(name,
		params.count("inner_radius")? params["inner_radius"]: 0.,
		params["radius"],
		params["length"],
		params.count("min_phi")? params["phi_min"]: 0.,
		params.count("max_phi")? params["phi_max"]: twopi
	);
	G4LogicalVolume* lCylinder = new G4LogicalVolume(sCylinder, mat(), "l" + name);

	//Make (in-)visible and give it a color
	auto color = G4Color(
		params.count("red")? params["red"]: 0.,
		params.count("green")? params["green"]: 1.,
		params.count("blue")? params["blue"]: 0.,
		params.count("alpha")? params["alpha"]: 0.8
	);
	auto lCylinderVisAtt = new G4VisAttributes(color); //(r, g, b , transparency)
	lCylinderVisAtt->SetVisibility(true);
	lCylinder->SetVisAttributes(lCylinderVisAtt);
	return lCylinder;
}

void geometries::run_placements(G4LogicalVolume* lWorld) {
	for (auto [thing, name, params, material] : record) {
		if (placers.count(thing)) {
			auto l_volume = placers[thing](name, params, material);
			auto rotation = new G4RotationMatrix();
			rotation->rotateX(params.count("rot_x")? params["rot_x"]: 0.);
			rotation->rotateY(params.count("rot_y")? params["rot_y"]: 0.);
			rotation->rotateZ(params.count("rot_z")? params["rot_z"]: 0.);
			G4ThreeVector pos(params["pos_x"], params["pos_y"], params["pos_z"]);
			new G4PVPlacement(rotation, pos, l_volume, "p" + name, lWorld, false, 0, true);
			G4cout << name << " has been created and placed!" << G4endl;
		}
	}
}

void geometries::add_placer(std::string name, placer_func func) {
	placers[name] = func;
}
