
#include "URDetectorConstruction.hh"
#include "G4SDManager.hh"
#include "G4NistManager.hh"
#include "G4Element.hh"
#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Cons.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4SubtractionSolid.hh"
#include "G4UnionSolid.hh"
#include "G4IntersectionSolid.hh"
#include "G4GeometryManager.hh"

#include "G4UnitsTable.hh"
#include "globals.hh"

#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include <iostream>
#include <sstream>
#include "G4String.hh"
#include "G4ios.hh"
#include <stdio.h>



URDetectorConstruction::URDetectorConstruction()
  :	DE_log(0),target_log(0),E1_log(0)
{
	DE_log = new G4LogicalVolume*[24];
    E1_log = new G4LogicalVolume*[24];
	target_log = new G4LogicalVolume*;
}

URDetectorConstruction::~URDetectorConstruction()
{
	delete [] DE_log;
    delete [] E1_log;
	delete [] target_log;
}

G4VPhysicalVolume* URDetectorConstruction::Construct()
{
  DefineMaterials();

    
  return ConstructDetector();
}

void URDetectorConstruction::DefineMaterials()
{

    G4NistManager* man = G4NistManager::Instance();
    
    HVAC  = man->FindOrBuildMaterial("G4_Galactic");
    Air  = man->FindOrBuildMaterial("G4_AIR");
    BGO  = man->FindOrBuildMaterial("G4_BGO");
    Al  = man->FindOrBuildMaterial("G4_Al");
    Ge  = man->FindOrBuildMaterial("G4_Ge");
    Si = man->FindOrBuildMaterial("G4_Si");
    Sm = man->FindOrBuildMaterial("G4_Sm");
    Au = man->FindOrBuildMaterial("G4_Au");

	
// Print out Elements and Materials
/*  G4cout << "\n\n ####-------------------------------------------------------#### \n";
  G4cout << "\n\n\n\n\t\t #### List of elements used #### \n";
  G4cout << *(G4Element::GetElementTable());
  G4cout << "\n\n\n\n\t\t #### List of materials used #### \n";
  G4cout << *(G4Material::GetMaterialTable());
  G4cout << "\n\n ####-------------------------------------------------------#### \n";
*/
}

