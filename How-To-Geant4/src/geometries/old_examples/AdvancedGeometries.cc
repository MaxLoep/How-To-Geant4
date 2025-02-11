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
  // 
  // Parameters
  // 
  TargetDia = 40.*mm;
  TargetLen = 4.*mm;
  
  TubeDia = 40.*mm;
  TubeLen = 4.*cm;

  //Collimator parameters
  //Beam diameter is ~2cm, so 1cm is used for geometrical calculations
  colli_angle     = 0.92*degree;      //half opening angle - max = 0.92° to reach an Exit radius of 3cm
  x = 1*cm / (std::tan(colli_angle)); //helpful variable for smaller calculation steps
  Entrance_Radius = 2.*cm;
  // Inner_Radius    = 2.*cm;
  // Exit_Radius     = 3.*cm;
  Inner_Radius    = (4.*cm + x) * std::tan(colli_angle);
  Exit_Radius     = (124.*cm + x) * std::tan(colli_angle);

  // 
  // Materials
  // 
  world_mat = nist->FindOrBuildMaterial("G4_Galactic");

  TargetMat = nist->FindOrBuildMaterial("G4_GRAPHITE");
  DetectorMat = nist->FindOrBuildMaterial("G4_Galactic");
  TubeMat = nist->FindOrBuildMaterial("G4_Galactic");
  
  Copper = nist->FindOrBuildMaterial("G4_Cu");
  Tungsten = nist->FindOrBuildMaterial("G4_W");
  Iron = nist->FindOrBuildMaterial("G4_Fe");
  Nickel = nist->FindOrBuildMaterial("G4_Ni");

  //Define Densimet180 (Manufacturer: Plansee)
  Densimet180 = new G4Material("Densimet180",      //name
                                      18.0*g/cm3,          //density
                                      3);                  //number of elements

  //Add Elements to Material
  Densimet180->AddMaterial(Tungsten, 95.*perCent);
  Densimet180->AddMaterial(Iron, 1.6*perCent);
  Densimet180->AddMaterial(Nickel, 3.4*perCent);

  // 
  //Combine two boxes to Copper collimator and cut a a cylinder because Tungsten Colli does not cover whole range
  // 
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

  // 
  // Subtract
  // 
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

  // 
  //Combine two cylinders to Tungsten collimator
  // 
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

  // 
  //Combine Cones for collimating shape
  // 
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
//AmBe Neutron Source - Capsule type X21 - Emission: ~2.2x10^6 n/s/Ci (Amersham) -> ~6x10^-5 n/s/Bq (ISO 8529-1:2021)
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------
#pragma region
  // 
  // Materials
  // 
  world_mat  = nist->FindOrBuildMaterial("G4_Galactic");
  Vacuum     = nist->FindOrBuildMaterial("G4_Galactic");
  Steel      = nist->FindOrBuildMaterial("G4_STAINLESS-STEEL");
  Be         = nist->FindOrBuildMaterial("G4_Be");  //1.848 g/cm3
  O          = nist->FindOrBuildMaterial("G4_O");   //13.67 g/cm3
  Am         = nist->FindOrBuildMaterial("G4_Am");  //0.001429 g/cm3

  //Define AmBeO via number of atoms
  //AmO2-Be
  //AmO2: 0.37g
  //Be:   4.6g
  G4int ncomponents, natoms;
  BeO =  new G4Material("BeO", 3.05*g/cm3, ncomponents=3);
  BeO->AddMaterial(Am, natoms=1);
  BeO->AddMaterial(Be, natoms=1);
  BeO->AddMaterial(O, natoms=1);

  //Define AmBeO via percent
  AmBeO= new G4Material("AmBeO",                           //name
                                      3.05*g/cm3,          //density
                                      3);                  //number of elements

  //Add Elements to Material
  AmBeO->AddMaterial(Am, 95.*perCent);
  AmBeO->AddMaterial(Be, 1.6*perCent);
  AmBeO->AddMaterial(O, 3.4*perCent);

  //
  // Cylinder - outer Steel Cylinder 
  //
  G4Tubs* sCylinder_out = 
    new G4Tubs("Cylinder_out",                   //name
              0., 7.8/2*mm,                      //inner radius, outer radius
              15./2*mm,                          //z half length
              0., twopi);                        //min phi, max phi

  G4LogicalVolume* lCylinder_out = 
    new G4LogicalVolume(sCylinder_out,           //shape
                        Steel,                   //material
                        "Cylinder_out");         //name

  new G4PVPlacement(0,                           //no rotation
              G4ThreeVector(0,0,0),              //position
              lCylinder_out,                     //logical volume
              "Cylinder_out",                    //name
              logicWorld,                        //mother  volume
              false,                             //boolean operation?
              0,                                 //copy number
              true);                             //overlaps checking?

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
              11.8/2*mm,                         //z half length
              0., twopi);                        //min phi, max phi

  G4LogicalVolume* lCylinder_in = 
    new G4LogicalVolume(sCylinder_in,            //shape
                        BeO,                     //material
                        "Cylinder_in");          //name

  new G4PVPlacement(0,                           //no rotation
              G4ThreeVector(0,0,0),              //position
              lCylinder_in,                      //logical volume
              "Cylinder_in",                     //name
              lCylinder_out,                     //mother  volume
              false,                             //boolean operation?
              0,                                 //copy number
              true);                             //overlaps checking?

  //Make (in-)visible and give it a color
  auto lCylinder_inVisAtt = new G4VisAttributes(G4Color(1, 0, 0, 0.8)); //(r, g, b , transparency)
  lCylinder_inVisAtt->SetVisibility(true);
  lCylinder_in->SetVisAttributes(lCylinder_inVisAtt);

