#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4UIterminal.hh"
#include "G4ios.hh"

//#include "LHEP_PRECO_HP.hh"
#include "QGSP_BIC_HP.hh"
//#include "tbjcPhysicsList.hh"


#include <fstream>
#include <iostream>
#include <stdlib.h>

#include "TTree.h"
#include "TFile.h"
#include "TROOT.h"
#include "TRint.h"
#include "TPluginManager.h"

#include "telescope.hh"
#include <ctime>
#include "URDetectorConstruction.hh"
#include "URPrimaryGeneratorAction.hh"
#include "URRunAction.hh"
#include "UREventAction.hh"
#include "URSteppingAction.hh"
#include "Randomize.hh"
#include "time.h"

#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif

int ringID;
int sectorID;
float part_ene;

G4String DEname[24][8];
G4String E1name[24][8];
G4String layername[5];
G4String Gename[5];

event_t DE;
event_t E1;
double layerEne[5];

TFile *newfile;
TTree *t;
TBranch *ebranch;


int main(int argc, char** argv)
{

	//Supposed to update the seeds in the simulation. Not sure yet if helps.
	
  // Construct the default run manager
  G4RunManager *runManager = new G4RunManager;
  ////Changed Random number engine in Geant. Not sure it matters much now.
 // CLHEP::HepRandom::setTheEngine(new CLHEP::RanluxEngine());


// set mandatory initialization classes
	URDetectorConstruction *URDet = new URDetectorConstruction;
	runManager->SetUserInitialization(URDet);
//  runManager->SetUserInitialization(new LHEP_PRECO_HP);//Library for neutrons
	runManager->SetUserInitialization(new QGSP_BIC_HP);//tbjcPhysicsList());

  // set optional user action class
  runManager->SetUserAction(new RunActionUR);
	//Feed in an argument argv[1], the second thing read in which is the file name

  
  // set mandatory user action class
  runManager->SetUserAction(new URPrimaryGeneratorAction);
	//Feed in an argument argv[2], the third thing read in which is the num?

  UREventAction *eventAction = new UREventAction;
   runManager->SetUserAction(eventAction);

  runManager->SetUserAction(new URSteppingAction(URDet)); 

  // Initialize G4 kernels
  runManager->Initialize();

   #ifdef G4VIS_USE
      G4VisManager* visManager = new G4VisExecutive;
      visManager->Initialize();

 #endif   
	
    // Get the pointer to the User Interface manager
    G4UImanager* UImanager = G4UImanager::GetUIpointer();
    if (argc!=1) {

        // interactive mode : define UI session
#ifdef G4UI_USE

        G4UIExecutive* ui = new G4UIExecutive(argc, argv,"Qt");

        UImanager->ApplyCommand("/control/execute vis.mac");
	UImanager->ApplyCommand("/run/numberOfThreads 5");
        ui->SessionStart();
        delete ui;
#endif
    }
	////High Beam rate here, can be changed around depending on needs.
<<<<<<< HEAD
  runManager->BeamOn(1.0E8);
=======
  runManager->BeamOn(1.0E7);
>>>>>>> a6e5f61f736c05a277be50dcbaed6a4a7d27b04f


  // job termination

  newfile->Close();
  G4cout << "### Rootfile closed!" << G4endl;

  delete runManager;
  G4cout << "### runManager closed!" << G4endl;

<<<<<<< HEAD
 

=======
>>>>>>> a6e5f61f736c05a277be50dcbaed6a4a7d27b04f
  return 0;
}
	//UImanager->ApplyCommand("/run/numberOfThreads num") next to other UImanager;





