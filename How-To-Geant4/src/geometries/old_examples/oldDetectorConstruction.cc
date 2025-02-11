/*
This file contains examples on how to define materials and how to create geometries and place them.
There are no external dependencies, you find everything in this file. -> You can copy code snippets and paste them in your simulation.
Remember to include the header-files in your simulation, e.g. if you want to place a Box, you have to put ' #include "G4Box.hh" ' in your file as well.
*/

/*
E- and B-Fields (see B5 maybe?)
Scoring
New ways to create materials , see Book for Application Developer
*/

#include "DetectorConstruction.hh"      //Header file where functions classes and variables may be defined (...)
#include "DetectorMessenger.hh"         //Header file for own macro commands
#include "G4RunManager.hh"              //Necessary. You need this.

#include "G4NistManager.hh"             //for getting material definitions from the NIST database
#include "G4Material.hh"

#include "G4Box.hh"                     //for cuboid
#include "G4Tubs.hh"                    //for cylinder
#include "G4Sphere.hh"                  //for sphere
#include "G4Cons.hh"                    //for cone
#include "G4Trd.hh"                     //for trapezoid
#include "G4Tet.hh"                     //for tetrahedra
#include "G4Torus.hh"                   //for torus

#include "G4LogicalVolume.hh"           //Necessary. You need this.
#include "G4PVPlacement.hh"             //Necessary. You need this.
#include "G4RotationMatrix.hh"          //for rotations
#include "G4SubtractionSolid.hh"        //for the boolean subtraction operation
#include "G4UnionSolid.hh"              //for the boolean combination operation
#include "G4SystemOfUnits.hh"           //for units
#include "G4UnitsTable.hh"              //for units
#include "G4PhysicalConstants.hh"       //for physial constants like pi

#include "G4VisAttributes.hh"           //for Visualization
#include "G4Color.hh"                   //for Visualization

//Primitive Scorer from example B4d
#include "G4SDManager.hh"
#include "G4SDChargedFilter.hh"
#include "G4MultiFunctionalDetector.hh"
#include "G4VPrimitiveScorer.hh"
#include "G4PSEnergyDeposit.hh"
#include "G4PSTrackLength.hh"
#include "G4PSFlatSurfaceCurrent.hh"

#include "G4SDParticleWithEnergyFilter.hh"
#include "G4SDParticleFilter.hh"
#include "G4SDChargedFilter.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"

#include "BoxSD.hh"                     //the SensitiveDetector

#include "CADMesh.hh"                   // for importing CAD-files (.stl, .obj, ...). Read all about it at: https://github.com/christopherpoole/CADMesh

DetectorConstruction::DetectorConstruction()
:G4VUserDetectorConstruction(),
 fAbsorMaterial(nullptr), fLAbsor(nullptr), fWorldMaterial(nullptr), world_mat(nullptr),
 fWorldVolume(nullptr), fDetectorMessenger(nullptr),
 fScoringVolume(0)
{
  // default geometrical parameters
  fAbsorThickness = 1*cm;
  fAbsorSizeYZ    = 1*cm;
  fWorldSizeX     = 1.2*fAbsorThickness;
  fWorldSizeYZ    = 1.2*fAbsorSizeYZ;

  //Variables which can be changed by macro commands should be set here
  boxsizeX      = 10. *cm;
  boxsizeYZ     = 10. *cm;

  // materials
  DefineMaterials(); // see below for this function
  SetAbsorMaterial("G4_Co");

  // create commands for interactive definition of the geometry
  fDetectorMessenger = new DetectorMessenger(this);
}

DetectorConstruction::~DetectorConstruction()
{ delete fDetectorMessenger;}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  return ConstructVolumes();
}

