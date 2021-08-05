/*
This file contains examples on how to define materials and how to create geometries and place them.
There are no external dependencies, you find everything in this file. -> You can copy code snippets and paste them in your simulation.
Remember to include the header-files in your simulation, e.g. if you want to place a Box, you have to put ' #include "G4Box.hh" ' in your file as well.
*/
#include "B1DetectorConstruction.hh"    //Header file where functions classes and variables may be defined (...)
#include "G4RunManager.hh"              //Nessesary. You need this.

#include "G4NistManager.hh"             //for getting material definitions from the NIST database

#include "G4Box.hh"                     //for cuboid
#include "G4Tubs.hh"                    //for cylinder
#include "G4Sphere.hh"                  //for sphere
#include "G4Cons.hh"                    //for cone
#include "G4Trd.hh"                     //for trapezoid
#include "G4Tet.hh"                     //for tetrahedra
#include "G4Torus.hh"                   //for torus

#include "G4LogicalVolume.hh"           //Nessesary. You need this.
#include "G4PVPlacement.hh"             //Nessesary. You need this.
#include "G4RotationMatrix.hh"          //for rotations
#include "G4SubtractionSolid.hh"        //for the boolean subtraction operation
#include "G4SystemOfUnits.hh"           //for units
#include "G4UnitsTable.hh"              //for units
#include "G4PhysicalConstants.hh"       //for physial constants like pi


B1DetectorConstruction::B1DetectorConstruction()
: G4VUserDetectorConstruction(),
  fScoringVolume(0)
{ }

B1DetectorConstruction::~B1DetectorConstruction()
{ }


