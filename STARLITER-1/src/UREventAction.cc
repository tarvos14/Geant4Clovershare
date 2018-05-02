// 
// 

#include "UREventAction.hh"
#include "URRunAction.hh"

#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4VVisManager.hh"
#include "G4UImanager.hh"
#include "G4ios.hh"
#include <stdio.h>
#include "Randomize.hh"

int hit, i=0;

G4int GoodEvent=0;
UREventAction::UREventAction()
{}

UREventAction::~UREventAction()
{}

void UREventAction::BeginOfEventAction(const G4Event* evt)
{
  sectorID=0;
  ringID=0;
	
	for (sectorID=0;sectorID<8;sectorID++){
		for (ringID=0;ringID<24;ringID++){
			DE.energy[ringID][sectorID] = -1.0*eV; //zero the arrays before going to next event
			DE.theta[ringID][sectorID] = -1.0*deg;
			DE.ring[ringID] = -1;
			DE.sector[sectorID] = -1;
			E1.energy[ringID][sectorID] = -1.0*eV; //zero the arrays for E1 before going to next event
			E1.theta[ringID][sectorID] = -1.0*deg;
			E1.ring[ringID] = -1;
			E1.sector[sectorID] = -1;
	}
  }
    
    for(int nLayer=0; nLayer<5; nLayer++){
        layerEne[nLayer] = -1.0*keV;
    }
}

void UREventAction::EndOfEventAction(const G4Event* evt)
{
	if ((evt->GetEventID()+1) % 100000 == 0) // prints out run number to screen
		G4cout << ">>> Event " << evt->GetEventID()+1 << G4endl;
    
/*
 //test if the output data is correct
 G4cout << "new event -----------" << G4endl;

    sectorID=0;
    ringID=0;
    
    for (sectorID=0;sectorID<8;sectorID++){
        for (ringID=0;ringID<24;ringID++){
            G4cout << DEname[ringID][sectorID] << "\t" << ringID << "\t" << sectorID << "\t" << DE.energy[ringID][sectorID] << "\t" << E1.energy[ringID][sectorID] << G4endl;
        }
    }
*/  
 

     t->Fill();
}