//Define materials and compositions you want to use in the simulation
void DetectorConstruction::DefineMaterials()
{
  //MATERIALS:
  //
  //How to define Materials using the NIST database
  //see https://geant4-userdoc.web.cern.ch/UsersGuides/ForApplicationDeveloper/html/Appendix/materialNames.html for a list of available materials
  //(They will produce warnings if they are unused variables in the simulation therefore they are commented out.)
  //
  // Get nist material manager
  G4NistManager* nist = G4NistManager::Instance();

  //G4Material* world_mat = nist->FindOrBuildMaterial("G4_Galactic");
  // world_mat = nist->FindOrBuildMaterial("G4_Galactic");
  // material1 = nist->FindOrBuildMaterial("G4_Co");

  world_mat  = nist->FindOrBuildMaterial("G4_Galactic");
  material1  = nist->FindOrBuildMaterial("G4_Cu");

  // //How to define Elements with NIST in their natural abundance
  // G4Material* H  = nist->FindOrBuildMaterial("G4_H");
  // G4Material* He = nist->FindOrBuildMaterial("G4_He");
  // G4Material* Be = nist->FindOrBuildMaterial("G4_Be");

  // //Special NIST Compounds (just a selection, not complete)
  // G4Material* Air      = nist->FindOrBuildMaterial("G4_AIR");
  // G4Material* Water    = nist->FindOrBuildMaterial("G4_WATER");
  // G4Material* Concrete = nist->FindOrBuildMaterial("G4_CONCRETE");
  // G4Material* Stilbene = nist->FindOrBuildMaterial("G4_STILBENE");
  // Graphite = nist->FindOrBuildMaterial("G4_GRAPHITE");
  // G4Material* Steel    = nist->FindOrBuildMaterial("G4_STAINLESS-STEEL");
  // G4Material* Galactic = nist->FindOrBuildMaterial("G4_Galactic");
 
  //How to define Isotopes by hand
  //G4Isotope* name = new G4Isotope("name", #protons, #nucleons);
  G4Isotope* U235 = new G4Isotope("U235", 92, 235);
  G4Isotope* U238 = new G4Isotope("U238", 92, 238);

  //How to define an Element by using isotopes with their abundance
  //G4Element* name = new G4Element("name", "symbol", #isotopes);
  G4Element* enrichedUranium= new G4Element("enriched uranium", "U", 2);
  //
  //Add Isotopes to Element
  enrichedUranium->AddIsotope(U235, 90.*perCent);
  enrichedUranium->AddIsotope(U238, 10.*perCent);

  //How to define Elements by hand
  //G4Element* name = new G4Element("name", "symbol", atomic number, molar mass);
  G4Element* Hydrogen = new G4Element("Hydrogen", "H", 1., 1.0079*g/mole);
  G4Element* Carbon   = new G4Element("Carbon", "C", 6., 12.011*g/mole);
  G4Element* Oxygen   = new G4Element("Oxygen", "o", 8., 16.*g/mole);

  //How to define a Material by hand
  //G4Material* name = new G4Material("name", atomic number, molar mass, density)
  //G4Material* U = new G4Material("Uranium", 92., 238.03*g/mole, 18.950*g/cm3);

  //How to define an Material by using elements with their abundance
  G4Material* BC400 = new G4Material("BC400",      //name
                                      1.032*g/cm3, //density
                                      2);          //number of elements

  //Add Elements to Material
  BC400->AddElement(Hydrogen, 8.5*perCent);
  BC400->AddElement(Carbon, 91.5*perCent);

  //How to define an Material by using elements with their composition number
  // pressurized water
  G4Material* H2O = new G4Material("Water_ts",    //name
                                    1.000*g/cm3,  //density
                                    2,            //number of elements
                                    kStateLiquid, //state of matter
                                    593*kelvin,   //temperature
                                    150*bar);     //pressure
  H2O->AddElement(Hydrogen, 2);
  H2O->AddElement(Oxygen, 1);
  H2O->GetIonisation()->SetMeanExcitationEnergy(78.0*eV);

/*
  //NE213
  G4Element* H  = new G4Element("Hydrogen" ,"H" , 1.,  1.01*g/mole);
  G4Element* C  = new G4Element("Carbon"   ,"C" , 6., 12.00*g/mole);
  G4Material* ne213 = 
  new G4Material("NE213", 0.874*g/cm3, 2);
  ne213->AddElement(H,    9.2*perCent);
  ne213->AddElement(C,   90.8*perCent);
*/

  //Print all defined materials to console
  G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}


//
//Functions for custom GUI and macro commands - see DetectorMessenger.cc
//
G4Material* DetectorConstruction::MaterialWithSingleIsotope( G4String name,
                           G4String symbol, G4double density, G4int Z, G4int A)
{
 // define a material from an isotope
 //
 G4int ncomponents;
 G4double abundance, massfraction;

 G4Isotope* isotope = new G4Isotope(symbol, Z, A);
 
 G4Element* element  = new G4Element(name, symbol, ncomponents=1);
 element->AddIsotope(isotope, abundance= 100.*perCent);
 
 G4Material* material = new G4Material(name, density, ncomponents=1);
 material->AddElement(element, massfraction=100.*perCent);

 return material;
}

G4VPhysicalVolume* DetectorConstruction::ConstructVolumes()
{
  // Cleanup old geometry
  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();

  //SOLIDS, GEOMETRIES, PLACEMENT, ETC.
  /*
  How to create solids
  It's basically a process with 3 steps:
  1.: Create a Geometry e.g. a Box, Cylinder, Sphere or even a Box minus a Cylinder (-> see boolean operation)
  2.: Make it a Logical Volume by assigning a material to it
  3.: Place it in your simulation
  */ 

  //Start with creating a World-Volume to place things in
  // 
  // World
  // 
  G4double world_sizeXYZ = 10*m;
  
  G4Box* solidWorld =    
    new G4Box("World",                       //its name
       0.5*world_sizeXYZ, 0.5*world_sizeXYZ, 0.5*world_sizeXYZ);     //its size
      
  G4LogicalVolume* logicWorld =                         
    new G4LogicalVolume(solidWorld,          //its solid
                        world_mat,           //its material
                        "World");            //its name
                                   
  G4VPhysicalVolume* physWorld = 
    new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(),       //at (0,0,0)
                      logicWorld,            //its logical volume
                      "World",               //its name
                      0,                     //its mother  volume
                      false,                 //boolean operation?
                      0,                     //copy number
                      true);                 //overlaps checking?

  //Make world-volume invisible
  auto logicWorldVisAtt = new G4VisAttributes(G4Color(1, 1, 1, 0.01)); //(r, g, b , transparency)
  logicWorldVisAtt->SetVisibility(true);
  logicWorld->SetVisAttributes(logicWorldVisAtt);