#pragma endregion
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------

// 
//Import Mobile Faraday Cup and place it in surrounding CupBox in BoxBox
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------
#pragma region 
  // 
  // Materials
  // 
  //Define borated PE (Röchling- Polystone M nuclear with 5% Boron)
  G4Material* BoratedPE = new G4Material("BoratedPE",      //name
                                      1.03*g/cm3,          //density
                                      3);                  //number of elements

  //Add Elements to Material
  BoratedPE->AddMaterial(nist->FindOrBuildMaterial("G4_H"), 14.*perCent);
  BoratedPE->AddMaterial(nist->FindOrBuildMaterial("G4_C"), 81.*perCent);
  BoratedPE->AddMaterial(nist->FindOrBuildMaterial("G4_B"), 5.*perCent);

  dummyMat     = nist->FindOrBuildMaterial("BoratedPE");
  
  // 
  // surounding BoxBox for the box surrounding the Mobile Faraday Cup
  // 
    G4Box* solidBoxBox =    
    new G4Box("BoxBox",                       //its name
       (150.*mm+b)/2, (120.*mm+b)/2, (298.*mm+b/2)/2 );     //its size - b-size shielding in x- and y-direction but only b/2 in z-direction because not shielding in front
      
  G4LogicalVolume* logicBoxBox =                         
    new G4LogicalVolume(solidBoxBox,          //its solid
                        dummyMat,           //its material
                        "BoxBox");            //its name
                                   
  G4VPhysicalVolume* physBoxBox = 
    new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(0, 0,(298.*mm+b/2)/2),       //at (0,0,0)
                      logicBoxBox,            //its logical volume
                      "BoxBox",               //its name
                      logicWorld,                     //its mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      true);                 //overlaps checking

  //Make BoxBox-volume invisible
  auto logicBoxBoxVisAtt = new G4VisAttributes(G4Color(1, 1, 1, 0.1)); //(r, g, b , transparency)
  logicBoxBoxVisAtt->SetVisibility(true);
  logicBoxBox->SetVisAttributes(logicBoxBoxVisAtt);

  // 
  // surounding Box for Mobile Faraday Cup
  // 
    G4Box* solidCupBox =    
    new G4Box("CupBox",                       //its name
       150.*mm/2, 120.*mm/2, 298.*mm/2);     //its size
      
  G4LogicalVolume* logicCupBox =                         
    new G4LogicalVolume(solidCupBox,          //its solid
                        world_mat,           //its material
                        "CupBox");            //its name
                                   
  G4VPhysicalVolume* physCupBox = 
    new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(0, 0, -b/4),       //at (0,0,0)  - move -b/4 in z-direction for no shielding in front but b-size shielding in the back
                      logicCupBox,            //its logical volume
                      "CupBox",               //its name
                      logicBoxBox,                     //its mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      true);                 //overlaps checking

  //Make CupBox-volume invisible
  auto logicCupBoxVisAtt = new G4VisAttributes(G4Color(1, 1, 1, 0.1)); //(r, g, b , transparency)
  logicCupBoxVisAtt->SetVisibility(true);
  logicCupBox->SetVisAttributes(logicCupBoxVisAtt);


  ////////////////////
  // CADMesh :: OBJ // - ASSEMBLIES
  ////////////////////
  //import .obj-file containing an assembly
  auto FaradayCup_mesh = CADMesh::TessellatedMesh::FromOBJ("./FaradayCupMobile.obj");
  FaradayCup_mesh->SetScale(1);

  //Volumen 1
  auto logicDumpTarget = 
    new G4LogicalVolume( FaradayCup_mesh->GetSolid("DumpTarget"), //its solid - this is an assembly so you have to specify which part you want to load. Parts are named in the .obj-file and lines containing part names start with "o"
                                         Graphite,                 //its material         
                                         "DumpTarget",         //its name
                                         0, 0, 0
  );

  new G4PVPlacement( 0,                                 //no rotation
                     G4ThreeVector(0,0,-16.*mm),   //position
                     logicDumpTarget,              //its logical volume
                     "DumpTarget",                 //its name
                     logicCupBox,                        //its mother volume
                     false,                             //boolean operation?
                     0,                                 //copy number
                     true);                             //overlaps checking?

  auto logicDumpTargetVisAtt = new G4VisAttributes(G4Color(132,135,137, 1)); //(r, g, b , transparency)
  logicDumpTargetVisAtt->SetVisibility(true);
  logicDumpTarget->SetVisAttributes(logicDumpTargetVisAtt);

  //Volumen 2
  auto logicDumpCube = 
    new G4LogicalVolume( FaradayCup_mesh->GetSolid("DumpCube"), //its solid - this is an assembly so you have to specify which part you want to load. Parts are named in the .obj-file and lines containing part names start with "o"
                                         Aluminum,                 //its material         
                                         "DumpCube",         //its name
                                         0, 0, 0
  );

  new G4PVPlacement( 0,                                 //no rotation
                     G4ThreeVector(0,0,-16.*mm),   //position
                     logicDumpCube,              //its logical volume
                     "DumpCube",                 //its name
                     logicCupBox,                        //its mother volume
                     false,                             //boolean operation?
                     0,                                 //copy number
                     true);                             //overlaps checking?

  auto logicDumpCubeVisAtt = new G4VisAttributes(G4Color(132,135,137, 1)); //(r, g, b , transparency)
  logicDumpCubeVisAtt->SetVisibility(true);
  logicDumpCube->SetVisAttributes(logicDumpCubeVisAtt);

  //Volumen 3
  auto logicRingInside = 
    new G4LogicalVolume( FaradayCup_mesh->GetSolid("RingInside"), //its solid - this is an assembly so you have to specify which part you want to load. Parts are named in the .obj-file and lines containing part names start with "o"
                                         world_mat,                 //its material         
                                         "RingInside",         //its name
                                         0, 0, 0
  );

  new G4PVPlacement( 0,                                 //no rotation
                     G4ThreeVector(0,0,-16.*mm),   //position
                     logicRingInside,              //its logical volume
                     "RingInside",                 //its name
                     logicCupBox,                        //its mother volume
                     false,                             //boolean operation?
                     0,                                 //copy number
                     true);                             //overlaps checking?

  auto logicRingInsideVisAtt = new G4VisAttributes(G4Color(132,135,137, 1)); //(r, g, b , transparency)
  logicRingInsideVisAtt->SetVisibility(true);
  logicRingInside->SetVisAttributes(logicRingInsideVisAtt);

  //Volumen 4
  auto logic4WayCross = 
    new G4LogicalVolume( FaradayCup_mesh->GetSolid("4WayCross"), //its solid - this is an assembly so you have to specify which part you want to load. Parts are named in the .obj-file and lines containing part names start with "o"
                                         world_mat,                 //its material         
                                         "4WayCross",         //its name
                                         0, 0, 0
  );

  new G4PVPlacement( 0,                                 //no rotation
                     G4ThreeVector(0,0,-16.*mm),   //position
                     logic4WayCross,              //its logical volume
                     "4WayCross",                 //its name
                     logicCupBox,                        //its mother volume
                     false,                             //boolean operation?
                     0,                                 //copy number
                     true);                             //overlaps checking?

  auto logic4WayCrossVisAtt = new G4VisAttributes(G4Color(132,135,137, 1)); //(r, g, b , transparency)
  logic4WayCrossVisAtt->SetVisibility(true);
  logic4WayCross->SetVisAttributes(logic4WayCrossVisAtt);

  //Volumen 5
  auto logicCylinderkathode = 
    new G4LogicalVolume( FaradayCup_mesh->GetSolid("Cylinderkathode"), //its solid - this is an assembly so you have to specify which part you want to load. Parts are named in the .obj-file and lines containing part names start with "o"
                                         world_mat,                 //its material         
                                         "Cylinderkathode",         //its name
                                         0, 0, 0
  );

  new G4PVPlacement( 0,                                 //no rotation
                     G4ThreeVector(0,0,-16.*mm),   //position
                     logicCylinderkathode,              //its logical volume
                     "Cylinderkathode",                 //its name
                     logicCupBox,                        //its mother volume
                     false,                             //boolean operation?
                     0,                                 //copy number
                     true);                             //overlaps checking?

  auto logicCylinderkathodeVisAtt = new G4VisAttributes(G4Color(132,135,137, 1)); //(r, g, b , transparency)
  logicCylinderkathodeVisAtt->SetVisibility(true);
  logicCylinderkathode->SetVisAttributes(logicCylinderkathodeVisAtt);

  //Volumen 6
  auto logicBlindflansch1 = 
    new G4LogicalVolume( FaradayCup_mesh->GetSolid("Blindflansch1"), //its solid - this is an assembly so you have to specify which part you want to load. Parts are named in the .obj-file and lines containing part names start with "o"
                                         world_mat,                 //its material         
                                         "Blindflansch1",         //its name
                                         0, 0, 0
  );

  new G4PVPlacement( 0,                                 //no rotation
                     G4ThreeVector(0,0,-16.*mm),   //position
                     logicBlindflansch1,              //its logical volume
                     "Blindflansch1",                 //its name
                     logicCupBox,                        //its mother volume
                     false,                             //boolean operation?
                     0,                                 //copy number
                     true);                             //overlaps checking?

  auto logicBlindflansch1VisAtt = new G4VisAttributes(G4Color(132,135,137, 1)); //(r, g, b , transparency)
  logicBlindflansch1VisAtt->SetVisibility(true);
  logicBlindflansch1->SetVisAttributes(logicBlindflansch1VisAtt);

  //Volumen 7
  auto logicBlindflansch2 = 
    new G4LogicalVolume( FaradayCup_mesh->GetSolid("Blindflansch2"), //its solid - this is an assembly so you have to specify which part you want to load. Parts are named in the .obj-file and lines containing part names start with "o"
                                         world_mat,                 //its material         
                                         "Blindflansch2",         //its name
                                         0, 0, 0
  );

  new G4PVPlacement( 0,                                 //no rotation
                     G4ThreeVector(0,0,-16.*mm),   //position
                     logicBlindflansch2,              //its logical volume
                     "Blindflansch2",                 //its name
                     logicCupBox,                        //its mother volume
                     false,                             //boolean operation?
                     0,                                 //copy number
                     true);                             //overlaps checking?

  auto logicBlindflansch2VisAtt = new G4VisAttributes(G4Color(132,135,137, 1)); //(r, g, b , transparency)
  logicBlindflansch2VisAtt->SetVisibility(true);
  logicBlindflansch2->SetVisAttributes(logicBlindflansch2VisAtt);

  //Volumen 8
  auto logicLenardWindow = 
    new G4LogicalVolume( FaradayCup_mesh->GetSolid("LenardWindow"), //its solid - this is an assembly so you have to specify which part you want to load. Parts are named in the .obj-file and lines containing part names start with "o"
                                         world_mat,                 //its material         
                                         "LenardWindow",         //its name
                                         0, 0, 0
  );

  new G4PVPlacement( 0,                                 //no rotation
                     G4ThreeVector(0,0,-16.*mm),   //position
                     logicLenardWindow,              //its logical volume
                     "LenardWindow",                 //its name
                     logicCupBox,                        //its mother volume
                     false,                             //boolean operation?
                     0,                                 //copy number
                     true);                             //overlaps checking?

  auto logicLenardWindowVisAtt = new G4VisAttributes(G4Color(132,135,137, 1)); //(r, g, b , transparency)
  logicLenardWindowVisAtt->SetVisibility(true);
  logicLenardWindow->SetVisAttributes(logicLenardWindowVisAtt);

  //Volumen 9
  auto logicFoil = 
    new G4LogicalVolume( FaradayCup_mesh->GetSolid("Foil"), //its solid - this is an assembly so you have to specify which part you want to load. Parts are named in the .obj-file and lines containing part names start with "o"
                                         world_mat,                 //its material         
                                         "FoilCup",         //its name
                                         0, 0, 0
  );

  new G4PVPlacement( 0,                                 //no rotation
                     G4ThreeVector(0,0,-16.*mm),   //position
                     logicFoil,              //its logical volume
                     "Foil",                 //its name
                     logicCupBox,                        //its mother volume
                     false,                             //boolean operation?
                     0,                                 //copy number
                     true);                             //overlaps checking?

  auto logicFoilVisAtt = new G4VisAttributes(G4Color(132,135,137, 1)); //(r, g, b , transparency)
  logicFoilVisAtt->SetVisibility(true);
  logicFoil->SetVisAttributes(logicFoilVisAtt);

  //Volumen 10
  auto logicRingOutside = 
    new G4LogicalVolume( FaradayCup_mesh->GetSolid("RingOutside"), //its solid - this is an assembly so you have to specify which part you want to load. Parts are named in the .obj-file and lines containing part names start with "o"
                                         world_mat,                 //its material         
                                         "RingOutside",         //its name
                                         0, 0, 0
  );

  new G4PVPlacement( 0,                                 //no rotation
                     G4ThreeVector(0,0,-16.*mm),   //position
                     logicRingOutside,              //its logical volume
                     "RingOutside",                 //its name
                     logicCupBox,                        //its mother volume
                     false,                             //boolean operation?
                     0,                                 //copy number
                     true);                             //overlaps checking?

  auto logicRingOutsideVisAtt = new G4VisAttributes(G4Color(132,135,137, 1)); //(r, g, b , transparency)
  logicRingOutsideVisAtt->SetVisibility(true);
  logicRingOutside->SetVisAttributes(logicRingOutsideVisAtt);

  //     
  // Experimental Hall Floor
  // 
  G4Box* sFloor =    
    new G4Box("sFloor",                        //its name
        14.*m/2, 1.*m/2, 14.*m /2);                   //its size: half x, half y, half z
      
  G4LogicalVolume* lFloor =                         
    new G4LogicalVolume(sFloor,                //its solid
                        Concrete,           //its material
                        "Floor");              //its name
    
    new G4PVPlacement(0,                     //no rotation
              G4ThreeVector(0,-1.33*m - 50.*cm,0),     //position
              lFloor,                          //its logical volume
              "Floor",                         //its name
              logicWorld,                    //its mother  volume
              false,                         //boolean operation?
              0,                             //copy number
              true);                         //overlaps checking?

  //Make (in-)visible and give it a color
  auto lFloorVisAtt = new G4VisAttributes(G4Color(133/255,138/255,126/255, 0.8)); //(r, g, b , transparency)
  lFloorVisAtt->SetVisibility(true);
  lFloor->SetVisAttributes(lFloorVisAtt);

