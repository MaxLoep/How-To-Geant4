// SOLIDS, GEOMETRIES, PLACEMENT, ETC.
// How to create solids
// It's basically a process with 3 steps:
// 1.: Create a Geometry e.g. a Box, Cylinder, Sphere or even a Box minus a Cylinder (-> see boolean operation)
// 2.: Make it a Logical Volume by assigning a material to it
// 3.: Place it in your simulation

// if-defing different scenarios
#ifdef DEBUG
G4cout << "-------------------------------------------------------------------------" << G4endl;
G4cout << "Compiled with DEBUG at " __DATE__ ", " __TIME__ "." <<  G4endl;
G4cout << "-------------------------------------------------------------------------" << G4endl;

//create 5 flat boxes to use as Sensitive Detector (SD)
// SD1
G4Box* sSD1 =
  new G4Box("sSD1",                        				//its name
      30.*cm/2, 30.*cm/2, 0.02*mm /2);            //its size: half x, half y, half z

G4LogicalVolume* lSD1 =
  new G4LogicalVolume(sSD1,                				//its solid
                      Vacuum(),		                //its material
                      // Aluminum(),
                      "lSD1");                    //its name

  new G4PVPlacement(0,                     				//no rotation
            G4ThreeVector(0,0,10.*cm),     		    //position
            lSD1,                          		    //its logical volume
            "pSD1",                         	    //its name
            lWorld,								                //its mother  volume
            false,                         		    //any boolean operation?
            0,                             		    //copy number
            true);                         		    //overlaps checking?

//Make (in-)visible and give it a color
auto lSD1VisAtt = new G4VisAttributes(G4Color(0, 0, 1, 0.8)); //(r, g, b , transparency)
lSD1VisAtt->SetVisibility(true);
lSD1->SetVisAttributes(lSD1VisAtt);

// SD2
G4Box* sSD2 =
  new G4Box("sSD2",                        				//its name
      30.*cm/2, 30.*cm/2, (5*4.5+30)*um  /2);            //its size: half x, half y, half z

G4LogicalVolume* lSD2 =
  new G4LogicalVolume(sSD2,                				//its solid
                      // Vacuum(),		                //its material
                      Aluminum(),
                      "lSD2");	                	//its name

  new G4PVPlacement(0,                     				//no rotation
            G4ThreeVector(0,0,20.*cm),     		    //position
            lSD2,                          		    //its logical volume
            "pSD2",                         	    //its name
            lWorld,								                //its mother  volume
            false,                         	    	//any boolean operation?
            0,                             		    //copy number
            true);                         		    //overlaps checking?

//Make (in-)visible and give it a color
auto lSD2VisAtt = new G4VisAttributes(G4Color(0, 0, 1, 0.8)); //(r, g, b , transparency)
lSD2VisAtt->SetVisibility(true);
lSD2->SetVisAttributes(lSD2VisAtt);

// SD3
G4Box* sSD3 =
  new G4Box("sSD3",                        				//its name
      30.*cm/2, 30.*cm/2, 0.02*mm/2);            //its size: half x, half y, half z

G4LogicalVolume* lSD3 =
  new G4LogicalVolume(sSD3,                				//its solid
                      Vacuum(),		                //its material
                      // Aluminum(),
                      "lSD3");		                //its name

  new G4PVPlacement(0,                     				//no rotation
            G4ThreeVector(0,0,30.*cm),     		    //position
            lSD3,                          		    //its logical volume
            "pSD3",                             	//its name
            lWorld,								                //its mother  volume
            false,                         		    //any boolean operation?
            0,                             		    //copy number
            true);                         	    	//overlaps checking?

//Make (in-)visible and give it a color
auto lSD3VisAtt = new G4VisAttributes(G4Color(0, 0, 1, 0.8)); //(r, g, b , transparency)
lSD3VisAtt->SetVisibility(true);
lSD3->SetVisAttributes(lSD3VisAtt);

// SD4
G4Box* sSD4 =
  new G4Box("sSD4",                        				//its name
      30.*cm/2, 30.*cm/2, 6.*cm /2);            //its size: half x, half y, half z

