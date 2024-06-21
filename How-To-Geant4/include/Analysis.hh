#ifndef Analysis_h
#define Analysis_h 1


#include "G4AnalysisManager.hh" //for being able to use .root as file format


//i believe this comes from the 'Activation' Example and is neccessary to count produced particles? 
const G4int kMaxHisto1 = 24;
const G4int kMaxHisto2 = 44;

// CAN BE REMOVED?
//From Activation Example
//#include "globals.hh"
//#include <map>


//#include "G4GenericAnalysisManager.hh"
//using G4AnalysisManager = G4GenericAnalysisManager;

/*
//From Activation Example
class HistoManager
{
  public:
   HistoManager();
  ~HistoManager();

  private:
    void Book();

private:
    G4String fFileName;
};
*/

#endif
