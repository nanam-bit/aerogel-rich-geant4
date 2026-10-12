
#include "EventAction.hh"
#include "G4Event.hh"
#include "G4AnalysisManager.hh"

namespace rich
{

void EventAction::BeginOfEventAction(const G4Event*)
{
  // leave empty for now
}

void EventAction::EndOfEventAction(const G4Event* event)
{
  G4int eventID = event->GetEventID();
  if (eventID % 100 == 0) {
    G4cout << "---> progress tracking: 100 events " << eventID << G4endl;
  }
}

}  // namespace rich