G4LogicalVolume* lSD4 =
  new G4LogicalVolume(sSD4,                				//its solid
                      // Vacuum(),		                //its material
                      Air(),
                      "lSD4");		                //its name

  new G4PVPlacement(0,                     				//no rotation
            G4ThreeVector(0,0,40.*cm),     		    //position
            lSD4,                          		    //its logical volume
            "pSD4",                         	    //its name
            lWorld,								                //its mother  volume
            false,                         		    //any boolean operation?
            0,                             		    //copy number
            true);                         		    //overlaps checking?

//Make (in-)visible and give it a color
auto lSD4VisAtt = new G4VisAttributes(G4Color(0, 0, 1, 0.8)); //(r, g, b , transparency)
lSD4VisAtt->SetVisibility(true);
lSD4->SetVisAttributes(lSD4VisAtt);

// SD5
G4Box* sSD5 =
  new G4Box("sSD5",                        				//its name
      30.*cm/2, 30.*cm/2, 0.02*mm /2);            //its size: half x, half y, half z

G4LogicalVolume* lSD5 =
  new G4LogicalVolume(sSD5,                				//its solid
                      Vacuum(),		                //its material
                      "lSD5");		                //its name

  new G4PVPlacement(0,                     				//no rotation
            G4ThreeVector(0,0,50.*cm),     		    //position
            lSD5,                          		    //its logical volume
            "pSD5",                         	    //its name
            lWorld,								                //its mother  volume
            false,                         	    	//any boolean operation?
            0,                             		    //copy number
            true);                         		    //overlaps checking?

//Make (in-)visible and give it a color
auto lSD5VisAtt = new G4VisAttributes(G4Color(0, 0, 1, 0.8)); //(r, g, b , transparency)
lSD5VisAtt->SetVisibility(true);
lSD5->SetVisAttributes(lSD5VisAtt);

#endif


#ifdef Range  //Geometry for Range-Analysis
G4cout << "-------------------------------------------------------------------------" << G4endl;
G4cout << "Compiled with Range-Geometry at " __DATE__ ", " __TIME__ "." <<  G4endl;
G4cout << "-------------------------------------------------------------------------" << G4endl;
// create a box to be used as Tracklength Primitive Scorer (PS) and place it in the world volume
// Box
G4Box* sBox =
  new G4Box("sBox",                  			//its name
      50.*cm/2, 50.*cm/2, 50.*cm/2);      //its size: half x, half y, half z

G4LogicalVolume* lBox =
  new G4LogicalVolume(sBox,               //its solid
                      dummyMat,		        //its material
                      "lBox");            //its name

//G4VPhysicalVolume* physBox=             //you can declare a varibale for placement but it will create a warning if unused
  new G4PVPlacement(0,               			//no rotation
            G4ThreeVector(0,0,80.*cm),  	//position
            lBox,                      		//its logical volume
            "pBox",                     	//its name
            lWorld,							        	//its mother  volume
            false,                     		//any boolean operation?
            0,                         		//copy number
            true);                     		//overlaps checking?

//Make (in-)visible and give it a color
//lBox->SetVisAttributes (G4VisAttributes::GetInvisible());
auto lBoxVisAtt = new G4VisAttributes(G4Color(1, 0, 0, 0.8)); //(r, g, b , transparency)
lBoxVisAtt->SetVisibility(true);
lBox->SetVisAttributes(lBoxVisAtt);
#endif


#ifdef TNY  //Geometry for Target-Neutron-Yield Analysis
G4cout << "-------------------------------------------------------------------------" << G4endl;
G4cout << "Compiled with TNY-Geometry at " __DATE__ ", " __TIME__ "." <<  G4endl;
G4cout << "-------------------------------------------------------------------------" << G4endl;

// Sphere - SD to detect gammas
G4Sphere* sSphere =
  new G4Sphere("sSphere",                    				//name
            20.*cm, 20.01*cm,                  	    //inner radius, outer radius
            0., twopi,                      	      //min phi, max phi
            0., pi);                        	      //min rho, max rho

G4LogicalVolume* lSphere =
  new G4LogicalVolume(sSphere,              				//shape
                      Vacuum(),		                  //material
                      "lSphere");		                //name

new G4PVPlacement(0,                        				//no rotation
            G4ThreeVector(0,0,0),           	      //position
            lSphere,                        	      //logical volume
            "pSphere",                       	      //name
            lWorld,								                  //mother volume
            false,                          	      //any boolean operation?
            0,                              	      //copy number
            true);                          	      //overlaps checking?

