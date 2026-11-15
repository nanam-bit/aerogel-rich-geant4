
#include "SensitiveDetector.hh"
#include "EventAction.hh"
#include "G4EventManager.hh"
#include "G4Track.hh"
#include "G4OpticalPhoton.hh"
#include "G4Step.hh"
#include "G4AnalysisManager.hh"
#include "G4SystemOfUnits.hh"

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

  auto evAction = static_cast<EventAction*>
  (
    G4EventManager::GetEventManager()->GetUserEventAction()
  );
  evAction->AddHit();

  G4double hitX = hitPos.x();
  G4double hitY = hitPos.y();
  G4double hitZ = hitPos.z();

  G4double energy = preStepPoint->GetTotalEnergy() / CLHEP::eV;

  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->FillNtupleDColumn(0, 0, hitX);
  analysisManager->FillNtupleDColumn(0, 1, hitY);
  analysisManager->FillNtupleDColumn(0, 2, hitZ);
  analysisManager->FillNtupleDColumn(0, 3, energy);
  analysisManager->AddNtupleRow(0);

  track->SetTrackStatus(fStopAndKill);

  return true;
}

}  // namespace rich
