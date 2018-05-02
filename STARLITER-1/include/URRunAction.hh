//
//

#ifndef RunActionUR_h
#define RunActionUR_h

#include "telescope.hh"
#include "G4UserRunAction.hh"
#include "globals.hh"
//include libraries for root
#include "TTree.h"
#include "TFile.h"

extern  TFile *newfile; // = new TFile("geant_out.root","recreate");
extern  TTree * t;// = new TTree("t","output from geant");
extern  TBranch * ebranch;// = t->Branch("etest",&DeposE[0][0],"etest/D");

extern int ringID, sectorID;
extern G4String DEname[24][8];
extern G4String E1name[24][8];
extern G4String layername[5];

extern event_t DE;
extern event_t E1;
extern double layerEne[5];
//extern char InputFileName;
extern double BeamWidth;
extern double BeamOffset;

class G4Run;

class RunActionUR : public G4UserRunAction
{
  public:
    RunActionUR();
    virtual ~RunActionUR();

  public:
    virtual void BeginOfRunAction(const G4Run* aRun);
    virtual void EndOfRunAction(const G4Run* aRun);

  private:
    G4int runIDcounter;
};

#endif
