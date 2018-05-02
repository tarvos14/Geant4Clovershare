//
#ifndef URDetectorConstruction_h
#define URDetectorConstruction_h 1

#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"
  
class G4VPhysicalVolume;
class G4LogicalVolume;
class G4Material;
class B2aDetectorMessenger;

extern  G4String DEname[24][8];
extern  G4String E1name[24][8];
extern  G4String Gename[5];
extern G4String layername[5];
extern int ringID, sectorID;

class URDetectorConstruction : public G4VUserDetectorConstruction{
   public:
      URDetectorConstruction();
      ~URDetectorConstruction();

   public:
    G4VPhysicalVolume* Construct();

   private:
      void DefineMaterials();
    G4Material *HVAC, *Air, *BGO, *Al, *Ge, *Si, *Sm, *Au;

      G4VPhysicalVolume* ConstructDetector();
	
	G4LogicalVolume** DE_log;
	G4LogicalVolume** target_log;
    G4LogicalVolume** E1_log;

};
 

#endif