//How to import one object from an .stl-file
// #pragma region
//   ////////////////////
//   // CADMesh :: STL //
//   ////////////////////
//   // import a .stl-file
//   // auto bunny_mesh = CADMesh::TessellatedMesh::FromSTL("./Lochflansch.stl");

//   // auto bunny_logical = new G4LogicalVolume( bunny_mesh->GetSolid(), 
//   //                                            world_mat,
//   //                                            "logical",
//   //                                            0, 0, 0
//   // );

//   // new G4PVPlacement( 0,
//   //                   G4ThreeVector(10.*cm, 10*cm, 10*cm),
//   //                   bunny_logical,
//   //                   "bunny",
//   //                   logicWorld,
//   //                   false,
//   //                   0,
//   //                   true
//   // );
// #pragma endregion

// //How to import one object from an .obj-file
// #pragma region
//   ////////////////////
//   // CADMesh :: OBJ //
//   ////////////////////
//   //import .obj-file containing a single part
//   auto Flansch_mesh = CADMesh::TessellatedMesh::FromOBJ("./Lochflansch.obj");
//   Flansch_mesh->SetScale(1);

//   auto logicFlansch = 
//     new G4LogicalVolume( Flansch_mesh->GetSolid(),  //its solid - no further steps are neccessary since there is only one object in THIS .obj-file
//                                          world_mat, //its material
//                                          "Flansch", //its name
//                                          0, 0, 0);

//   new G4PVPlacement( 0,                               //no rotation
//                      G4ThreeVector(240.*cm,0,50.*cm), //position
//                      logicFlansch,                    //its logical volume
//                      "Flansch",                       //its name
//                      logicWorld,                      //its mother volume
//                      false,                           //boolean operation?
//                      0,                               //copy number
//                      true);                           //overlaps checking?

//   auto logicFlanschVisAtt = new G4VisAttributes(G4Color(132,135,137, 1)); //(r, g, b , transparency)
//   logicFlanschVisAtt->SetVisibility(true);
//   logicFlansch->SetVisAttributes(logicFlanschVisAtt);

// #pragma endregion

// //How to import an assembly from an .obj-file
// #pragma region
//   ////////////////////
//   // CADMesh :: OBJ // - ASSEMBLIES
//   ////////////////////
//   //import .obj-file containing an assembly
//   auto FlanschAssembly_mesh = CADMesh::TessellatedMesh::FromOBJ("./Austrittsfenster.obj");
//   FlanschAssembly_mesh->SetScale(1);

//   auto logicFlanschAssembly = 
//     new G4LogicalVolume( FlanschAssembly_mesh->GetSolid("Flansch"), //its solid - this is an assembly so you have to specify which part you want to load. Parts are named in the .obj-file and lines containing part names start with "o"
//                                          world_mat,                 //its material         
//                                          "FlanschAssembly",         //its name
//                                          0, 0, 0
//   );

//   new G4PVPlacement( 0,                                 //no rotation
//                      G4ThreeVector(240.*cm,0,80.*cm),   //position
//                      logicFlanschAssembly,              //its logical volume
//                      "FlanschAssembly",                 //its name
//                      logicWorld,                        //its mother volume
//                      false,                             //boolean operation?
//                      0,                                 //copy number
//                      true);                             //overlaps checking?

//   auto logicFlanschAssemblyVisAtt = new G4VisAttributes(G4Color(132,135,137, 1)); //(r, g, b , transparency)
//   logicFlanschAssemblyVisAtt->SetVisibility(true);
//   logicFlanschAssembly->SetVisAttributes(logicFlanschAssemblyVisAtt);

//   //import .obj-file containing an assembly
//   auto Ring_mesh = CADMesh::TessellatedMesh::FromOBJ("./Austrittsfenster.obj");
//   Ring_mesh->SetScale(1);

//   auto logicRing = 
//     new G4LogicalVolume( Ring_mesh->GetSolid("Ring"), //its solid - this is an assembly so you have to specify which part you want to load. Parts are named in the .obj-file and lines containing part names start with "o"
//                                          world_mat,   //its material
//                                          "Ring",      //its name
//                                          0, 0, 0);

//   new G4PVPlacement( 0,                               //no rotation
//                      G4ThreeVector(240.*cm,0,80.*cm), //position
//                      logicRing,                       //its logical volume
//                      "Ring",                          //its name
//                      logicWorld,                      //its mother volume
//                      false,                           //boolean operation?
//                      0,                               //copy number
//                      true);                           //overlaps checking?

//   auto logicRingVisAtt = new G4VisAttributes(G4Color(132,135,137, 1)); //(r, g, b , transparency)
//   logicRingVisAtt->SetVisibility(true);
//   logicRing->SetVisAttributes(logicRingVisAtt);

//   //import .obj-file containing an assembly
//   auto Deckel_mesh = CADMesh::TessellatedMesh::FromOBJ("./Austrittsfenster.obj");
//   Deckel_mesh->SetScale(1);

