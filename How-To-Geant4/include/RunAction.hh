#ifndef RunAction_h
#define RunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"
#include "G4Accumulable.hh"

class DetectorConstruction;
class Run;
class PrimaryGeneratorAction;
class HistoManager;


class RunAction : public G4UserRunAction
{
  public:
    RunAction(DetectorConstruction*, PrimaryGeneratorAction*);
   ~RunAction();

  public:
    virtual G4Run* GenerateRun();  
    virtual void BeginOfRunAction(const G4Run*);
    virtual void   EndOfRunAction(const G4Run*);

    void AddEdep (G4double edep); 
                            
  private:
    DetectorConstruction*      fDetector;
    PrimaryGeneratorAction*    fPrimary;
    Run*                       fRun;    
    HistoManager*              fHistoManager;

  private:
    G4Accumulable<G4double> fEdep;
    G4Accumulable<G4double> fEdep2;
        
};


#endif
