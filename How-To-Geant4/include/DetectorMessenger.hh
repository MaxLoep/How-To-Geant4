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
class G4UIcmdWithABool;


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
    G4UIdirectory*             fGDMLDir;

    G4UIcmdWithAString*        fOutFoldCmd;
    G4UIcmdWithAString*        fMaterCmd;

    G4UIcmdWithAString*        fTheLoadCommand;
    G4UIcmdWithAString*        fTheWriteCommand;
    G4UIcmdWithABool*          fTheOnlyLoadCommand;

    G4UIcmdWithADoubleAndUnit* fchange_aCmd;
    G4UIcmdWithADoubleAndUnit* fchange_bCmd;
    G4UIcmdWithADoubleAndUnit* fchange_cCmd;
    G4UIcmdWithADoubleAndUnit* fchange_dCmd;
    G4UIcmdWithADoubleAndUnit* fchange_eCmd;
    G4UIcmdWithADoubleAndUnit* fchange_fCmd; 
};


#endif
