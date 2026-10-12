
#include "SensitiveDetector.hh"
#include "G4Track.hh"
#include "G4OpticalPhoton.hh"
#include "G4Step.hh"
#include "G4AnalysisManager.hh"

namespace rich
{

SensitiveDetector::SensitiveDetector(const G4String& name)
  : G4VSensitiveDetector(name)
{
  collectionName.insert("PhotonHitsCollection");
}

SensitiveDetector::~SensitiveDetector()
{
  // empty, no manual deletions needed here
}

G4bool SensitiveDetector::ProcessHits(G4Step* step, G4TouchableHistory* history)
{
  // get only the optical photons (ignore everything else)
  G4Track* track = step->GetTrack();
  if (track->GetDefinition() != G4OpticalPhoton::OpticalPhotonDefinition())
  {
    return false; 
  }
  
  G4StepPoint* preStepPoint = step->GetPreStepPoint();
  G4ThreeVector hitPos = preStepPoint->GetPosition();

  G4double hitX = hitPos.x();
  G4double hitY = hitPos.y();
  G4double hitZ = hitPos.z();

  G4double edep = step->GetTotalEnergyDeposit();

  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->FillNtupleDColumn(0, hitX);
  analysisManager->FillNtupleDColumn(1, hitY);
  analysisManager->FillNtupleDColumn(2, hitZ);
  analysisManager->FillNtupleDColumn(3, edep);
  analysisManager->AddNtupleRow();

  track->SetTrackStatus(fStopAndKill);

  return true;
}

}  // namespace rich
