#include "DetectorConstruction.h"
#include "LKG4RunManager.h"

#include "G4MaterialPropertiesTable.hh"
#include "G4MaterialPropertyVector.hh"

#include "G4RunManager.hh"
#include "G4TransportationManager.hh"
#include "G4SubtractionSolid.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Element.hh"
#include "G4Material.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4GlobalMagFieldMessenger.hh"
#include "G4SDManager.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"
#include "G4RotationMatrix.hh"

#include "G4DataInterpolation.hh"

#include "G4LogicalBorderSurface.hh"
#include "G4OpticalSurface.hh"

using namespace CLHEP;

DetectorConstruction::DetectorConstruction()
  : G4VUserDetectorConstruction()
{
}

DetectorConstruction::~DetectorConstruction()
{
}

G4VPhysicalVolume* DetectorConstruction::Construct()
{  
  auto runManager = (LKG4RunManager*) G4RunManager::GetRunManager();
  //Elements
  // G4Elements( name, symbol, Z number, A )
  G4Element* elH = new G4Element("H","H", 1., 1.00794*g/mole);
  G4Element* elHe = new G4Element("He", "He", 2., 4.002602*g/mole);
  G4Element* elC = new G4Element("C", "C", 6., 12.011*g/mole);
  G4Element* elN = new G4Element("N", "N", 7., 14.00674*g/mole);
  G4Element* elO = new G4Element("O", "O", 8., 15.9994*g/mole);
  G4Element* elSi= new G4Element("Si","Si", 14., 28.0855*g/mole);
  G4Element* elCs= new G4Element("Cs","Cs", 55., 132.9055*g/mole);
  G4Element* elI= new G4Element("I","I", 53., 126.9045*g/mole);


  G4double density = universe_mean_density;
  // G4Material( name, density, # of consisting atom, state )
  G4Material* Vacuum = new G4Material("Vacuum", density, 2);
  Vacuum->AddElement(elN, .7);
  Vacuum->AddElement(elO, .3);
  //N(70%), O(30%)
  
  // World
  G4double world_size = 5*m;

  G4Box* solidWorld = new G4Box("World", 0.5*world_size, 0.5*world_size, 0.5*world_size);
  G4LogicalVolume* logicWorld = new G4LogicalVolume(solidWorld, Vacuum, "World");
  G4VPhysicalVolume* physWorld = new G4PVPlacement(0, G4ThreeVector(0,0,0), logicWorld, "World", 0, false, -1, true);
//(회전,두는 위치,무엇을,이름,?,?,?,?)
//G4Box -> logicWorld와 physWorld를 모두 활용하여 선언

  //Water Material
  G4Material* He4_gas = new G4Material("He4_gas",0.00017*g/cm3,1);
  He4_gas->AddElement(elHe,1);
  //Water->AddElement(elO,1);

  //Water Detector
  G4double box1_size=126.0*cm;

  G4Box* GasBox1 = new G4Box("GasBox1", 0.12*box1_size, 0.12*box1_size, 0.5*box1_size);
  G4LogicalVolume* logicGasBox1 = new G4LogicalVolume(GasBox1, He4_gas, "GasBox1");
  G4VPhysicalVolume* physGasBox1 = new G4PVPlacement(0, G4ThreeVector(0, 0, 62.5*cm), logicGasBox1, "GasBox1", logicWorld, false, 1, true);

  G4Material* C2H4 = new G4Material("C2H4",1.18e-3*g/cm3,2);
  C2H4->AddElement(elC,.33);
  C2H4->AddElement(elH,.67);

  G4Material* Si14_solid = new G4Material("Si14_solid",2.33*g/cm3,1);
  Si14_solid->AddElement(elSi,1);

  G4Material* CsI_solid = new G4Material("CsI_solid",4.51*g/cm3,2);
  CsI_solid->AddElement(elCs,.5);
  CsI_solid->AddElement(elI,.5);

  G4double C2H4_size = 7.0*cm;
  G4double C2H4_thickness = 0.2*cm;
  G4Box* Target = new G4Box("Target", 0.5*C2H4_size, 0.5*C2H4_size, 0.5*C2H4_thickness);
  G4LogicalVolume* logicTarget = new G4LogicalVolume(Target, C2H4, "Target");
  G4VPhysicalVolume* physTarget = new G4PVPlacement(0, G4ThreeVector(0, 0, -62.4*cm), logicTarget, "Target", logicGasBox1, false, 10, true);

  G4double Si_size = 7.5*cm;
  G4double Si_thickness = 20*um;
  G4Box* Board_Si1 = new G4Box("Board_Si1", 0.5*Si_size, 0.27*Si_size, 0.5*Si_thickness);
  G4LogicalVolume* logicBoard_Si1 = new G4LogicalVolume(Board_Si1, Si14_solid, "Board_Si1");
  //G4VPhysicalVolume* physBoard_Si1 = new G4PVPlacement(0, G4ThreeVector(6.3*cm, 0, 71.585*cm), logicBoard_Si1, "Board_Si1", logicGasBox1, false, 4, true);
  G4VPhysicalVolume* physBoard_Si1 = new G4PVPlacement(0, G4ThreeVector(6.3*cm, 0, 9.085*cm), logicBoard_Si1, "Board_Si1", logicGasBox1, false, 4, true);

  //G4double Si_thickness2 = 1.5*mm;
  G4Box* Board_Si2 = new G4Box("Board_Si2", 0.5*Si_size, 0.27*Si_size, 0.5*Si_thickness);
  G4LogicalVolume* logicBoard_Si2 = new G4LogicalVolume(Board_Si2, Si14_solid, "Board_Si2");
  //G4VPhysicalVolume* physBoard_Si2 = new G4PVPlacement(0, G4ThreeVector(-6.3*cm, 0, 71.585*cm), logicBoard_Si2, "Board_Si2", logicGasBox1, false, 5, true);
  G4VPhysicalVolume* physBoard_Si2 = new G4PVPlacement(0, G4ThreeVector(-6.3*cm, 0, 9.085*cm), logicBoard_Si2, "Board_Si2", logicGasBox1, false, 5, true);


  Si_size = 7.5*cm;
  Si_thickness = 20*um;

  G4double PositionThetaInDegree = 40;
  G4double PositionThetaInRad = PositionThetaInDegree/180.*3.14159;

  G4RotationMatrix* RotateDetector = new G4RotationMatrix();
  RotateDetector->rotateX(PositionThetaInRad);

    G4ThreeVector OriginalPosition(17.5*cm, 0, 67.5*cm);
    G4ThreeVector RevisedPosition = OriginalPosition.rotateX(-PositionThetaInRad);
    RevisedPosition.setZ(RevisedPosition.z());

  //G4double box2_size=50*cm;
  G4double box2_size=40*cm;

  G4Box* GasBox2 = new G4Box("GasBox2", 0.5*box2_size, 0.5*box2_size, 0.5*box2_size);
  G4LogicalVolume* logicGasBox2 = new G4LogicalVolume(GasBox2, He4_gas, "GasBox2");
  G4VPhysicalVolume* physGasBox2 = new G4PVPlacement(RotateDetector, RevisedPosition, logicGasBox2, "GasBox2", logicWorld, false, 2, true);

  //RotateDetector = new G4RotationMatrix();
  //RotateDetector->rotateX(PositionThetaInRad);

  G4Box* Board_Si3 = new G4Box("Board_Si3", 0.5*Si_size, 0.27*Si_size, 0.5*Si_thickness);
  G4LogicalVolume* logicBoard_Si3 = new G4LogicalVolume(Board_Si3, Si14_solid, "Board_Si3");
  G4VPhysicalVolume* physBoard_Si3 = new G4PVPlacement(new G4RotationMatrix, G4ThreeVector(-12.5*cm,2.77*cm,6.51*cm), logicBoard_Si3, "Board_Si3", logicGasBox2, false, 6, true);

  G4Box* Board_Si5 = new G4Box("Board_Si5", 0.5*Si_size, 0.27*Si_size, 0.5*Si_thickness);
  G4LogicalVolume* logicBoard_Si5 = new G4LogicalVolume(Board_Si5, Si14_solid, "Board_Si5");
  G4VPhysicalVolume* physBoard_Si5 = new G4PVPlacement(new G4RotationMatrix, G4ThreeVector(-12.5*cm,-2.77*cm,6.51*cm), logicBoard_Si5, "Board_Si5", logicGasBox2, false, 7, true);

  //RotateDetector = new G4RotationMatrix();
  //RotateDetector->rotateX(PositionThetaInRad);

    //OriginalPosition = G4ThreeVector(0, 0, 75.365*cm);
    //RevisedPosition = OriginalPosition.rotateX(-PositionThetaInRad);
    //RevisedPosition.setZ(RevisedPosition.z());

  G4double CsI_size = 5.0*cm;
  G4double CsI_thickness = 5.0*cm;

  G4Box* Board_CsI1 = new G4Box("Board_CsI1", 1*CsI_size, 0.5*CsI_size, 0.5*CsI_thickness);
  G4LogicalVolume* logicBoard_CsI1 = new G4LogicalVolume(Board_CsI1, CsI_solid, "Board_CsI1");
  G4VPhysicalVolume* physBoard_CsI1 = new G4PVPlacement(new G4RotationMatrix, G4ThreeVector(-12.5*cm,2.98*cm,10.365*cm), logicBoard_CsI1, "Board_CsI1", logicGasBox2, false, 8, true);

  G4Box* Board_CsI3 = new G4Box("Board_CsI3", 1*CsI_size, 0.5*CsI_size, 0.5*CsI_thickness);
  G4LogicalVolume* logicBoard_CsI3 = new G4LogicalVolume(Board_CsI3, CsI_solid, "Board_CsI3");
  G4VPhysicalVolume* physBoard_CsI3 = new G4PVPlacement(new G4RotationMatrix, G4ThreeVector(-12.5*cm,-3.02*cm,10.365*cm), logicBoard_CsI1, "Board_CsI3", logicGasBox2, false, 19, true);


  G4double TPC_size = 10.0*cm;
  G4double TPC_thickness = 15.0*cm;

  G4Box* TPC1 = new G4Box("TPC1", 0.5*TPC_size, 0.5*TPC_size, 0.5*TPC_thickness);
  G4LogicalVolume* logicTPC1 = new G4LogicalVolume(TPC1, He4_gas, "TPC1");
  G4VPhysicalVolume* physTPC1 = new G4PVPlacement(new G4RotationMatrix, G4ThreeVector(-12.5*cm,0,-10.0*cm), logicTPC1, "TPC1", logicGasBox2, false, 15, true);


  RotateDetector = new G4RotationMatrix();
  RotateDetector->rotateX(-PositionThetaInRad);

    OriginalPosition = G4ThreeVector(-17.5*cm, 0, 67.5*cm);
    RevisedPosition = OriginalPosition.rotateX(PositionThetaInRad);
    RevisedPosition.setZ(RevisedPosition.z());

  G4Box* GasBox3 = new G4Box("GasBox3", 0.5*box2_size, 0.5*box2_size, 0.5*box2_size);
  G4LogicalVolume* logicGasBox3 = new G4LogicalVolume(GasBox3, He4_gas, "GasBox3");
  G4VPhysicalVolume* physGasBox3 = new G4PVPlacement(RotateDetector, RevisedPosition, logicGasBox3, "GasBox3", logicWorld, false, 3, true);


//  RotateDetector = new G4RotationMatrix();
//  RotateDetector->rotateX(-PositionThetaInRad);

    //OriginalPosition = G4ThreeVector(0, 0, 71.51*cm);
    //RevisedPosition = OriginalPosition.rotateX(PositionThetaInRad);
    //RevisedPosition.setZ(RevisedPosition.z());

  G4Box* Board_Si4 = new G4Box("Board_Si4", 0.5*Si_size, 0.27*Si_size, 0.5*Si_thickness);
  G4LogicalVolume* logicBoard_Si4= new G4LogicalVolume(Board_Si4, Si14_solid, "Board_Si4");
  //G4VPhysicalVolume* physBoard_Si4 = new G4PVPlacement(RotateDetector, RevisedPosition, logicBoard_Si4, "Board_Si4", logicGasBox3, false, 7, true);
  G4VPhysicalVolume* physBoard_Si4 = new G4PVPlacement(new G4RotationMatrix, G4ThreeVector(12.5*cm,2.77*cm,6.51*cm), logicBoard_Si4, "Board_Si4", logicGasBox3, false, 7, true);

  G4Box* Board_Si6 = new G4Box("Board_Si6", 0.5*Si_size, 0.27*Si_size, 0.5*Si_thickness);
  G4LogicalVolume* logicBoard_Si6= new G4LogicalVolume(Board_Si6, Si14_solid, "Board_Si6");
  G4VPhysicalVolume* physBoard_Si6 = new G4PVPlacement(new G4RotationMatrix, G4ThreeVector(12.5*cm,-2.77*cm,6.51*cm), logicBoard_Si4, "Board_Si6", logicGasBox3, false, 18, true);


//  RotateDetector = new G4RotationMatrix();
//  RotateDetector->rotateX(-PositionThetaInRad);

    //OriginalPosition = G4ThreeVector(0, 0, 75.365*cm);
    //RevisedPosition = OriginalPosition.rotateX(PositionThetaInRad);
    //RevisedPosition.setZ(RevisedPosition.z());

  CsI_size = 5.0*cm;
  CsI_thickness = 5.0*cm;

  G4Box* Board_CsI2 = new G4Box("Board_CsI2", 1*CsI_size, 0.5*CsI_size, 0.5*CsI_thickness);
  G4LogicalVolume* logicBoard_CsI2 = new G4LogicalVolume(Board_CsI2, CsI_solid, "Board_CsI2");
  G4VPhysicalVolume* physBoard_CsI2 = new G4PVPlacement(new G4RotationMatrix, G4ThreeVector(12.5*cm,2.98*cm,10.365*cm), logicBoard_CsI2, "Board_CsI2", logicGasBox3, false, 9, true);

  G4Box* Board_CsI4 = new G4Box("Board_CsI4", 1*CsI_size, 0.5*CsI_size, 0.5*CsI_thickness);
  G4LogicalVolume* logicBoard_CsI4 = new G4LogicalVolume(Board_CsI4, CsI_solid, "Board_CsI4");
  G4VPhysicalVolume* physBoard_CsI4 = new G4PVPlacement(new G4RotationMatrix, G4ThreeVector(12.5*cm,-3.02*cm,10.365*cm), logicBoard_CsI2, "Board_CsI4", logicGasBox3, false, 20, true);


  G4Box* TPC2 = new G4Box("TPC2", 0.5*TPC_size, 0.5*TPC_size, 0.5*TPC_thickness);
  G4LogicalVolume* logicTPC2 = new G4LogicalVolume(TPC2, He4_gas, "TPC2");
  G4VPhysicalVolume* physTPC2 = new G4PVPlacement(new G4RotationMatrix, G4ThreeVector(12.5*cm,0,-10.0*cm), logicTPC2, "TPC2", logicGasBox3, false, 16, true);


  G4double BDC_size=16.0*cm;

  G4Box* BDC = new G4Box("BDC", 0.5*BDC_size, 0.5*BDC_size, 0.3*BDC_size);
  G4LogicalVolume* logicBDC = new G4LogicalVolume(BDC, He4_gas, "BDC");
  G4VPhysicalVolume* physBDC = new G4PVPlacement(0, G4ThreeVector(0, 0, 208.3*cm), logicBDC, "BDC", logicWorld, false, 11, true);


  G4double SC_size1=40.0*cm;
  G4double SC_thickness1=200*um;

  G4Box* SC1 = new G4Box("SC1", 0.5*SC_size1, 0.5*SC_size1, 0.5*SC_thickness1);
  G4LogicalVolume* logicSC1 = new G4LogicalVolume(SC1, He4_gas, "SC1");
  G4VPhysicalVolume* physSC1 = new G4PVPlacement(0, G4ThreeVector(0, 0, -12.3*cm), logicSC1, "SC1", logicWorld, false, 12, true);

  G4double SC_size2=50.0*cm;
  G4double SC_thickness2=0.3*cm;

  G4Box* SC2 = new G4Box("SC2", 0.5*SC_size2, 0.5*SC_size2, 0.5*SC_thickness1);
  G4LogicalVolume* logicSC2 = new G4LogicalVolume(SC2, He4_gas, "SC2");
  G4VPhysicalVolume* physSC2 = new G4PVPlacement(0, G4ThreeVector(0, 0, 196.3*cm), logicSC2, "SC2", logicWorld, false, 13, true);

  G4Box* SC3 = new G4Box("SC3", 0.5*SC_size2, 0.5*SC_size2, 0.5*SC_thickness2);
  G4LogicalVolume* logicSC3 = new G4LogicalVolume(SC3, He4_gas, "SC3");
  G4VPhysicalVolume* physSC3 = new G4PVPlacement(0, G4ThreeVector(0, 0, 220.3*cm), logicSC3, "SC3", logicWorld, false, 14, true);

  runManager->SetSensitiveDetector(physTPC2);

  return physWorld;
}
