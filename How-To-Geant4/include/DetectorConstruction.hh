#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "G4GDMLParser.hh"


class G4VPhysicalVolume;
class G4LogicalVolume;
class G4Material;
class DetectorMessenger;


class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:

    DetectorConstruction();
   ~DetectorConstruction();

  public:

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

  public:  

   G4double GetAbsorThickness()    {return boxX;};
   G4double GetAbsorSizeYZ()       {return boxX;};
   G4Material* GetAbsorMaterial()  {return boxMaterial;};

   G4double GetWorldSizeX()   {return boxX;};
   G4double GetWorldSizeYZ()  {return boxX;};

   void PrintParameters();

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

  // for the box
   G4double           boxX;
   G4double           boxY;
   G4double           boxZ;

// dummy variables
   G4double           a;
   G4double           b;
   G4double           c;
   G4double           d;
   G4double           e;

   G4Material*        fAbsorMaterial;
   G4LogicalVolume*   fLAbsor;

   G4Material*        world_mat;
   G4Material*        boxMaterial;
   G4Material*        dummyMat; 

   G4Material*        Vacuum;
   G4Material*        Copper;
   G4Material*        Iron;
   G4Material*        Titanium;
   G4Material*        Aluminum;

   DetectorMessenger* fDetectorMessenger;

  private:

   void               DefineMaterials();
   G4VPhysicalVolume* ConstructVolumes(); 
   G4VPhysicalVolume* ConstructVolumesGDML(); 
   G4VPhysicalVolume* fWorldPhysVol; ; 
   G4LogicalVolume* lWorld;

  protected:
    G4LogicalVolume*  fScoringVolume;    
};


#endif
