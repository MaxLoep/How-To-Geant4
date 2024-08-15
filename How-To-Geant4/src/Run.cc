/*
REWORK NEEDED!
TO-DO'S:
- nothing right now
*/

#include "platform.hh"
#include "Run.hh"
#include "DetectorConstruction.hh"
#include "PrimaryGeneratorAction.hh"
#include "Analysis.hh"

#include "G4Threading.hh"
#include "G4AutoLock.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

#include <cmath>
#include <filesystem>
#include <regex>
namespace fs = std::filesystem;

// get folderName from where it is defined (RunAction.cc) - the really dirty way
extern std::string folderName;
// Standard folder name for the 'ListOfGeneratedParticles' files
std::string ListFolder = "Lists_of_generated_Particles";

// mutex in a file scope
namespace 
{
	//Mutex to lock updating the global ion map
	G4Mutex ionIdMapMutex = G4MUTEX_INITIALIZER;
}

std::map<G4String,G4int> Run::fgIonMap;
G4int Run::fgIonId = kMaxHisto1;

// old code from example -> can be removed?
Run::Run(DetectorConstruction* det) : G4Run(), fDetector(det), fParticle(nullptr), fEkin(0.)
{ }

Run::~Run()
{ }

void Run::Merge(std::map<G4String, ParticleData>& destinationMap,
								const std::map<G4String, ParticleData>& sourceMap) const
{
	for ( const auto& particleData : sourceMap ) {
		G4String name = particleData.first;
		const ParticleData& localData = particleData.second;
		if ( destinationMap.find(name) == destinationMap.end()) {
			destinationMap[name] = ParticleData(
				localData.fCount,
				localData.fTmean
			);
		} else {
			ParticleData& data = destinationMap[name];
			data.fCount += localData.fCount;
			data.fTmean = localData.fTmean;
		}
	}
}

void Run::SetPrimary(G4ParticleDefinition* particle, G4double energy) {
	fParticle = particle;
	fEkin = energy;
}

// Function to count processes and fill the map fProcCounter
void Run::CountProcesses(const G4VProcess* process) 
{
	G4String procName = process->GetProcessName();
	std::map<G4String,G4int>::iterator it = fProcCounter.find(procName);
	if ( it == fProcCounter.end()) {
		fProcCounter[procName] = 1;
	}
	else {
		fProcCounter[procName]++;
	}
}

// Function to count particles and fill the map fParticleDataMap1
void Run::ParticleCount(G4String name, G4double meanLife)
{
	std::map<G4String, ParticleData>::iterator it = fParticleDataMap1.find(name);
	if ( it == fParticleDataMap1.end()) {
		fParticleDataMap1[name] = ParticleData(1, meanLife);
	}
	else {
		ParticleData& data = it->second;
		data.fCount++;
		data.fTmean = meanLife;
	}
}

// Function to count particles leaving the world volumeand fill the map fParticleDataMap2
void Run::ParticleFlux(G4String name)
{
	std::map<G4String, ParticleData>::iterator it = fParticleDataMap2.find(name);
	if ( it == fParticleDataMap2.end()) {
		fParticleDataMap2[name] = ParticleData(1, -1*ns);
	}
	else {
		ParticleData& data = it->second;
		data.fCount++;
		data.fTmean = -1*ns;
	}
}

// WHAT DOES THIS DO? SEEMS TO COUNT IONS OR SOMETHING... 
G4int Run::GetIonId(G4String ionName)
{
	G4AutoLock lock(&ionIdMapMutex);
	// updating the global ion map needs to be locked

	std::map<G4String,G4int>::const_iterator it = fgIonMap.find(ionName);
	if ( it == fgIonMap.end()) {
		fgIonMap[ionName] = fgIonId;
		if (fgIonId < (kMaxHisto2 - 1)) fgIonId++;
	}
	return fgIonMap[ionName];
}

