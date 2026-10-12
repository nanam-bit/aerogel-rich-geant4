
#include "ActionInitialization.hh"
#include "DetectorConstruction.hh"

#include "FTFP_BERT.hh"
#include "G4OpticalPhysics.hh"
#include "G4OpticalParameters.hh"

#include "G4RunManagerFactory.hh"
#include "G4UIExecutive.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"


int main(int argc, char** argv)
{
  // default run manager?
  // auto* runManager = G4RunManagerFactory::CreateRunManager(G4RunManagerType::Default);
  auto runManager = G4RunManagerFactory::CreateRunManager();

  // set mandatory class
  runManager->SetUserInitialization(new rich::DetectorConstruction());

  // set physics lists (configure with optical physics enabled)
  G4VModularPhysicsList* physicsList = new FTFP_BERT;
  auto opticalPhysics = new G4OpticalPhysics();
  auto opticalParams = G4OpticalParameters::Instance();

  // activate cherenkov, disable scintillation
  opticalParams->SetProcessActivation("Cerenkov", true);
  opticalParams->SetProcessActivation("Scintillation", false);

  // there are other options here for opticalParameters that are not used:
  // SetCerenkovMaxPhotonsPerStep(G4int max) --> caps number of photons per step
  // SetCerenkovMaxBetaChange(G4double max) --> slice tracking steps to track (delta) velocity 
  // SetCerenkovVerboseLevel(G4int level) --> controls verbosity; step-by-step diagnostics
  // SetCerenkovTrackSecondariesFirst(G4bool boolean) --> tracks secondaries and pauses primary track until detection

  physicsList->RegisterPhysics(opticalPhysics);
  runManager->SetUserInitialization(physicsList);

  // action initialization
  runManager->SetUserInitialization(new rich::ActionInitialization());

  // define UI session
  G4UIExecutive* ui = nullptr;
  if (argc == 1) {
    ui = new G4UIExecutive(argc, argv);
  }

  // initialize visualization
  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize();

  // get the pointer to the User Interface manager
  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  if (ui) {
    // interactive mode (start UI session)
    UImanager->ApplyCommand("/control/execute init_vis.mac");
    ui->SessionStart();
    delete ui;
  }
  else {
    // batch mode (execute macro file)
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    UImanager->ApplyCommand(command + fileName);
  }

  // clean memory
  delete visManager;
  delete runManager;
  return 0;
}

