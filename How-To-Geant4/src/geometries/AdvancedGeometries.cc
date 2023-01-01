/*
This file contains examples on how to define materials and how to create geometries and place them.
There are no external dependencies, you find everything in this file. -> You can copy code snippets and paste them in your simulation.
Remember to include the header-files in your simulation, e.g. if you want to place a Box, you have to put ' #include "G4Box.hh" ' in your file as well.
*/



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

How to import one object from an .stl-file
#pragma region
  ////////////////////
  // CADMesh :: STL //
  ////////////////////
  // import a .stl-file
  // auto bunny_mesh = CADMesh::TessellatedMesh::FromSTL("./Lochflansch.stl");

  // auto bunny_logical = new G4LogicalVolume( bunny_mesh->GetSolid(), 
  //                                            world_mat,
  //                                            "logical",
  //                                            0, 0, 0
  // );

  // new G4PVPlacement( 0,
  //                   G4ThreeVector(10.*cm, 10*cm, 10*cm),
  //                   bunny_logical,
  //                   "bunny",
  //                   logicWorld,
  //                   false,
  //                   0,
  //                   true
  // );
#pragma endregion

//How to import one object from an .obj-file
#pragma region
  ////////////////////
  // CADMesh :: OBJ //
  ////////////////////
  //import .obj-file containing a single part
  auto Flansch_mesh = CADMesh::TessellatedMesh::FromOBJ("./Lochflansch.obj");
  Flansch_mesh->SetScale(1);

  auto logicFlansch = 
    new G4LogicalVolume( Flansch_mesh->GetSolid(),  //its solid - no further steps are neccessary since there is only one object in THIS .obj-file
                                         world_mat, //its material
                                         "Flansch", //its name
                                         0, 0, 0);

  new G4PVPlacement( 0,                               //no rotation
                     G4ThreeVector(240.*cm,0,50.*cm), //position
                     logicFlansch,                    //its logical volume
                     "Flansch",                       //its name
                     logicWorld,                      //its mother volume
                     false,                           //boolean operation?
                     0,                               //copy number
                     true);                           //overlaps checking?

  auto logicFlanschVisAtt = new G4VisAttributes(G4Color(132,135,137, 1)); //(r, g, b , transparency)
  logicFlanschVisAtt->SetVisibility(true);
  logicFlansch->SetVisAttributes(logicFlanschVisAtt);

#pragma endregion

//How to import an assembly from an .obj-file
#pragma region
  ////////////////////
  // CADMesh :: OBJ // - ASSEMBLIES
  ////////////////////
  //import .obj-file containing an assembly
  auto FlanschAssembly_mesh = CADMesh::TessellatedMesh::FromOBJ("./Austrittsfenster.obj");
  FlanschAssembly_mesh->SetScale(1);

  auto logicFlanschAssembly = 
    new G4LogicalVolume( FlanschAssembly_mesh->GetSolid("Flansch"), //its solid - this is an assembly so you have to specify which part you want to load. Parts are named in the .obj-file and lines containing part names start with "o"
                                         world_mat,                 //its material         
                                         "FlanschAssembly",         //its name
                                         0, 0, 0
  );

  new G4PVPlacement( 0,                                 //no rotation
                     G4ThreeVector(240.*cm,0,80.*cm),   //position
                     logicFlanschAssembly,              //its logical volume
                     "FlanschAssembly",                 //its name
                     logicWorld,                        //its mother volume
                     false,                             //boolean operation?
                     0,                                 //copy number
                     true);                             //overlaps checking?

  auto logicFlanschAssemblyVisAtt = new G4VisAttributes(G4Color(132,135,137, 1)); //(r, g, b , transparency)
  logicFlanschAssemblyVisAtt->SetVisibility(true);
  logicFlanschAssembly->SetVisAttributes(logicFlanschAssemblyVisAtt);

  //import .obj-file containing an assembly
  auto Ring_mesh = CADMesh::TessellatedMesh::FromOBJ("./Austrittsfenster.obj");
  Ring_mesh->SetScale(1);

  auto logicRing = 
    new G4LogicalVolume( Ring_mesh->GetSolid("Ring"), //its solid - this is an assembly so you have to specify which part you want to load. Parts are named in the .obj-file and lines containing part names start with "o"
                                         world_mat,   //its material
                                         "Ring",      //its name
                                         0, 0, 0);

  new G4PVPlacement( 0,                               //no rotation
                     G4ThreeVector(240.*cm,0,80.*cm), //position
                     logicRing,                       //its logical volume
                     "Ring",                          //its name
                     logicWorld,                      //its mother volume
                     false,                           //boolean operation?
                     0,                               //copy number
                     true);                           //overlaps checking?

  auto logicRingVisAtt = new G4VisAttributes(G4Color(132,135,137, 1)); //(r, g, b , transparency)
  logicRingVisAtt->SetVisibility(true);
  logicRing->SetVisAttributes(logicRingVisAtt);

  //import .obj-file containing an assembly
  auto Deckel_mesh = CADMesh::TessellatedMesh::FromOBJ("./Austrittsfenster.obj");
  Deckel_mesh->SetScale(1);

  auto logicDeckel = 
    new G4LogicalVolume( Deckel_mesh->GetSolid("Deckel"), //its solid - this is an assembly so you have to specify which part you want to load. Parts are named in the .obj-file and lines containing part names start with "o"
                                         world_mat,       //its material
                                         "Deckel",        //its name
                                         0, 0, 0);

  new G4PVPlacement( 0,
                     G4ThreeVector(240.*cm,0,80.*cm),
                     logicDeckel,
                     "Deckel",
                     logicWorld,
                     false,
                     0,
                     true);

  auto logicDeckelVisAtt = new G4VisAttributes(G4Color(132,135,137, 1)); //(r, g, b , transparency)
  logicDeckelVisAtt->SetVisibility(true);
  logicDeckel->SetVisAttributes(logicDeckelVisAtt);

