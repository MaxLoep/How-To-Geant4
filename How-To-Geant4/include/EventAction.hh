#ifndef EventAction_h
#define EventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"

#include "G4THitsMap.hh"

//Empty class for some reason...
class RunAction;

class EventAction : public G4UserEventAction
{
  public:
    //EventAction(RunAction* runAction);
    EventAction();
   ~EventAction();

    virtual void BeginOfEventAction(const G4Event*);
    virtual void   EndOfEventAction(const G4Event*);
            
                
  private: 
    RunAction* fRunAction;

  //from example B4d
  // methods
  G4THitsMap<G4double>* GetHitsCollection(G4int hcID,
                                          const G4Event* event) const;
  G4double GetSum(G4THitsMap<G4double>* hitsMap) const;

};

#endif