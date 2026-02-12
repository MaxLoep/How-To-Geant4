#include "particle_data.hh"

ParticleData::ParticleData() {
  this->fCount = 0;
  this->fTmean = 0.;
}

ParticleData::ParticleData(G4int count, G4double meanLife) {
  this->fCount = count;
  this->fTmean = meanLife;
}