//Make (in-)visible and give it a color
auto lSphereVisAtt = new G4VisAttributes(G4Color(0, 1, 0, 0.8)); //(r, g, b , transparency)
lSphereVisAtt->SetVisibility(true);
lSphere->SetVisAttributes(lSphereVisAtt);

// Target cylinder - change thickness with parameter e
G4Tubs* solidCylinder =
  new G4Tubs("Cylinder",                     				//name
      0, 1.5*mm,                      				      //inner radius, outer radius
      1.*cm/2,                              			  //z half length
      0., twopi);                       				    //min phi, max phi

G4LogicalVolume* logicCylinder =
  new G4LogicalVolume(solidCylinder,        				//shape
          // Vacuum(),
          dummyMat,
          "Cylinder");           					          //name

new G4PVPlacement(0,                        				//no rotation
      G4ThreeVector(0,0,0),      						        //position
      logicCylinder,                  				      //logical volume
      "Cylinder",                     				      //name
      lWorld,                     					        //mother  volume
      false,                          				      //any boolean operation?
      0,                              				      //copy number
      true);                          				      //overlaps checking?

// Make (in-)visible and give it a color
auto logicCylinderVisAtt = new G4VisAttributes(G4Color(1, 0, 0, 0.8)); //(r, g, b , transparency)
logicCylinderVisAtt->SetVisibility(true);
logicCylinder->SetVisAttributes(logicCylinderVisAtt);
#endif


#ifdef Collimator   //Geometry for the Neutron Collimator
G4cout << "-------------------------------------------------------------------------" << G4endl;
G4cout << "Compiled with Collimator-Geometry at " __DATE__ ", " __TIME__ "." <<  G4endl;
G4cout << "-------------------------------------------------------------------------" << G4endl;

// 
//Copper Colimator with double conical Tungsten Inlet
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 
// Parameters
// 

G4double TargetDia = 40.*mm;
G4double TargetLen = 3.*mm;

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
	2.01*m /2, 2.01*m /2, 248.1*cm/4);                //its size: half x, half y, half z  


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
					Vacuum(),                                 //its material
					"logic Rotation Box");                    //its name

new G4PVPlacement(BoxRotation,                      //no rotation
			G4ThreeVector(0,0,0),                         //at position - origin of Collimator is the original origin of sFullRotationBox
			lRotationBox,                                 //its logical volume
			"Rotation Box",                               //its name
			lWorld,                                       //its mother  volume
			true,                                         //boolean operation?
			0,                                            //copy number
			true);                                        //overlaps checking?

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
      20.*cm, 20.*cm, 62.*cm);               //its size: half x, half y, half z

G4Box* sOuterShieldBox =    
  new G4Box("sOuterShieldBox",               //its name
      20.*cm + a , 20.*cm + a , 62.*cm);     //its size: half x, half y, half z

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
                      BoratedPE(),                 //its material
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
                      Copper(),                         //its material
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
					Graphite(),                               //material
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
auto logicTargetVisAtt = new G4VisAttributes(G4Color(0, 1, 0, 1)); //(r, g, b , transparency)
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
                      Densimet180(),                      //its material
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
                      Vacuum(),             //its material
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

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------
// SD1
G4Box* sSD1 =
  new G4Box("sSD1",                        				//its name
      30.*cm/2, 30.*cm/2, 0.02*mm /2);            //its size: half x, half y, half z

G4LogicalVolume* lSD1 =
  new G4LogicalVolume(sSD1,                				//its solid
                      Vacuum(),		                //its material
                      "lSD1");                    //its name

  new G4PVPlacement(0,                     				//no rotation
            G4ThreeVector(0,0,150.*cm),     		    //position
            lSD1,                          		    //its logical volume
            "pSD1",                         	    //its name
            lWorld,								                //its mother  volume
            false,                         		    //any boolean operation?
            0,                             		    //copy number
            true);                         		    //overlaps checking?

//Make (in-)visible and give it a color
auto lSD1VisAtt = new G4VisAttributes(G4Color(0, 0, 1, 0.8)); //(r, g, b , transparency)
lSD1VisAtt->SetVisibility(true);
lSD1->SetVisAttributes(lSD1VisAtt);

// SD2
G4Box* sSD2 =
  new G4Box("sSD2",                        				//its name
      30.*cm/2, 30.*cm/2, 0.02*mm /2);            //its size: half x, half y, half z

