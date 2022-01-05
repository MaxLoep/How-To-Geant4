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

#include "DetectorConstruction.hh"    //Header file where functions classes and variables may be defined (...)
#include "DetectorMessenger.hh"       //Header file for own macro commands
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


DetectorConstruction::DetectorConstruction()
: G4VUserDetectorConstruction(),
  fDetectorMessenger(nullptr), fScoringVolume(0) 
{ 
  // create commands for interactive definition of the geometry
  fDetectorMessenger = new DetectorMessenger(this);

  //Variables which can be changed by macro commands should be set here
  boxsizeX      = 10. *cm;
  boxsizeYZ     = 10. *cm;
}

DetectorConstruction::~DetectorConstruction()
{ delete fDetectorMessenger; }


G4VPhysicalVolume* DetectorConstruction::Construct()
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

  G4Material* world_mat = nist->FindOrBuildMaterial("G4_Galactic");
  G4Material* material1 = nist->FindOrBuildMaterial("G4_Galactic");

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
  // G4Material* Galactic = nist->FindOrBuildMaterial("G4_Galactic");
 
  //How to define Isotopes by hand
  //G4Isotope* name = new G4Isotope("name", #protons, #nucleons);
  G4Isotope* U235 = new G4Isotope("U235", 92, 235);
  G4Isotope* U238 = new G4Isotope("U238", 92, 238);

  //How to define Elements by hand
  //G4Element* name = new G4Element("name", "symbol", atomic number, molar mass);
  G4Element* Hydrogen = new G4Element("Hydrogen", "H", 1., 1.0079*g/mole);
  G4Element* Carbon   = new G4Element("Carbon", "C", 6., 12.011*g/mole);
  G4Element* Oxygen   = new G4Element("Oxygen", "o", 8., 16.*g/mole);

  //How to define an Element by using isotopes with their abundance
  //G4Element* name= new G4Element("name", "symbol", #isotopes);
  G4Element* enrichedUranium= new G4Element("enriched uranium", "U", 2);

  //Add Isotopes to Element
  enrichedUranium->AddIsotope(U235, 90.*perCent);
  enrichedUranium->AddIsotope(U238, 10.*perCent);

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
  G4Material* H20 = new G4Material("Water", 1.0*g/mole, 2);
  H20->AddElement(Hydrogen, 2);
  H20->AddElement(Oxygen, 1);

  //NE213
  G4Element* H  = new G4Element("Hydrogen" ,"H" , 1.,  1.01*g/mole);
  G4Element* C  = new G4Element("Carbon"   ,"C" , 6., 12.00*g/mole);
  G4Material* ne213 = 
  new G4Material("NE213", 0.874*g/cm3, 2);
  ne213->AddElement(H,    9.2*perCent);
  ne213->AddElement(C,   90.8*perCent);

  //Cleanup old geometry
  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();

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

  //Make world-volume invisible
  auto logicWorldVisAtt = new G4VisAttributes(G4Color(1, 1, 1, 1)); //(r, g, b , transparency)
  logicWorldVisAtt->SetVisibility(false);
  logicWorld->SetVisAttributes(logicWorldVisAtt);

  /*
  How to create solids
  It's basically a process with 3 steps:
  1.: Create a Geometry e.g. a Box, Cylinder, Sphere or even a Box minus a Cylinder (-> see boolean operation)
  2.: Make it a Logical Volume by assigning a material to it
  3.: Place it in your simulation
  */ 

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
              false,                         //no boolean operation
              0,                             //copy number
              true);                         //overlaps checking

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
           
  new G4PVPlacement(BoxRotation,             // <----------- ROTATION HAPPENS HERE!
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


  //B1 SCORING METHOD
  //You need also Code for this one to work in:
  //SteppingAction.cc,  RunAction.cc, EventAction.cc          
  // Set logical Box volume as scoring volume (must be a logical volume)
  //This is a public variable defined in the header file to make it accessible from other files
  fScoringVolume = logicBox;

  //
  //always return the physical World
  //
  return physWorld;
}

//
//Functions for custom GUI and macro commands - see DetectorMessenger.cc
//

//Function to change the variable 'boxsizeX' 
void DetectorConstruction::SetAbsorThickness(G4double value)
{
  //assign a new 'value' to the variable 'boxsizeX' 
  boxsizeX = value;
  //Resetting a variable dies not change your geometry; you need to reinitialize the whole geometry
  G4RunManager::GetRunManager()->ReinitializeGeometry();
  //Output message to console for confirmaton that something happened
  G4cout  << "\n boxsizeX is now " << boxsizeX / cm << " cm" << G4endl;
}

//Same as above
void DetectorConstruction::SetAbsorSizeYZ(G4double value)
{
  boxsizeYZ = value;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
  G4cout  << "\n boxsizeYZ is now " << boxsizeYZ / cm << " cm" << G4endl;
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
  //BoxSD.cc, RunAction.cc
  //Make a Volume a Sensitive Detector (SD); SD are able to access Track/Step information of Particles going through e.g. :
  //Kinetic energy, Momentum

  //Declare a Sensitive Detector
  auto boxSD = new BoxSD("BoxSD");
  G4SDManager::GetSDMpointer()->AddNewDetector(boxSD);
  //Apply Sensitive Detector to Volume
  SetSensitiveDetector("Box", boxSD);


  // // 
  // //PRIMITIVE SCORERS
  // //You need also Code for this one to work in:
  // //RunAction.cc, EventAction.cc
  // //Make a Volume a Primitive Scorer (PS); PS are able to save information on events related to inside the volume e.g. :
  // //energy deposit, track length, current, flux


  // //Filters
  // //Declare filters on Particles, Charge, Energy
  // G4String fltName,particleName;

  // //charged particle filter
  // auto charged = new G4SDChargedFilter("chargedFilter");
  // G4SDChargedFilter* chargedFilter = new G4SDChargedFilter(fltName="chargedFilter");

  // //Proton filter
  // G4SDParticleFilter* protonFilter =
  // new G4SDParticleFilter(fltName="protonFilter", particleName="proton");
  
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
  // auto absDetector = new G4MultiFunctionalDetector("Absorber");
  // G4SDManager::GetSDMpointer()->AddNewDetector(absDetector);

  // //Declare what quantity should be scored and apply filters
  // //
  // //Score Deposited Energy (of protons)
  // G4VPrimitiveScorer* primitive;
  // primitive = new G4PSEnergyDeposit("Edep");
  // //primitive ->SetFilter(protonFilter);

  // //Register Filters to Scorer
  // absDetector->RegisterPrimitive(primitive);

  // //Score TrackLength (of protons/charged particle)
  // primitive = new G4PSTrackLength("TrackLength");
  // //primitive ->SetFilter(protonFilter);
  // //primitive ->SetFilter(charged);

  // //Register Filters to Scorer
  // absDetector->RegisterPrimitive(primitive);  

  // //Apply Scorer to Volume
  // SetSensitiveDetector("Box",absDetector);
  

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
