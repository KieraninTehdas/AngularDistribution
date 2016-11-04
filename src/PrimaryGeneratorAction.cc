



#include "PrimaryGeneratorAction.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4GenericMessenger.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

//----------------------------------------------

PrimaryGeneratorAction::PrimaryGeneratorAction()
: G4VUserPrimaryGeneratorAction(),     
  fParticleGun(0)
{
    G4int n_particle = 1;
    fParticleGun  = new G4ParticleGun(n_particle);
    
    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    G4String particleName;
    //G4ParticleDefinition* photon = particleTable->FindParticle(particleName="gamma");
    fProton = particleTable->FindParticle(particleName="proton");
    
    G4double kineticEnergy = 50.0*MeV;
    
    // default particle kinematics
    fParticleGun->SetParticleDefinition(fProton);
	fParticleGun->SetParticleEnergy(kineticEnergy);
    
}

//----------------------------------------------

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
    delete fParticleGun;
    
}

//----------------------------------------------

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
 
 //set position of particle source   
  
	G4double source_x = 0.0*m;
	G4double source_y = 0.0*m;
	G4double source_z = 0.0*m;
  
	fParticleGun->SetParticlePosition(G4ThreeVector(source_x,source_y,source_z));

//create an isotropic source

	G4double cosTheta = 0.0 + 1.0*G4UniformRand();
	G4double Theta = acos(cosTheta);
	G4double phi = CLHEP::twopi*G4UniformRand();
	G4double sinTheta = sqrt(1.0 - cosTheta*cosTheta);

	fParticleGun -> SetParticleMomentumDirection(G4ThreeVector(sinTheta*cos(phi),
								sinTheta*sin(phi),
							    cosTheta));

	//G4cout << Theta << "," << phi << G4endl;
			
	
	//fParticleGun -> SetParticleMomentumDirection(G4ThreeVector(1,0,0));
	fParticleGun->GeneratePrimaryVertex(anEvent);
}