G4LogicalVolume* lSD2 =
  new G4LogicalVolume(sSD2,                				//its solid
                      Vacuum(),		                //its material
                      "lSD2");	                	//its name

  new G4PVPlacement(0,                     				//no rotation
            G4ThreeVector(0,0,400.*cm),     		    //position
            lSD2,                          		    //its logical volume
            "pSD2",                         	    //its name
            lWorld,								                //its mother  volume
            false,                         	    	//any boolean operation?
            0,                             		    //copy number
            true);                         		    //overlaps checking?

//Make (in-)visible and give it a color
auto lSD2VisAtt = new G4VisAttributes(G4Color(0, 0, 1, 0.8)); //(r, g, b , transparency)
lSD2VisAtt->SetVisibility(true);
lSD2->SetVisAttributes(lSD2VisAtt);

// Sphere - SD to detect gammas
G4Sphere* sSphere =
  new G4Sphere("sSphere",                    				//name
            200.*cm, 200.01*cm,                  	    //inner radius, outer radius
            0., twopi,                      	      //min phi, max phi
            0., pi);                        	      //min rho, max rho

G4LogicalVolume* lSphere =
  new G4LogicalVolume(sSphere,              				//shape
                      Vacuum(),		                  //material
                      "lSphere");		                //name

new G4PVPlacement(0,                        				//no rotation
            G4ThreeVector(0,0,0),           	      //position
            lSphere,                        	      //logical volume
            "pSphere",                       	      //name
            lWorld,								                  //mother volume
            false,                          	      //any boolean operation?
            0,                              	      //copy number
            true);                          	      //overlaps checking?

//Make (in-)visible and give it a color
auto lSphereVisAtt = new G4VisAttributes(G4Color(0, 1, 0, 0.1)); //(r, g, b , transparency)
lSphereVisAtt->SetVisibility(true);
lSphere->SetVisAttributes(lSphereVisAtt);

#endif

#ifdef BoxPS
// create a box to be used as Primitive Scorer (PS) and place it in the world volume
//
// Box
G4Box* sBox =
	new G4Box("sBox",                        		//its name
			boxX/2, boxY/2, boxZ/2);                //its size: half x, half y, half z

G4LogicalVolume* lBox =
	new G4LogicalVolume(sBox,                		//its solid
											Vacuum(),		            //its material
											"lBox");                //its name

//G4VPhysicalVolume* physBox=              		//you can declare a varibale for placement but it will create a warning if unused
	new G4PVPlacement(0,                     		//no rotation
						G4ThreeVector(0,0,80.*cm),     		//position
						lBox,                          		//its logical volume
						"pBox",                         	//its name
						lWorld,								            //its mother  volume
						false,                         		//any boolean operation?
						0,                             		//copy number
						true);                         		//overlaps checking?

//Make (in-)visible and give it a color
//lBox->SetVisAttributes (G4VisAttributes::GetInvisible());
auto lBoxVisAtt = new G4VisAttributes(G4Color(1, 0, 0, 0.8)); //(r, g, b , transparency)
lBoxVisAtt->SetVisibility(true);
lBox->SetVisAttributes(lBoxVisAtt);
#endif


#ifdef Shielding
G4cout << "-------------------------------------------------------------------------" << G4endl;
G4cout << "Compiled with Shielding at " __DATE__ ", " __TIME__ "." <<  G4endl;
G4cout << "-------------------------------------------------------------------------" << G4endl;

// Shielding Box
G4Box* sShielding =
	new G4Box("sShielding",                        			//its name
			40.*cm/2, 40.*cm/2, b/2);                   		//its size: half x, half y, half z

G4LogicalVolume* lShielding =
	new G4LogicalVolume(sShielding,                			//its solid
											// Vacuum(),		                //its material
                      // dummyMat,
                      BoratedPE(),
											"lShielding");                  //its name

//G4VPhysicalVolume* physShielding=               		//you can declare a varibale for placement but it will create a warning if unused
	new G4PVPlacement(0,                     				    //no rotation
						G4ThreeVector(0,0,b/2),     		        //position
						lShielding,                          		  //its logical volume
						"pShielding",                         	  //its name
						lWorld,								                    //its mother  volume
						false,                         		        //any boolean operation?
						0,                             		        //copy number
						true);                         		        //overlaps checking?

