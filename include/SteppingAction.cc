#include "SteppingAction.hh"
#include "EventAction.hh"

#include "G4Step.hh"
#include "G4StepPoint.hh"
#include "G4Track.hh"
#include "G4OpticalPhoton.hh"
#include "G4OpBoundaryProcess.hh"
#include "G4ProcessManager.hh"
#include "G4ProcessVector.hh"
#include "G4AnalysisManager.hh"
#include "G4SystemOfUnits.hh"

namespace rich
{

void SteppingAction::UserSteppingAction(const G4Step* step)
{
  if (step->GetTrack()->GetDefinition() != G4OpticalPhoton::Definition()) return;

  G4StepPoint* preP  = step->GetPreStepPoint();
  G4StepPoint* postP = step->GetPostStepPoint();
  auto pre  = preP->GetPhysicalVolume();
  auto post = postP->GetPhysicalVolume();
  if (!pre || !post) return;

  // only the step that crosses into the detector
  if (postP->GetStepStatus() != fGeomBoundary) return;
  if (post->GetName() != "DetectorLV" || pre->GetName() == "DetectorLV") return;

  fEventAction->AddArrival();

  // find the optical boundary process once per thread
  if (!fBoundary) {
    auto pv = G4OpticalPhoton::Definition()->GetProcessManager()
                ->GetPostStepProcessVector(typeDoIt);
    for (G4int i = 0; i < (G4int)pv->entries(); ++i) {
      fBoundary = dynamic_cast<G4OpBoundaryProcess*>((*pv)[i]);
      if (fBoundary) break;
    }
  }

  if (fBoundary && fBoundary->GetStatus() == Detection) {
    fEventAction->AddHit();

    G4ThreeVector pos = postP->GetPosition();       // point on the detector face
    G4double energy = preP->GetTotalEnergy() / eV;

    auto am = G4AnalysisManager::Instance();
    am->FillNtupleDColumn(0, 0, pos.x());
    am->FillNtupleDColumn(0, 1, pos.y());
    am->FillNtupleDColumn(0, 2, pos.z());
    am->FillNtupleDColumn(0, 3, energy);
    am->AddNtupleRow(0);
  }
}

}  // namespace rich