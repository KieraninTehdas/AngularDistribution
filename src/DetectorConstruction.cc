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
#include "G4Material.hh"

//--------------------------------------------------

DetectorConstruction::DetectorConstruction()
: G4VUserDetectorConstruction()
{ 
DefineMaterials();
}

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

    fWorldLogic = worldLogical;
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
    
    //Target disk positioned at global origin
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
							
//-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -

//Crystal ball detector shell

	//Dimensions of shell

	G4double innerSphereRadius = 25.3*cm;
	G4double outerSphereRadius = 30.01*cm;
	G4double startingPhiSphere = 0.0*deg;
	G4double spanningPhiSphere = 360.0*deg;
	G4double startingThetaSphere = 0.0*deg;
	G4double spanningThetaSphere = 180.0*deg;

	G4VSolid* sphereSolid = new G4Sphere("SphereSolid", 
											innerSphereRadius, 
											outerSphereRadius,
											startingPhiSphere,
											spanningPhiSphere,
											startingThetaSphere,
											spanningThetaSphere);

	G4LogicalVolume* sphereLog = new G4LogicalVolume(sphereSolid, air, "SphereLogical");
	
	G4VPhysicalVolume* spherePhysical 
								= new G4PVPlacement(0,
								G4ThreeVector(pos_x,pos_y,pos_z),
								sphereLog,
								"Sphere",
								worldLogical,
								false,
								0);

//Other kit inside the crystal ball
//This code is taken from A2Crystal Ball simulation by Dr L. Zana.
	//Create PID
	fPID=new A2DetPID();
	fPID->Construct2(fWorldLogic,fPIDZ);
	//Return the physical world
	return worldPhysical;

}

//Define the materials required for the A2 sim stuff.
void DetectorConstruction::DefineMaterials()
{ 
  G4double density,z,a,fractionmass;
  G4int ncomponents;

  //Add materials not contained in default Nist list
  G4NistManager* NistManager=G4NistManager::Instance();

  //Rohacell. From cbsim.  The formula is (C4 H7 N 0)n. Density is 0.057 g/cm3.  (V.K)
 G4Material *Roha=new G4Material("A2_ROHACELL",density=0.057*g/cm3, ncomponents=4);  
 Roha->AddElement(NistManager->FindOrBuildElement(6),4);
 Roha->AddElement(NistManager->FindOrBuildElement(1),7);
 Roha->AddElement(NistManager->FindOrBuildElement(7),1);
 Roha->AddElement(NistManager->FindOrBuildElement(8),1);
 //NistManager->RegisterMaterial(Roha);

 //Wire Chamber Material. 74.5% Ar, 25% ethane, 0.5% freon. Density is 0.046g/cm3.
 G4Material *WCmat=new G4Material("A2_WCMAT", density=0.046*g/cm3, ncomponents=3,kStateGas);
 WCmat->AddElement(NistManager->FindOrBuildElement(18),fractionmass=0.745);
 WCmat->AddMaterial(NistManager->FindOrBuildMaterial("G4_ETHANE"), fractionmass=0.25);
 WCmat->AddMaterial(NistManager->FindOrBuildMaterial("G4_FREON-12"), fractionmass=0.005);
 //NistManager->RegisterMaterial(WCmat);

 //Cardboard.From cbsim. MWPC electronics
 G4Material *cboard=new G4Material("A2_CBOARD",density=0.69*g/cm3,ncomponents=3);
 cboard->AddElement(NistManager->FindOrBuildElement(6),fractionmass=0.285714286);
 cboard->AddElement(NistManager->FindOrBuildElement(1),fractionmass=0.476190476);
 cboard->AddElement(NistManager->FindOrBuildElement(8),fractionmass=0.238095238);
 //NistManager->RegisterMaterial(cboard);


 //Fibre GLass. From cbsim.
 G4Material *fglass=new G4Material("A2_FGLASS",density=2.0*g/cm3, ncomponents=8);
 fglass->AddElement(NistManager->FindOrBuildElement(8),fractionmass=0.557);
 fglass->AddElement(NistManager->FindOrBuildElement(14),fractionmass=0.347);
 fglass->AddElement(NistManager->FindOrBuildElement(20),fractionmass=0.3580000E-02);
 fglass->AddElement(NistManager->FindOrBuildElement(13),fractionmass=0.1590000E-02);
 fglass->AddElement(NistManager->FindOrBuildElement(5),fractionmass=0.6840000E-01);
 fglass->AddElement(NistManager->FindOrBuildElement(11),fractionmass=0.7420000E-02);
 fglass->AddElement(NistManager->FindOrBuildElement(19),fractionmass=0.1250000E-01);
 fglass->AddElement(NistManager->FindOrBuildElement(3),fractionmass=0.2320000E-02);
 //NistManager->RegisterMaterial(fglass);

 //Plastic. From cbsim 
 G4Material *plastic=new G4Material("A2_PLASTIC",density=1.19*g/cm3, ncomponents=2);
 plastic->AddElement(NistManager->FindOrBuildElement(6),fractionmass=0.8562844);
 plastic->AddElement(NistManager->FindOrBuildElement(1),fractionmass=0.1437155);
 //NistManager->RegisterMaterial(plastic);

 //Base material. From cbsim
 G4Material *basemat=new G4Material("A2_BASEMAT",density=2.26*g/cm3, ncomponents=2);
 basemat->AddElement(NistManager->FindOrBuildElement(6),fractionmass=0.8562844);
 basemat->AddElement(NistManager->FindOrBuildElement(1),fractionmass=0.1437155);
 //NistManager->RegisterMaterial(basemat);

 //mumetal for pmt shielding. From cbsim.
 G4Material* mumetal=new G4Material("A2_MUMETAL",density=8.7*g/cm3,ncomponents=5);
 mumetal->AddElement(NistManager->FindOrBuildElement(28),fractionmass=0.8);
 mumetal->AddElement(NistManager->FindOrBuildElement(42),fractionmass=0.5000000E-01);
 mumetal->AddElement(NistManager->FindOrBuildElement(14),fractionmass=0.5000000E-02);
 mumetal->AddElement(NistManager->FindOrBuildElement(29),fractionmass=0.2000000E-03);
 mumetal->AddElement(NistManager->FindOrBuildElement(26),fractionmass=0.1448);
 //NistManager->RegisterMaterial(mumetal);

 //brass for pid support
 G4Material* brass=new G4Material("A2_BRASS",density=8.5*g/cm3,ncomponents=2);
 brass->AddElement(NistManager->FindOrBuildElement(29),fractionmass=0.62);
 brass->AddElement(NistManager->FindOrBuildElement(30),fractionmass=0.38);
 //NistManager->RegisterMaterial(brass);

 //Liquid deuterium. From cbsim.
 // NistManager->RegisterMaterial(new G4Material("A2_LD2", z=1., a= 2.014*g/mole, density= 0.162*g/cm3));
 new G4Material("A2_lD2", z=1., a= 2.014*g/mole, density= 0.162*g/cm3);
 new G4Material("A2_lHe3", z=2., a= 3.0160*g/mole, density= 0.059*g/cm3);

}