#pragma endregion
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------

// 
//Import Lenard Window and place it in LenardWindowCylinder
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------- 
#pragma region 
  //
  // Cylinder
  //
  G4Tubs* solidLenardWindowCylinder = 
    new G4Tubs("LenardWindowCylinder",                    //name
             0., 65.*mm,                      //inner radius, outer radius
             130.*mm/2,                          //z half length
             0., twopi);                      //min phi, max phi

  G4LogicalVolume* logicLenardWindowCylinder = 
    new G4LogicalVolume(solidLenardWindowCylinder,        //shape
                       world_mat,             //material
                       "LenardWindowCylinder");           //name

  new G4PVPlacement(0,                        //no rotation
              G4ThreeVector(0.*cm,0,0.*cm),      //position
              logicLenardWindowCylinder,                  //logical volume
              "LenardWindowCylinder",                     //name
              logicWorld,                     //mother  volume
              false,                          //boolean operation?
              0,                              //copy number
              true);                          //overlaps checking?

  //Make (in-)visible and give it a color
  auto logicLenardWindowCylinderVisAtt = new G4VisAttributes(G4Color(1, 0, 0, 0.8)); //(r, g, b , transparency)
  logicLenardWindowCylinderVisAtt->SetVisibility(true);
  logicLenardWindowCylinder->SetVisAttributes(logicLenardWindowCylinderVisAtt);

  ////////////////////
  // CADMesh :: OBJ // - ASSEMBLIES
  ////////////////////
  //import .obj-file containing an assembly
  auto LenardWindow_mesh = CADMesh::TessellatedMesh::FromOBJ("./LenardWindowAssembly60.obj");
  LenardWindow_mesh->SetScale(1);

  //Volumen 1
  auto logicTube = 
    new G4LogicalVolume( LenardWindow_mesh->GetSolid("Tube"), //its solid - this is an assembly so you have to specify which part you want to load. Parts are named in the .obj-file and lines containing part names start with "o"
                                         world_mat,                 //its material         
                                         "Tube",         //its name
                                         0, 0, 0
  );

  new G4PVPlacement( 0,                                 //no rotation
                     G4ThreeVector(0,0,55.*mm),   //position
                     logicTube,              //its logical volume
                     "Tube",                 //its name
                     logicLenardWindowCylinder,                        //its mother volume
                     false,                             //boolean operation?
                     0,                                 //copy number
                     true);                             //overlaps checking?

  auto logicTubeVisAtt = new G4VisAttributes(G4Color(132,135,137, 1)); //(r, g, b , transparency)
  logicTubeVisAtt->SetVisibility(true);
  logicTube->SetVisAttributes(logicTubeVisAtt);

  //Volumen 2
  auto logicLenardWindowExit = 
    new G4LogicalVolume( LenardWindow_mesh->GetSolid("LenardWindow"), //its solid - this is an assembly so you have to specify which part you want to load. Parts are named in the .obj-file and lines containing part names start with "o"
                                         world_mat,                 //its material         
                                         "LenardWindowExit",         //its name
                                         0, 0, 0
  );

  new G4PVPlacement( 0,                                 //no rotation
                     G4ThreeVector(0,0,55.*mm),   //position
                     logicLenardWindowExit,              //its logical volume
                     "LenardWindowExit",                 //its name
                     logicLenardWindowCylinder,                        //its mother volume
                     false,                             //boolean operation?
                     0,                                 //copy number
                     true);                             //overlaps checking?

  auto logicLenardWindowExitVisAtt = new G4VisAttributes(G4Color(132,135,137, 1)); //(r, g, b , transparency)
  logicLenardWindowExitVisAtt->SetVisibility(true);
  logicLenardWindowExit->SetVisAttributes(logicLenardWindowExitVisAtt);

  //Volumen 3
  auto logicRingExit = 
    new G4LogicalVolume( LenardWindow_mesh->GetSolid("Ring"), //its solid - this is an assembly so you have to specify which part you want to load. Parts are named in the .obj-file and lines containing part names start with "o"
                                         world_mat,                 //its material         
                                         "RingExit",         //its name
                                         0, 0, 0
  );

  new G4PVPlacement( 0,                                 //no rotation
                     G4ThreeVector(0,0,55.*mm),   //position
                     logicRingExit,              //its logical volume
                     "RingExit",                 //its name
                     logicLenardWindowCylinder,                        //its mother volume
                     false,                             //boolean operation?
                     0,                                 //copy number
                     true);                             //overlaps checking?

  auto logicRingExitVisAtt = new G4VisAttributes(G4Color(132,135,137, 1)); //(r, g, b , transparency)
  logicRingExitVisAtt->SetVisibility(true);
  logicRingExit->SetVisAttributes(logicRingExitVisAtt);

  //Volumen 4
  auto logicFoilExit = 
    new G4LogicalVolume( LenardWindow_mesh->GetSolid("Foil"), //its solid - this is an assembly so you have to specify which part you want to load. Parts are named in the .obj-file and lines containing part names start with "o"
                                         world_mat,                 //its material         
                                         "FoilExit",         //its name
                                         0, 0, 0
  );

  new G4PVPlacement( 0,                                 //no rotation
                     G4ThreeVector(0,0,55.*mm),   //position
                     logicFoilExit,              //its logical volume
                     "FoilExit",                 //its name
                     logicLenardWindowCylinder,                        //its mother volume
                     false,                             //boolean operation?
                     0,                                 //copy number
                     true);                             //overlaps checking?

  auto logicFoilExitVisAtt = new G4VisAttributes(G4Color(132,135,137, 1)); //(r, g, b , transparency)
  logicFoilExitVisAtt->SetVisibility(true);
  logicFoilExit->SetVisAttributes(logicFoilExitVisAtt);

  //
  // Cylinder Vacuum in Tube
  //
  G4Tubs* solidVacuumInTube = 
    new G4Tubs("VacuumInTube",                    //name
             0., 102.*mm/2,                      //inner radius, outer radius
             108.*mm/2,                          //z half length
             0., twopi);                      //min phi, max phi

  G4LogicalVolume* logicVacuumInTube = 
    new G4LogicalVolume(solidVacuumInTube,        //shape
                       world_mat,             //material
                       "VacuumInTube");           //name

  new G4PVPlacement(0,                        //no rotation
              G4ThreeVector(0.*cm,0,-11.*mm),      //position
              logicVacuumInTube,                  //logical volume
              "VacuumInTube",                     //name
              logicLenardWindowCylinder,                     //mother  volume
              false,                          //boolean operation?
              0,                              //copy number
              true);                          //overlaps checking?

  //Make (in-)visible and give it a color
  auto logicVacuumInTubeVisAtt = new G4VisAttributes(G4Color(1, 0, 0, 0.8)); //(r, g, b , transparency)
  logicVacuumInTubeVisAtt->SetVisibility(true);
  logicVacuumInTube->SetVisAttributes(logicVacuumInTubeVisAtt);

