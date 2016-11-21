



#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "A2DetPID.hh"
#include "A2DetMWPC.hh"
#include "A2Detector.hh"
#include "G4NistManager.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;
class G4Material;

class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    DetectorConstruction();
    virtual ~DetectorConstruction();

  public:

    virtual G4VPhysicalVolume* Construct();

    void DefineMaterials();

  private:

  	G4LogicalVolume*   fWorldLogic;
  	A2DetPID* fPID;   //PID detector
    G4double fPIDZ;
    A2DetMWPC* fMWPC; //wire chamber
};

#endif