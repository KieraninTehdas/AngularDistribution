



#include "DetectorConstruction.hh"
#include "ActionInitialization.hh"

#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif

#include "G4UImanager.hh"
#include "FTFP_BERT.hh"
#include "G4StepLimiterPhysics.hh"

#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include "G4ScoringManager.hh"

//------------------------------------------------------

int main(int argc,char** argv)
{
  //Detect interactive mode (if no argument) and define UI session
  G4UIExecutive* ui = 0;
  if ( argc == 1 ) { //No commands line argument
    //Let G4UIExecutive guess what is the best available UI
    ui = new G4UIExecutive(argc,argv);
  }


//-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

  // Construct the default run manager
  // Note that if we have built G4 with support for Multi-threading we set it here
#ifdef G4MULTITHREADED
    G4MTRunManager* runManager = new G4MTRunManager;
    //Set the default number of threads to be the number of available cores of the machine
    //If not specified use 2 threads
    runManager->SetNumberOfThreads( G4Threading::G4GetNumberOfCores() );
#else
    G4RunManager* runManager = new G4RunManager;
#endif

    G4Random::setTheEngine(new CLHEP::HepJamesRandom);
//-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

// Set mandatory initialization classes
  //
  // Detector construction
  runManager->SetUserInitialization(new DetectorConstruction());

  //The Physics
    G4VModularPhysicsList* physicsList = new FTFP_BERT;
    physicsList->RegisterPhysics(new G4StepLimiterPhysics());
    runManager->SetUserInitialization(physicsList);

    // User action initialization
    runManager->SetUserInitialization(new ActionInitialization());
   // runManager->SetUserAction(new RunAction);

    // Initialize visualization
  //
  G4VisManager* visManager = new G4VisExecutive;
  // G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
  // G4VisManager* visManager = new G4VisExecutive("Quiet");
  visManager->Initialize();

   // Get the pointer to the User Interface manager
  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  // Process macro or start UI session
  //
  if ( ! ui ) { 
    // batch mode
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    UImanager->ApplyCommand(command+fileName);
  }
  else { 
    // interactive mode
    UImanager->ApplyCommand("/control/execute init_vis.mac");
    ui->SessionStart();
    delete ui;
  }

  // Job termination
  // Free the store: user actions, physics_list and detector_description are
  // owned and deleted by the run manager, so they should not be deleted 
  // in the main() program !
  
  delete visManager;
  delete runManager;
}

