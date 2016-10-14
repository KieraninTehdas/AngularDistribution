



#include "RunAction.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include <assert.h>

//--------------------------------

RunAction::RunAction()
 : G4UserRunAction()
{  }

//-- -- -- -- -- -- -- -- -- -- --

RunAction::~RunAction()
{}

//-- -- -- -- -- -- -- -- -- -- --

void RunAction::BeginOfRunAction(const G4Run*)
{ 
  //inform the runManager to save random number seed
  G4RunManager::GetRunManager()->SetRandomNumberStore(false);
}

//-- -- -- -- -- -- -- -- -- -- --

void RunAction::EndOfRunAction(const G4Run* )
{}