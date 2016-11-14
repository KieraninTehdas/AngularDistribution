/* Implementation of detector construction class.
Builds world volume and lead target disk.
*/


#include "DetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"

//--------------------------------------------------

DetectorConstruction::DetectorConstruction()
: G4VUserDetectorConstruction()
{ }

//--------------------------------------------------

DetectorConstruction::~DetectorConstruction()
{ }

//--------------------------------------------------

G4VPhysicalVolume* DetectorConstruction::Construct()
{

	// Get nist material manager
	G4NistManager* nist = G4NistManager::Instance();

	//Build required materials from database
	G4Material* air = nist->FindOrBuildMaterial("G4_AIR");
	G4Material* lead = nist->FindOrBuildMaterial("G4_Pb");

	//Switch on volume overlap checking
	G4bool checkOverlaps = true;

//-- -- -- -- -- -- -- -- -- -- - - - - -- -- -- -- -- -- --- 

	//Construct the world volume
	G4double world_x = 1.0*m;
	G4double world_y = 1.0*m;
	G4double world_z = 1.0*m;


	G4VSolid* worldSolid = new G4Box("worldBox", 
										world_x, 
										world_y, 
										world_z);

    G4LogicalVolume* worldLogical= new G4LogicalVolume(worldSolid,
    														  air, 
    												"worldLogical");
    
    G4VPhysicalVolume* worldPhysical = new G4PVPlacement(0, 
    										G4ThreeVector(),
    										worldLogical,
    										"worldPhysical",
    										0,
    										false,
    										0,
    										checkOverlaps);

//-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -

    //Define lead target dimensions
    G4double innerRadius = 0.0*cm;
    G4double outerRadius = 1.0*cm;
    G4double hz = 0.5*mm;
    G4double startAngle = 0.0*deg;
    G4double spanningAngle = 360.0*deg;

    //Construct target disk
    G4Tubs* targetDisc = new G4Tubs("Target", innerRadius, outerRadius, hz, startAngle, spanningAngle);               
    
    G4LogicalVolume* targetLog = new G4LogicalVolume(targetDisc, lead, "Target");
    
    G4double pos_x =  0.0*meter;
	G4double pos_y =  0.0*meter;
	G4double pos_z =  0.0*meter;
	
	G4VPhysicalVolume* targetPhysical
		= new G4PVPlacement(0,
							G4ThreeVector(pos_x, pos_y, pos_z),
							targetLog,
							"Target",
							worldLogical,
							false,
							0);



	//Return the physical world
	return worldPhysical;
}