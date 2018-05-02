//
//

#ifndef URPrimaryGeneratorAction_h
#define URPrimaryGeneratorAction_h 

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"


extern float part_ene;
extern double BeamWidth;
extern double BeamOffset;


//class URDetectorConstruction;
class G4ParticleGun;
class G4Event;
//NDTAS has an Event Array
//class URPrimaryGeneratorMessenger;

class URPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    URPrimaryGeneratorAction();
    ~URPrimaryGeneratorAction();

  public:
    void GeneratePrimaries(G4Event *anEvent);
//    void SetRndmFlag(G4String val){rndmFlag = val;};

  private:
// NDTAS TREATS IT AS AN ARRAY
    G4ParticleGun* particleGun;
  //    G4ParticleGun* particleGun2;
  //  G4ParticleGun* particleGun3;


//    URPrimaryGeneratorMessenger *gunMessenger;
//    G4String rndmFlag;
};

#endif
