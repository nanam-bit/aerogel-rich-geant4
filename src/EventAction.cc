
#include "EventAction.hh"
#include "G4Event.hh"
#include "G4AnalysisManager.hh"

namespace rich
{

void EventAction::BeginOfEventAction(const G4Event*)
{
  fArrivals = 0;
  fHits = 0;
}

void EventAction::EndOfEventAction(const G4Event* event)
{
  G4int eventID = event->GetEventID();
  if (eventID % 100 == 0) {
    G4cout << "---> progress tracking: 100 events " << eventID << G4endl;
  }

  auto am = G4AnalysisManager::Instance();
  am->FillNtupleIColumn(1, 0, fArrivals);
  am->FillNtupleIColumn(1, 1, fHits);
  am->AddNtupleRow(1);
}

}  // namespace rich
