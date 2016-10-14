




#ifndef RUNACTION_HH
#define RUNACTION_HH

#include "G4UserRunAction.hh"
#include "globals.hh"

class G4Run;

class RunAction : public G4UserRunAction {

public:

  // Constructor
  RunAction();

  // Destructor
  virtual ~RunAction();

  // Methods  
  //virtual G4Run* GenerateRun();
  
  void BeginOfRunAction(const G4Run*);
  void EndOfRunAction(const G4Run*);
    
};

#endif