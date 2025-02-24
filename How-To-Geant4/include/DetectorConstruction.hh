#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "G4GDMLParser.hh"
#include "G4NistManager.hh"             //for getting material definitions from the NIST database
#include <functional>


class G4VPhysicalVolume;
class G4LogicalVolume;
class G4Material;
class DetectorMessenger;


class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:

    DetectorConstruction();
   ~DetectorConstruction();

    virtual G4VPhysicalVolume* Construct();
    virtual void ConstructSDandField();

    G4LogicalVolume* GetScoringVolume() const { return fScoringVolume; }

    void SetOutputFolder (std::string);
    void SetAbsorMaterial (G4String);

    // Writing and Reading GDML
    void SetLoadGDMLFile( const G4String& File );  // for the macro command to load a GDML file
    void SetWriteGDMLFile( const G4String& File ); // for the macro command to save to a GDML file
    void SetOnlyLoadGDML( bool value );                     // for the macro command to decide if only a GDML should be loaded

    void LoadGDML( const G4String& File );         // function for loading a GDML file
    void SaveGDML( const G4String& File );         // function for saving to a GDML file

    void change_a   (G4double);
    void change_b   (G4double);
    void change_c   (G4double);
    void change_d   (G4double);
    void change_e   (G4double);
    void change_f   (G4double);

    G4Material* GetAbsorMaterial()  {return boxMaterial;};

    void PrintParameters(); // function that prints parameters of ONE hardcoded object - can be deleted i guess

  private:

    // GDMLparser
    G4GDMLParser fParser;

    // Reading and Writing Settings
    G4String fLoadFile;    // name of GDML file that should be loaded
    G4String fWriteFile;   // name of the GDML file that should be written
    G4int fWritingChoice;  // variable to control if GDML should be written
    G4int fLoadingChoice;  // variable to control if GDML should be loaded
    G4bool fOnlyLoadChoice; // variable to control if only a GDML file should be loaded without building additional geometries via Geant4/DetectorConstruction

    // Define Variables for Materials and geometries you want to change per macro-file HERE:
    //world size
    G4double world_sizeXYZ;

    // dummy variables that can be changed with UI-commands
    G4double           a;
    G4double           b;
    G4double           c;
    G4double           d;
    G4double           e;
    G4double           f;

    G4Material*        fAbsorMaterial;
    G4LogicalVolume*   fLAbsor;

    G4Material*        boxMaterial;
    G4Material*        dummyMat;


    // NIST Material Manager
    G4NistManager* nist;


  	typedef std::function<G4Material*()> MaterialMaker;
    // List of all Materials that get defined in Materials.cc
    // NIST Materials
    MaterialMaker Vacuum;
    MaterialMaker Hydrogen;
    MaterialMaker Lithium;
    MaterialMaker Boron;
    MaterialMaker Carbon;
    MaterialMaker Aluminum;
    MaterialMaker Silicon;
    MaterialMaker Scandium;
    MaterialMaker Titanium;
    MaterialMaker Vanadium;
    MaterialMaker Iron;
    MaterialMaker Nickel;
    MaterialMaker Copper;
    MaterialMaker Cadmium;
    MaterialMaker Tungsten;
    MaterialMaker Gold;

    // NIST pre-defined Compounds
    MaterialMaker Air;
    MaterialMaker Concrete;
    MaterialMaker Graphite;
    MaterialMaker Polyethylene;
    MaterialMaker Steel;
    MaterialMaker Water;

    // Self-definded Materials
    MaterialMaker BoratedPE;
    MaterialMaker Densimet180;

    DetectorMessenger* fDetectorMessenger;

    void               DefineMaterials();
    G4VPhysicalVolume* ConstructVolumes();
    G4VPhysicalVolume* ConstructVolumesGDML();
    G4VPhysicalVolume* fWorldPhysVol;
    G4LogicalVolume* lWorld;

  protected:
    G4LogicalVolume*  fScoringVolume;
};


#endif
