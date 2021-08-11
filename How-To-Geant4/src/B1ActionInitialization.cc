#include "B1ActionInitialization.hh"
#include "PrimaryGeneratorAction.hh"
#include "B1RunAction.hh"
#include "B1EventAction.hh"
#include "B1SteppingAction.hh"


B1ActionInitialization::B1ActionInitialization()
 : G4VUserActionInitialization()
{}


B1ActionInitialization::~B1ActionInitialization()
{}


void B1ActionInitialization::BuildForMaster() const
{
  B1RunAction* runAction = new B1RunAction;
  SetUserAction(runAction);
}


void B1ActionInitialization::Build() const
{
  SetUserAction(new PrimaryGeneratorAction);

  B1RunAction* runAction = new B1RunAction;
  SetUserAction(runAction);
  
  B1EventAction* eventAction = new B1EventAction(runAction);
  SetUserAction(eventAction);
  
  SetUserAction(new B1SteppingAction(eventAction));
}  

