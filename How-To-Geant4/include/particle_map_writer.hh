#ifndef particle_map_writer_def
#define particle_map_writer_def 1

#include "particle_data.hh"
#include "G4String.hh"
#include "G4UnitsTable.hh"
#include <map>
#include <regex>

typedef std::map<G4String, ParticleData> particle_map_t;
namespace pmap_writer {

template <typename Ostream>
void write_pmap_to_stream(particle_map_t& particle_map, Ostream& stream);

}

// TODO: why does this not work when it is in the correct file???
template <typename Ostream>
void pmap_writer::write_pmap_to_stream(particle_map_t& particle_map, Ostream& stream){
	// iterate over particle map and generate output of isotopes, count, half_life
	for ( const auto& particleData : particle_map ) {
		G4String name = particleData.first;
		std::replace( name.begin(), name.end(), '[', '_');

		name = std::regex_replace(name, std::regex("[+]"), "_plus");
		name = std::regex_replace(name, std::regex("[-]"), "_minus");

		name.erase(std::remove(name.begin(), name.end(), ']'), name.end());
		std::replace( name.begin(), name.end(), '.', '_');
		ParticleData data = particleData.second;
		G4int count = data.fCount;
		G4double meanLife = data.fTmean;
		stream
			<< "[" << name << "]\n"
			<< "count = " << count << "\n"
			<< "stable = " << ((meanLife >= 0.) ? "false" : "true") << "\n"
			<< "half_life = " << ((meanLife >= 0.) ? meanLife * 1e-9: NAN) << "\n";
		if (meanLife >= 0.)
			stream << "human_readable_half_life = \""  << G4BestUnit(meanLife, "Time") << "\"\n";
		stream << "\n";
	}
  return;
}

#endif