//   auto logicDeckel = 
//     new G4LogicalVolume( Deckel_mesh->GetSolid("Deckel"), //its solid - this is an assembly so you have to specify which part you want to load. Parts are named in the .obj-file and lines containing part names start with "o"
//                                          world_mat,       //its material
//                                          "Deckel",        //its name
//                                          0, 0, 0);

//   new G4PVPlacement( 0,
//                      G4ThreeVector(240.*cm,0,80.*cm),
//                      logicDeckel,
//                      "Deckel",
//                      logicWorld,
//                      false,
//                      0,
//                      true);

//   auto logicDeckelVisAtt = new G4VisAttributes(G4Color(132,135,137, 1)); //(r, g, b , transparency)
//   logicDeckelVisAtt->SetVisibility(true);
//   logicDeckel->SetVisAttributes(logicDeckelVisAtt);

// #pragma endregion

//How to create cubes, boxes and squares and rotate things
#pragma region
  //     
  // Box
  // 
  G4Box* solidBox =    
    new G4Box("sBox",                        //its name
        boxsizeX, boxsizeYZ, boxsizeYZ);     //its size: half x, half y, half z
      
  G4LogicalVolume* logicBox =                         
    new G4LogicalVolume(solidBox,            //its solid
                        material1,           //its material
                        "Box");              //its name
  
  //G4VPhysicalVolume* physBox=              //you can declare a varibale for placement but it will create a warning if unused   
    new G4PVPlacement(0,                     //no rotation
              G4ThreeVector(0,0,50.*cm),     //position
              logicBox,                      //its logical volume
              "Box",                         //its name
              logicWorld,                    //its mother  volume
              false,                         //boolean operation?
              0,                             //copy number
              true);                         //overlaps checking?

  //Make (in-)visible and give it a color
  //logicBox->SetVisAttributes (G4VisAttributes::GetInvisible());
  auto logicBoxVisAtt = new G4VisAttributes(G4Color(1, 0, 0, 0.8)); //(r, g, b , transparency)
  logicBoxVisAtt->SetVisibility(true);
  logicBox->SetVisAttributes(logicBoxVisAtt);

  //
  // How to rotate an object
  //
  //Create a Rotation Matrix
  G4RotationMatrix* BoxRotation = new G4RotationMatrix();
  BoxRotation->rotateX(0*deg);
  BoxRotation->rotateY(20*deg);
  BoxRotation->rotateZ(0*deg);

  G4Box* solidBoxRotated =    
    new G4Box("BoxRotated",                  //its name
        10*cm, 10*cm, 10*cm);                //its size: x half length, y half length, z half length
      
  G4LogicalVolume* logicBoxRotated =                         
    new G4LogicalVolume(solidBoxRotated,     //its solid
                        material1,           //its material
                        "BoxRotated");       //its name
           
  new G4PVPlacement(BoxRotation,             // <----------- ROTATION HAPPENS HERE!
              G4ThreeVector(30.*cm,0,80.*cm),     //position
              logicBoxRotated,               //its logical volume
              "rotatedBox",                         //its name
              logicWorld,                    //its mother  volume
              false,                         //boolean operation?
              0,                             //copy number
              true);                         //overlaps checking?

  //Make (in-)visible and give it a color
  auto logicBox2VisAtt = new G4VisAttributes(G4Color(0, 1, 0, 0.8)); //(r, g, b , transparency)
  logicBox2VisAtt->SetVisibility(true);
  logicBoxRotated->SetVisAttributes(logicBox2VisAtt);

#pragma endregion

// //How to create cylinders
// #pragma region
//   //
//   // Cylinder
//   //
//   G4Tubs* solidCylinder = 
//     new G4Tubs("Cylinder",                    //name
//              0., 10.*cm,                      //inner radius, outer radius
//              10.*cm,                          //z half length
//              0., twopi);                      //min phi, max phi

//   G4LogicalVolume* logicCylinder = 
//     new G4LogicalVolume(solidCylinder,        //shape
//                        material1,             //material
//                        "Cylinder");           //name

//   new G4PVPlacement(0,                        //no rotation
//               G4ThreeVector(60.*cm,0,50.*cm),      //position
//               logicCylinder,                  //logical volume
//               "Cylinder",                     //name
//               logicWorld,                     //mother  volume
//               false,                          //boolean operation?
//               0,                              //copy number
//               true);                          //overlaps checking?

//   //Make (in-)visible and give it a color
//   auto logicCylinderVisAtt = new G4VisAttributes(G4Color(1, 0, 0, 0.8)); //(r, g, b , transparency)
//   logicCylinderVisAtt->SetVisibility(true);
//   logicCylinder->SetVisAttributes(logicCylinderVisAtt);

//   //
//   // Cylinder2
//   //
//   G4Tubs* solidCylinder2 = 
//     new G4Tubs("Cylinder2",                   //name
//              5.*cm, 10.*cm,                   //inner radius, outer radius
//              10.*cm,                          //z half length
//              0., pi);                         //min phi, max phi

//   G4LogicalVolume* logicCylinder2 = 
//     new G4LogicalVolume(solidCylinder2,       //shape
//                        material1,             //material
//                        "Cylinder2");          //name

