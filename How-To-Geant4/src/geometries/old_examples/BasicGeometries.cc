/*
This file contains examples on basic geometries.
You can copy code snippets and paste them in your simulation.
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

// 
//How to create cubes, boxes and squares and rotate things
// 
#pragma region
  //     
  // simple Box
  // 
  G4Box* solidBox =    
    new G4Box("sBox",                        //its name
        x/2, y/2, z/2);                      //its size: half x, half y, half z -> if you want a box with size 100cm, you must put 50cm here
      
  G4LogicalVolume* logicBox =                         
    new G4LogicalVolume(solidBox,            //its solid it is refering to
                        material,            //its material
                        "Box");              //its name
  
  //G4VPhysicalVolume* physBox=              //you can declare a varibale for placement but it will create a warning if unused   
    new G4PVPlacement(0,                     //0 means no rotation
              G4ThreeVector(0,0,50.*cm),     //position of the center in the simulation
              logicBox,                      //its logical volume it is refering to
              "Box",                         //its name
              logicWorld,                    //its mother volume = the volume it is placed inside of
              false,                         //boolean operation?
              0,                             //copy number
              true);                         //overlaps checking? -> will give a warning at the start of the simulation if there is an overlap with another geometry

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
        x/2, y/2, z/2);                //its size: x half length, y half length, z half length
      
  G4LogicalVolume* logicBoxRotated =                         
    new G4LogicalVolume(solidBoxRotated,     //its solid
                        material,           //its material
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

//How to create cylinders
#pragma region
  //
  // Cylinder
  //
  G4Tubs* solidCylinder = 
    new G4Tubs("Cylinder",                     //name
             r_in, r_out,                      //inner radius, outer radius
             z/2,                              //z half length
             0., twopi);                       //min phi, max phi

  G4LogicalVolume* logicCylinder = 
    new G4LogicalVolume(solidCylinder,        //shape
                       material,             //material
                       "Cylinder");           //name

  new G4PVPlacement(0,                        //no rotation
              G4ThreeVector(60.*cm,0,50.*cm),      //position
              logicCylinder,                  //logical volume
              "Cylinder",                     //name
              logicWorld,                     //mother  volume
              false,                          //boolean operation?
              0,                              //copy number
              true);                          //overlaps checking?

  //Make (in-)visible and give it a color
  auto logicCylinderVisAtt = new G4VisAttributes(G4Color(1, 0, 0, 0.8)); //(r, g, b , transparency)
  logicCylinderVisAtt->SetVisibility(true);
  logicCylinder->SetVisAttributes(logicCylinderVisAtt);

#pragma endregion

//How to create spheres
#pragma region
  //
  // Sphere
  //
  G4Sphere* solidSphere =
    new G4Sphere("Sphere",                    //name
              r_in, r_out,                     //inner radius, outer radius
              0., twopi,                      //min phi, max phi
              0., pi);                        //min rho, max rho
            
  G4LogicalVolume* logicSphere =
    new G4LogicalVolume(solidSphere,          //shape
                        material,            //material
                        "Sphere");            //name

  new G4PVPlacement(0,                        //no rotation
              G4ThreeVector(90.*cm,0,50.*cm),    //position
              logicSphere,                    //logical volume
              "Sphere",                       //name
              logicWorld,                     //mother volume
              false,                          //boolean operation?
              0,                              //copy number
              true);                          //overlaps checking?

  //Make (in-)visible and give it a color
  auto logicSphereVisAtt = new G4VisAttributes(G4Color(1, 0, 0, 0.8)); //(r, g, b , transparency)
  logicSphereVisAtt->SetVisibility(true);
  logicSphere->SetVisAttributes(logicSphereVisAtt);
                

#pragma endregion

//How to create cones
#pragma region
  //
  // Cone
  //
  G4Cons* solidCone =    
      new G4Cons("Cone",                      //name
      0., 0*cm,                               //inner radius side A, outer radius side A (negative side)
      0., 10*cm,                              //inner radius side B, outer radius side B (positive side)
      z/2,                                  //z half length
      0., twopi);                             //min phi, max phi
                        
  G4LogicalVolume* logicCone =                         
    new G4LogicalVolume(solidCone,            //its solid
                        material,            //its material
                        "Cone");              //its name
                
  new G4PVPlacement(0,                        //no rotation
              G4ThreeVector(120.*cm,0,50.*cm),   //at position
              logicCone,                      //its logical volume
              "Cone",                         //its name
              logicWorld,                     //its mother  volume
              false,                          //boolean operation?
              0,                              //copy number
              true);                          //overlaps checking?

  //Make (in-)visible and give it a color
  auto logicConeVisAtt = new G4VisAttributes(G4Color(1, 0, 0, 0.8)); //(r, g, b , transparency)
  logicConeVisAtt->SetVisibility(true);
  logicCone->SetVisAttributes(logicConeVisAtt);


#pragma endregion

//How to create trapezoids
#pragma region
  //
  // Trapezoid
  //
  G4Trd* solidTrapezoid =  
    new G4Trd("Trapezoid",                    //its name
              5., 10*cm,                      //half lenth side A in x direction, half lenth side B in x direction, 
              5., 10*cm,                      //half lenth side A in y direction, half lenth side B in y direction, 
               z/2);                        //half z length
                
  G4LogicalVolume* logicTrapezoid =                         
    new G4LogicalVolume(solidTrapezoid,       //its solid
                        material,            //its material
                        "Trapezoid");         //its name
               
  new G4PVPlacement(0,                        //no rotation
                G4ThreeVector(150.*cm,0,50.*cm),   //at position
                logicTrapezoid,               //its logical volume
                "Trapezoid",                  //its name
                logicWorld,                   //its mother  volume
                false,                        //boolean operation?
                0,                            //copy number
                true);                        //overlaps checking?

  //Make (in-)visible and give it a color
  auto logicTrapezoidVisAtt = new G4VisAttributes(G4Color(1, 0, 0, 0.8)); //(r, g, b , transparency)
  logicTrapezoidVisAtt->SetVisibility(true);
  logicTrapezoid->SetVisAttributes(logicTrapezoidVisAtt);


#pragma endregion

//How to create torus
#pragma region
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
                        material,            //its material
                        "Torus");             //its name

  new G4PVPlacement(0,                        //no rotation
                G4ThreeVector(180.*cm,0,50.*cm),    //at position
                logicTorus,                   //its logical volume
                "Torus",                      //its name
                logicWorld,                   //its mother  volume
                false,                        //boolean operation?
                0,                            //copy number
                true);                        //overlaps checking?                

  //Make (in-)visible and give it a color
  auto logicTorusVisAtt = new G4VisAttributes(G4Color(1, 0, 0, 0.8)); //(r, g, b , transparency)
  logicTorusVisAtt->SetVisibility(true);
  logicTorus->SetVisAttributes(logicTorusVisAtt);

#pragma endregion

//How to use boolean operations (subtract, combine)
#pragma region
  //
  // Solids and Boolean operations
  //(Works but has problems being rendered correctly with OpenGL. OpenGL draws a solid with cloud of points. RayTracer should be fine to render any geometry)
  //
  //Subtract solid B from Solid A
  G4SubtractionSolid* solidBoxMinusCylinder =
    new G4SubtractionSolid("Box-Cylinder",     //its name
                  solidBox,                   //Solid A
                  solidCylinder,              //Solid B
                  0,                          //Rotation of B relative to A
                  G4ThreeVector(0,0,5.*cm));  //Translation of B relative to A

  G4LogicalVolume* logicBoxMinusCylinder =                         
    new G4LogicalVolume(solidBoxMinusCylinder,//its solid
                        material,            //its material
                        "Box-Cylinder");       //its name

  new G4PVPlacement(0,                        //no rotation
              G4ThreeVector(30.*cm,0,120.*cm),           //at position
              logicBoxMinusCylinder,          //its logical volume
              "Box-Cylinder",                  //its name
              logicWorld,                     //its mother  volume
              true,                           //boolean operation?
              0,                              //copy number
              true);                          //overlaps checking?  

  //Make (in-)visible and give it a color
  auto logicBoxMinusCylinderAtt = new G4VisAttributes(G4Color(0, 0, 1, 0.8)); //(r, g, b , transparency)
  logicBoxMinusCylinderAtt->SetVisibility(true);
  logicBoxMinusCylinder->SetVisAttributes(logicBoxMinusCylinderAtt);  

  //Combine solid A and B
  G4UnionSolid* solidBoxPlusCylinder =
    new G4UnionSolid("Box+Cylinder",     //its name
                  solidBox,                   //Solid A
                  solidCylinder,              //Solid B
                  0,                          //Rotation of B relative to A
                  G4ThreeVector(0,0,10.*cm));      //Translation of B relative to A

  G4LogicalVolume* logicBoxPlusCylinder =                         
    new G4LogicalVolume(solidBoxPlusCylinder,//its solid
                        material,            //its material
                        "Box+Cylinder");       //its name

  new G4PVPlacement(0,                        //no rotation
              G4ThreeVector(30.*cm,0,160.*cm),           //at position
              logicBoxPlusCylinder,          //its logical volume
              "Box+Cylinder",                  //its name
              logicWorld,                     //its mother  volume
              true,                           //boolean operation?
              0,                              //copy number
              true);                          //overlaps checking?

  //Make (in-)visible and give it a color
  auto logicBoxPlusCylinderAtt = new G4VisAttributes(G4Color(0, 0, 1, 0.8)); //(r, g, b , transparency)
  logicBoxPlusCylinderAtt->SetVisibility(true);
  logicBoxPlusCylinder->SetVisAttributes(logicBoxPlusCylinderAtt);  

#pragma endregion

//How to put geometries in other geometries and place coppies of them
#pragma region
  //     
  // Placing solids in other solids - useful to make copies of assemblies
  //  
  //Mothervolume
  G4Box* solidMother =    
    new G4Box("Mother",                      //its name
        10.*cm, 10.*cm, 15.*cm);             //its size
      
  G4LogicalVolume* logicMother =                         
    new G4LogicalVolume(solidMother,         //its solid
                        material,           //its material
                        "Mother");           //its name

  //place first entity of Mothervolume            
  new G4PVPlacement(0,                       //no rotation
              G4ThreeVector(210.*cm,0,50.*cm), //at position
              logicMother,                   //its logical volume
              "Mother",                      //its name
              logicWorld,                    //its mother  volume
              false,                         //boolean operation?
              0,                             //copy number
              true);                         //overlaps checking?
  //place second entity of Mothervolume
  new G4PVPlacement(0,                       //no rotation
              G4ThreeVector(210.*cm,0,120.*cm), //at position
              logicMother,                   //its logical volume
              "Mother2",                     //its name
              logicWorld,                    //its mother  volume
              false,                         //boolean operation?
              0,                             //copy number
              true);                         //overlaps checking?
 
  //     
  // Put Cone in Mothervolume
  //  
  new G4PVPlacement(0,                       //no rotation
                    G4ThreeVector(),         //at position
                    logicCone,               //its logical volume
                    "ConeInBox",             //its name
                    logicMother,             //its mother  volume
                    false,                   //boolean operation?
                    0,                       //copy number
                    true);                   //overlaps checking?

#pragma endregion

  PrintParameters(); 
  //always return the root volume
  //
  return physWorld;
}


