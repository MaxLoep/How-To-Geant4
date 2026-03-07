#ifndef ParticleDataHEADER
#define ParticleDataHEADER 1

#include "G4Types.hh"

struct ParticleData {
  ParticleData();
  ParticleData(G4int count, G4double meanLife);
  ParticleData operator+(int);
  ParticleData operator+(ParticleData&);
  G4int     fCount;
  G4double  fTmean;
};

#endif
