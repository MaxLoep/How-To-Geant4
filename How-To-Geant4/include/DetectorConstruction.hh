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

   G4double GetAbsorThickness()    {return fAbsorThickness;};
   G4double GetAbsorSizeYZ()       {return fAbsorSizeYZ;};
   G4Material* GetAbsorMaterial()  {return material1;};

   G4double GetWorldSizeX()   {return fWorldSizeX;};
   G4double GetWorldSizeYZ()  {return fWorldSizeYZ;};

   void PrintParameters();

  private:

  // Define Variables for Materials and geometries you want to change per macro-file HERE:
   G4double           boxsizeX;
   G4double           boxsizeYZ;

   G4double           fAbsorThickness;
   G4double           fAbsorSizeYZ;
   G4Material*        fAbsorMaterial;
   G4LogicalVolume*   fLAbsor;

   G4double           fWorldSizeX;
   G4double           fWorldSizeYZ;
   G4Material*        fWorldMaterial;
   G4Material*        world_mat;
   G4Material*        material1;
   G4Material*        Graphite;
   G4VPhysicalVolume* fWorldVolume;                        

   DetectorMessenger* fDetectorMessenger;

  private:

   void               DefineMaterials();
   G4VPhysicalVolume* ConstructVolumes(); 

  protected:
    G4LogicalVolume*  fScoringVolume;    
};


#endif
