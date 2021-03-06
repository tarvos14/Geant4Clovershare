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
// $Id: tbjcPhysicsList.hh 68058 2013-03-13 14:47:43Z gcosmo $
//
/// \file tbjcPhysicsList.hh
/// \brief Definition of the tbjcPhysicsList class

#ifndef tbjcPhysicsList_h
#define tbjcPhysicsList_h 1

#include "G4VModularPhysicsList.hh"

class PhysisListMessenger;
class G4VPhysicsConstructor;

/// Modular physics list
///
/// It includes the folowing physics builders
/// - G4DecayPhysics
/// - G4RadioactiveDecayPhysics
/// - G4EmStandardPhysics

class tbjcPhysicsList: public G4VModularPhysicsList
{
public:
  tbjcPhysicsList();
  virtual ~tbjcPhysicsList();
    virtual void ConstructParticle();
    virtual void ConstructProcess();
    
    void AddPhysicsList(const G4String& name);
  virtual void SetCuts();
  G4VPhysicsConstructor* fEmPhysicsList;
  G4String               fEmName;
  PhysisListMessenger*   fMessenger;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

