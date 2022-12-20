#ifndef SteppingAction_h
#define SteppingAction_h 1

#include "G4UserSteppingAction.hh"
#include "globals.hh"

class DetectorConstruction;
class EventAction;
class G4LogicalVolume;

class SteppingAction : public G4UserSteppingAction
{
  public:
    SteppingAction(DetectorConstruction*,EventAction*);
   ~SteppingAction();

    virtual void UserSteppingAction(const G4Step*);
    
  private:
    DetectorConstruction* fDetector;
    EventAction*         fEventAction; 

  private:
    G4LogicalVolume* fScoringVolume;   
};


#endif
