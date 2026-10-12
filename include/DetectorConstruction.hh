
// this is the header file for the Detector Construction

#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

// pulling from the core Geant4 base class definition
#include "G4VUserDetectorConstruction.hh"

// forward declarations (introducing class names; no implementation)
class G4VPhysicalVolume;
class G4LogicalVolume;
class G4Material;
class G4OpticalSurface;

namespace rich
{
class DetectorConstruction : public G4VUserDetectorConstruction
{
  public: 
    DetectorConstruction();
    ~DetectorConstruction() override = default;

    // ensure that these will be included in src
    G4VPhysicalVolume* Construct() override; 
    void ConstructSDandField() override;

  private:
    void DefineOpticalProperties();
    
    // materials
    G4Material* fAerogel = nullptr;
    G4Material* fVacuum = nullptr;
    G4Material* fSiC = nullptr;

    // others
    G4OpticalSurface* fPhotocathodeSurface = nullptr;
    G4OpticalSurface* fRadiatorSurface = nullptr;
    G4VPhysicalVolume* fPhysDetector = nullptr;
    G4VPhysicalVolume* fPhysRadiator = nullptr;
    G4VPhysicalVolume* fPhysWorld = nullptr;

};

} // namespace rich

#endif