//Make (in-)visible and give it a color
//lShielding->SetVisAttributes (G4VisAttributes::GetInvisible());
auto lShieldingVisAtt = new G4VisAttributes(G4Color(1, 0, 0, 0.8)); //(r, g, b , transparency)
lShieldingVisAtt->SetVisibility(true);
lShielding->SetVisAttributes(lShieldingVisAtt);

// Concrete Box
G4Box* sConcrete =
	new G4Box("sConcrete",                        			//its name
			50.*cm/2,50.*cm/2,100.*cm/2);                   		//its size: half x, half y, half z

G4LogicalVolume* lConcrete =
	new G4LogicalVolume(sConcrete,                			//its solid
											// Vacuum(),		                //its material
                      // dummyMat,
                      Concrete(),
											"lConcrete");                  //its name

//G4VPhysicalVolume* physConcrete=               		//you can declare a varibale for placement but it will create a warning if unused
	new G4PVPlacement(0,                     				    //no rotation
						G4ThreeVector(0,45.*cm,30.*cm),     		        //position
						lConcrete,                          		  //its logical volume
						"pConcrete",                         	  //its name
						lWorld,								                    //its mother  volume
						false,                         		        //any boolean operation?
						0,                             		        //copy number
						true);                         		        //overlaps checking?

//Make (in-)visible and give it a color
//lConcrete->SetVisAttributes (G4VisAttributes::GetInvisible());
auto lConcreteVisAtt = new G4VisAttributes(G4Color(0, 1, 0, 0.8)); //(r, g, b , transparency)
lConcreteVisAtt->SetVisibility(true);
lConcrete->SetVisAttributes(lConcreteVisAtt);

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------
// SD1
G4Box* sSD1 =
  new G4Box("sSD1",                        				//its name
      2.54*cm/2, 2.54*cm/2, 50.*mm /2);            //its size: half x, half y, half z

G4LogicalVolume* lSD1 =
  new G4LogicalVolume(sSD1,                				//its solid
                      Vacuum(),		                //its material
                      "lSD1");                    //its name

  new G4PVPlacement(0,                     				//no rotation
            G4ThreeVector(10.*cm,0,60.*cm),     		    //position
            // G4ThreeVector(0,0,b + 0.02*mm /2),     		    //position
            lSD1,                          		    //its logical volume
            "pSD1",                         	    //its name
            lWorld,								                //its mother  volume
            false,                         		    //any boolean operation?
            0,                             		    //copy number
            true);                         		    //overlaps checking?

//Make (in-)visible and give it a color
auto lSD1VisAtt = new G4VisAttributes(G4Color(0, 0, 1, 0.8)); //(r, g, b , transparency)
lSD1VisAtt->SetVisibility(true);
lSD1->SetVisAttributes(lSD1VisAtt);

// SD2
// G4Box* sSD2 =
//   new G4Box("sSD2",                        				//its name
//       30.*cm/2, 30.*cm/2, 0.02*mm /2);            //its size: half x, half y, half z

// G4Tubs* sSD2 =
//   new G4Tubs("sSD2",                     				//name
//       0, 20.*cm,                      				      //inner radius, outer radius
//       0.02*mm /2,                              			  //z half length
//       0., twopi);                       				    //min phi, max phi

G4Sphere* sSD2 =
  new G4Sphere("sSD2",                    				//name
            0.*cm, 20.*cm,                  	    //inner radius, outer radius
            0., twopi,                      	      //min phi, max phi
            0., pi);                        	      //min rho, max rho

G4LogicalVolume* lSD2 =
  new G4LogicalVolume(sSD2,                				//its solid
                      Vacuum(),		                //its material
                      "lSD2");	                	//its name

  new G4PVPlacement(0,                     				//no rotation
            G4ThreeVector(-15*cm,0,60.*cm),     		    //position
            lSD2,                          		    //its logical volume
            "pSD2",                         	    //its name
            lWorld,								                //its mother  volume
            false,                         	    	//any boolean operation?
            0,                             		    //copy number
            true);                         		    //overlaps checking?

//Make (in-)visible and give it a color
auto lSD2VisAtt = new G4VisAttributes(G4Color(0, 0, 1, 0.8)); //(r, g, b , transparency)
lSD2VisAtt->SetVisibility(true);
lSD2->SetVisAttributes(lSD2VisAtt);

#endif