#pragma endregion

// 
//Copper Colimator with double conical Tungsten Inlet
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------
#pragma region
  //Combine two boxes to Copper collimator and cut a a cylinder because Tungsten Colli does not cover whole range
  G4Box* sSmallCuBox =    
    new G4Box("sSmallCuBox",                  //its name
        8.*cm, 8.*cm, 33.*cm);                //its size: half x, half y, half z
  
  G4Box* sBigCuBox =    
    new G4Box("sBigCuBox",                    //its name
        10.*cm, 10.*cm, 29.*cm);              //its size: half x, half y, half z
      
  G4Tubs* sSmallCuCylinder = 
    new G4Tubs("sSmallCuCylinder",            //name
             0., 3.07*cm,                     //inner radius, outer radius
             2.0*cm,                          //z half length
             0., twopi);                      //min phi, max phi


  G4UnionSolid* sCuBoxes =
    new G4UnionSolid("solid Copper Boxes",          //its name
                  sSmallCuBox,                      //Solid A
                  sBigCuBox,                        //Solid B
                  0,                                //Rotation of B relative to A
                  G4ThreeVector(0,0,62.*cm));       //Translation of B relative to A

  //Subtract 
  G4SubtractionSolid* sCuColli =
    new G4SubtractionSolid("solid Copper Collimator",     //its name
                  sCuBoxes,                               //Solid A
                  sSmallCuCylinder,                       //Solid B
                  0,                                      //Rotation of B relative to A
                  G4ThreeVector(0,0,-31.*cm));            //Translation of B relative to A

  G4LogicalVolume* lCuColli =                         
    new G4LogicalVolume(sCuColli,                   //its solid
                        Copper,                     //its material
                        "logic Copper Collimator"); //its name

  new G4PVPlacement(0,                        //no rotation
              G4ThreeVector(0,0,33.*cm),      //at position
              lCuColli,                       //its logical volume
              "Copper Collimator",            //its name
              logicWorld,                     //its mother  volume
              true,                           //boolean operation?
              0,                              //copy number
              true);                          //overlaps checking?

  //Colorcode Copper
  auto logicCopperCollimatorVisAtt = new G4VisAttributes(G4Color(188./255, 80./255, 47./255, 0.8)); //(r, g, b , transparency)
  logicCopperCollimatorVisAtt->SetVisibility(true);
  lCuColli->SetVisAttributes(logicCopperCollimatorVisAtt);

  //Combine two cylinders to Tungsten collimator
  G4Tubs* sSmallWCylinder = 
    new G4Tubs("sSmallWCylinder",       //name
             0., 3.07*cm,               //inner radius, outer radius
             32.5*cm,                   //z half length
             0., twopi);                //min phi, max phi
  
  G4Tubs* sBigWCylinder = 
    new G4Tubs("sBigWCylinder",         //name
             0., 3.2*cm,                //inner radius, outer radius
             27.5*cm,                   //z half length
             0., twopi);                //min phi, max phi


  G4UnionSolid* sWColli =
    new G4UnionSolid("solid Tungsten Collimator",    //its name
                  sSmallWCylinder,                   //Solid A
                  sBigWCylinder,                     //Solid B
                  0,                                 //Rotation of B relative to A
                  G4ThreeVector(0,0,60.*cm));        //Translation of B relative to A

  G4LogicalVolume* lWColli =                         
    new G4LogicalVolume(sWColli,                            //its solid
                        Densimet180,                        //its material
                        "logic Tungsten Collimator");       //its name

  new G4PVPlacement(0,                        //no rotation
              G4ThreeVector(0,0,3.5*cm),      //at position
              lWColli,                        //its logical volume
              "Tungsten Collimator",          //its name
              lCuColli,                       //its mother  volume
              true,                           //boolean operation?
              0,                              //copy number
              true);                          //overlaps checking?

  //Colorcode Tungsten
  auto logicTungstenInletVisAtt = new G4VisAttributes(G4Color(120./255, 124./255, 133./255, 0.8)); //(r, g, b , transparency)
  logicTungstenInletVisAtt->SetVisibility(true);
  lWColli->SetVisAttributes(logicTungstenInletVisAtt);

  //Combine Cones for collimating shape
  G4Cons* sSmallCone =    
      new G4Cons("solid small Cone",          //name
      0., Entrance_Radius,                               //inner radius side A, outer radius side A (negative side)
      0., Inner_Radius,                               //inner radius side B, outer radius side B (positive side)
      7.5*cm,                                 //z half length
      0., twopi);                             //min phi, max phi

  G4Cons* sBigCone =    
      new G4Cons("solid big Cone",            //name
      0., Inner_Radius,                               //inner radius side A, outer radius side A (negative side)
      0., Exit_Radius,                               //inner radius side B, outer radius side B (positive side)
      52.5*cm,                                //z half length
      0., twopi);                             //min phi, max phi

  G4UnionSolid* sColliShape =
  new G4UnionSolid("solid Collimator Shape",  //its name
                sSmallCone,                   //Solid A
                sBigCone,                     //Solid B
                0,                            //Rotation of B relative to A
                G4ThreeVector(0,0,60.*cm));   //Translation of B relative to A 

  G4LogicalVolume* lColliShape =                         
    new G4LogicalVolume(sColliShape,          //its solid
                        world_mat,            //its material
                        "Collimator Shape");  //its name
                
  new G4PVPlacement(0,                      //no rotation
              G4ThreeVector(0,0,-25.*cm),   //at position
              lColliShape,                  //its logical volume
              "Collimator Shape",           //its name
              lWColli,                      //its mother  volume
              false,                        //boolean operation?
              0,                            //copy number
              true);                        //overlaps checking?

  //Make (in-)visible and give it a color
  auto logicConeVisAtt = new G4VisAttributes(G4Color(1, 1, 1, 0.8)); //(r, g, b , transparency)
  logicConeVisAtt->SetVisibility(true);
  lColliShape->SetVisAttributes(logicConeVisAtt);

