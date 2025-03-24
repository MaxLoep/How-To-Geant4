#ifndef COLLIMATORGEO
#define COLLIMATORGEO
#include "Geometries.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"

#include "DetectorConstruction.hh"      //Header file where functions classes and variables may be defined (...)
#include "DetectorMessenger.hh"         //Header file for own macro commands
#include "G4RunManager.hh"              //Necessary. You need this.

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

#include "SensitiveDetector.hh"         // the SensitiveDetector file
#include "CADMesh.hh"                   // for importing CAD-files (.stl, .obj, ...). Read all about it at: https://github.com/christopherpoole/CADMesh
#include "G4GDMLParser.hh"              // for importing/exporting GDML-files



G4LogicalVolume* collimator(std::string name, std::map<std::string, double>& params, Materials::MaterialMaker mat) {
	G4double TargetDia = 50.*mm;
	G4double TargetLen = 3.1*mm;
	double a = params.count("a")? params["a"] : 20.*cm;  //thickness of shielding
	double b = params.count("b")? params["b"] : 4.*cm;   //Entrance_Diameter of the tungsten colli; MAX 6.14cm
	double c = params.count("c")? params["c"] : 2.*cm;   //inner diameter (choke) of the tungsten colli; MAX 6.14cm
	double d = params.count("d")? params["d"] : 4.*cm;   //Exit_Diameter of the tungsten colli; MAX 6.4cm
	double e = params.count("e")? params["e"] : 0.;      //rotation of the collimator
	double f = params.count("f")? params["f"] : 33.9*mm; // position of the target; MAX 4.0cm - NEED TO CHECK!    

//Collimator parameters (old, when the Collimator was built with fixed Entrance and Exit radius
//Beam diameter is ~2cm, so 1cm is used for geometrical calculations
// G4double colli_angle     = 0.92*degree;      //half opening angle - max = 0.92° to reach an Exit radius of 3cm
// G4double x = 1*cm / (std::tan(colli_angle)); //helpful variable for smaller calculation steps
// G4double Entrance_Radius = 2.*cm;
// G4double Inner_Radius    = (4.*cm + x) * std::tan(colli_angle);
// G4double Exit_Radius     = (124.*cm + x) * std::tan(colli_angle);

//create a box for the collimator and everything else. rotations are done to this box
//
// Box
//
//Create a Rotation Matrix
G4RotationMatrix* BoxRotation = new G4RotationMatrix();
BoxRotation->rotateX(0*deg);
BoxRotation->rotateY(e);      // parameter e is the rotation of the system        e = rotation
BoxRotation->rotateZ(0*deg);

G4Box* sFullRotationBox =
new G4Box("sFullRotationBox",                       //its name
		2.*m /2, 2.*m /2, 248.1*cm/2);                      //its size: half x, half y, half z - x and y just big and z long enough to fit everything

G4Box* sHalfRotationBox =
new G4Box("sHalfRotationBox",                       //its name
		2.01*m /2, 2.01*m /2, 248.11*cm/4);                //its size: half x, half y, half z


//
// Subtract HalfRotationBox from FullRotationBox
//
G4SubtractionSolid* sRotationBox =                  // subtract sHalfRotationBox from sFullRotationBox
new G4SubtractionSolid("Rotation Box",              //its name
					sFullRotationBox,                           //Solid A
					sHalfRotationBox,                           //Solid B
					0,                                          //Rotation of B relative to A
					G4ThreeVector(0,0,-248.11*cm/4));           //Translation of B relative to A = minus Half length of sHalfRotationBox

G4LogicalVolume* lRotationBox =
new G4LogicalVolume(sRotationBox,                   //its solid
						Materials::Vacuum(),                                 //its material
						"logic Rotation Box");                    //its name

//Make (in-)visible and give it a color
//lRotationBox->SetVisAttributes (G4VisAttributes::GetInvisible());
auto lRotationBoxVisAtt = new G4VisAttributes(G4Color(1, 1, 1, 0.1)); //(r, g, b , transparency)
// lRotationBoxVisAtt->SetVisibility(true);
lRotationBoxVisAtt->SetVisibility(true);
lRotationBox->SetVisAttributes(lRotationBoxVisAtt);

//
//Copper Colimator with double conical Tungsten Inlet and shielding
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------

//Subtract two boxes to create a shielding from 4 sides
//

G4Box* sInnerShieldBox =
  new G4Box("InnerShieldBox",                //its name
      10.*cm, 10.*cm, 62.*cm);               //its size: half x, half y, half z

G4Box* sOuterShieldBox =
  new G4Box("sOuterShieldBox",               //its name
      10.*cm + a , 10.*cm + a , 62.*cm);     //its size: half x, half y, half z

//
// Subtract InnerShieldBox from OuterShieldBox
//
G4SubtractionSolid* sShieldBox =                 // subtract air-cylinder from Copper boxes
  new G4SubtractionSolid("solid Shield Box",     //its name
                sOuterShieldBox,                 //Solid A
                sInnerShieldBox,                 //Solid B
                0,                               //Rotation of B relative to A
                G4ThreeVector(0,0,0));           //Translation of B relative to A - minus Half length of SmallCuBox + Half Length of SmallCuCylinder

G4LogicalVolume* lShieldBox =
  new G4LogicalVolume(sShieldBox,                //its solid
                      Materials::BoratedPE(),                 //its material
                      "logic Shield Box");       //its name

new G4PVPlacement(0,                             //no rotation
            G4ThreeVector(0,0,62.*cm),           //at position - origin of Collimator is the original origin of SmallCuBox
            lShieldBox,                          //its logical volume
            "Shield Box",                        //its name
            lRotationBox,                        //its mother  volume
            true,                                //boolean operation?
            0,                                   //copy number
            true);                               //overlaps checking?

//Colorcode PE
auto logicShieldBoxVisAtt = new G4VisAttributes(G4Color(255./255, 226./255, 181./255, 0.5)); //(r, g, b , transparency)
logicShieldBoxVisAtt->SetVisibility(true);
lShieldBox->SetVisAttributes(logicShieldBoxVisAtt);

//
//Combine two boxes to Copper collimator and cut a air-cylinder because Tungsten Colli does not cover whole copper
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

G4UnionSolid* sCuBoxes =                          // combine Copper boxes - BigCuBox is added to SmallCuBox
  new G4UnionSolid("solid Copper Boxes",          //its name
                sSmallCuBox,                      //Solid A
                sBigCuBox,                        //Solid B
                0,                                //Rotation of B relative to A
                G4ThreeVector(0,0,62.*cm));       //Translation of B relative to A - Half length of SmallCuBox + Half Length of BigCuBox

//
// Subtract air-cylinder
//
G4SubtractionSolid* sCuColli =                          // subtract air-cylinder from Copper boxes
  new G4SubtractionSolid("solid Copper Collimator",     //its name
                sCuBoxes,                               //Solid A
                sSmallCuCylinder,                       //Solid B
                0,                                      //Rotation of B relative to A
                G4ThreeVector(0,0,-31.*cm));            //Translation of B relative to A - minus Half length of SmallCuBox + Half Length of SmallCuCylinder

G4LogicalVolume* lCuColli =
  new G4LogicalVolume(sCuColli,                         //its solid
                      Materials::Copper(),                         //its material
                      "logic Copper Collimator");       //its name

new G4PVPlacement(0,                        //no rotation
            G4ThreeVector(0,0,33.*cm),      //at position - origin of Collimator is the original origin of SmallCuBox
            lCuColli,                       //its logical volume
            "Copper Collimator",            //its name
            lRotationBox,                     //its mother  volume
            true,                           //boolean operation?
            0,                              //copy number
            true);                          //overlaps checking?

//Colorcode Copper
auto logicCopperCollimatorVisAtt = new G4VisAttributes(G4Color(188./255, 80./255, 47./255, 0.8)); //(r, g, b , transparency)
logicCopperCollimatorVisAtt->SetVisibility(true);
lCuColli->SetVisAttributes(logicCopperCollimatorVisAtt);

//
// Target Cylinder
//
G4Tubs* sC_Target =
new G4Tubs("C_Target",                              //name
				0, TargetDia/2,                               //inner radius, outer radius
				TargetLen/2,                                  //z half length
				0., twopi);                                   //min phi, max phi

G4LogicalVolume* lC_Target =
new G4LogicalVolume(sC_Target,                      //shape
						Materials::Graphite(),                               //material
						"C_Target");                              //name

new G4PVPlacement(0,                                //no rotation
				G4ThreeVector(0,0,TargetLen/2 + f),           //position              f = target position
				lC_Target,                                    //logical volume
				"C_Target",                                   //name
				lRotationBox,                                 //mother  volume
				false,                                        //boolean operation?
				0,                                            //copy number
				true);                                        //overlaps checking?

//Make (in-)visible and give it a color
auto logicTargetVisAtt = new G4VisAttributes(G4Color(0, 0, 0, 1)); //(r, g, b , transparency)
logicTargetVisAtt->SetVisibility(true);
lC_Target->SetVisAttributes(logicTargetVisAtt);

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


G4UnionSolid* sWColli =                            // combine Tungsten Cylinder - BigWCylinder is added to SmallWCylinder
  new G4UnionSolid("solid Tungsten Collimator",    //its name
                sSmallWCylinder,                   //Solid A
                sBigWCylinder,                     //Solid B
                0,                                 //Rotation of B relative to A
                G4ThreeVector(0,0,60.*cm));        //Translation of B relative to A - half lenght Small + Half length Big

G4LogicalVolume* lWColli =
  new G4LogicalVolume(sWColli,                            //its solid
                      Materials::Densimet180(),                      //its material
                      "logic Tungsten Collimator");       //its name

new G4PVPlacement(0,                        //no rotation
            G4ThreeVector(0,0,3.5*cm),      //at position - origin of Inlet is the original origin of SmallWCylinder
            lWColli,                        //its logical volume
            "Tungsten Collimator",          //its name
            lCuColli,                       //its mother volume - place TungstenCylinder in CopperBox
            true,                           //boolean operation?
            0,                              //copy number
            true);                          //overlaps checking?

//Colorcode Tungsten
auto logicTungstenInletVisAtt = new G4VisAttributes(G4Color(120./255, 124./255, 133./255, 0.8)); //(r, g, b , transparency)
logicTungstenInletVisAtt->SetVisibility(true);
lWColli->SetVisAttributes(logicTungstenInletVisAtt);

//
//Combine Cones for collimating shape and place in TungstenCylinder to remove the Tungsten and replace by Vacuum/Air
//
G4Cons* sSmallCone =
    new G4Cons("solid small Cone",          //name
    0., b/2,                                //inner radius side A, outer radius side A (negative side) - Entrance_Radius
    0., c/2,                                //inner radius side B, outer radius side B (positive side) - Inner_Radius
    7.5*cm,                                 //z half length
    0., twopi);                             //min phi, max phi

G4Cons* sBigCone =
    new G4Cons("solid big Cone",            //name
    0., c/2,                                //inner radius side A, outer radius side A (negative side) - Inner_Radius
    0., d/2,                                //inner radius side B, outer radius side B (positive side) - Exit_Radius
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
                      Materials::Vacuum(),             //its material
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

  return lRotationBox;
}
#endif