//   new G4PVPlacement(0,                        //no rotation
//               G4ThreeVector(60.*cm,0,80.*cm),      //position
//               logicCylinder2,                 //logical volume
//               "Cylinder2",                    //name
//               logicWorld,                     //mother  volume
//               false,                          //boolean operation?
//               0,                              //copy number
//               true);                          //overlaps checking?

//   //Make (in-)visible and give it a color
//   auto logicCylinder2VisAtt = new G4VisAttributes(G4Color(0, 1, 0, 0.8)); //(r, g, b , transparency)
//   logicCylinder2VisAtt->SetVisibility(true);
//   logicCylinder2->SetVisAttributes(logicCylinder2VisAtt);

// #pragma endregion

// //How to create spheres
// #pragma region
//   //
//   // Sphere
//   //
//   G4Sphere* solidSphere =
//     new G4Sphere("Sphere",                    //name
//               0., 10.*cm,                     //inner radius, outer radius
//               0., twopi,                      //min phi, max phi
//               0., pi);                        //min rho, max rho
            
//   G4LogicalVolume* logicSphere =
//     new G4LogicalVolume(solidSphere,          //shape
//                         material1,            //material
//                         "Sphere");            //name

//   new G4PVPlacement(0,                        //no rotation
//               G4ThreeVector(90.*cm,0,50.*cm),    //position
//               logicSphere,                    //logical volume
//               "Sphere",                       //name
//               logicWorld,                     //mother volume
//               false,                          //boolean operation?
//               0,                              //copy number
//               true);                          //overlaps checking?

//   //Make (in-)visible and give it a color
//   auto logicSphereVisAtt = new G4VisAttributes(G4Color(1, 0, 0, 0.8)); //(r, g, b , transparency)
//   logicSphereVisAtt->SetVisibility(true);
//   logicSphere->SetVisAttributes(logicSphereVisAtt);
                
//   //
//   // Sphere2
//   //
//   G4Sphere* solidSphere2 =
//     new G4Sphere("Sphere2",                   //name
//               5.*cm, 10.*cm,                  //inner radius, outer radius
//               0., pi,                         //min phi, max phi
//               0., pi);                        //min rho, max rho
            
//   G4LogicalVolume* logicSphere2 =
//     new G4LogicalVolume(solidSphere2,         //shape
//                         material1,            //material
//                         "Sphere2");           //name

//   new G4PVPlacement(0,                        //no rotation
//               G4ThreeVector(90.*cm,0,80.*cm),    //position
//               logicSphere2,                   //logical volume
//               "Sphere2",                      //name
//               logicWorld,                     //mother volume
//               false,                          //boolean operation?
//               0,                              //copy number
//               true);                          //overlaps checking?

//   //Make (in-)visible and give it a color
//   auto logicSphere2VisAtt = new G4VisAttributes(G4Color(0, 1, 0, 0.8)); //(r, g, b , transparency)
//   logicSphere2VisAtt->SetVisibility(true);
//   logicSphere2->SetVisAttributes(logicSphere2VisAtt);

// #pragma endregion

// //How to create cones
// #pragma region
//   //
//   // Cone
//   //
//   G4Cons* solidCone =    
//       new G4Cons("Cone",                      //name
//       0., 0*cm,                               //inner radius side A, outer radius side A (negative side)
//       0., 10*cm,                              //inner radius side B, outer radius side B (positive side)
//       10*cm,                                  //z half length
//       0., twopi);                             //min phi, max phi
                        
//   G4LogicalVolume* logicCone =                         
//     new G4LogicalVolume(solidCone,            //its solid
//                         material1,            //its material
//                         "Cone");              //its name
                
//   new G4PVPlacement(0,                        //no rotation
//               G4ThreeVector(120.*cm,0,50.*cm),   //at position
//               logicCone,                      //its logical volume
//               "Cone",                         //its name
//               logicWorld,                     //its mother  volume
//               false,                          //boolean operation?
//               0,                              //copy number
//               true);                          //overlaps checking?

//   //Make (in-)visible and give it a color
//   auto logicConeVisAtt = new G4VisAttributes(G4Color(1, 0, 0, 0.8)); //(r, g, b , transparency)
//   logicConeVisAtt->SetVisibility(true);
//   logicCone->SetVisAttributes(logicConeVisAtt);

//   //
//   // Cone2
//   //
//   G4Cons* solidCone2 =    
//       new G4Cons("Cone2",                     //name
//       2.*cm, 5*cm,                            //inner radius side A, outer radius side A
//       7.*cm, 10*cm,                           //inner radius side B, outer radius side B
//       10*cm,                                  //z half length
//       0., pi);                                //min phi, max phi
                        
//   G4LogicalVolume* logicCone2 =                         
//     new G4LogicalVolume(solidCone2,           //its solid
//                         material1,            //its material
//                         "Cone2");             //its name
                
//   new G4PVPlacement(0,                        //no rotation
//               G4ThreeVector(120.*cm,0,80.*cm),   //at position
//               logicCone2,                     //its logical volume
//               "Cone2",                        //its name
//               logicWorld,                     //its mother  volume
//               false,                          //boolean operation?
//               0,                              //copy number
//               true);                          //overlaps checking?

