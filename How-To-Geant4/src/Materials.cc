#include "DetectorConstruction.hh"      //Header file where functions classes and variables may be defined (...)
#include "G4NistManager.hh"             //for getting material definitions from the NIST database
#include "G4Material.hh"

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
  Vacuum      = nist->FindOrBuildMaterial("G4_Galactic");
  Copper      = nist->FindOrBuildMaterial("G4_Cu");
  Iron        = nist->FindOrBuildMaterial("G4_Fe");
  Titanium    = nist->FindOrBuildMaterial("G4_Ti");
  Aluminum    = nist->FindOrBuildMaterial("G4_Al");

  // boxMaterial  = nist->FindOrBuildMaterial("G4_Galactic");
  dummyMat     = nist->FindOrBuildMaterial("G4_Galactic");

  //Print all defined materials to console
  // G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}