#pragma endregion
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------

// 
//AmBe Neutron Source
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------
#pragma region
  //
  // Cylinder - outer Steel Cylinder 
  //
  G4Tubs* sCylinder_out = 
    new G4Tubs("Cylinder_out",                    //name
              0., 7.8/2*mm,                      //inner radius, outer radius
              15./2*mm,                          //z half length
              0., twopi);                      //min phi, max phi

  G4LogicalVolume* lCylinder_out = 
    new G4LogicalVolume(sCylinder_out,        //shape
                        Steel,             //material
                        "Cylinder_out");           //name

  new G4PVPlacement(0,                        //no rotation
              G4ThreeVector(0,0,0),      //position
              lCylinder_out,                  //logical volume
              "Cylinder_out",                     //name
              logicWorld,                     //mother  volume
              false,                          //boolean operation?
              0,                              //copy number
              true);                          //overlaps checking?

  //Make (in-)visible and give it a color
  auto lCylinder_outVisAtt = new G4VisAttributes(G4Color(0, 0, 1, 0.9)); //(r, g, b , transparency)
  lCylinder_outVisAtt->SetVisibility(true);
  lCylinder_out->SetVisAttributes(lCylinder_outVisAtt);

  //
  // Cylinder - inner Beryllium-Oxide Cylinder 
  //
  G4Tubs* sCylinder_in = 
    new G4Tubs("Cylinder_in",                    //name
              0., 4.6/2*mm,                      //inner radius, outer radius
              13./2*mm,                          //z half length
              0., twopi);                      //min phi, max phi

  G4LogicalVolume* lCylinder_in = 
    new G4LogicalVolume(sCylinder_in,        //shape
                        BeO,             //material
                        "Cylinder_in");           //name

  new G4PVPlacement(0,                        //no rotation
              G4ThreeVector(0,0,0),      //position
              lCylinder_in,                  //logical volume
              "Cylinder_in",                     //name
              lCylinder_out,                     //mother  volume
              false,                          //boolean operation?
              0,                              //copy number
              true);                          //overlaps checking?

  //Make (in-)visible and give it a color
  auto lCylinder_inVisAtt = new G4VisAttributes(G4Color(1, 0, 0, 0.8)); //(r, g, b , transparency)
  lCylinder_inVisAtt->SetVisibility(true);
  lCylinder_in->SetVisAttributes(lCylinder_inVisAtt);

#pragma endregion
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------

  PrintParameters();
  //always return the root volume
  //
  return physWorld;
}