G4VPhysicalVolume* URDetectorConstruction::ConstructDetector()
{

//..........EXPERIMENTAL ROOM............
    // room is a cylinder, inner radius 0 cm, radius 100 cm and length 300 cm
  G4Tubs* room_tube = new G4Tubs("room", 0.0*cm, 100.0*cm, 300.0*cm, 0.0*deg, 360.0*deg);
    //room is full of vacuum and is called 'room'
  G4LogicalVolume* room_log = new G4LogicalVolume(room_tube,HVAC,"room",0,0,0);
    // place it in the center of the coordinate system
  G4VPhysicalVolume* room_phys = new G4PVPlacement(0,G4ThreeVector(0.0*cm,0.0*cm,0.0*cm),"room",room_log,NULL,false,0);

 

//.........DIMENSIONS OF target..........
// target is a cylinder, inner radius 0 cm, radius 0.5cm and length 0.0001 cm. Adjust target size to raise statistics
    G4Tubs* target_tube = new G4Tubs("target", 0.0*cm, 0.5*cm, 0.0001*cm, 0.0*deg, 360.0*deg);
    //target is full of gold and is called 'target'
    G4LogicalVolume* target_log = new G4LogicalVolume(target_tube,Au,"target",0,0,0);
    // place it in the center of the coordinate system
    new G4PVPlacement(0,G4ThreeVector(0*cm,0*cm,0*cm),target_log,"target",room_log,false,0);

    

//.........DIMENSIONS OF detectors..........
    // dE Detector
    G4double innerR = 11*mm;             // 11mm borehole
    G4double thickness = 0.5*0.14*mm;              // thickness of the DE detector
    
    G4double startAngle = 0.0*deg;
   G4double spanAngle = 45.0*deg;
    
    G4Tubs* DE_tub[24];
    G4LogicalVolume* DE_log[24];

    // E1 Detector is 1 mm thick
    G4double E1_innerR = 11*mm;             // 11mm borehole
    G4double E1_thickness = 0.5*1*mm;              // thickness of the E1 detector
    
    G4double E1_startAngle = 0.0*deg;
    G4double E1_spanAngle = 45.0*deg;
    
    G4Tubs* E1_tub[24];
    G4LogicalVolume* E1_log[24];
    
    char buf1[20], buf2[20];
    
    //germanium leaves
    double leafLength = 25*mm; //half of the length
    double leafRadius = 25*mm;
    double leafCutOff = 5*mm; //cut on the touching sides of leaves for tighter packing
    //ge Casing
    double geCasingLength = 35*mm;
    double geCasingWidth = 50*mm;
    double geCasingThickness = 1*mm;
    double geCasingWindowThickness = 0.5*mm; // window thickness (implement later)
    //target chamber
    double chTopThickness = 3.8*mm;
    double chOuterThickness = 3.8*mm;
    double chOuterRadius = 125*mm + chOuterThickness;
    double chHeight = 105*mm;
    //BGO shield
    double bgoShieldLength = 0.5*(275-113)*mm; //half of the length of shield befor the tampering
    double bgoOuterWidth = 130*mm; //external width of the shield
    double bgoInnerWidth = 51*mm; //opening in the center to hold the Ge detector
    double bgoShieldHeight = 0.5*115*mm; //casing height estimated from the drawings
    //BGO crystal dimensions
    double bgoLength = 125*mm;
    double bgoOuterBack = 91.25*mm;
    double bgoInnerBack = 69*mm;
    double bgoOuterFront = 72*mm;
    double bgoInnerFront = 50*mm;
    
    double bgoInnerAl = 0.8*mm;
    double bgoOuterAl = 1.5*mm;
    double bgoFrontAl = 5*mm;
    
    double bgoTrapLength = 0.5*113*mm;
    double bgoTrapFrontWidth = 0.5*119*mm;
    
    //dead layers thicknesses
    double dShield = 0.5*12e-6*m; //12um
    double alLayer = 0.5*.1e-6*m; //0.1um
    double auLayer = 0.5*.3e-6*m; //0.3um
    
    double dShOuterR = 35*mm;
    double dShInnerR = 10*mm;
    double dShTubeLength = 4*mm;

// ====================== Placing the volumes ==============================//
    
   // telescope

  G4double Pos_x = 0.0*mm;
  G4double Pos_y = 0.0*mm;
  G4double Pos_z = 0.0*mm;  
  
    
  G4int sectorID = 0;
  ringID=0;
  sectorID=0;
  std::ostringstream oss;
  G4String s1, s2;
	
for (int i=0;i<24;i++)
 {
	 for (int j=0;j<8;j++){
		oss.str("");
		oss << i;
		s1=oss.str();
		oss.str("");
		oss << j;
		s2=oss.str();
         DEname[i][j]="DE" + s1 + "_"+ s2;
         E1name[i][j]="E1" + s1 + "_"+ s2;

	}
 }
	
	
	// Rotation Matrix
	G4RotationMatrix* rotZ_45 = new G4RotationMatrix();
	rotZ_45->rotateZ(45*deg);
	G4RotationMatrix* rotZ_90 = new G4RotationMatrix();
	rotZ_90->rotateZ(90*deg);
	G4RotationMatrix* rotZ_135 = new G4RotationMatrix();
	rotZ_135->rotateZ(135*deg);
	G4RotationMatrix* rotZ_180 = new G4RotationMatrix();
	rotZ_180->rotateZ(180*deg);
	G4RotationMatrix* rotZ_225 = new G4RotationMatrix();
	rotZ_225->rotateZ(225*deg);
	G4RotationMatrix* rotZ_270 = new G4RotationMatrix();
	rotZ_270->rotateZ(270*deg);
	G4RotationMatrix* rotZ_315 = new G4RotationMatrix();
	rotZ_315->rotateZ(315*deg);
	
	////Commented out, didn't have DeltaE detector in experiment
/*
	 detector placement for DE detector
	for (int i=0;i<24;i++){
        
		sprintf(buf1,"DE_tub%d",i);
		DE_tub[i] = new G4Tubs(buf1,innerR+(double)i*mm,innerR+((double)i+1.)*mm,thickness,startAngle,spanAngle);
        
		sprintf(buf1,"DE_log%d",i);
		DE_log[i] = new G4LogicalVolume(DE_tub[i],Si,buf2,0,0,0);
        
		new G4PVPlacement(rotZ_315,G4ThreeVector(Pos_x,Pos_y,Pos_z),DE_log[i],DEname[i][0],room_log,false,0);
		new G4PVPlacement(rotZ_270,G4ThreeVector(Pos_x,Pos_y,Pos_z),DE_log[i],DEname[i][1],room_log,false,1);
		new G4PVPlacement(rotZ_225,G4ThreeVector(Pos_x,Pos_y,Pos_z),DE_log[i],DEname[i][2],room_log,false,2);
		new G4PVPlacement(rotZ_180,G4ThreeVector(Pos_x,Pos_y,Pos_z),DE_log[i],DEname[i][3],room_log,false,3);
		new G4PVPlacement(rotZ_135,G4ThreeVector(Pos_x,Pos_y,Pos_z),DE_log[i],DEname[i][4],room_log,false,4);
		new G4PVPlacement(rotZ_90,G4ThreeVector(Pos_x,Pos_y,Pos_z),DE_log[i],DEname[i][5],room_log,false,5);
		new G4PVPlacement(rotZ_45,G4ThreeVector(Pos_x,Pos_y,Pos_z),DE_log[i],DEname[i][6],room_log,false,6);
		new G4PVPlacement(0,G4ThreeVector(Pos_x,Pos_y,Pos_z),DE_log[i],DEname[i][7],room_log,false,7);
        
	}
*/
    // detector placement for E1 detector
    
    ////Changed position of E1 detector to 25 mm, also reflected in the dector dead layer positions.
    G4double E1_Pos_x = 0.0*mm;
    G4double E1_Pos_y = 0.0*mm;
    G4double E1_Pos_z = 25.0*mm;
    
    
	for (int i=0;i<24;i++){
        
		sprintf(buf1,"E1_tub%d",i);
		E1_tub[i] = new G4Tubs(buf1,E1_innerR+(double)i*mm,E1_innerR+((double)i+1.)*mm,E1_thickness,E1_startAngle,E1_spanAngle);
        
		sprintf(buf1,"E1_log%d",i);
		E1_log[i] = new G4LogicalVolume(E1_tub[i],Si,buf2,0,0,0);
        
		new G4PVPlacement(rotZ_315,G4ThreeVector(E1_Pos_x,E1_Pos_y,E1_Pos_z),E1_log[i],E1name[i][0],room_log,false,0);
		new G4PVPlacement(rotZ_270,G4ThreeVector(E1_Pos_x,E1_Pos_y,E1_Pos_z),E1_log[i],E1name[i][1],room_log,false,1);
		new G4PVPlacement(rotZ_225,G4ThreeVector(E1_Pos_x,E1_Pos_y,E1_Pos_z),E1_log[i],E1name[i][2],room_log,false,2);
		new G4PVPlacement(rotZ_180,G4ThreeVector(E1_Pos_x,E1_Pos_y,E1_Pos_z),E1_log[i],E1name[i][3],room_log,false,3);
		new G4PVPlacement(rotZ_135,G4ThreeVector(E1_Pos_x,E1_Pos_y,E1_Pos_z),E1_log[i],E1name[i][4],room_log,false,4);
		new G4PVPlacement(rotZ_90,G4ThreeVector(E1_Pos_x,E1_Pos_y,E1_Pos_z),E1_log[i],E1name[i][5],room_log,false,5);
		new G4PVPlacement(rotZ_45,G4ThreeVector(E1_Pos_x,E1_Pos_y,E1_Pos_z),E1_log[i],E1name[i][6],room_log,false,6);
		new G4PVPlacement(0,G4ThreeVector(E1_Pos_x,E1_Pos_y,E1_Pos_z),E1_log[i],E1name[i][7],room_log,false,7);
        
	}
    
    //dead layers
	//These dead layers come before the E1 detectors. There should be an aluminum and a gold layer, but no shield and no layers after
	//the removed deltaE detector.
    
    for (int i=0;i<5;i++)
    {
            oss.str("");
            oss << i;
            s1=oss.str();
            layername[i]="layer" + s1;
    }
    
    
    //G4Tubs* dShield_tub = new G4Tubs("dShiled_tube",dShInnerR,dShOuterR,dShield,0*deg,360*deg);
    //G4Tubs* dShield2_tub = new G4Tubs("dShiled2_tube",dShInnerR-dShield,dShInnerR,dShTubeLength,0*deg,360*deg);
    G4Tubs* alLayer_tub = new G4Tubs("alLayer_tube",innerR,innerR+24*mm,alLayer,0*deg,360*deg);
    G4Tubs* auLayer_tub = new G4Tubs("auLayer_tube",innerR,innerR+24*mm,auLayer,0*deg,360*deg);
    
   // G4LogicalVolume* dShield_log = new G4LogicalVolume(dShield_tub,Al,"dShield_log",0,0,0);
   // G4VPhysicalVolume* dShield_phys = new G4PVPlacement(0,G4ThreeVector(0*mm,0*mm,16*mm),dShield_log,layername[0],room_log,false,0);
    
    //G4LogicalVolume* dShield2_log = new G4LogicalVolume(dShield2_tub,Al,"dShield2_log",0,0,0);
    //G4VPhysicalVolume* dShield2_phys = new G4PVPlacement(0,G4ThreeVector(0*mm,0*mm,16*mm+dShTubeLength),dShield2_log,"dShield2",room_log,false,0);
    
    G4LogicalVolume* alLayer_log = new G4LogicalVolume(alLayer_tub,Al,"alLayer_log",0,0,0);
    //G4VPhysicalVolume* alLayer1_phys = new G4PVPlacement(0,G4ThreeVector(0*mm,0*mm,18*mm-thickness-alLayer-.01*mm),alLayer_log,layername[1],room_log,false,0);
    G4VPhysicalVolume* alLayer2_phys = new G4PVPlacement(0,G4ThreeVector(0*mm,0*mm,25.0*mm+E1_thickness+alLayer+.01*mm),alLayer_log,layername[2],room_log,false,0);
    
    G4LogicalVolume* auLayer_log = new G4LogicalVolume(auLayer_tub,Au,"auLayer_log",0,0,0);
   // G4VPhysicalVolume* auLayer1_phys = new G4PVPlacement(0,G4ThreeVector(0*mm,0*mm,18*mm+thickness+auLayer+.01*mm),auLayer_log,layername[3],room_log,false,0);
    G4VPhysicalVolume* auLayer2_phys = new G4PVPlacement(0,G4ThreeVector(0*mm,0*mm,25.0*mm-E1_thickness-auLayer-.01*mm),auLayer_log,layername[4],room_log,false,0);


    
    //---------Chamber, BGO and clover -----------------------------------------------------------//
    
    //--------- Logical volumes ------------------------------------------------------------------//
    
    //leaf
    G4Tubs* geLeaf_tube = new G4Tubs("geLeaf_tube",0*mm,leafRadius,leafLength,0*deg, 360*deg);
    G4Box* geLeaf_cutBox = new G4Box("geLeaf_cutBox",leafRadius,leafRadius,leafLength);
    
    G4IntersectionSolid* geLeaf_cut1 = new G4IntersectionSolid("geLeaf_cut1",geLeaf_cutBox,geLeaf_tube,0,G4ThreeVector(-leafCutOff,-leafCutOff,0));
    G4IntersectionSolid* geLeaf_cut2 = new G4IntersectionSolid("geLeaf_cut2",geLeaf_cutBox,geLeaf_tube,0,G4ThreeVector(leafCutOff,-leafCutOff,0));
    G4IntersectionSolid* geLeaf_cut3 = new G4IntersectionSolid("geLeaf_cut3",geLeaf_cutBox,geLeaf_tube,0,G4ThreeVector(leafCutOff,leafCutOff,0));
    G4IntersectionSolid* geLeaf_cut4 = new G4IntersectionSolid("geLeaf_cut4",geLeaf_cutBox,geLeaf_tube,0,G4ThreeVector(-leafCutOff,leafCutOff,0));
    
    G4LogicalVolume* geLeaf_log1 = new G4LogicalVolume(geLeaf_cut1,Ge,"leaf_log1",0,0,0);
    G4LogicalVolume* geLeaf_log2 = new G4LogicalVolume(geLeaf_cut2,Ge,"leaf_log2",0,0,0);
    G4LogicalVolume* geLeaf_log3 = new G4LogicalVolume(geLeaf_cut3,Ge,"leaf_log3",0,0,0);
    G4LogicalVolume* geLeaf_log4 = new G4LogicalVolume(geLeaf_cut4,Ge,"leaf_log4",0,0,0);
    
    //casing
    G4Box* geCasing_box2 = new G4Box("geCasing_box2",geCasingWidth,geCasingWidth,geCasingLength);
    G4Box* geCasing_box1 = new G4Box("geCasing_box1",geCasingWidth-geCasingThickness,geCasingWidth-geCasingThickness,geCasingLength-geCasingThickness);
    G4SubtractionSolid* geCasing_sub = new G4SubtractionSolid("geCasing_sub",geCasing_box2,geCasing_box1);
    G4LogicalVolume* geCasing_log = new G4LogicalVolume(geCasing_sub,Al,"geCasing_log",0,0,0);
    
    //chamber
    G4Tubs* alChamberTop_tube = new G4Tubs("alChamberTop_tube",0*mm,chOuterRadius,chTopThickness,0*deg,360*deg);
    G4Tubs* alChamberSide_tube = new G4Tubs("alChamberSide_tube",chOuterRadius-chOuterThickness,chOuterRadius,chHeight,0*deg,360*deg);
    G4Tubs* alChamberBottom_tube = new G4Tubs("alChamberBottom_tube",0*mm,chOuterRadius,chOuterThickness,0*deg,360*deg);
    //add all the pieces of the chamber together
    G4UnionSolid* alChamber1_union = new G4UnionSolid("alChamber1_union",alChamberSide_tube,alChamberTop_tube,0,G4ThreeVector(0*mm,0*mm,chHeight+0.5*chTopThickness));
    G4UnionSolid* alChamber_union = new G4UnionSolid("alChamber_union",alChamber1_union,alChamberBottom_tube,0,G4ThreeVector(0*mm,0*mm,-chHeight-chOuterThickness));
    G4LogicalVolume* alChamber_log = new G4LogicalVolume(alChamber_union,Al,"alChamber_log",0,0,0);
    
    //BGO shield
    //Al casing
    //rectangular part
    G4Box* bgoAl_box2 = new G4Box("bgoAl_box2",bgoOuterWidth,bgoOuterWidth,bgoShieldLength);
    G4Box* bgoAl_box1 = new G4Box("bgoAl_box2",bgoOuterWidth-2*bgoOuterAl,bgoOuterWidth-2*bgoOuterAl,bgoShieldLength+4*mm);
    G4SubtractionSolid* bgoAl_sub = new G4SubtractionSolid("bgoAl_sub",bgoAl_box2,bgoAl_box1,0,G4ThreeVector(0*mm,0*mm,0*mm));
    //trapezoidal part
    G4Trd* bgoAl_trd2 = new G4Trd("bgoAl_trd2",bgoOuterWidth,bgoTrapFrontWidth,bgoOuterWidth,bgoTrapFrontWidth,bgoTrapLength);
    G4Trd* bgoAl_trd1 = new G4Trd("bgoAl_trd1",bgoOuterWidth-bgoOuterAl,bgoTrapFrontWidth-bgoOuterAl,bgoOuterWidth-bgoOuterAl,bgoTrapFrontWidth-bgoOuterAl,bgoTrapLength-bgoFrontAl+4*mm);
    G4SubtractionSolid* bgoAl_sub2 = new G4SubtractionSolid("bgoAl_sub2",bgoAl_trd2,bgoAl_trd1,0,G4ThreeVector(0*mm,0*mm,-bgoFrontAl-1*mm));
    //the two together
    G4UnionSolid* bgoAl_union = new G4UnionSolid("bgoAl_union",bgoAl_sub,bgoAl_sub2,0,G4ThreeVector(0*mm,0*mm,bgoShieldLength+bgoTrapLength));
    //opening for the Ge detector
    G4Box* bgoAl_box3 = new G4Box("bgoAl_box3",bgoInnerWidth+bgoInnerAl,bgoInnerWidth+bgoInnerAl,bgoShieldLength+bgoTrapLength);
    G4UnionSolid *bgoAl_shape2 = new G4UnionSolid("bgoAl_shape2",bgoAl_union,bgoAl_box3,0,G4ThreeVector(0*mm,0*mm,bgoTrapLength));
    G4Box* bgoAl_box4 = new G4Box("bgoAl_box4",bgoInnerWidth,bgoInnerWidth,bgoShieldLength+bgoTrapLength+2*mm);
    G4SubtractionSolid *bgoAl_shape = new G4SubtractionSolid("bgoAl_shape",bgoAl_shape2,bgoAl_box4,0,G4ThreeVector(0*mm,0*mm,bgoTrapLength));
    G4LogicalVolume* bgoAl_log = new G4LogicalVolume(bgoAl_shape,Al,"bgoAl_log",0,0,0);
    
    //Al plate to imitate the Ge dewar and BGO PMTs
    G4Box* bgoAl_box5 = new G4Box("bgoAl_box5",bgoOuterWidth-bgoOuterAl,bgoOuterWidth-bgoOuterAl,70*mm);
    G4LogicalVolume* bgoAlBack_log = new G4LogicalVolume(bgoAl_box5,Al,"bgoAlBack_log",0,0,0);
    
    //BGO crystal
    G4Trd* bgo_trd2 = new G4Trd("bgo_trd2",bgoOuterBack,bgoOuterFront,bgoOuterBack,bgoOuterFront,bgoLength);
    G4Trd* bgo_trd1 = new G4Trd("bgo_trd1",bgoInnerBack,bgoInnerFront,bgoInnerBack,bgoInnerFront,bgoLength+1*mm);
    G4SubtractionSolid *bgo_shape = new G4SubtractionSolid("bgo_shape",bgo_trd2,bgo_trd1,0,G4ThreeVector(0*mm,0*mm,0*mm));
    //shape to cut the fron eges
    G4UnionSolid *bgo_cutter = new G4UnionSolid("bgo_cutter",bgoAl_box1,bgoAl_trd1,0,G4ThreeVector(0*mm,0*mm,bgoShieldLength+bgoTrapLength));
    G4IntersectionSolid* bgo_cut = new G4IntersectionSolid("bgo_cut",bgo_shape,bgo_cutter,0,G4ThreeVector(0*mm,0*mm,-bgoShieldLength+bgoFrontAl));
    G4LogicalVolume* bgo_log = new G4LogicalVolume(bgo_cut,BGO,"bgo_log",0,0,0);
   

 
    //--------- Physical volumes ------------------------------------------------------------------//
    //    // Rotation Matries
    G4RotationMatrix* rotZ90 = new G4RotationMatrix();
    rotZ90->rotateZ(90*deg);
    G4RotationMatrix* rotZ180 = new G4RotationMatrix();
    rotZ180->rotateZ(180*deg);
    G4RotationMatrix* rotZ270 = new G4RotationMatrix();
    rotZ270->rotateZ(270*deg);
    
    G4RotationMatrix* rotX90 = new G4RotationMatrix();
    rotX90->rotateX(90*deg);
    
    G4RotationMatrix* rotY90 = new G4RotationMatrix();
    rotY90->rotateY(90*deg);
	
    G4RotationMatrix* rotY270 = new G4RotationMatrix();
    rotY270->rotateY(270*deg);

    G4RotationMatrix* rotY45 = new G4RotationMatrix();
    rotY45->rotateY(45*deg);

    G4RotationMatrix* rotY135 = new G4RotationMatrix();
    rotY135->rotateY(135*deg);

    G4RotationMatrix* rotY225 = new G4RotationMatrix();
    rotY225->rotateY(225*deg);

    G4RotationMatrix* rotY315 = new G4RotationMatrix();
    rotY315->rotateY(315*deg);
    
    //coordinates of the center of the clover window
    double posX = 32*mm;
    double posY = 0*mm;
    double posZ = 0*mm;
    
    //leaf offset within clover (distance in X and Y from the clover center)
    double leafX = leafRadius;
    double leafY = leafRadius;
    
    G4String s;
    
    
    
    //chamber
    G4VPhysicalVolume* chamber_phys = new G4PVPlacement(rotX90,G4ThreeVector(0*mm,0*mm,0*mm),alChamber_log,"alChamber",room_log,false,0);
    
//First clover being made

    //clover casing
    G4VPhysicalVolume* casing_phys = new G4PVPlacement(rotY90,G4ThreeVector(posX+geCasingLength+chOuterRadius,posY,posZ),geCasing_log,"geCasing",room_log,false,0);
    
    for (int i=0;i<5;i++)
    {
        oss.str("");   //clear the ostringstream
        oss << i;  //stream int into ostringstream
        s=oss.str();   //copy ostringstream to G4string
        Gename[i]="leaf" + s;
    }
    Gename[4]="bgo";
  

 // G4cout << "----------------------------- \n"<<bgoAl_log<<G4endl;
  

    //BGO shield
    G4VPhysicalVolume* bgoAl_phys = new G4PVPlacement(rotY90,G4ThreeVector(chOuterRadius+2*bgoTrapLength+bgoShieldLength,posY,posZ),bgoAl_log,"bgoShield",room_log,false,0);
  //  G4VPhysicalVolume* bgo_phys = new G4PVPlacement(rotY90,G4ThreeVector(chOuterRadius+bgoLength,posY,posZ),bgo_log,Gename[5],room_log,false,0);
    G4VPhysicalVolume* bgo_phys = new G4PVPlacement(rotY90,G4ThreeVector(chOuterRadius+bgoLength,posY,posZ),bgo_log,Gename[4],room_log,false,0);
    G4VPhysicalVolume* bgoAlBack_phys = new G4PVPlacement(rotY90,G4ThreeVector(chOuterRadius+2*bgoShieldLength+3*bgoTrapLength,posY,posZ),bgoAlBack_log,"bgoBack",room_log,false,0);
   

//  return room_phys;

 
    //clover
    G4VPhysicalVolume* leaf_phys1 = new G4PVPlacement(rotY90,G4ThreeVector(posX+geCasingLength+chOuterRadius,leafY,leafX),geLeaf_log1,Gename[1],room_log,false,0);
    G4VPhysicalVolume* leaf_phys2 = new G4PVPlacement(rotY90,G4ThreeVector(posX+geCasingLength+chOuterRadius,leafY,-leafX),geLeaf_log2,Gename[2],room_log,false,0);
    G4VPhysicalVolume* leaf_phys3 = new G4PVPlacement(rotY90,G4ThreeVector(posX+geCasingLength+chOuterRadius,-leafY,-leafX),geLeaf_log3,Gename[3],room_log,false,0);
    G4VPhysicalVolume* leaf_phys4 = new G4PVPlacement(rotY90,G4ThreeVector(posX+geCasingLength+chOuterRadius,-leafY,leafX),geLeaf_log4,Gename[4],room_log,false,0);
	


////2nd clover construction, now fully operational. Takes parameters of 1st detector and swings them around to different angles.
 //clover casing
    G4VPhysicalVolume* Acasing_phys = new G4PVPlacement(rotY45,G4ThreeVector(sin(pi/4.0)*(posX+geCasingLength+chOuterRadius),posY,cos(pi/4.0)*-(posX+geCasingLength+chOuterRadius)),geCasing_log,"AgeCasing",room_log,false,0);
    
    for (int i=0;i<5;i++)
    {
        oss.str("");   //clear the ostringstream
        oss << i;  //stream int into ostringstream
        s=oss.str();   //copy ostringstream to G4string
        Gename[i]="Aleaf" + s;
    }
    Gename[4]="Abgo";
  

 // G4cout << "----------------------------- \n"<<bgoAl_log<<G4endl;
  

    //BGO shield
    G4VPhysicalVolume* AbgoAl_phys = new G4PVPlacement(rotY45,G4ThreeVector(sin(pi/4.0)*(chOuterRadius+2*bgoTrapLength+bgoShieldLength),posY,-(chOuterRadius+2*bgoTrapLength+bgoShieldLength)*cos(pi/4.0)),bgoAl_log,"AbgoShield",room_log,false,0);
   // G4VPhysicalVolume* bgo_phys = new G4PVPlacement(rotY90,G4ThreeVector(chOuterRadius+bgoLength,posY,posZ),bgo_log,Gename[5],room_log,false,0);
    G4VPhysicalVolume* Abgo_phys = new G4PVPlacement(rotY45,G4ThreeVector(sin(pi/4.0)*(chOuterRadius+bgoLength),posY,-(chOuterRadius+bgoLength)*cos(pi/4.0)),bgo_log,Gename[4],room_log,false,0);
    G4VPhysicalVolume* AbgoAlBack_phys = new G4PVPlacement(rotY45,G4ThreeVector(sin(pi/4.0)*(chOuterRadius+2*bgoShieldLength+3*bgoTrapLength),posY,-(chOuterRadius+2*bgoShieldLength+3*bgoTrapLength)*cos(pi/4.0)),bgoAlBack_log,"AbgoBack",room_log,false,0);
   

//  return room_phys;

 
    //clover
    G4VPhysicalVolume* Aleaf_phys1 = new G4PVPlacement(rotY45,G4ThreeVector(sin(pi/4.0)*(posX+geCasingLength+chOuterRadius+leafX),leafY,-cos(pi/4.0)*(posX+geCasingLength+chOuterRadius-leafX)),geLeaf_log1,Gename[1],room_log,false,0);
    G4VPhysicalVolume* Aleaf_phys2 = new G4PVPlacement(rotY45,G4ThreeVector(sin(pi/4.0)*(posX+geCasingLength+chOuterRadius-leafX),leafY,-cos(pi/4.0)*(posX+geCasingLength+chOuterRadius+leafX)),geLeaf_log2,Gename[2],room_log,false,0);
    G4VPhysicalVolume* Aleaf_phys3 = new G4PVPlacement(rotY45,G4ThreeVector(sin(pi/4.0)*(posX+geCasingLength+chOuterRadius-leafX),-leafY,-cos(pi/4.0)*(posX+geCasingLength+chOuterRadius+leafX)),geLeaf_log3,Gename[3],room_log,false,0);
    G4VPhysicalVolume* Aleaf_phys4 = new G4PVPlacement(rotY45,G4ThreeVector(sin(pi/4.0)*(posX+geCasingLength+chOuterRadius+leafX),-leafY,cos(pi/4.0)*-(posX+geCasingLength+chOuterRadius-leafX)),geLeaf_log4,Gename[4],room_log,false,0);


//3rd clover construction
 //clover casing
    G4VPhysicalVolume* Bcasing_phys = new G4PVPlacement(rotY270,G4ThreeVector(-1*posX+-1*geCasingLength+-1*chOuterRadius,posY,posZ),geCasing_log,"BgeCasing",room_log,false,0);
    
    for (int i=0;i<5;i++)
    {
        oss.str("");   //clear the ostringstream
        oss << i;  //stream int into ostringstream
        s=oss.str();   //copy ostringstream to G4string
        Gename[i]="Bleaf" + s;
    }
    Gename[4]="Bbgo";
  

 // G4cout << "----------------------------- \n"<<bgoAl_log<<G4endl;
  

    //BGO shield
    G4VPhysicalVolume* BbgoAl_phys = new G4PVPlacement(rotY270,G4ThreeVector(-1*chOuterRadius+-2*bgoTrapLength+-1*bgoShieldLength,posY,posZ),bgoAl_log,"BbgoShield",room_log,false,0);
  //  G4VPhysicalVolume* bgo_phys = new G4PVPlacement(rotY90,G4ThreeVector(chOuterRadius+bgoLength,posY,posZ),bgo_log,Gename[5],room_log,false,0);
    G4VPhysicalVolume* Bbgo_phys = new G4PVPlacement(rotY270,G4ThreeVector(-1*chOuterRadius+-1*bgoLength,posY,posZ),bgo_log,Gename[4],room_log,false,0);
    G4VPhysicalVolume* BbgoAlBack_phys = new G4PVPlacement(rotY270,G4ThreeVector(-1*chOuterRadius+-2*bgoShieldLength+-3*bgoTrapLength,posY,posZ),bgoAlBack_log,"BbgoBack",room_log,false,0);
   

//  return room_phys;

 
    //clover
    G4VPhysicalVolume* Bleaf_phys1 = new G4PVPlacement(rotY270,G4ThreeVector(-1*posX+-1*geCasingLength+-1*chOuterRadius,leafY,-leafX),geLeaf_log1,Gename[1],room_log,false,0);
    G4VPhysicalVolume* Bleaf_phys2 = new G4PVPlacement(rotY270,G4ThreeVector(-1*posX+-1*geCasingLength+-1*chOuterRadius,leafY,leafX),geLeaf_log2,Gename[2],room_log,false,0);
    G4VPhysicalVolume* Bleaf_phys3 = new G4PVPlacement(rotY270,G4ThreeVector(-1*posX+-1*geCasingLength+-1*chOuterRadius,-leafY,leafX),geLeaf_log3,Gename[3],room_log,false,0);
    G4VPhysicalVolume* Bleaf_phys4 = new G4PVPlacement(rotY270,G4ThreeVector(-1*posX+-1*geCasingLength+-1*chOuterRadius,-leafY,-leafX),geLeaf_log4,Gename[4],room_log,false,0);
	


//4th clover construction, now fully operational. Use basic idea on other detectors.
 //clover casing

    G4VPhysicalVolume* Ccasing_phys = new G4PVPlacement(rotY135,G4ThreeVector(sin(3*pi/4.0)*(posX+geCasingLength+chOuterRadius),posY,cos(3*pi/4.0)*-(posX+geCasingLength+chOuterRadius)),geCasing_log,"CgeCasing",room_log,false,0);
    
    for (int i=0;i<5;i++)
    {
        oss.str("");   //clear the ostringstream
        oss << i;  //stream int into ostringstream
        s=oss.str();   //copy ostringstream to G4string
        Gename[i]="Cleaf" + s;
    }
    Gename[4]="Cbgo";
  

 // G4cout << "----------------------------- \n"<<bgoAl_log<<G4endl;
  

    //BGO shield
    G4VPhysicalVolume* CbgoAl_phys = new G4PVPlacement(rotY135,G4ThreeVector(sin(3*pi/4.0)*(chOuterRadius+2*bgoTrapLength+bgoShieldLength),posY,-(chOuterRadius+2*bgoTrapLength+bgoShieldLength)*cos(3*pi/4.0)),bgoAl_log,"CbgoShield",room_log,false,0);
   // G4VPhysicalVolume* bgo_phys = new G4PVPlacement(rotY90,G4ThreeVector(chOuterRadius+bgoLength,posY,posZ),bgo_log,Gename[5],room_log,false,0);
    G4VPhysicalVolume* Cbgo_phys = new G4PVPlacement(rotY135,G4ThreeVector(sin(3*pi/4.0)*(chOuterRadius+bgoLength),posY,-(chOuterRadius+bgoLength)*cos(3*pi/4.0)),bgo_log,Gename[4],room_log,false,0);
    G4VPhysicalVolume* CbgoAlBack_phys = new G4PVPlacement(rotY135,G4ThreeVector(sin(3*pi/4.0)*(chOuterRadius+2*bgoShieldLength+3*bgoTrapLength),posY,-(chOuterRadius+2*bgoShieldLength+3*bgoTrapLength)*cos(3*pi/4.0)),bgoAlBack_log,"CbgoBack",room_log,false,0);
   

//  return room_phys;

 
    //clover
    G4VPhysicalVolume* Cleaf_phys1 = new G4PVPlacement(rotY135,G4ThreeVector(sin(3*pi/4.0)*(posX+geCasingLength+chOuterRadius-leafX),leafY,-cos(3*pi/4.0)*(posX+geCasingLength+chOuterRadius+leafX)),geLeaf_log1,Gename[1],room_log,false,0);
    G4VPhysicalVolume* Cleaf_phys2 = new G4PVPlacement(rotY135,G4ThreeVector(sin(3*pi/4.0)*(posX+geCasingLength+chOuterRadius+leafX),leafY,-cos(3*pi/4.0)*(posX+geCasingLength+chOuterRadius-leafX)),geLeaf_log2,Gename[2],room_log,false,0);
    G4VPhysicalVolume* Cleaf_phys3 = new G4PVPlacement(rotY135,G4ThreeVector(sin(3*pi/4.0)*(posX+geCasingLength+chOuterRadius+leafX),-leafY,-cos(3*pi/4.0)*(posX+geCasingLength+chOuterRadius-leafX)),geLeaf_log3,Gename[3],room_log,false,0);
    G4VPhysicalVolume* Cleaf_phys4 = new G4PVPlacement(rotY135,G4ThreeVector(sin(3*pi/4.0)*(posX+geCasingLength+chOuterRadius-leafX),-leafY,cos(3*pi/4.0)*-(posX+geCasingLength+chOuterRadius+leafX)),geLeaf_log4,Gename[4],room_log,false,0);


//5th clover construction, now fully operational. Use basic idea on other detectors.
 //clover casing
    G4VPhysicalVolume* Dcasing_phys = new G4PVPlacement(rotY225,G4ThreeVector(sin(5*pi/4.0)*(posX+geCasingLength+chOuterRadius),posY,cos(5*pi/4.0)*-(posX+geCasingLength+chOuterRadius)),geCasing_log,"DgeCasing",room_log,false,0);
    
    for (int i=0;i<5;i++)
    {
        oss.str("");   //clear the ostringstream
        oss << i;  //stream int into ostringstream
        s=oss.str();   //copy ostringstream to G4string
        Gename[i]="Dleaf" + s;
    }
    Gename[4]="Dbgo";
  

 // G4cout << "----------------------------- \n"<<bgoAl_log<<G4endl;
  

    //BGO shield
    G4VPhysicalVolume* DbgoAl_phys = new G4PVPlacement(rotY225,G4ThreeVector(sin(5*pi/4.0)*(chOuterRadius+2*bgoTrapLength+bgoShieldLength),posY,-(chOuterRadius+2*bgoTrapLength+bgoShieldLength)*cos(5*pi/4.0)),bgoAl_log,"AbgoShield",room_log,false,0);
   // G4VPhysicalVolume* bgo_phys = new G4PVPlacement(rotY90,G4ThreeVector(chOuterRadius+bgoLength,posY,posZ),bgo_log,Gename[5],room_log,false,0);
    G4VPhysicalVolume* Dbgo_phys = new G4PVPlacement(rotY225,G4ThreeVector(sin(5*pi/4.0)*(chOuterRadius+bgoLength),posY,-(chOuterRadius+bgoLength)*cos(5*pi/4.0)),bgo_log,Gename[4],room_log,false,0);
    G4VPhysicalVolume* DbgoAlBack_phys = new G4PVPlacement(rotY225,G4ThreeVector(sin(5*pi/4.0)*(chOuterRadius+2*bgoShieldLength+3*bgoTrapLength),posY,-(chOuterRadius+2*bgoShieldLength+3*bgoTrapLength)*cos(5*pi/4.0)),bgoAlBack_log,"AbgoBack",room_log,false,0);
   

//  return room_phys;

 
    //clover
    G4VPhysicalVolume* Dleaf_phys1 = new G4PVPlacement(rotY225,G4ThreeVector(sin(5*pi/4.0)*(posX+geCasingLength+chOuterRadius+leafX),leafY,-cos(5*pi/4.0)*(posX+geCasingLength+chOuterRadius-leafX)),geLeaf_log1,Gename[1],room_log,false,0);
    G4VPhysicalVolume* Dleaf_phys2 = new G4PVPlacement(rotY225,G4ThreeVector(sin(5*pi/4.0)*(posX+geCasingLength+chOuterRadius-leafX),leafY,-cos(5*pi/4.0)*(posX+geCasingLength+chOuterRadius+leafX)),geLeaf_log2,Gename[2],room_log,false,0);
    G4VPhysicalVolume* Dleaf_phys3 = new G4PVPlacement(rotY225,G4ThreeVector(sin(5*pi/4.0)*(posX+geCasingLength+chOuterRadius-leafX),-leafY,-cos(5*pi/4.0)*(posX+geCasingLength+chOuterRadius+leafX)),geLeaf_log3,Gename[3],room_log,false,0);
    G4VPhysicalVolume* Dleaf_phys4 = new G4PVPlacement(rotY225,G4ThreeVector(sin(5*pi/4.0)*(posX+geCasingLength+chOuterRadius+leafX),-leafY,cos(5*pi/4.0)*-(posX+geCasingLength+chOuterRadius-leafX)),geLeaf_log4,Gename[4],room_log,false,0);


//6th clover construction, now fully operational. Use basic idea on other detectors.
 //clover casing
    G4VPhysicalVolume* Ecasing_phys = new G4PVPlacement(rotY315,G4ThreeVector(sin(7*pi/4.0)*(posX+geCasingLength+chOuterRadius),posY,cos(7*pi/4.0)*-(posX+geCasingLength+chOuterRadius)),geCasing_log,"EgeCasing",room_log,false,0);
    
    for (int i=0;i<5;i++)
    {
        oss.str("");   //clear the ostringstream
        oss << i;  //stream int into ostringstream
        s=oss.str();   //copy ostringstream to G4string
        Gename[i]="Eleaf" + s;
    }
    Gename[4]="Ebgo";
  

 // G4cout << "----------------------------- \n"<<bgoAl_log<<G4endl;
  

    //BGO shield
    G4VPhysicalVolume* EbgoAl_phys = new G4PVPlacement(rotY315,G4ThreeVector(sin(7*pi/4.0)*(chOuterRadius+2*bgoTrapLength+bgoShieldLength),posY,-(chOuterRadius+2*bgoTrapLength+bgoShieldLength)*cos(7*pi/4.0)),bgoAl_log,"EbgoShield",room_log,false,0);
   // G4VPhysicalVolume* bgo_phys = new G4PVPlacement(rotY90,G4ThreeVector(chOuterRadius+bgoLength,posY,posZ),bgo_log,Gename[5],room_log,false,0);
    G4VPhysicalVolume* Ebgo_phys = new G4PVPlacement(rotY315,G4ThreeVector(sin(7*pi/4.0)*(chOuterRadius+bgoLength),posY,-(chOuterRadius+bgoLength)*cos(7*pi/4.0)),bgo_log,Gename[4],room_log,false,0);
    G4VPhysicalVolume* EbgoAlBack_phys = new G4PVPlacement(rotY315,G4ThreeVector(sin(7*pi/4.0)*(chOuterRadius+2*bgoShieldLength+3*bgoTrapLength),posY,-(chOuterRadius+2*bgoShieldLength+3*bgoTrapLength)*cos(7*pi/4.0)),bgoAlBack_log,"EbgoBack",room_log,false,0);
   

//  return room_phys;

 
    //clover
    G4VPhysicalVolume* Eleaf_phys1 = new G4PVPlacement(rotY315,G4ThreeVector(sin(7*pi/4.0)*(posX+geCasingLength+chOuterRadius-leafX),leafY,-cos(7*pi/4.0)*(posX+geCasingLength+chOuterRadius+leafX)),geLeaf_log1,Gename[1],room_log,false,0);
    G4VPhysicalVolume* Eleaf_phys2 = new G4PVPlacement(rotY315,G4ThreeVector(sin(7*pi/4.0)*(posX+geCasingLength+chOuterRadius+leafX),leafY,-cos(7*pi/4.0)*(posX+geCasingLength+chOuterRadius-leafX)),geLeaf_log2,Gename[2],room_log,false,0);
    G4VPhysicalVolume* Eleaf_phys3 = new G4PVPlacement(rotY315,G4ThreeVector(sin(7*pi/4.0)*(posX+geCasingLength+chOuterRadius+leafX),-leafY,-cos(7*pi/4.0)*(posX+geCasingLength+chOuterRadius-leafX)),geLeaf_log3,Gename[3],room_log,false,0);
    G4VPhysicalVolume* Eleaf_phys4 = new G4PVPlacement(rotY315,G4ThreeVector(sin(7*pi/4.0)*(posX+geCasingLength+chOuterRadius-leafX),-leafY,cos(7*pi/4.0)*-(posX+geCasingLength+chOuterRadius+leafX)),geLeaf_log4,Gename[4],room_log,false,0);


//========================== Visualization attributes =========================================//

  room_log->SetVisAttributes (G4VisAttributes::Invisible);

//visualization for scintillators = GREEN
  G4VisAttributes *GreenAttr = new G4VisAttributes(G4Colour(0.,1.,0.));     
  GreenAttr->SetVisibility(true);
  GreenAttr->SetForceSolid(true);

//visualization for reflector = PURPLE
  G4VisAttributes *PurpleAttr = new G4VisAttributes(G4Colour(1.,0.,1.));  
  PurpleAttr->SetVisibility(true);
  PurpleAttr->SetForceSolid(true);

//visualization for aluminum = GREY
  G4VisAttributes *GreyAttr = new G4VisAttributes(G4Colour(0.5,0.5,0.5,.1));
  GreyAttr->SetVisibility(true);
  GreyAttr->SetForceSolid(true);

//visualization for BLUE
  G4VisAttributes *BlueAttr = new G4VisAttributes(G4Colour(0.,0.,1.));
  BlueAttr->SetVisibility(true);
  BlueAttr->SetForceSolid(true);

//visualization for RED
  G4VisAttributes *RedAttr = new G4VisAttributes(G4Colour(1.,0.,0.));
  RedAttr->SetVisibility(true);
  RedAttr->SetForceSolid(true);
    
//visualization for WHITE
    G4VisAttributes *WhiteAttr = new G4VisAttributes(G4Colour(1.,1.,1.));
    WhiteAttr->SetVisibility(true);
    WhiteAttr->SetForceSolid(true);

//visualization for YELLOW
	G4VisAttributes *YellowAttr= new G4VisAttributes(G4Colour(1.,1.,0.));
	YellowAttr->SetVisibility(true);
	YellowAttr->SetForceSolid(true);


// applying the color scheme

	////DeltaE info commented out.
/*
    for(int i=0;i<12;i++){
		DE_log[2*i]->SetVisAttributes(GreenAttr);
		DE_log[2*i+1]->SetVisAttributes(RedAttr);
    }
   */
    for(int i=0;i<12;i++){
		E1_log[2*i]->SetVisAttributes(BlueAttr);
		E1_log[2*i+1]->SetVisAttributes(PurpleAttr);
    }
    
    //dead layers
    //dShield_log->SetVisAttributes(WhiteAttr);
    //dShield2_log->SetVisAttributes(WhiteAttr);
    alLayer_log->SetVisAttributes(WhiteAttr);
    auLayer_log->SetVisAttributes(WhiteAttr);
    
    // color the target
    target_log->SetVisAttributes(YellowAttr);
    
    bgoAl_log->SetVisAttributes(GreenAttr);
    geLeaf_log1->SetVisAttributes(PurpleAttr);
    geLeaf_log2->SetVisAttributes(PurpleAttr);
    geLeaf_log3->SetVisAttributes(PurpleAttr);
    geLeaf_log4->SetVisAttributes(PurpleAttr);
    geCasing_log->SetVisAttributes(GreyAttr);
    alChamber_log->SetVisAttributes(GreyAttr);
    bgo_log->SetVisAttributes(WhiteAttr);
    bgoAlBack_log ->SetVisAttributes(WhiteAttr);
	
    
    
  return room_phys;
}