#pragma endregion
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------

// 
//Geometry from Celinas Bachelor Thesis
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------
#pragma region 
  
  Cylinder - Outer Steel
  
  G4Tubs* solidOutCylinder = 
    new G4Tubs("OutCylinder",                    //name
             0., 5.1*cm,                      //inner radius, outer radius
             5.*cm,                          //z half length
             0., twopi);                      //min phi, max phi

  G4LogicalVolume* logicOutCylinder = 
    new G4LogicalVolume(solidOutCylinder,        //shape
                       air,             //material
                       "OuterCylinder");           //name

  new G4PVPlacement(0,                        //no rotation
              G4ThreeVector(0,0,15.*cm),      //position
              logicOutCylinder,                  //logical volume
              "OutCylinder",                     //name
              logicWorld,                     //mother  volume
              false,                          //boolean operation?
              0,                              //copy number
              true);                          //overlaps checking?

  //Make (in-)visible and give it a color
  auto logicOutCylinderVisAtt = new G4VisAttributes(G4Color(132,135,137, 0.8)); //(r, g, b , transparency)
  logicOutCylinderVisAtt->SetVisibility(true);
  logicOutCylinder->SetVisAttributes(logicOutCylinderVisAtt);

  //
  // Cylinder - Inner Air
  //
  G4Tubs* solidInnerCylinder = 
    new G4Tubs("InnerCylinder",                    //name
             0., 5.*cm,                      //inner radius, outer radius
             5.*cm,                          //z half length
             0., twopi);                      //min phi, max phi

  G4LogicalVolume* logicInnerCylinder = 
    new G4LogicalVolume(solidInnerCylinder,        //shape
                      //  air,             //material
                      Air_selfmade,
                       "InnerCylinder");           //name

  new G4PVPlacement(0,                        //no rotation
              G4ThreeVector(0,0,0),      //position
              logicInnerCylinder,                  //logical volume
              "InnerCylinder",                     //name
              logicOutCylinder,                     //mother  volume
              false,                          //boolean operation?
              0,                              //copy number
              true);                          //overlaps checking?

  //Make (in-)visible and give it a color
  auto logicInnerCylinderVisAtt = new G4VisAttributes(G4Color(1,0,0, 0.9)); //(r, g, b , transparency)
  logicInnerCylinderVisAtt->SetVisibility(true);
  logicInnerCylinder->SetVisAttributes(logicInnerCylinderVisAtt);

  //
  // Cylinder - foil
  //
  G4Tubs* solidFoilCylinder = 
    new G4Tubs("FoilCylinder",                    //name
             0., 5.1*cm,                      //inner radius, outer radius
             0.025*mm /2,                          //z half length
             0., twopi);                      //min phi, max phi

  G4LogicalVolume* logicFoilCylinder = 
    new G4LogicalVolume(solidFoilCylinder,        //shape
                       foil,             //material
                       "foil");           //name

  new G4PVPlacement(0,                        //no rotation
              G4ThreeVector(0,0,10.*cm - 0.025*mm /2),      //position
              logicFoilCylinder,                  //logical volume
              "FoilCylinder1",                     //name
              logicWorld,                     //mother  volume
              false,                          //boolean operation?
              0,                              //copy number
              true);                          //overlaps checking?

  new G4PVPlacement(0,                        //no rotation
              G4ThreeVector(0,0,20.*cm + 0.025*mm /2),      //position
              logicFoilCylinder,                  //logical volume
              "FoilCylinder2",                     //name
              logicWorld,                     //mother  volume
              false,                          //boolean operation?
              0,                              //copy number
              true);                          //overlaps checking?

  //Make (in-)visible and give it a color
  auto logicFoilCylinderVisAtt = new G4VisAttributes(G4Color(132,135,137, 0.8)); //(r, g, b , transparency)
  logicFoilCylinderVisAtt->SetVisibility(true);
  logicFoilCylinder->SetVisAttributes(logicFoilCylinderVisAtt);
#pragma endregion
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------

  PrintParameters();
  //always return the root volume
  //
  return physWorld;
}
