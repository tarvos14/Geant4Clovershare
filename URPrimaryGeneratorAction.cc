//    
//

#include "URPrimaryGeneratorAction.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh" 
#include "G4ParticleDefinition.hh"      

#include "G4UImanager.hh"

#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4IonTable.hh"

#include "globals.hh"
#include "Randomize.hh"
#include <ctime>
#include "time.h"


#include <iostream>
#include <fstream>
#include <iomanip>

//Extra libraries introducted to try to sort out the randomness issue.
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include "TRandom.h"

G4IonTable* BeTable=G4IonTable::GetIonTable();


URPrimaryGeneratorAction::URPrimaryGeneratorAction()

{
G4int n_particle=1;
  // Setting the default particle
  G4ParticleTable *pTable = G4ParticleTable::GetParticleTable();
//G4ParticleTable *ionTable=G4ParticleDefinition::GetIon();
  

//G4ParticleTable class with method G4ParticleDefinition* GetIon
  G4String pName;
//Need to change this to 7Be for our experiment, might have to construct it.
 // G4ParticleDefinition *particle = pTable->FindParticle(pName="deuteron");
 // G4ParticleDefinition *particle = pTable->FindParticle(pName="alpha");
   
 //G4ParticleDefinition *particle= GetIon(atomicNum,massNum,ExcEnergy);
  particleGun = new G4ParticleGun(n_particle);

	////For 7Be experiment, energies are set in Generate Primaries. For standard particles, set energies and such here.
	//34 MeV is the nominal energy for the 6Li beam. Is only 30.45 MeV by the time it reaches Twinsol
	//7Li energy has an energy of 28 MeV
 	//G4int part_ene = 28*MeV; //particle energy in MeV


    

}


URPrimaryGeneratorAction::~URPrimaryGeneratorAction()
{
  delete particleGun;
}

void URPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)    
{



gRandom->SetSeed(0);

////Various attempts to seed Geant libraries and get them to act properly. Still in development.
//time_t clock;
//G4long seed=time(&clock);
//CLHEP::HepRandom::setTheSeed(seed,4);
//G4Random::setTheSeed(seed,3);
//int startSeed= G4Random::getTheSeed();
//G4cout<<seed<< " "<<startSeed<<G4endl;


//Setting parameters for G4ParticleDefinition below
G4int atomicNum=4;
G4int massNum=7;
G4int part_ene = 30.45*MeV; //particle energy in MeV
//.429 MeV for 7Be, .479 MeV for 7Li
G4double ExcEnergy=.479*MeV;


	//Creating our 7Be/7Li particles
   G4ParticleDefinition *particle=BeTable->GetIon(atomicNum,massNum,ExcEnergy);
  particleGun->SetParticleDefinition(particle);



 G4double px, py, pz, costheta, sintheta, phi;       // CoM frame
 G4double x, y, z;
 G4double xAve=0;
 G4double yAve=0;

	// zero the momentum unit vector
    px = 0.0;
    py = 0.0;
    pz = 1.0;
	
   	//// Sets the original postion of the particle. Usually at origin, unless your beam is actually awful.
	//.23 mm for X and Y seems to reproduce the conditions of the Li peak decently well.
	//we had a 9.6 mm radius iris which constrains the beam size
	// 2 mm and 1 mm is too much for Li, apparantly.
	//Accidently was using Uniform, now doing all in Gaussian
	//0 try .80 pos and 3.0 sig for fitting Li.
	//1 try 0.0 pos and 4.8 sig for fitting Be.
	//2 try point Gaussian beam.
	//3 try 0.0 pos and 5 mm for fitting Be.
	//5 0 pos 4.75 sig for fitting Be
	//6 Be, same pos, but .05 less sig than previous
	//iterate
	//From 16 onward, it starts at 4.20 (lol) and goes in steps of .1, 20 was skipped for some reason
	//23 is at 3.50 mm
	//24 starts with a 0 mm thick beam and goes in .10 interval up from there.
	//Ends at 31 with a width of .6 mm?
	//32 Starts with a width of 2.00 mm and goes to 3.00 mm at 42 
	//43_0-9 are 1/10 of a 0 center 3 mm wide 7Be beam.
	//44 is a uniform beam of 4 mm,
	G4double beamPosX = 0.0*mm;
	G4double beamPosY = 0.0*mm;
	G4double xUncert = 5.00*mm;
	G4double yUncert = 5.00*mm;

	
	


	//Here is an ideal, point like beam.
	//x=beamPosX;
	//y=beamPosY;


	//Here is a spread out but uniform beam.
	x=beamPosX+ xUncert*(0.5-G4UniformRand());
	y=beamPosY+ yUncert*(0.5-G4UniformRand());


	
		
	//Here is the Gaussian profile of the beam.

	//This is producing many values with Geant Libraries, but they are the same for all runs.
	
	//x = G4RandGauss::shoot(beamPosX,xUncert); //(beamPosX + 2*xUncert*G4UniformRand()-xUncert); //G4RandGauss, G4UniformRand
	//y = G4RandGauss::shoot(beamPosY,yUncert); //(beamPosY + 2*yUncert*G4UniformRand()-yUncert);

	//This appears to be creating fairly random values, but Geant might be reacting strangely to them. Still investigating.
	//x= gRandom->Gaus(beamPosX,xUncert)*mm;
	//y=gRandom->Gaus(beamPosY,yUncert)*mm;

	//Setting the beam behind the target.
        z = -10.0*mm;
    
   ////Angles and such if you are creating a uniformaly expanding beam, instead of scattering off a target.
   // xAve+=x;
   // costheta=0.5*G4UniformRand()+0.5;                           // generates a random value for costheta between 1 and 0.5
//	sintheta=std::sqrt(1.0-costheta*costheta);        // sin^2 + cos^2 = 1
  //  phi=twopi* G4UniformRand();                                 // generates a random angle between 0 and 2pi
    //px=sintheta * std::cos(phi);                      // calculates x = sintheta * cosphi
   // py=sintheta * std::sin(phi);                      // calculates y = sintheta * sinphi
   // pz=costheta;                                           // calculates z = costheta



   // Sets the particles momentum direction
	particleGun->SetParticleMomentumDirection(G4ThreeVector(px, py, pz));

   // Sets particle intial coordinates
	particleGun->SetParticlePosition(G4ThreeVector(x, y, z));
 
  // Sets particle energy
  //Currently runs at beam energy with a few 100 keV spread. Still seems to be losing some energy somewhere in set-up though. Change 
	//Energy spread. Almost zero for Li, at least 500 keV for the Be.
    G4double part_unc=0.5*MeV;
    G4double part_energy = gRandom->Gaus(part_ene,part_unc);

     particleGun->SetParticleEnergy(part_energy);
   // G4cout<<x<<" "<< y<< " "<< part_energy<< " "<< costheta<<G4endl;
  // To generate an event
     particleGun->GeneratePrimaryVertex(anEvent);

   

}













