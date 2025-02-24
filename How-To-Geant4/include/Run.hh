#include <fstream>
#ifndef Run_h
#define Run_h 1

#include "G4Run.hh"
#include "G4VProcess.hh"
#include "globals.hh"
#include <map>

class DetectorConstruction;
class G4ParticleDefinition;

class Run : public G4Run
{
  public:
    Run(DetectorConstruction*);
   ~Run();

    void SetPrimary(G4ParticleDefinition* particle, G4double energy);
    void CountProcesses(const G4VProcess* process);
    void ParticleCount(G4String, G4double);
    void ParticleFlux(G4String);

    G4int GetIonId (G4String);

    virtual void Merge(const G4Run*);
    void EndOfRun();

  private:
    struct ParticleData {
     ParticleData()
       : fCount(0), fTmean(-1.) {}
     ParticleData(G4int count, G4double meanLife)
       : fCount(count), fTmean(meanLife) {}
     G4int     fCount;
     G4double  fTmean;
    };

    // utility function
    template<typename Ostream>
    void OutputParticleData(std::map<G4String,ParticleData>& particle_map, Ostream& stream);
    void Merge(std::map<G4String,ParticleData>& destinationMap,
               const std::map<G4String,ParticleData>& sourceMap) const;


    // WHAT IS fgIonMap AND WHAT DOES IT DO?
    static std::map<G4String,G4int> fgIonMap;
    static G4int fgIonId;

    DetectorConstruction* fDetector;
    G4ParticleDefinition* fParticle;
    G4double              fEkin;

    G4double fEnergyDeposit, fEnergyDeposit2;
    G4double fEnergyFlow,    fEnergyFlow2;

    // map to count processes
    std::map<G4String,G4int>        fProcCounter;

    // map to count generated Particles in TOTAL
    std::map<G4String,ParticleData> fParticleDataMap1;

    // map to count generated Particles leaving the World volume
    std::map<G4String,ParticleData> fParticleDataMap2;
};

#endif