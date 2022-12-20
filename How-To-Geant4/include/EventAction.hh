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

  public:
    virtual void BeginOfEventAction(const G4Event*);
    virtual void   EndOfEventAction(const G4Event*);
    
    void AddEdep (G4double Edep);
    void AddEflow(G4double Eflow);          
                
  private:
    G4double fTotalEnergyDeposit;
    G4double fTotalEnergyFlow; 

  private:
    RunAction* fRunAction;
    G4double     fEdep;
    G4double  fEnergyAbs;
    G4double  fEnergyGap;
    G4double  fTrackLAbs; 
    G4double  fTrackLGap;

  //from example B4d
  // methods
  G4THitsMap<G4double>* GetHitsCollection(G4int hcID,
                                          const G4Event* event) const;
  G4double GetSum(G4THitsMap<G4double>* hitsMap) const;
  void PrintEventStatistics(G4double absoEdep, G4double absoTrackLength,
                            G4double gapEdep, G4double gapTrackLength) const;
  
  // data members                   
  G4int  fAbsoEdepHCID;
  G4int  fGapEdepHCID;
  G4int  fAbsoTrackLengthHCID;
  G4int  fGapTrackLengthHCID;

};


#endif