void Run::Merge(const G4Run* run)
{
	const Run* localRun = static_cast<const Run*>(run);

	//primary particle info
	fParticle = localRun->fParticle;
	fEkin     = localRun->fEkin;

	//merge map for processe count
	for ( const auto& procCounter : localRun->fProcCounter ) {
		G4String procName = procCounter.first;
		G4int localCount = procCounter.second;
		if ( fProcCounter.find(procName) == fProcCounter.end()) {
			fProcCounter[procName] = localCount;
		}
		else {
			fProcCounter[procName] += localCount;
		}
	}

	// merge map for count for created particles
	Merge(fParticleDataMap1, localRun->fParticleDataMap1);

	// merge map for count for particles leaving World volume
	Merge(fParticleDataMap2, localRun->fParticleDataMap2);

	G4Run::Merge(run);
}


template <typename Ostream>
void Run::OutputParticleData(std::map<G4String, ParticleData>& particle_map, Ostream& stream) 
{
	// type and number of primary particles used in this run
	G4String Particle = fParticle->GetParticleName();
	stream
		<< "[" << "Primaries_" << Particle  << "]\n"
		<< "count = " << numberOfEvent << "\n"
		<< "stable = " << "true" << "\n"
		<< "half_life = " <<  NAN << "\n";
		stream << "\n";

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
}

void Run::EndOfRun()
{
	G4int prec = 5; //, wid = prec + 2;
	G4int dfprec = G4cout.precision(prec);

	// generate a string to console what the primary particles of the run were, like:
	// 'The run consisted of X PARTICLENAME as primary particles'
	G4String Particle = fParticle->GetParticleName();
	G4cout  << "\n The run consisted of " << numberOfEvent << " "<< Particle
			<<"s"
			// << " of " << G4BestUnit(fEkin,"Energy")
			<<" as primary particles"
			<< G4endl;

	if (numberOfEvent == 0) { G4cout.precision(dfprec);   return;}

	//print the frequency of processes to console
	G4cout << "\n Process calls frequency :" << G4endl;
	G4int index = 0;
	for ( const auto& procCounter : fProcCounter ) {
		 G4String procName = procCounter.first;
		 G4int    count    = procCounter.second;
		 G4String space = " "; if (++index%3 == 0) space = "\n";
		 G4cout << " " << std::setw(20) << procName << "="<< std::setw(7) << count
						<< space;
	}
	G4cout << G4endl;

	//List of generated particles: to console (the old style)
	G4cout << "\n List of generated particles:" << G4endl;
	for ( const auto& particleData : fParticleDataMap1 )
	{
		G4String name = particleData.first;
		ParticleData data = particleData.second;
		G4int count = data.fCount;
		G4double meanLife = data.fTmean;

		G4cout << "  " << std::setw(13) << name << ": " << std::setw(7) << count;
		if (meanLife >= 0.)
			G4cout << "\thalf life = " << G4BestUnit(meanLife, "Time")   << G4endl;
		else G4cout << "\tstable" << G4endl;
	}

	//List of generated Particles (LogP) to file
	// create a folder for the files
	fs::create_directory(folderName);
	fs::create_directory(folderName + "/" + ListFolder);

	// get epoch time and system clock nanosecond value that were used as seeds in main() to create file name
	G4long time 	= G4Random::getTheSeeds()[0];
	G4long time_ns 	= G4Random::getTheSeeds()[1];
	// set file name
	std::string fileName = std::to_string(time) + "_" + std::to_string(time_ns) + "_LogP" + ".txt";

	// flush output to file
	std::ofstream outFile(folderName + "/" + ListFolder + "/" + fileName);

	// List of generated Particles in TOTAL to file
	OutputParticleData(fParticleDataMap1, outFile);

	// List of generated Particles leaving the World volume to console
	OutputParticleData(fParticleDataMap2, G4cout);
	
	//remove all contents in fProcCounter, fCount
	fProcCounter.clear();
	fParticleDataMap1.clear();
	fParticleDataMap2.clear();
	fgIonMap.clear();

	//restore default format
	G4cout.precision(dfprec);
}