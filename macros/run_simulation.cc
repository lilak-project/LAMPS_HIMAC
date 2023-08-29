#include "LKLogger.h"
#include "LKG4RunManager.h"
#include "LKParameterContainer.h"
#include "LKPrimaryGeneratorAction.h"
#include "LKEventAction.h"
#include "LKTrackingAction.h"
#include "LKSteppingAction.h"

#include "QGSP_BERT.hh"
#include "G4StepLimiterPhysics.hh"
#include "ATTPCDetectorConstruction.h"

int main(int argc, char** argv)
{
    lk_logger("data/log");

    auto runManager = new LKG4RunManager();

    G4VModularPhysicsList* physicsList = new QGSP_BERT;
    physicsList -> RegisterPhysics(new G4StepLimiterPhysics());
    runManager -> SetUserInitialization(physicsList);
    runManager -> AddParameterContainer(argv[1]);
    runManager -> GetPar() -> Print();
    runManager -> SetUserInitialization(new ATTPCDetectorConstruction());
    runManager -> Initialize();
    runManager -> Run(argc, argv);

    delete runManager;

    return 0;
}