//   //Make (in-)visible and give it a color
//   auto logicCone2VisAtt = new G4VisAttributes(G4Color(0, 1, 0, 0.8)); //(r, g, b , transparency)
//   logicCone2VisAtt->SetVisibility(true);
//   logicCone2->SetVisAttributes(logicCone2VisAtt);

// #pragma endregion

// //How to create trapezoids
// #pragma region
//   //
//   // Trapezoid
//   //
//   G4Trd* solidTrapezoid =  
//     new G4Trd("Trapezoid",                    //its name
//               0., 10*cm,                      //half lenth side A in x direction, half lenth side B in x direction, 
//               0., 10*cm,                      //half lenth side A in y direction, half lenth side B in y direction, 
//                10*cm);                        //half z length
                
//   G4LogicalVolume* logicTrapezoid =                         
//     new G4LogicalVolume(solidTrapezoid,       //its solid
//                         material1,            //its material
//                         "Trapezoid");         //its name
               
//   new G4PVPlacement(0,                        //no rotation
//                 G4ThreeVector(150.*cm,0,50.*cm),   //at position
//                 logicTrapezoid,               //its logical volume
//                 "Trapezoid",                  //its name
//                 logicWorld,                   //its mother  volume
//                 false,                        //boolean operation?
//                 0,                            //copy number
//                 true);                        //overlaps checking?

//   //Make (in-)visible and give it a color
//   auto logicTrapezoidVisAtt = new G4VisAttributes(G4Color(1, 0, 0, 0.8)); //(r, g, b , transparency)
//   logicTrapezoidVisAtt->SetVisibility(true);
//   logicTrapezoid->SetVisAttributes(logicTrapezoidVisAtt);

//   //
//   // Trapezoid2
//   //
//   G4Trd* solidTrapezoid2 =  
//     new G4Trd("Trapezoid2",                   //its name
//               5*cm, 10*cm,                    //half lenth side A in x direction, half lenth side B in x direction, 
//               5*cm, 10*cm,                    //half lenth side A in y direction, half lenth side B in y direction, 
//                10*cm);                        //half z length
                
//   G4LogicalVolume* logicTrapezoid2 =                         
//     new G4LogicalVolume(solidTrapezoid2,       //its solid
//                         material1,             //its material
//                         "Trapezoid2");         //its name
               
//   new G4PVPlacement(0,                         //no rotation
//                 G4ThreeVector(150.*cm,0,80.*cm),    //at position
//                 logicTrapezoid2,               //its logical volume
//                 "Trapezoid2",                  //its name
//                 logicWorld,                    //its mother  volume
//                 false,                         //boolean operation?
//                 0,                             //copy number
//                 true);                         //overlaps checking?

//   //Make (in-)visible and give it a color
//   auto logicTrapezoid2VisAtt = new G4VisAttributes(G4Color(0, 1, 0, 0.8)); //(r, g, b , transparency)
//   logicTrapezoid2VisAtt->SetVisibility(true);
//   logicTrapezoid2->SetVisAttributes(logicTrapezoid2VisAtt);

// #pragma endregion

// //How to create torus
// #pragma region
//   //
//   // Torus
//   //
//   G4Torus* solidTorus =  
//     new G4Torus("Torus",                      //its name
//                 0.*cm,                        //Rmin 
//                 5.*cm,                        //Rmax
//                 10.*cm,                       //RTorus
//                 twopi,                        //Sphi
//                 twopi);                       //Dphi
                
//   G4LogicalVolume* logicTorus =                         
//     new G4LogicalVolume(solidTorus,           //its solid
//                         material1,            //its material
//                         "Torus");             //its name

//   new G4PVPlacement(0,                        //no rotation
//                 G4ThreeVector(180.*cm,0,50.*cm),    //at position
//                 logicTorus,                   //its logical volume
//                 "Torus",                      //its name
//                 logicWorld,                   //its mother  volume
//                 false,                        //boolean operation?
//                 0,                            //copy number
//                 true);                        //overlaps checking?                

//   //Make (in-)visible and give it a color
//   auto logicTorusVisAtt = new G4VisAttributes(G4Color(1, 0, 0, 0.8)); //(r, g, b , transparency)
//   logicTorusVisAtt->SetVisibility(true);
//   logicTorus->SetVisAttributes(logicTorusVisAtt);

// #pragma endregion

// //How to use boolean operations (subtract, combine)
// #pragma region
//   //
//   // Solids and Boolean operations
//   //(Works but has problems being rendered correctly with OpenGL. OpenGL draws a solid with cloud of points. RayTracer should be fine to render any geometry)
//   //
//   //Subtract solid B from Solid A
//   G4SubtractionSolid* solidBoxMinusCylinder =
//     new G4SubtractionSolid("Box-Cylinder",     //its name
//                   solidBox,                   //Solid A
//                   solidCylinder,              //Solid B
//                   0,                          //Rotation of B relative to A
//                   G4ThreeVector(0,0,5.*cm));  //Translation of B relative to A

//   G4LogicalVolume* logicBoxMinusCylinder =                         
//     new G4LogicalVolume(solidBoxMinusCylinder,//its solid
//                         material1,            //its material
//                         "Box-Cylinder");       //its name

