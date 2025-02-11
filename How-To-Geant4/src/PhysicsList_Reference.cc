#include "PhysicsList.hh"                     //Necessary. You need this.
// #include "G4SystemOfUnits.hh"
// #include "G4UnitsTable.hh"
#include "G4ProcessManager.hh"                //Necessary. You need this.

//Reference Physics Lists
#include "QBBC.hh"                        //works!

#include "FTF_BIC.hh"                     //works!
#include "FTFP_INCLXX.hh"                 //works!
#include "FTFP_INCLXX_HP.hh"              //works!
#include "FTFP_BERT.hh"                   //works!
#include "FTFP_BERT_HP.hh"                //works!
#include "FTFP_BERT_ATL.hh"               //works!
#include "FTFP_BERT_TRV.hh"               //works!
#include "FTFQGSP_BERT.hh"                //works!

#include "QGSP_BIC.hh"                    //works!
#include "QGSP_BIC_HP.hh"                 //works!
#include "QGSP_BIC_AllHP.hh"              //works! but system environmental variable 'G4PARTICLEHPDATA' needs to be set to path to data library e.g. TENDL
#include "QGSP_INCLXX.hh"                 //works!
#include "QGSP_INCLXX_HP.hh"              //works!
#include "QGSP_BERT.hh"                   //works!
#include "QGSP_BERT_HP.hh"                //works!
#include "QGSP_FTFP_BERT.hh"              //works!
#include "Shielding.hh"                   //works!

PhysicsList_Reference::PhysicsList_Reference()
:G4VModularPhysicsList()
{
	// G4VModularPhysicsList* physicsList = new QBBC;
	// G4VModularPhysicsList* physicsList = new FTF_BIC;
	// G4VModularPhysicsList* physicsList = new FTFP_INCLXX;
	// G4VModularPhysicsList* physicsList = new FTFP_INCLXX_HP;

	// G4VModularPhysicsList* physicsList = new FTFP_BERT;
	// G4VModularPhysicsList* physicsList = new FTFP_BERT_HP;
	// G4VModularPhysicsList* physicsList = new FTFP_BERT_ATL;
	// G4VModularPhysicsList* physicsList = new FTFP_BERT_TRV;
	// G4VModularPhysicsList* physicsList = new FTFQGSP_BERT;

	// G4VModularPhysicsList* physicsList = new QGSP_BIC;
	// G4VModularPhysicsList* physicsList = new QGSP_BIC_HP;
	G4VModularPhysicsList* physicsList = new QGSP_BIC_AllHP;  //system environmental variable 'G4PARTICLEHPDATA' needs to be set to path to data library e.g. TENDL
	//G4VModularPhysicsList* physicsList = new QGSP_INCLXX;
	//G4VModularPhysicsList* physicsList = new QGSP_INCLXX_HP;
	// G4VModularPhysicsList* physicsList = new QGSP_BERT;
	// G4VModularPhysicsList* physicsList = new QGSP_BERT_HP;
	// G4VModularPhysicsList* physicsList = new QGSP_FTFP_BERT;

	// G4VModularPhysicsList* physicsList = new Shielding;

	
}



PhysicsList_Reference::~PhysicsList_Reference()
{ }
