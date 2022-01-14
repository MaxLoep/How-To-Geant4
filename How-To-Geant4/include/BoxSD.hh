#ifndef BoxSD_h
#define BoxSD_h 1

#include "G4VSensitiveDetector.hh"

class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;

class BoxSD : public G4VSensitiveDetector
{
  public:
    BoxSD(const G4String& name);
    virtual ~BoxSD();

    virtual void   Initialize(G4HCofThisEvent* hce);
    virtual G4bool ProcessHits(G4Step* step, G4TouchableHistory* history);
    virtual void   EndOfEvent(G4HCofThisEvent* hce);
};

#endif
