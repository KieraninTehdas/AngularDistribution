





#include "SteppingAction.hh"
#include "EventAction.hh"
#include "DetectorConstruction.hh"

#include "G4Step.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4Track.hh"
#include "G4UnitsTable.hh"

//---------------------------------

SteppingAction::SteppingAction(EventAction* eventAction)
: G4UserSteppingAction(),
  fEventAction(eventAction)
{}

//-- -- -- -- -- -- -- -- -- -- -- -- -- --

SteppingAction::~SteppingAction()
{}

//-- -- -- -- -- -- -- -- -- -- -- -- -- --

void SteppingAction::UserSteppingAction(const G4Step* step)
{

	G4StepPoint* preStepPoint = step->GetPreStepPoint();
	G4StepPoint* postStepPoint = step->GetPostStepPoint();

	if (preStepPoint->GetStepStatus() == fGeomBoundary)
	{

		G4cout << "Step starts on boundary." << G4endl;
		
		G4TouchableHandle touch1 = postStepPoint->GetTouchableHandle();
		G4VPhysicalVolume* volume = touch1->GetVolume();
		G4String name = volume->GetName();

		G4TouchableHandle touch2 = preStepPoint->GetTouchableHandle();
		G4VPhysicalVolume* volume2 = touch2->GetVolume();
		G4String name2 = volume2->GetName();

		G4Track* track = step->GetTrack();
		G4double kineticEnergy = track->GetKineticEnergy();

		

		G4cout << "Start volume name: " << name2 
		 << "End Volume name: " << name << G4endl;
		 G4cout << "Kinetic Energy: " << G4BestUnit(kineticEnergy, "Energy") << G4endl;

	}

	if (postStepPoint->GetStepStatus() == fGeomBoundary)
	{

		G4cout << "Step ends on boundary."  << G4endl;

		G4TouchableHandle touch1 = postStepPoint->GetTouchableHandle();
		G4VPhysicalVolume* volume = touch1->GetVolume();
		G4String name = volume->GetName();

		G4TouchableHandle touch2 = preStepPoint->GetTouchableHandle();
		G4VPhysicalVolume* volume2 = touch2->GetVolume();
		G4String name2 = volume2->GetName();

		G4ThreeVector endPosition = postStepPoint->GetPosition();

		G4Track* track = step->GetTrack();
		G4double kineticEnergy = track->GetKineticEnergy();

		G4cout << "Start volume name: " << name2 
		 << "End Volume name: " << name << G4endl;

		 G4cout << "Kinetic Energy: " << G4BestUnit(kineticEnergy, "Energy") << G4endl;


	}


//Check energy loss in transport.
/*
	G4Track* aTrack = step->GetTrack();
	G4double aKineticEnergy = aTrack->GetKineticEnergy();
	G4cout << "At the end of this step, E = " << G4BestUnit(aKineticEnergy, "Energy") << G4endl;
*/
}