//   new G4PVPlacement(0,                        //no rotation
//               G4ThreeVector(30.*cm,0,120.*cm),           //at position
//               logicBoxMinusCylinder,          //its logical volume
//               "Box-Cylinder",                  //its name
//               logicWorld,                     //its mother  volume
//               true,                           //boolean operation?
//               0,                              //copy number
//               true);                          //overlaps checking?  

//   //Make (in-)visible and give it a color
//   auto logicBoxMinusCylinderAtt = new G4VisAttributes(G4Color(0, 0, 1, 0.8)); //(r, g, b , transparency)
//   logicBoxMinusCylinderAtt->SetVisibility(true);
//   logicBoxMinusCylinder->SetVisAttributes(logicBoxMinusCylinderAtt);  

//   //Combine solid A and B
//   G4UnionSolid* solidBoxPlusCylinder =
//     new G4UnionSolid("Box+Cylinder",     //its name
//                   solidBox,                   //Solid A
//                   solidCylinder,              //Solid B
//                   0,                          //Rotation of B relative to A
//                   G4ThreeVector(0,0,10.*cm));      //Translation of B relative to A

//   G4LogicalVolume* logicBoxPlusCylinder =                         
//     new G4LogicalVolume(solidBoxPlusCylinder,//its solid
//                         material1,            //its material
//                         "Box+Cylinder");       //its name

//   new G4PVPlacement(0,                        //no rotation
//               G4ThreeVector(30.*cm,0,160.*cm),           //at position
//               logicBoxPlusCylinder,          //its logical volume
//               "Box+Cylinder",                  //its name
//               logicWorld,                     //its mother  volume
//               true,                           //boolean operation?
//               0,                              //copy number
//               true);                          //overlaps checking?

//   //Make (in-)visible and give it a color
//   auto logicBoxPlusCylinderAtt = new G4VisAttributes(G4Color(0, 0, 1, 0.8)); //(r, g, b , transparency)
//   logicBoxPlusCylinderAtt->SetVisibility(true);
//   logicBoxPlusCylinder->SetVisAttributes(logicBoxPlusCylinderAtt);  

// #pragma endregion

// //How to put geometries in other geometries and place coppies of them
// #pragma region
//   //     
//   // Placing solids in other solids - useful to make copies of assemblies
//   //  
//   //Mothervolume
//   G4Box* solidMother =    
//     new G4Box("Mother",                      //its name
//         10.*cm, 10.*cm, 15.*cm);             //its size
      
//   G4LogicalVolume* logicMother =                         
//     new G4LogicalVolume(solidMother,         //its solid
//                         material1,           //its material
//                         "Mother");           //its name

//   //place first entity of Mothervolume            
//   new G4PVPlacement(0,                       //no rotation
//               G4ThreeVector(210.*cm,0,50.*cm), //at position
//               logicMother,                   //its logical volume
//               "Mother",                      //its name
//               logicWorld,                    //its mother  volume
//               false,                         //boolean operation?
//               0,                             //copy number
//               true);                         //overlaps checking?
//   //place second entity of Mothervolume
//   new G4PVPlacement(0,                       //no rotation
//               G4ThreeVector(210.*cm,0,120.*cm), //at position
//               logicMother,                   //its logical volume
//               "Mother2",                     //its name
//               logicWorld,                    //its mother  volume
//               false,                         //boolean operation?
//               0,                             //copy number
//               true);                         //overlaps checking?
 
//   //     
//   // Put Cone in Mothervolume
//   //  
//   new G4PVPlacement(0,                       //no rotation
//                     G4ThreeVector(),         //at position
//                     logicCone,               //its logical volume
//                     "ConeInBox",             //its name
//                     logicMother,             //its mother  volume
//                     false,                   //boolean operation?
//                     0,                       //copy number
//                     true);                   //overlaps checking?

// #pragma endregion

  //B1 SCORING METHOD
  //You need also Code for this one to work in:
  //SteppingAction.cc,  RunAction.cc, EventAction.cc          
  // Set logical Box volume as scoring volume (must be a logical volume)
  //This is a public variable defined in the header file to make it accessible from other files
  fScoringVolume = logicBox;

  PrintParameters();
  
  //always return the root volume
  //
  return physWorld;
}

void DetectorConstruction::PrintParameters()
{
  G4cout << "\n The Absorber is " << G4BestUnit(fAbsorThickness,"Length")
         << " of " << material1->GetName() 
         << "\n \n" << material1 << G4endl;
}


//
//Functions for custom GUI and macro commands - see DetectorMessenger.cc
//
void DetectorConstruction::SetAbsorMaterial(G4String materialChoice)
{
  // search the material by its name
  G4Material* pttoMaterial =
     G4NistManager::Instance()->FindOrBuildMaterial(materialChoice);   
  
  if (pttoMaterial) { 
    fAbsorMaterial = pttoMaterial;
    if(fLAbsor) { fLAbsor->SetMaterial(fAbsorMaterial); }
    G4RunManager::GetRunManager()->PhysicsHasBeenModified();
  } else {
    G4cout << "\n--> warning from DetectorConstruction::SetMaterial : "
           << materialChoice << " not found" << G4endl;
  }              
}

