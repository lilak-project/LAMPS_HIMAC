#include "ATTPCDetectorConstruction.h"
#include "ATTPCRectanglePad.h"
#include "ATTPCHoneyCombPad.h"
#include "ATTPC20RectanglePad.h"
#include "ATTPCStripPad.h"

#include "LKG4RunManager.h"
#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4FieldManager.hh"
#include "G4UniformMagField.hh"
#include "G4GlobalMagFieldMessenger.hh"
#include "G4VisAttributes.hh"
#include "G4UserLimits.hh"

ATTPCDetectorConstruction::ATTPCDetectorConstruction()
: G4VUserDetectorConstruction()
{
}

ATTPCDetectorConstruction::~ATTPCDetectorConstruction()
{
}

G4VPhysicalVolume* ATTPCDetectorConstruction::Construct()
{  
  auto runManager = (LKG4RunManager *) G4RunManager::GetRunManager();

  auto par = runManager -> GetParameterContainer();

  G4NistManager* nist = G4NistManager::Instance();

  G4double worldX = par -> GetParDouble("worldX");
  G4double worldY = par -> GetParDouble("worldY");
  G4double worldZ = par -> GetParDouble("worldZ");

  G4double tpcX = par -> GetParDouble("tpcX");
  G4double tpcY = par -> GetParDouble("tpcY");
  G4double tpcZ = par -> GetParDouble("tpcZ");

  G4double bfieldX = par -> GetParDouble("bfieldX");
  G4double bfieldY = par -> GetParDouble("bfieldY");
  G4double bfieldZ = par -> GetParDouble("bfieldZ");

  auto PadPlaneType = par -> GetParString("PadPlaneType");
  G4double PadWidth;  
  G4double PadHeight;  
  G4double PadGap;  

  if(PadPlaneType == "RectanglePad"){
    ATTPCRectanglePad *fPadPlane = new ATTPCRectanglePad(); 
    PadWidth = fPadPlane ->GetPadWidth();
    PadHeight = fPadPlane ->GetPadHeight();
    PadGap = fPadPlane ->GetPadGap();
  }
  else if(PadPlaneType == "HoneyCombPad"){
    ATTPCHoneyCombPad *fPadPlane = new ATTPCHoneyCombPad(); 
    PadWidth = fPadPlane ->GetPadWidth();
    PadHeight = fPadPlane ->GetPadHeight();
    PadGap = fPadPlane ->GetPadGap();  
  }
  if(PadPlaneType == "20RectanglePad"){
    ATTPC20RectanglePad *fPadPlane = new ATTPC20RectanglePad(); 
    PadWidth = fPadPlane ->GetPadWidth();
    PadHeight = fPadPlane ->GetPadHeight();
    PadGap = fPadPlane ->GetPadGap();
  }
  if(PadPlaneType == "StripPad"){
    ATTPCStripPad *fPadPlane = new ATTPCStripPad(); 
    PadWidth = fPadPlane ->GetPadWidth() +50.;
    PadHeight = fPadPlane ->GetPadHeight();
    PadGap = fPadPlane ->GetPadGap();
  }

  G4double Temperature = par -> GetParDouble("temperature");
  G4double STPTemperature = 273.15;
  G4double labTemperature = (STPTemperature + Temperature)*kelvin;
  G4double Pressure = par -> GetParDouble("pressure");
  TString detMatName = par -> GetParString("detMatName");
  G4double iC4H10Ratio = par -> GetParDouble("iC4H10Ratio");

  //-----------------------gas definition----------------------------------
  
  G4double densityArGas = 0.001782 *g/cm3*STPTemperature/labTemperature;
  G4double densityMethane = 0.000717 *g/cm3*STPTemperature/labTemperature;
  G4double densityHeGas = 0.000179 *g/cm3*STPTemperature/labTemperature;
  G4double densityIsobutaneGas = 0.0026756 *g/cm3*STPTemperature/labTemperature;
  G4double densityVacuum = 1e-15*g/cm3;
 
  G4Element *element_C  = new G4Element("Carbon",   "C",  6,  12.011*g/mole);
  G4Element *element_H  = new G4Element("Hydrogen", "H",  1,  1.00794*g/mole);
  
  G4Material *ArGas = new G4Material("ArgonGas", 18, 39.948*g/mole, densityArGas, kStateGas, labTemperature);
  G4Material* methane = nist -> FindOrBuildMaterial("G4_METHANE");
  G4Material *MethaneGas = new G4Material("MethaneGas", densityMethane, methane, kStateGas, labTemperature);
  G4Material* HeGas = nist -> FindOrBuildMaterial("G4_He");
  G4Material *IsobutaneGas = new G4Material("iC4H10", densityIsobutaneGas, 2, kStateGas, labTemperature);
  IsobutaneGas -> AddElement(element_C, 4);
  IsobutaneGas -> AddElement(element_H, 10);

  G4Material *matGas = nullptr;
  
  if (detMatName == "p10"){
    G4double densityGas = (.9*densityArGas + .1*densityMethane)*Pressure/760.;
    matGas = new G4Material("matP10", densityGas, 2, kStateGas, labTemperature);
    matGas -> AddMaterial(ArGas, 90 *perCent);
    matGas -> AddMaterial(MethaneGas, 10 *perCent);
  }

  else if(detMatName == "4He"){
    G4double densityGas = densityHeGas*Pressure/760.;
    matGas = new G4Material("mat4He", densityGas, 1, kStateGas, labTemperature);
    matGas -> AddMaterial(HeGas, 100 *perCent);
  }

  else if(detMatName == "iC4H10"){
    G4double densityGas = densityIsobutaneGas*Pressure/760.;
    matGas = new G4Material("matiC4H10", densityGas, 1, kStateGas, labTemperature);
    matGas -> AddMaterial(IsobutaneGas, 100 *perCent);
  }

  else if(detMatName == "4He_iC4H10"){
    G4double densityGas = ((densityIsobutaneGas*iC4H10Ratio*0.01)+(densityHeGas*(100.-iC4H10Ratio)*0.01))*Pressure/760.;
    matGas = new G4Material("matHe4_iC4H10", densityGas, 2, kStateGas, labTemperature);
    matGas -> AddMaterial(IsobutaneGas, iC4H10Ratio *perCent);
    matGas -> AddMaterial(HeGas, (100.-iC4H10Ratio) *perCent);
  }

  G4Material *Vacuum = new G4Material("Vacuum", densityVacuum, 1, kStateGas, labTemperature);
  Vacuum -> AddMaterial(HeGas, 100 *perCent);
  //-----------------------------------------------------------------



  //-----------------------World definition -------------------------
  
  G4Box *solidWorld = new G4Box("World", worldX *mm, worldY *mm, worldZ *mm);
  G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld, Vacuum, "World");
  G4PVPlacement *physWorld = new G4PVPlacement(0, G4ThreeVector(0,0,0), logicWorld, "World", 0, false, 0, true);
  
  //-----------------------------------------------------------------



  //----------------------Drift chamber definition----------------------

  G4Box* solidTPC = new G4Box("TPC", tpcX *mm, tpcY *mm, tpcZ *mm);
  G4LogicalVolume* logicTPC = new G4LogicalVolume(solidTPC, matGas, "Detector");
  {
    G4VisAttributes* attTPC = new G4VisAttributes(G4Colour(G4Colour::Gray()));
    attTPC -> SetForceWireframe(true);
    logicTPC -> SetVisAttributes(attTPC);
  }
  logicTPC -> SetUserLimits(new G4UserLimits(1*mm, 1*mm));    
  auto pvp = new G4PVPlacement(0, G4ThreeVector(0,0,0), logicTPC, "TPC", logicWorld, false, 1, true);
  //auto pvp = new G4PVPlacement(0, G4ThreeVector(tpcX-PadWidth/2-PadGap/2,tpcY-PadHeight/2-PadGap/2,tpcZ), logicTPC, "TPC", logicWorld, false, 1, true);
  runManager -> SetSensitiveDetector(pvp);

  //------------------------------------------------------------------

  
  new G4GlobalMagFieldMessenger(G4ThreeVector(bfieldX*tesla, bfieldY*tesla, bfieldZ*tesla));

  G4Region* region = new G4Region("regionTPC");
  region->AddRootLogicalVolume(logicTPC);

  runManager->SetSensitiveDetector(pvp);

  return physWorld;
}
