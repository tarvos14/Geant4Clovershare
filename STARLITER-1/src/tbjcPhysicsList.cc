//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// $Id: tbjcPhysicsList.cc 68058 2013-03-13 14:47:43Z gcosmo $
//
/// \file tbjcPhysicsList.cc
/// \brief Implementation of the tbjcPhysicsList class

#include "tbjcPhysicsList.hh"

#include "G4DecayPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"
#include "G4EmStandardPhysics.hh"
#include "tbjcPhysListEmStandard.hh"

#include "G4LossTableManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

#include "G4EmStandardPhysics_option1.hh"
#include "G4EmStandardPhysics_option2.hh"
#include "G4EmStandardPhysics_option3.hh"
#include "G4EmStandardPhysics_option4.hh"


#include "G4EmPenelopePhysics.hh"

#include "G4EmLivermorePhysics.hh"




//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


tbjcPhysicsList::tbjcPhysicsList() 
    : G4VModularPhysicsList(),fEmPhysicsList(0),fEmName("local"),fMessenger(0)
{    
    G4LossTableManager::Instance();


  //  fMessenger = new PhysicsListMessenger(this);

    SetVerboseLevel(1);

    // EM physics
    fEmName = G4String("local");
    fEmPhysicsList = new tbjcPhysListEmStandard(fEmName);

    //add new units for cross sections
    // 
    new G4UnitDefinition( "mm2/g", "mm2/g","Surface/Mass", mm2/g);
    new G4UnitDefinition( "um2/mg", "um2/mg","Surface/Mass", um*um/mg);
    new G4UnitDefinition("GeV","GeV","Energy",1000*MeV);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// Bosons
#include "G4ChargedGeantino.hh"
#include "G4Geantino.hh"
#include "G4Gamma.hh"
#include "G4OpticalPhoton.hh"

// leptons
#include "G4MuonPlus.hh"
#include "G4MuonMinus.hh"
#include "G4NeutrinoMu.hh"
#include "G4AntiNeutrinoMu.hh"

#include "G4Electron.hh"
#include "G4Positron.hh"
#include "G4NeutrinoE.hh"
#include "G4AntiNeutrinoE.hh"

// Mesons
#include "G4PionPlus.hh"
#include "G4PionMinus.hh"
#include "G4PionZero.hh"
#include "G4Eta.hh"
#include "G4EtaPrime.hh"

#include "G4KaonPlus.hh"
#include "G4KaonMinus.hh"
#include "G4KaonZero.hh"
#include "G4AntiKaonZero.hh"
#include "G4KaonZeroLong.hh"
#include "G4KaonZeroShort.hh"

// Baryons
#include "G4Proton.hh"
#include "G4AntiProton.hh"
#include "G4Neutron.hh"
#include "G4AntiNeutron.hh"

// Nuclei
#include "G4Deuteron.hh"
#include "G4Triton.hh"
#include "G4Alpha.hh"
#include "G4GenericIon.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void tbjcPhysicsList::ConstructParticle()
{
    // pseudo-particles
    G4Geantino::GeantinoDefinition();
    G4ChargedGeantino::ChargedGeantinoDefinition();

    // gamma
    G4Gamma::GammaDefinition();

    // optical photon
    G4OpticalPhoton::OpticalPhotonDefinition();

    // leptons
    G4Electron::ElectronDefinition();
    G4Positron::PositronDefinition();
    G4MuonPlus::MuonPlusDefinition();
    G4MuonMinus::MuonMinusDefinition();

    G4NeutrinoE::NeutrinoEDefinition();
    G4AntiNeutrinoE::AntiNeutrinoEDefinition();
    G4NeutrinoMu::NeutrinoMuDefinition();
    G4AntiNeutrinoMu::AntiNeutrinoMuDefinition();  

    // mesons
    G4PionPlus::PionPlusDefinition();
    G4PionMinus::PionMinusDefinition();
    G4PionZero::PionZeroDefinition();
    G4Eta::EtaDefinition();
    G4EtaPrime::EtaPrimeDefinition();
    G4KaonPlus::KaonPlusDefinition();
    G4KaonMinus::KaonMinusDefinition();
    G4KaonZero::KaonZeroDefinition();
    G4AntiKaonZero::AntiKaonZeroDefinition();
    G4KaonZeroLong::KaonZeroLongDefinition();
    G4KaonZeroShort::KaonZeroShortDefinition();

    // barions
    G4Proton::ProtonDefinition();
    G4AntiProton::AntiProtonDefinition();
    G4Neutron::NeutronDefinition();
    G4AntiNeutron::AntiNeutronDefinition();

    // ions
    G4Deuteron::DeuteronDefinition();
    G4Triton::TritonDefinition();
    G4Alpha::AlphaDefinition();
    G4GenericIon::GenericIonDefinition();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4EmProcessOptions.hh"

void tbjcPhysicsList::ConstructProcess()
{
    // Transportation
    //
    AddTransportation();

    // Electromagnetic physics list
    //
    fEmPhysicsList->ConstructProcess();

    // Em options
    //
    // Main options and setting parameters are shown here.
    // Several of them have default values.
    //
    G4EmProcessOptions emOptions;

    //physics tables
    //
    //emOptions.SetMinEnergy(100*eV);        //default    
    //emOptions.SetMaxEnergy(100*TeV);        //default  
    //emOptions.SetDEDXBinning(12*20);        //default=12*7  
    //emOptions.SetLambdaBinning(12*20);        //default=12*7

    emOptions.SetBuildCSDARange(true);     
    emOptions.SetMaxEnergyForCSDARange(10*GeV);
    //emOptions.SetDEDXBinningForCSDARange(12*20);

    //emOptions.SetSplineFlag(true);        //default

    emOptions.SetVerbose(0);  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void tbjcPhysicsList::AddPhysicsList(const G4String& name)
{
    if (verboseLevel>0) {
        G4cout << "PhysicsList::AddPhysicsList: <" << name << ">" << G4endl;
    }

    if (name == fEmName) return;

    if (name == "local") {

        fEmName = name;
        delete fEmPhysicsList;
        fEmPhysicsList = new tbjcPhysListEmStandard(name);

    } else if (name == "emstandard_opt0"){
        fEmName = name;
        delete fEmPhysicsList;
        fEmPhysicsList = new G4EmStandardPhysics();

    } else if (name == "emstandard_opt1"){
        fEmName = name;
        delete fEmPhysicsList;
        fEmPhysicsList = new G4EmStandardPhysics_option1();

    } else if (name == "emstandard_opt2"){
        fEmName = name;
        delete fEmPhysicsList;
        fEmPhysicsList = new G4EmStandardPhysics_option2();

    } else if (name == "emstandard_opt3"){
        fEmName = name;
        delete fEmPhysicsList;
        fEmPhysicsList = new G4EmStandardPhysics_option3();

    } else if (name == "emstandard_opt4"){
        fEmName = name;
        delete fEmPhysicsList;
        fEmPhysicsList = new G4EmStandardPhysics_option4();

    } else if (name == "empenelope"){
        fEmName = name;
        delete fEmPhysicsList;
        fEmPhysicsList = new G4EmPenelopePhysics();

    } else if (name == "emlivermore"){
        fEmName = name;
        delete fEmPhysicsList;
        fEmPhysicsList = new G4EmLivermorePhysics();

    } else {

        G4cout << "PhysicsList::AddPhysicsList: <" << name << ">"
            << " is not defined"
            << G4endl;
    }
}








//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

tbjcPhysicsList::~tbjcPhysicsList()
{ 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void tbjcPhysicsList::SetCuts()
{
    G4VUserPhysicsList::SetCuts();
}  