void DetectorConstruction::SetAbsorThickness(G4double value)
{
  fAbsorThickness = value;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

void DetectorConstruction::SetAbsorSizeYZ(G4double value)
{
  fAbsorSizeYZ = value;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}


//
//Assign Detectors and Scorers to Volume
//
void DetectorConstruction::ConstructSDandField()
{
  G4SDManager::GetSDMpointer()->SetVerboseLevel(1);

  //
  //SENSITIVE DETECTORS
  //You need also Code for this one to work in:
  //BoxSD.cc to specify what to quantity to track (Energy, position, etc.)
  //RunAction.cc to open a file and declare ntuple or histograms to save data in
  //Make a Volume a Sensitive Detector (SD); SD are able to access Track/Step information of Particles going through e.g. :
  //Kinetic energy, Momentum


  //Declare a Sensitive Detector
  auto boxSD = new BoxSD("BoxSD");                      //create a new Sensitive Detector
  G4SDManager::GetSDMpointer()->AddNewDetector(boxSD);  //add new SD to SDManager
  SetSensitiveDetector("Box", boxSD);                   //Apply Sensitive Detector 'boxSD' to Volume 'Box'


  // // 
  // //PRIMITIVE SCORERS
  // //You need also Code for this one to work in:
  // //RunAction.cc, EventAction.cc
  // //Make a Volume a Primitive Scorer (PS); PS are able to save information on events related to inside the volume e.g. :
  // //energy deposit, track length, current, flux


  // //Filters
  //Declare filters on Particles, Charge, Energy
  G4String fltName,particleName;

  // //charged particle filter
  // auto charged = new G4SDChargedFilter("chargedFilter");
  // G4SDChargedFilter* chargedFilter = new G4SDChargedFilter(fltName="chargedFilter");

  // //Proton filter
  G4SDParticleFilter* protonFilter =
  new G4SDParticleFilter(fltName="protonFilter", particleName="proton");
  
  // //Electron filter
  // G4SDParticleFilter* electronFilter =
  // new G4SDParticleFilter(fltName="electronFilter");
  // electronFilter->add(particleName="e+");   // accept electrons.
  // electronFilter->add(particleName="e-");   // accept positrons.
  
  // //Neutron filter
  // G4SDParticleFilter* neutronFilter =
  // new G4SDParticleFilter(fltName="neutronFilter", particleName="neutron");
  
  // //Gamma filter
  // G4SDParticleFilter* gammaFilter =
  // new G4SDParticleFilter("gammaFilter", "gamma");

  // //Proton energy filter (Protons in energy range)
  // G4SDParticleWithEnergyFilter* protonEnergy=
  // new G4SDParticleWithEnergyFilter(fltName="protonEnergy");
  // protonEnergy->add("proton");
  // protonEnergy->SetKineticEnergy(200*MeV, 300*MeV); //Only particles with an energy between these values are counted as long as they are between these values

  // //Neutron energy filter (Neutrons in energy range)
  // G4SDParticleWithEnergyFilter* neutronEnergy=
  // new G4SDParticleWithEnergyFilter(fltName="neutronEnergy");
  // neutronEnergy->add("neutron");
  // neutronEnergy->SetKineticEnergy(100*keV, 300*MeV);

  // //Declare a volume as a MultiFunctionalDetector scorer 
  auto boxPS = new G4MultiFunctionalDetector("Scorer");
  G4SDManager::GetSDMpointer()->AddNewDetector(boxPS);

  // //Declare what quantity should be scored and apply filters
  // //
  // //Score Deposited Energy (of protons)
  G4VPrimitiveScorer* primitive;
  primitive = new G4PSEnergyDeposit("Edep");
  primitive ->SetFilter(protonFilter);

  //Register Filters to Scorer
  boxPS->RegisterPrimitive(primitive);

  // //Score TrackLength (of protons/charged particle)
  // primitive = new G4PSTrackLength("TrackLength");
  // //primitive ->SetFilter(protonFilter);
  // //primitive ->SetFilter(charged);

  // //Register Filters to Scorer
  // boxPS->RegisterPrimitive(primitive);  

  //Apply Scorer to Volume
  SetSensitiveDetector("Box",boxPS);
  

  // //Declare a volume as a MultiFunctionalDetector scorer 
  // //(Same as above. Copied from Example B4d. It has 2 Primitive Scorers)
  // auto gapDetector = new G4MultiFunctionalDetector("Gap");
  // G4SDManager::GetSDMpointer()->AddNewDetector(gapDetector);

  // //Declare what quantity should be scored and apply filters
  // //
  // //Score Deposited Energy
  // primitive = new G4PSEnergyDeposit("Edep");
  // //primitive ->SetFilter(protonEnergy);
  // //primitive ->SetFilter(neutronFilter);

  // //Register Filters to Scorer
  // gapDetector->RegisterPrimitive(primitive);
  
  // //Score TrackLength (of protons/charged particle/neutrons)
  // primitive = new G4PSTrackLength("TrackLength");
  // //primitive ->SetFilter(chargedFilter);
  // //primitive ->SetFilter(protonEnergy);
  // //primitive ->SetFilter(neutronFilter);

  // //Register Filters to Scorer
  // gapDetector->RegisterPrimitive(primitive);  
  
  // //Apply Scorer to Volume
  // SetSensitiveDetector("Box",gapDetector); 

}