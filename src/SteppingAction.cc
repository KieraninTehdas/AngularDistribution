





#include "SteppingAction.hh"
#include "EventAction.hh"
#include "DetectorConstruction.hh"
#include "Analysis.hh"

#include "G4Step.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4Track.hh"
#include "G4UnitsTable.hh"

#include <fstream>
  using namespace std;

  //file to print results
//ofstream outFile("output.txt");
//---------------------------------

SteppingAction::SteppingAction(EventAction* eventAction)
: G4UserSteppingAction(),
  fEventAction(eventAction)
{
	
}

//-- -- -- -- -- -- -- -- -- -- -- -- -- --

SteppingAction::~SteppingAction()
{}

//-- -- -- -- -- -- -- -- -- -- -- -- -- --

void SteppingAction::UserSteppingAction(const G4Step* step)
{

	G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

	G4StepPoint* preStepPoint = step->GetPreStepPoint();
	G4StepPoint* postStepPoint = step->GetPostStepPoint();

	//Check if step begins or ends on a boundary
	/*
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
	*/

	if (postStepPoint->GetStepStatus() == fGeomBoundary)
	{

		//G4cout << "Step ends on boundary."  << G4endl;

		G4TouchableHandle touch1 = postStepPoint->GetTouchableHandle();
		G4VPhysicalVolume* volume = touch1->GetVolume();
		G4String name = volume->GetName();

		G4TouchableHandle touch2 = preStepPoint->GetTouchableHandle();
		G4VPhysicalVolume* volume2 = touch2->GetVolume();
		G4String name2 = volume2->GetName();

		//If step ends on a boundary, get position and calculate theta and phi.
		/*
		G4ThreeVector endPosition = postStepPoint->GetPosition();
		G4double x = endPosition.x();
		G4double y = endPosition.y();
		G4double z = endPosition.z();
		*/

		//Get momentum direction to calculate theta and phi
		G4ThreeVector endMomentumDirection = postStepPoint->GetMomentumDirection();
		G4double x = endMomentumDirection.x();
		G4double y = endMomentumDirection.y();
		G4double z = endMomentumDirection.z();

		G4double r = sqrt(pow(x,2) + pow(y,2) + pow(z,2));

		G4double theta = acos(z/r) * (180/M_PI);
		G4double phi = atan(y/x) * (180/M_PI);

		//Get track to get kinetic energy

		G4Track* track = step->GetTrack();
		G4double kineticEnergy = track->GetKineticEnergy();

		//G4cout << track->GetVertexKineticEnergy() << G4endl;


		G4double energyLoss = track->GetVertexKineticEnergy() - kineticEnergy;

		//G4cout << energyLoss << G4endl;

		//Get primary vertex momentum direction to calculate initial theta and phi
		G4ThreeVector initialMomentumDirection = track->GetVertexMomentumDirection();
		G4double x0 = initialMomentumDirection.x();
		G4double y0 = initialMomentumDirection.y();
		G4double z0 = initialMomentumDirection.z();

		G4double r0 = sqrt(pow(x0,2) + pow(y0,2) + pow(z0,2));
		//G4double theta0 = acos(z0/r0)*(180/M_PI);
		//G4double phi0 = atan(y0/x0) * (180/M_PI);
		G4double theta0 = acos(z0/r0);
		G4double phi0 = atan(y0/x0);

/*
		G4cout << "Start volume name: " << name2 
		 << "End Volume name: " << name << G4endl;

		 G4cout << "Kinetic Energy: " << G4BestUnit(kineticEnergy, "Energy") << G4endl;
		 //G4cout << x << " " << y << " " << z << G4endl;
		 G4cout << "Theta = " << theta << " Phi = " << phi << G4endl;
		 G4cout << "Init Theta = " << theta0 << "Init Phi = " << phi0 << G4endl;

		 analysisManager->FillNtupleDColumn(0, kineticEnergy);
		 analysisManager->FillNtupleDColumn(1, theta);
		 analysisManager->FillNtupleDColumn(2, phi);
		 analysisManager->FillNtupleDColumn(3, theta0);
		 analysisManager->FillNtupleDColumn(4, phi0);
		 analysisManager->AddNtupleRow();

		 //analysisManager->FillH1(0, kineticEnergy);
	
*/
		 analysisManager->FillH2(0, theta0, energyLoss);

		 //print the data to the text file
		 //outFile << theta << " " << phi << " " << kineticEnergy << G4endl;

	}


//Check energy loss in transport.
/*
	G4Track* aTrack = step->GetTrack();
	G4double aKineticEnergy = aTrack->GetKineticEnergy();
	G4cout << "At the end of this step, E = " << G4BestUnit(aKineticEnergy, "Energy") << G4endl;
*/
}