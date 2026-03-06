#include "particle_data.hh"

#include <iostream>

ParticleData::ParticleData() {
  this->fCount = 0;
  this->fTmean = 0.;
}

ParticleData::ParticleData(G4int count, G4double meanLife) {
  this->fCount = count;
  this->fTmean = meanLife;
}

ParticleData ParticleData::operator+(ParticleData& other) {
  if (this->fTmean == 0.) this->fTmean = other.fTmean;
  return ParticleData(this->fCount + other.fCount, this->fTmean);
}

ParticleData ParticleData::operator+(int other){
  return ParticleData(this->fCount + other, this->fTmean);
}