G4VPhysicalVolume* B1DetectorConstruction::Construct()
{ 
  //BASICS:
  //
  //How to declare variables
  //It's basically C++, but everything starts with 'G4'
  G4int a    = 1;
  G4double b = 2.5;

  //
  //How to write stuff in output when executing
  //Again, basically C++ but starts with 'G4'
  G4cout  << "\n This is a comment! "
          << "\n  a = " << a
          << "\n  b = " << b << G4endl;

  //MATERIALS:
  //
  //How to define Materials using the NIST database
  //(They will produce warnings if they are unused variables in the simulation therefore they are commented out.)
  //
  // Get nist material manager
  G4NistManager* nist = G4NistManager::Instance();

  G4Material* world_mat = nist->FindOrBuildMaterial("G4_AIR");
  G4Material* material1 = nist->FindOrBuildMaterial("G4_Pb");

  // //How to define Elements with NIST in their natural abundance
  // G4Material* H  = nist->FindOrBuildMaterial("G4_H");
  // G4Material* He = nist->FindOrBuildMaterial("G4_He");
  // G4Material* Be = nist->FindOrBuildMaterial("G4_Be");

  // //Special NIST Compounds (just a selection, not complete)
  // G4Material* Air      = nist->FindOrBuildMaterial("G4_AIR");
  // G4Material* Water    = nist->FindOrBuildMaterial("G4_WATER");
  // G4Material* Concrete = nist->FindOrBuildMaterial("G4_CONCRETE");
  // G4Material* Stilbene = nist->FindOrBuildMaterial("G4_STILBENE");
  // G4Material* Graphite = nist->FindOrBuildMaterial("G4_GRAPHITE");
  // G4Material* Steel    = nist->FindOrBuildMaterial("G4_STAINLESS-STEEL");
  // G4Material* Galactic = nist->FindOrBuildMaterial("G4_GALACTIC");

  //How to define Elements by hand
  G4Element* Hydrogen = new G4Element("Hydrogen", "H", 1., 1.0079*g/mole);
  G4Element* Carbon   = new G4Element("Carbon", "C", 6., 12.011*g/mole);

  //How to define a Material by hand
  G4Material* BC400 = new G4Material("BC400",      //name
                                      1.032*g/cm3, //density
                                      2);          //number of elements

  //Add elements to material
  BC400->AddElement(Hydrogen, 8.5*perCent);
  BC400->AddElement(Carbon, 91.5*perCent);


  //SOLIDS, GEOMETRIES, PLACEMENT, ETC.
  //Start with creating a World-Volume to place things in
  // 
  // World
  // 
  G4double world_sizeXYZ = 2.*m;
  
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
                      false,                 //no boolean operation
                      0,                     //copy number
                      true);                 //overlaps checking
                     
  //     
  // Box
  //  
  G4Box* solidBox =    
    new G4Box("Box",                         //its name
        10*cm, 10*cm, 10*cm);                //its size: half x, half y, half z
      
  G4LogicalVolume* logicBox =                         
    new G4LogicalVolume(solidBox,            //its solid
                        material1,           //its material
                        "Box");              //its name
               
  new G4PVPlacement(0,                       //no rotation
              G4ThreeVector(0,0,50.*cm),     //position
              logicBox,                      //its logical volume
              "Box",                         //its name
              logicWorld,                    //its mother  volume
              false,                         //no boolean operation
              0,                             //copy number
              true);                         //overlaps checking

  //
  // How to rotate an object
  //
  G4RotationMatrix* BoxRotation = new G4RotationMatrix();
  BoxRotation->rotateX(45*deg);
  BoxRotation->rotateY(45*deg);
  BoxRotation->rotateZ(45*deg);

  G4Box* solidBoxRotated =    
    new G4Box("BoxRotated",                  //its name
        10*cm, 10*cm, 10*cm);                //its size: x half length, y half length, z half length
      
  G4LogicalVolume* logicBoxRotated =                         
    new G4LogicalVolume(solidBoxRotated,     //its solid
                        material1,           //its material
                        "BoxRotated");       //its name
               
  new G4PVPlacement(BoxRotation,             //no rotation
              G4ThreeVector(0,0,80.*cm),     //position
              logicBoxRotated,               //its logical volume
              "Box",                         //its name
              logicWorld,                    //its mother  volume
              false,                         //no boolean operation
              0,                             //copy number
              true);                         //overlaps checking

  //
  // Cylinder
  //
  G4Tubs* solidCylinder = 
    new G4Tubs("Cylinder",                    //name
             0., 10.*cm,                      //inner radius, outer radius
             10.*cm,                          //z half length
             0., twopi);                      //min phi, max phi

  G4LogicalVolume* logicCylinder = 
    new G4LogicalVolume(solidCylinder,        //shape
                       material1,             //material
                       "Cylinder");           //name

  new G4PVPlacement(0,                        //no rotation
              G4ThreeVector(0,50.*cm,0),      //position
              logicCylinder,                  //logical volume
              "Cylinder",                     //name
              logicWorld,                     //mother  volume
              false,                          //no boolean operation
              0,                              //copy number
              true);                          //overlaps checking

  //
  // Cylinder2
  //
  G4Tubs* solidCylinder2 = 
    new G4Tubs("Cylinder2",                   //name
             5.*cm, 10.*cm,                   //inner radius, outer radius
             10.*cm,                          //z half length
             0., pi);                         //min phi, max phi

  G4LogicalVolume* logicCylinder2 = 
    new G4LogicalVolume(solidCylinder2,       //shape
                       material1,             //material
                       "Cylinder2");          //name

  new G4PVPlacement(0,                        //no rotation
              G4ThreeVector(0,80.*cm,0),      //position
              logicCylinder2,                 //logical volume
              "Cylinder2",                    //name
              logicWorld,                     //mother  volume
              false,                          //no boolean operation
              0,                              //copy number
              true);                          //overlaps checking

  //
  // Sphere
  //
  G4Sphere* solidSphere =
    new G4Sphere("Sphere",                    //name
              0., 10.*cm,                     //inner radius, outer radius
              0., twopi,                      //min phi, max phi
              0., pi);                        //min rho, max rho
            
  G4LogicalVolume* logicSphere =
    new G4LogicalVolume(solidSphere,          //shape
                        material1,            //material
                        "Sphere");            //name

  new G4PVPlacement(0,                        //no rotation
              G4ThreeVector(50.*cm, 0, 0),    //position
              logicSphere,                    //logical volume
              "Sphere",                       //name
              logicWorld,                     //mother volume
              false,                          //no boolean operation
              0,                              //copy number
              true);                          //overlaps checking
                
  //
  // Sphere2
  //
  G4Sphere* solidSphere2 =
    new G4Sphere("Sphere2",                   //name
              5.*cm, 10.*cm,                  //inner radius, outer radius
              0., pi,                         //min phi, max phi
              0., pi);                        //min rho, max rho
            
  G4LogicalVolume* logicSphere2 =
    new G4LogicalVolume(solidSphere2,         //shape
                        material1,            //material
                        "Sphere2");           //name

  new G4PVPlacement(0,                        //no rotation
              G4ThreeVector(80.*cm, 0, 0),    //position
              logicSphere2,                   //logical volume
              "Sphere2",                      //name
              logicWorld,                     //mother volume
              false,                          //no boolean operation
              0,                              //copy number
              true);                          //overlaps checking

  //
  // Cone
  //
  G4Cons* solidCone =    
      new G4Cons("Cone",                      //name
      0., 0*cm,                               //inner radius side A, outer radius side A
      0., 10*cm,                              //inner radius side B, outer radius side B
      10*cm,                                  //z half length
      0., twopi);                             //min phi, max phi
                        
  G4LogicalVolume* logicCone =                         
    new G4LogicalVolume(solidCone,            //its solid
                        material1,            //its material
                        "Cone");              //its name
                
  new G4PVPlacement(0,                        //no rotation
              G4ThreeVector(-50.*cm, 0, 0),   //at position
              logicCone,                      //its logical volume
              "Cone",                         //its name
              logicWorld,                     //its mother  volume
              false,                          //no boolean operation
              0,                              //copy number
              true);                          //overlaps checking

  //
  // Cone2
  //
  G4Cons* solidCone2 =    
      new G4Cons("Cone2",                     //name
      2.*cm, 5*cm,                            //inner radius side A, outer radius side A
      7.*cm, 10*cm,                           //inner radius side B, outer radius side B
      10*cm,                                  //z half length
      0., pi);                                //min phi, max phi
                        
  G4LogicalVolume* logicCone2 =                         
    new G4LogicalVolume(solidCone2,           //its solid
                        material1,            //its material
                        "Cone2");             //its name
                
  new G4PVPlacement(0,                        //no rotation
              G4ThreeVector(-80.*cm, 0, 0),   //at position
              logicCone2,                     //its logical volume
              "Cone2",                        //its name
              logicWorld,                     //its mother  volume
              false,                          //no boolean operation
              0,                              //copy number
              true);                          //overlaps checking

  //
  // Trapezoid
  //
  G4Trd* solidTrapezoid =  
    new G4Trd("Trapezoid",                    //its name
              0., 10*cm,                      //half lenth side A in x direction, half lenth side B in x direction, 
              0., 10*cm,                      //half lenth side A in y direction, half lenth side B in y direction, 
               10*cm);                        //half z length
                
  G4LogicalVolume* logicTrapezoid =                         
    new G4LogicalVolume(solidTrapezoid,       //its solid
                        material1,            //its material
                        "Trapezoid");         //its name
               
  new G4PVPlacement(0,                        //no rotation
                G4ThreeVector(0,0,-50.*cm),   //at position
                logicTrapezoid,               //its logical volume
                "Trapezoid",                  //its name
                logicWorld,                   //its mother  volume
                false,                        //no boolean operation
                0,                            //copy number
                true);                        //overlaps checking

  //
  // Trapezoid2
  //
  G4Trd* solidTrapezoid2 =  
    new G4Trd("Trapezoid2",                   //its name
              5*cm, 10*cm,                    //half lenth side A in x direction, half lenth side B in x direction, 
              5*cm, 10*cm,                    //half lenth side A in y direction, half lenth side B in y direction, 
               10*cm);                        //half z length
                
  G4LogicalVolume* logicTrapezoid2 =                         
    new G4LogicalVolume(solidTrapezoid2,       //its solid
                        material1,             //its material
                        "Trapezoid2");         //its name
               
  new G4PVPlacement(0,                         //no rotation
                G4ThreeVector(0,0,-80.*cm),    //at position
                logicTrapezoid2,               //its logical volume
                "Trapezoid2",                  //its name
                logicWorld,                    //its mother  volume
                false,                         //no boolean operation
                0,                             //copy number
                true);                         //overlaps checking

  //
  // Torus
  //
  G4Torus* solidTorus =  
    new G4Torus("Torus",                      //its name
                0.*cm,                        //Rmin 
                5.*cm,                        //Rmax
                10.*cm,                       //RTorus
                twopi,                        //Sphi
                twopi);                       //Dphi
                
  G4LogicalVolume* logicTorus =                         
    new G4LogicalVolume(solidTorus,           //its solid
                        material1,            //its material
                        "Torus");             //its name
               
  new G4PVPlacement(0,                        //no rotation
                G4ThreeVector(0,-50*cm,0),    //at position
                logicTorus,                   //its logical volume
                "Torus",                      //its name
                logicWorld,                   //its mother  volume
                false,                        //no boolean operation
                0,                            //copy number
                true);                        //overlaps checking                

  //
  // Solids and Boolean operations
  //(Works but has problems being rendered correctly with OpenGL. OpenGL draws a solid with cloud of points. RayTracer should be fine to render any geometry)
  G4SubtractionSolid* solidBoxMinusCylinder =
    new G4SubtractionSolid("BoxCylinder",     //its name
                  solidBox,                   //Solid A
                  solidCylinder,              //Solid B
                  0,                          //Rotation
                  G4ThreeVector(0,0,0));      //Translation

  G4LogicalVolume* logicBoxMinusCylinder =                         
    new G4LogicalVolume(solidBoxMinusCylinder,//its solid
                        material1,            //its material
                        "BoxCylinder");       //its name

  new G4PVPlacement(0,                        //no rotation
              G4ThreeVector(0,0,0),           //at position
              logicBoxMinusCylinder,          //its logical volume
              "BoxCylinder",                  //its name
              logicWorld,                     //its mother  volume
              true,                           //no boolean operation
              0,                              //copy number
              true);                          //overlaps checking    

  //     
  // Placing solids in other solids - useful to make copies of assemblies
  //  
  //Mothervolume
  G4Box* solidMother =    
    new G4Box("Mother",                      //its name
        10.*cm, 10.*cm, 15.*cm);             //its size
      
  G4LogicalVolume* logicMother =                         
    new G4LogicalVolume(solidMother,         //its solid
                        material1,           //its material
                        "Mother");           //its name

  //place first entity of Mothervolume            
  new G4PVPlacement(0,                       //no rotation
              G4ThreeVector(60.*cm, 30*cm, 60*cm), //at position
              logicMother,                   //its logical volume
              "Mother",                      //its name
              logicWorld,                    //its mother  volume
              false,                         //no boolean operation
              0,                             //copy number
              true);                         //overlaps checking
  //place second entity of Mothervolume
  new G4PVPlacement(0,                       //no rotation
              G4ThreeVector(60.*cm, 90*cm, 60*cm), //at position
              logicMother,                   //its logical volume
              "Mother2",                     //its name
              logicWorld,                    //its mother  volume
              false,                         //no boolean operation
              0,                             //copy number
              true);                         //overlaps checking
 
  //     
  // Put Cone in Mothervolume
  //  
  new G4PVPlacement(0,                       //no rotation
                    G4ThreeVector(),         //at position
                    logicCone,               //its logical volume
                    "ConeInBox",             //its name
                    logicMother,             //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    true);                   //overlaps checking

                
  // Set Box as scoring volume
  //This is a public variable defined in the header file to make it accessible from other files
  fScoringVolume = logicBox;

  //
  //always return the physical World
  //
  return physWorld;
}


