//
//

#include "URRunAction.hh"

#include "G4Run.hh"
#include "G4UImanager.hh"
#include "G4VVisManager.hh"
#include "G4ios.hh"
#include "G4String.hh"

#include <stdio.h>
#include <sstream>
#include <iostream>
#include "globals.hh"
#include <fstream>



RunActionUR::RunActionUR()
{
  runIDcounter = 0;
}

RunActionUR::~RunActionUR()
{}

void RunActionUR::BeginOfRunAction(const G4Run* aRun)
{

  ((G4Run *)(aRun))->SetRunID(runIDcounter++);

  G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;

  srand(time(0));
   ////This file will need to be changed to account for different directories.
   char InputFileName[200];
   int RunNumber=BeamWidth*10+BeamOffset;

   sprintf(InputFileName, "/afs/crc.nd.edu/group/nsl/activetarget/users/shender4/Codes/GeantOutput/1stBeamGridSearch/BeamTest%d-root", RunNumber);



   newfile = new TFile(InputFileName,"recreate");

  t = new TTree("t","output from geant");

   ////Again, removed because there was no DeltaE detector
   // t->Branch("DE_ene",DE.energy,"energy[24][8]/D");
    //t->Branch("DE_theta",DE.theta,"theta[24][8]/D");
    //t->Branch("DE_ring",DE.ring,"ring[24]/I");
    //t->Branch("DE_sector",DE.sector,"sector[8]/I");

    t->Branch("E1_ene",E1.energy,"energy[24][8]/D");
    t->Branch("E1_theta",E1.theta,"theta[24][8]/D");
    t->Branch("E1_ring",E1.ring,"ring[24]/I");
    t->Branch("E1_sector",E1.sector,"sector[8]/I");

    t->Branch("dead_layer",layerEne,"layerEne[5]/D");

    //t->Branch("E1",&E1.energy,"energy[24][8]/D:theta[24][8]/D:ring[24]/I:sector[8]/I");

  if(G4VVisManager::GetConcreteInstance())
   {
     G4UImanager::GetUIpointer()->ApplyCommand("/vis~/clear/view");
     G4UImanager::GetUIpointer()->ApplyCommand("/vis~/draw/current");
   }

}



void RunActionUR::EndOfRunAction(const G4Run* aRun)
{
  G4cout << "### Run " << aRun->GetRunID() << " ended." << G4endl;

  if(G4VVisManager::GetConcreteInstance())
   {
     G4UImanager::GetUIpointer()->ApplyCommand("/vis~/show/view");
   }


t->Write();
}
