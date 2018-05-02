// 
// 

#ifndef URSteppingAction_h
#define URSteppingAction_h 1

#include "telescope.hh"
#include "G4UserSteppingAction.hh"
#include "globals.hh"

extern G4String DEname[24][8];
extern G4String E1name[24][8];
extern G4String layername[5];
extern int ringID, sectorID;

extern event_t DE;
extern event_t E1;
extern double layerEne[5];

class URDetectorConstruction;
class G4Track;

class URSteppingAction : public G4UserSteppingAction
{
  public:
    URSteppingAction(URDetectorConstruction* myDC);
    virtual ~URSteppingAction(){};

    virtual void UserSteppingAction(const G4Step*);

  private:
    URDetectorConstruction* myDetector;
};

#endif






