#ifndef ATTPCMODULARPHYSICSLIST_HH
#define ATTPCMODULARPHYSICSLIST_HH

#include "G4VModularPhysicsList.hh"
#include "QGSP_BERT_HP.hh"

#include "G4Region.hh"
#include "G4RegionStore.hh"
#include "G4ProductionCutsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"
#include "globals.hh"

class ATTPCPhysicsList : public G4VModularPhysicsList
{
  public:
    ATTPCPhysicsList();
    virtual~ATTPCPhysicsList();
    virtual void SetCuts();
    virtual void ConstructParticle();
    virtual void ConstructProcess();
  
  protected:
    void AddProcessParameter();
    void AddLimiterProcess();
  

};

#endif
