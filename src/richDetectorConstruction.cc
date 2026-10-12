// RICH Detector construction

#include "DetectorConstruction.hh"

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4Material.hh"
#include "G4Element.hh"
#include "G4MaterialTable.hh"
#include "G4MaterialPropertiesTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4ThreeVector.hh"
#include "G4LogicalVolumeStore.hh"

#include "globals.hh"
#include "G4LogicalVolumeStore.hh"

#include "G4NistManager.hh"
#include "G4RunManager.hh"
#include "G4SDManager.hh"

#include "SensitiveDetector.hh"

// #include "G4VSensitiveDetector.hh"

namespace rich
{

DetectorConstruction::DetectorConstruction()
{
  DefineOpticalProperties();
}

void DetectorConstruction::DefineOpticalProperties()
{
  G4NistManager* nist = G4NistManager::Instance();
  
  // NIST materials
  fVacuum = nist->FindOrBuildMaterial("G4_Galactic");
  G4Element* elemSi = nist->FindOrBuildElement("Si");
  G4Element* elemO = nist->FindOrBuildElement("O");
  G4Element* elemC = nist->FindOrBuildElement("C");

  // define aerogel composition --> n = 1.05. Silica aerogel SiO2
  fAerogel = new G4Material("Aerogel", .150 * g/cm3, 2);
  fAerogel->AddElement(elemSi, 1);
  fAerogel->AddElement(elemO, 2);

  // define silicon carbide SiC
  fSiC = new G4Material("SiC", 3.16 * g/cm3, 2);
  fSiC->AddElement(elemC, 1);
  fSiC->AddElement(elemSi, 1);

  // define photon array, rindex, abslength, and efficiency
  // Efficiency -- determine if a measurable hit occurred. This is quantum efficiency, or
  // the probability that a photon hitting the sensor plane will produce a measurable signal
  // (electron) rather than being absorbed/lost as heat.
  // Efficiency is defined as {.25, .35, .25} as it peaks at 35%. 
  std::vector<G4double> photonEnergy = {2.0*eV, 3.0*eV, 4.0*eV};
  std::vector<G4double> rIndexAerogel = {1.05, 1.05, 1.05};
  std::vector<G4double> rIndexVacuum = {1.0, 1.0, 1.0};
  std::vector<G4double> aerogelAbsLength = {100*cm, 100*cm, 100*cm};
  std::vector<G4double> siliconAbsLength = {1.0*um, 1.0*um, 1.0*um};
  std::vector<G4double> efficiency = {.25, .35, .25}; 

  // define refractive index and absorption length for aerogel
  G4MaterialPropertiesTable* aerogelMPT = new G4MaterialPropertiesTable();
  aerogelMPT->AddProperty("RINDEX", photonEnergy, rIndexAerogel);  
  aerogelMPT->AddProperty("ABSLENGTH", photonEnergy, aerogelAbsLength);
  fAerogel->SetMaterialPropertiesTable(aerogelMPT);

  // define refractive index for vacuum
  G4MaterialPropertiesTable* vacuumMPT = new G4MaterialPropertiesTable();
  vacuumMPT->AddProperty("RINDEX", photonEnergy, rIndexVacuum);  
  fVacuum->SetMaterialPropertiesTable(vacuumMPT);

  // also define absorption length for silicon detector
  G4MaterialPropertiesTable* siliconMPT = new G4MaterialPropertiesTable();
  siliconMPT->AddProperty("ABSLENGTH", photonEnergy, siliconAbsLength);
  fSiC->SetMaterialPropertiesTable(siliconMPT);

  // need finish on radiator exit - no efficiency set since photons pass through
  fRadiatorSurface = new G4OpticalSurface("RadiatorSurface");
  fRadiatorSurface->SetType(dielectric_dielectric);
  fRadiatorSurface->SetFinish(polished);

  G4MaterialPropertiesTable* RadSurfaceMPT = new G4MaterialPropertiesTable();
  fRadiatorSurface->SetMaterialPropertiesTable(RadSurfaceMPT);

  // optical surface object and bind to table
  fPhotocathodeSurface = new G4OpticalSurface("PhotocathodeSurface");
  fPhotocathodeSurface->SetType(dielectric_metal);
  fPhotocathodeSurface->SetFinish(polished);

  G4MaterialPropertiesTable* PhotoSurfaceMPT = new G4MaterialPropertiesTable();
  PhotoSurfaceMPT->AddProperty("EFFICIENCY", photonEnergy, efficiency);
  fPhotocathodeSurface->SetMaterialPropertiesTable(PhotoSurfaceMPT);

}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  // define necessary dimensions
  G4double worldXY = 15*cm;   // 30 cm
  G4double worldZ = 25*cm;    // 50 cm
  G4double radXY = 5*cm;      // 10 cm
  G4double radZ = 2.5*cm;     // 5 cm
  G4double detXY = 10*cm;     // 20 cm
  G4double detZ = .1*cm;      // .2 cm

  G4ThreeVector radPlacement = G4ThreeVector(0, 0, -worldXY);
  G4ThreeVector detPlacement = G4ThreeVector(0, 0, worldXY);

  // ------ > defining volumes: G4Box takes halves of the total box size
  // G4PVPlacement(Rotation, location, logicVol, name, mother volume, bool operation, copy num, overlaps)

  // world volume: 30x30x50 cm
  G4bool checkOverlaps = true;
  auto solidWorld = new G4Box("World", worldXY, worldXY, worldZ);
  auto logicWorld = new G4LogicalVolume(solidWorld, fVacuum, "WorldLV");
  fPhysWorld = new G4PVPlacement(nullptr, G4ThreeVector(), logicWorld, "World", nullptr, false, 0, checkOverlaps);

  // radiator volume: 10x10x5 cm
  auto solidRadiator = new G4Box("Radiator", radXY, radXY, radZ);
  auto logicRadiator = new G4LogicalVolume(solidRadiator, fAerogel, "RadiatorLV");
  fPhysRadiator = new G4PVPlacement(nullptr, radPlacement, logicRadiator, "Radiator", logicWorld, false, 0, checkOverlaps);

  // detector volume: 20x20x.2
  auto solidDetector = new G4Box("Detector", detXY, detXY, detZ);
  auto logicDetector = new G4LogicalVolume(solidDetector, fSiC, "DetectorLV");
  fPhysDetector = new G4PVPlacement(nullptr, detPlacement, logicDetector, "DetectorLV", logicWorld, false, 0, checkOverlaps);

  // optical surface boundary
  new G4LogicalBorderSurface("PhotocathodeBorder", fPhysWorld, fPhysDetector, fPhotocathodeSurface); // world --> detector
  new G4LogicalBorderSurface("RadiatorSurface", fPhysRadiator, fPhysWorld, fRadiatorSurface); // radiator --> world

  return fPhysWorld;

}

void DetectorConstruction::ConstructSDandField()
{
  // have to define sd manager here first
  G4SDManager* SDManager = G4SDManager::GetSDMpointer();

  // create instance of sensitive detector class
  G4VSensitiveDetector* richSD = new SensitiveDetector("RichPhotonDetectorSD");
  
  // register with global SD manager
  SDManager->AddNewDetector(richSD);
  
  // attatch to logical vol
  // attach via string name 
  // can also put this in if/else to make sure logicDetector even exists and then create error messg if not
  SetSensitiveDetector("DetectorLV", richSD);
  // another method is logicLV->SetSensitiveDetector(richSD)

}

} // namespace rich