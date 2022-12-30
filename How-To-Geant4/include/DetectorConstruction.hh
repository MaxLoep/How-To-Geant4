#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

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

    G4Material* 
    MaterialWithSingleIsotope(G4String, G4String, G4double, G4int, G4int);

    void SetAbsorThickness(G4double);
    void SetAbsorSizeYZ   (G4double);
    void SetAbsorMaterial (G4String);

  public:  

   G4double GetAbsorThickness()    {return boxX;};
   G4double GetAbsorSizeYZ()       {return boxX;};
   G4Material* GetAbsorMaterial()  {return boxMaterial;};

   G4double GetWorldSizeX()   {return boxX;};
   G4double GetWorldSizeYZ()  {return boxX;};

   void PrintParameters();

  private:

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

   G4Material*        Graphite;                       

   DetectorMessenger* fDetectorMessenger;

  private:

   void               DefineMaterials();
   G4VPhysicalVolume* ConstructVolumes(); 

  protected:
    G4LogicalVolume*  fScoringVolume;    
};


#endif
