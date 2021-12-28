#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

//Empty class for some reason...
//class G4VPhysicalVolume;
//class G4LogicalVolume;

class G4VPhysicalVolume;
class G4LogicalVolume;
class G4Material;
class DetectorMessenger;

/// Detector construction class to define materials and geometry.
class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    DetectorConstruction();
    virtual ~DetectorConstruction();

    virtual G4VPhysicalVolume* Construct();
    virtual void ConstructSDandField();
    
    G4LogicalVolume* GetScoringVolume() const { return fScoringVolume; }

  public:

    G4Material* 
    MaterialWithSingleIsotope(G4String, G4String, G4double, G4int, G4int);

    void SetAbsorThickness(G4double);
    void SetAbsorSizeYZ   (G4double);
    void SetAbsorMaterial (G4String);

  // public:  

  //  G4double GetAbsorThickness()    {return fAbsorThickness;};
  //  G4double GetAbsorSizeYZ()       {return fAbsorSizeYZ;};
  //  G4Material* GetAbsorMaterial()  {return fAbsorMaterial;};

  //  G4double GetWorldSizeX()   {return fWorldSizeX;};
  //  G4double GetWorldSizeYZ()  {return fWorldSizeYZ;};

  private:

   G4double           boxsizeX;
   G4double           boxsizeYZ;
   G4Material*        fAbsorMaterial;
   G4LogicalVolume*   fLAbsor;

   G4double           fWorldSizeX;
   G4double           fWorldSizeYZ;
   G4Material*        fWorldMaterial;
   G4VPhysicalVolume* fWorldVolume;                        

   DetectorMessenger* fDetectorMessenger;

  protected:
    G4LogicalVolume*  fScoringVolume;
};

#endif

