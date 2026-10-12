// this class opens+writes+closes the root file that needs to be generated
// RunAction is where G4AnalysisManager is initialized and managed

#include "RunAction.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4AnalysisManager.hh"

namespace rich
{

RunAction::RunAction()
{
  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->SetVerboseLevel(1);

  analysisManager->CreateNtuple("PhotonHits", "RICH Photon Detection");
  analysisManager->CreateNtupleDColumn("X"); //   ID: 0
  analysisManager->CreateNtupleDColumn("Y"); //       1
  analysisManager->CreateNtupleDColumn("Z"); //       2
  analysisManager->CreateNtupleDColumn("Edep"); //    3
  analysisManager->FinishNtuple();
}

void RunAction::BeginOfRunAction(const G4Run*)
{
  auto analysisManager = G4AnalysisManager::Instance();
  G4String fileName = "rich_sim_output.root";
  analysisManager->OpenFile(fileName); // should create file if name is not found
}

void RunAction::EndOfRunAction(const G4Run* run)
{
  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->Write();
  analysisManager->CloseFile();
}

}  // namespace rich
