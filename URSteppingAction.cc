// 
// 
#include "URSteppingAction.hh"

#include "URDetectorConstruction.hh"
#include "G4SteppingManager.hh"
#include "G4Track.hh"
#include "G4ios.hh"
#include <stdio.h>
#include <cstring>
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"



URSteppingAction::URSteppingAction(URDetectorConstruction* myDC):myDetector(myDC)
{ }

void URSteppingAction::UserSteppingAction(const G4Step* aStep)
{

 const G4VPhysicalVolume* currentVolume1 = aStep->GetPreStepPoint()-> GetPhysicalVolume();

    if (currentVolume1 != NULL)
    {
	//Apears to be more complicated that TAS, but not by much. Need to inclue the Coulomb scattering off gold?
        ringID=0;
        sectorID=0;
        for (sectorID=0;sectorID<8;sectorID++){
            for (ringID=0;ringID<24;ringID++){
	
	////I commented out this step, since we don't have a DeltaE detector in our experiment.
/*
                if (currentVolume1->GetName() == DEname[ringID][sectorID])
                {
                     if this is a first step into the given volume (to avoid calculating angle for scatter within the pixel)
                    if (aStep->GetPreStepPoint()->GetStepStatus() == fGeomBoundary)
                    {
                         get a vector of the inital particle momentum
                        G4ThreeVector MomentumDirection = aStep->GetPreStepPoint()->GetMomentum();
                         calculate the theta angle from momentum direction
                        DE.theta[ringID][sectorID] = atan(sqrt(MomentumDirection.x()*MomentumDirection.x() +
                                                               MomentumDirection.y()*MomentumDirection.y()) / MomentumDirection.z()) * 180./ pi;
                         set the ring and sector ID for the output tree
                        DE.ring[ringID]=ringID;
                        DE.sector[sectorID]=sectorID;
                    }
                     increment the energy deposition array
                    DE.energy[ringID][sectorID] += aStep->GetTotalEnergyDeposit()*1000.; //multiplied by 1000. to obtain energy in keV
                }

*/
               if (currentVolume1->GetName() == E1name[ringID][sectorID])
                {
                    // if this is a first step into the given volume (to avoid calculating angle for scatter within the pixel)
                    if (aStep->GetPreStepPoint()->GetStepStatus() == fGeomBoundary)
                    {
                        // get a vector of the inital particle momentum
                        G4ThreeVector MomentumDirection = aStep->GetPreStepPoint()->GetMomentum();
                        // calculate the theta angle from momentum direction
                        E1.theta[ringID][sectorID] = atan(sqrt(MomentumDirection.x()*MomentumDirection.x() +
                                                               MomentumDirection.y()*MomentumDirection.y()) / MomentumDirection.z()) * 180./ pi;
                        // set the ring and sector ID for the output tree
                        E1.ring[ringID]=ringID;
                        E1.sector[sectorID]=sectorID;
                    }
                    // increment the energy deposition array
                    E1.energy[ringID][sectorID] += aStep->GetTotalEnergyDeposit()*1000.; //multiplied by 1000. to obtain energy in keV
                }
            }
        }
        
        for(int nLayer=0;nLayer<5;nLayer++){
            if (currentVolume1->GetName() == layername[nLayer]){
                layerEne[nLayer] += aStep->GetTotalEnergyDeposit()*1000.;
            }
        }
    }
    

}









