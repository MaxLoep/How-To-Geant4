// if-defing different scenarios
#ifdef DEBUG
// print for DEBUGGING
G4cout << "---------------------------------------------------" << G4endl;
G4cout << "This is a DEBUG message" << G4endl;
G4cout << "---------------------------------------------------" << G4endl;
#endif

#ifdef Collimator
// 
//Copper Colimator with double conical Tungsten Inlet
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------
#pragma region
  // 
  // Parameters
  // 
  G4double TargetDia = 40.*mm;
  G4double TargetLen = 4.*mm;
  
  G4double TubeDia = 40.*mm;
  G4double TubeLen = 4.*cm;

  //Collimator parameters
  //Beam diameter is ~2cm, so 1cm is used for geometrical calculations
  G4double colli_angle     = 0.92*degree;      //half opening angle - max = 0.92° to reach an Exit radius of 3cm
  G4double x = 1*cm / (std::tan(colli_angle)); //helpful variable for smaller calculation steps
  G4double Entrance_Radius = 2.*cm;
  // Inner_Radius    = 2.*cm;
  // Exit_Radius     = 3.*cm;
  G4double Inner_Radius    = (4.*cm + x) * std::tan(colli_angle);
  G4double Exit_Radius     = (124.*cm + x) * std::tan(colli_angle);

  // 
  // Materials
  // 
  world_mat = nist->FindOrBuildMaterial("G4_Galactic");

  G4Material* TargetMat = nist->FindOrBuildMaterial("G4_GRAPHITE");
  G4Material* DetectorMat = nist->FindOrBuildMaterial("G4_Galactic");
  G4Material* TubeMat = nist->FindOrBuildMaterial("G4_Galactic");
  
  G4Material* Copper = nist->FindOrBuildMaterial("G4_Cu");
  G4Material* Tungsten = nist->FindOrBuildMaterial("G4_W");
  G4Material* Iron = nist->FindOrBuildMaterial("G4_Fe");
  G4Material* Nickel = nist->FindOrBuildMaterial("G4_Ni");

  //Define Densimet180 (Manufacturer: Plansee)
  G4Material* Densimet180 = new G4Material("Densimet180",      //name
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
              lWorld,                     //its mother  volume
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

#endif