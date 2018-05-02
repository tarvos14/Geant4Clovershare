// 
// 

#ifndef UREventAction_h
#define UREventAction_h 1

#include "telescope.hh"
#include "G4UserEventAction.hh"
#include "globals.hh"



extern int ringID, sectorID;

extern event_t DE;
extern event_t E1;
extern double layerEne[5];

class G4Event;

class UREventAction : public G4UserEventAction
{
  public:
    UREventAction();
    ~UREventAction();

  public:
    void BeginOfEventAction(const G4Event*);
    void EndOfEventAction(const G4Event*);

  private:

};

#endif

    






