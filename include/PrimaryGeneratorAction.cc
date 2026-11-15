//
// from the Geant4 manual:
// 2.1.3
// G4VUserPrimaryGeneratorAction is a mandatory class the user has to provide. It creates an instance of a
// primary particle generator... In this class the user must describe the initial state of the primary event.
// This class has a public virtual method named GeneratePrimaries() which will be invoked at the beginning of
// each event.
// 2.6.1
// Actual generation of primary particles will be done by concrete classes of G4VPrimaryGenerator, explained 
// in the following subsection. Your G4VUserPrimaryGeneratorAction concrete class just arranges the way primary 
// particles are generated.

#include "PrimaryGeneratorAction.hh"


#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4LogicalVolumeStore.hh"
#include "Randomize.hh"

namespace rich
{

PrimaryGeneratorAction::PrimaryGeneratorAction()
{
  G4int n_particle = 1;
  fParticleGun = new G4ParticleGun(n_particle);

  // G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  // G4String particleName;
  // G4ParticleDefinition* particle = particleTable->FindParticle(particleName = "pi-"); 
  // fParticleGun->SetParticleDefinition(particle);

  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0., 0., 1.)); // straight down Z axis
  fParticleGun->SetParticleEnergy(3 * GeV);

}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete fParticleGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* event)
{
  // this function is called at the begining of each event
  //
  // From the Geant4 manual:
  // 2.6.2
  // G4ParticleGun is a generator provided by GEANT4. This class generates primary particle(s) 
  // with a given momentum and position. It does not provide any sort of randomizing. 
  // The constructor of G4ParticleGun takes an integer which causes the generation of one or 
  // more primaries of exactly same kinematics.
  //
  // Therefore, any sort of randomization should be invoked in GeneratePrimaries() before 
  // invoking the GeneratePrimaryVertex().

  if (fParticleGun->GetParticleDefinition() == nullptr){
    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    G4ParticleDefinition* particle = particleTable->FindParticle("pi-");

    if (particle) {
      fParticleGun->SetParticleDefinition(particle);
    } else {
      G4cerr << "ERROR: Can not find particle definition." << G4endl;
      return;
    }
  }

  G4ThreeVector particlePos = G4ThreeVector(0, 0, -20 * cm);
  fParticleGun->SetParticlePosition(particlePos);
  fParticleGun->GeneratePrimaryVertex(event);
}

}  // namespace rich
