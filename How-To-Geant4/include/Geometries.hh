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

using placer_func = std::function<G4LogicalVolume*(std::string, std::map<std::string, double>&)>;
using fmap = std::map<std::string, placer_func>;

namespace geometries {
	void register_placement(std::string thing, std::string name, std::map<std::string, double> params);
	void run_placements(G4LogicalVolume* lWorld);
	G4LogicalVolume* cube(std::string name, std::map<std::string, double>& params);
	G4LogicalVolume* sphere(std::string name, std::map<std::string, double>& params);
	void add_placer(std::string name, placer_func func);
}

#endif
