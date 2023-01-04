#ifndef DetectorMessenger_h
#define DetectorMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"

class DetectorConstruction;
class G4UIdirectory;
class G4UIcommand;
class G4UIcmdWithAString;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithoutParameter;


class DetectorMessenger: public G4UImessenger
{
  public:
  
    DetectorMessenger(DetectorConstruction* );
   ~DetectorMessenger();
    
    virtual void SetNewValue(G4UIcommand*, G4String);
    
  private:
  
    DetectorConstruction*      fDetector;
    
    G4UIdirectory*             fTestemDir;
    G4UIdirectory*             fDetDir;

    G4UIcmdWithAString*        fOutFoldCmd;
    G4UIcmdWithAString*        fMaterCmd;

    G4UIcmdWithADoubleAndUnit* fchange_aCmd;
    G4UIcmdWithADoubleAndUnit* fchange_bCmd;
    G4UIcmdWithADoubleAndUnit* fchange_cCmd;
    G4UIcmdWithADoubleAndUnit* fchange_dCmd;
    G4UIcmdWithADoubleAndUnit* fchange_eCmd; 
};


#endif
