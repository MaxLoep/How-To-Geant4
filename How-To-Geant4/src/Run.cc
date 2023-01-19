#include "Run.hh"
#include "DetectorConstruction.hh"
#include "PrimaryGeneratorAction.hh"
#include "Analysis.hh"

#include "G4Threading.hh"
#include "G4AutoLock.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

#include <filesystem>
namespace fs = std::filesystem;

// get folderName from where it is defined (RunAction.cc) - the really dirty way 
extern std::string folderName;
// Standard folder name for the 'ListOfGeneratedParticles' files
std::string ListFolder = "Lists of generated Particles";

//
//Functions for custom GUI and macro commands - see DetectorConstruction.hh, DetectorMessenger.cc, DetectorMessenger.hh
//
// void DetectorConstruction::SetOutputFolder(G4String OutFoldName)
// void DetectorConstruction::SetOutputFolder(std::string OutFoldName)
// {
// folderName = OutFoldName;             
// }

// mutex in a file scope
namespace {
  //Mutex to lock updating the global ion map
  G4Mutex ionIdMapMutex = G4MUTEX_INITIALIZER;
}  

std::map<G4String,G4int> Run::fgIonMap;
G4int Run::fgIonId = kMaxHisto1;


Run::Run(DetectorConstruction* det)
: G4Run(),
  fDetector(det), fParticle(nullptr), fEkin(0.)
{
  fEnergyDeposit = fEnergyDeposit2 = 0.;
  fEnergyFlow    = fEnergyFlow2    = 0.;
}


Run::~Run()
{ }


void Run::Merge(std::map<G4String, ParticleData>& destinationMap,
                const std::map<G4String, ParticleData>& sourceMap) const
{
  for ( const auto& particleData : sourceMap ) {
    G4String name = particleData.first;
    const ParticleData& localData = particleData.second;   
    if ( destinationMap.find(name) == destinationMap.end()) {
      destinationMap[name]
       = ParticleData(localData.fCount, 
                      localData.fEmean, 
                      localData.fEmin, 
                      localData.fEmax,
                      localData.fTmean);
    }
    else {
      ParticleData& data = destinationMap[name];   
      data.fCount += localData.fCount;
      data.fEmean += localData.fEmean;
      G4double emin = localData.fEmin;
      if (emin < data.fEmin) data.fEmin = emin;
      G4double emax = localData.fEmax;
      if (emax > data.fEmax) data.fEmax = emax;
      data.fTmean = localData.fTmean;
    }   
  }
}


void Run::SetPrimary(G4ParticleDefinition* particle, G4double energy)
{ 
  fParticle = particle;
  fEkin = energy;
}
 

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
                  

void Run::ParticleCount(G4String name, G4double Ekin, G4double meanLife)
{
  std::map<G4String, ParticleData>::iterator it = fParticleDataMap1.find(name);
  if ( it == fParticleDataMap1.end()) {
    fParticleDataMap1[name] = ParticleData(1, Ekin, Ekin, Ekin, meanLife);
  }
  else {
    ParticleData& data = it->second;
    data.fCount++;
    data.fEmean += Ekin;
    //update min max
    G4double emin = data.fEmin;
    if (Ekin < emin) data.fEmin = Ekin;
    G4double emax = data.fEmax;
    if (Ekin > emax) data.fEmax = Ekin;
    data.fTmean = meanLife;
  }   
}
                 

void Run::AddEdep(G4double edep)
{ 
  fEnergyDeposit += edep;
  fEnergyDeposit2 += edep*edep;
}
                 

void Run::AddEflow(G4double eflow)
{ 
  fEnergyFlow += eflow;
  fEnergyFlow2 += eflow*eflow;
}                  

void Run::ParticleFlux(G4String name, G4double Ekin)
{
  std::map<G4String, ParticleData>::iterator it = fParticleDataMap2.find(name);
  if ( it == fParticleDataMap2.end()) {
    fParticleDataMap2[name] = ParticleData(1, Ekin, Ekin, Ekin, -1*ns);
  }
  else {
    ParticleData& data = it->second;
    data.fCount++;
    data.fEmean += Ekin;
    //update min max
    G4double emin = data.fEmin;
    if (Ekin < emin) data.fEmin = Ekin;
    G4double emax = data.fEmax;
    if (Ekin > emax) data.fEmax = Ekin;
    data.fTmean = -1*ns;
  }   
}

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
  //
  fParticle = localRun->fParticle;
  fEkin     = localRun->fEkin;
  
  // accumulate sums
  //
  fEnergyDeposit   += localRun->fEnergyDeposit;  
  fEnergyDeposit2  += localRun->fEnergyDeposit2;
  fEnergyFlow      += localRun->fEnergyFlow;
  fEnergyFlow2     += localRun->fEnergyFlow2;
      
  //map: processes count
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
  
  //map: created particles count
  Merge(fParticleDataMap1, localRun->fParticleDataMap1);    
  
  //map: particles flux count       
  Merge(fParticleDataMap2, localRun->fParticleDataMap2);    
  
  G4Run::Merge(run); 
} 


void Run::EndOfRun() 
{
  G4int prec = 5, wid = prec + 2;  
  G4int dfprec = G4cout.precision(prec);
  
  //run condition
  //
  G4Material* material = fDetector->GetAbsorMaterial();
  G4double density = material->GetDensity();
   
  G4String Particle = fParticle->GetParticleName();    
  G4cout << "\n The run is " << numberOfEvent << " "<< Particle 
         //<< " of " << G4BestUnit(fEkin,"Energy") 
         << " through " 
         << G4BestUnit(fDetector->GetAbsorThickness(),"Length") << " of "
         << material->GetName() << " (density: " 
         << G4BestUnit(density,"Volumic Mass") << ")" << G4endl;

  if (numberOfEvent == 0) { G4cout.precision(dfprec);   return;}

  //frequency of processes
  //
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
  
  //List of generated particles to console 
  //
  G4cout << "\n List of generated particles:" << G4endl;
     
 for ( const auto& particleData : fParticleDataMap1 ) {
    G4String name = particleData.first;
    ParticleData data = particleData.second;
    G4int count = data.fCount;
    G4double eMean = data.fEmean/count;
    G4double eMin = data.fEmin;
    G4double eMax = data.fEmax;
    G4double meanLife = data.fTmean;
         
    G4cout << "  " << std::setw(13) << name << ": " << std::setw(7) << count
           << "  Emean = " << std::setw(wid) << G4BestUnit(eMean, "Energy")
           << "\t( "  << G4BestUnit(eMin, "Energy")
           << " --> " << G4BestUnit(eMax, "Energy") << ")";
    if (meanLife >= 0.)
      G4cout << "\thalf life = " << G4BestUnit(meanLife, "Time")   << G4endl;
    else G4cout << "\tstable" << G4endl;
 }
   
  //List of generated particles to file
  //
  // Create an output file which increases in number if the simulation is run again

  // create a folder for the files
  // std::string folderName = "Lists of Generated Particles";
  // std::filesystem::create_directory(folderName);
  // std::filesystem::create_directory(folderName + "/" + ListFolder);
  fs::create_directory(folderName);
  fs::create_directory(folderName + "/" + ListFolder);

  //Get process ID
  G4long pid = getpid();

  // Check if "ListOfGeneratedParticles_pid.txt" is already existing; if yes, check if "ListOfGeneratedParticles_pid+1.txt" exists. 
  while(std::ifstream(folderName + "/" + ListFolder + "/" + "ListOfGeneratedParticles_" + std::to_string(pid) + ".txt"))
  {
    pid++;
  }
  // Set final file name 
  std::string fileName = "ListOfGeneratedParticles_" + std::to_string(pid) + ".txt";

  // flush output to file
  std::ofstream outFile(folderName + "/" + ListFolder + "/" + fileName);
  // std::ofstream outFile(fileName);

  outFile << "\n List of generated particles:" << G4endl;
     
 for ( const auto& particleData : fParticleDataMap1 ) {
    G4String name = particleData.first;
    ParticleData data = particleData.second;
    G4int count = data.fCount;
    G4double eMean = data.fEmean/count;
    G4double eMin = data.fEmin;
    G4double eMax = data.fEmax;
    G4double meanLife = data.fTmean;

    // tabulator divided for a more handy output
    outFile << "  " << std::setw(13) << name << "\t" << std::setw(7) << count
           << "\t  Emean = " << std::setw(wid) << G4BestUnit(eMean, "Energy")
           << "\t( "  << G4BestUnit(eMin, "Energy")
           << " --> " << G4BestUnit(eMax, "Energy") << ")";
    if (meanLife >= 0.)
      outFile << "\thalf life = \t" << G4BestUnit(meanLife, "Time")   << G4endl;
    else outFile << "\tstable\tstable" << G4endl;
 }

  // compute mean Energy deposited and rms
  //
  G4int TotNbofEvents = numberOfEvent;
  fEnergyDeposit /= TotNbofEvents; fEnergyDeposit2 /= TotNbofEvents;
  G4double rmsEdep = fEnergyDeposit2 - fEnergyDeposit*fEnergyDeposit;
  if (rmsEdep>0.) rmsEdep = std::sqrt(rmsEdep);
  else            rmsEdep = 0.;
  
  G4cout << "\n Mean energy deposit per event = "
         << G4BestUnit(fEnergyDeposit,"Energy") << ";  rms = "
         << G4BestUnit(rmsEdep,      "Energy") 
         << G4endl;
  
  // compute mean Energy flow and rms
  //
  fEnergyFlow /= TotNbofEvents; fEnergyFlow2 /= TotNbofEvents;
  G4double rmsEflow = fEnergyFlow2 - fEnergyFlow*fEnergyFlow;
  if (rmsEflow>0.) rmsEflow = std::sqrt(rmsEflow);
  else             rmsEflow = 0.;
  
  G4cout << " Mean energy flow per event    = "
         << G4BestUnit(fEnergyFlow,"Energy") << ";  rms = "
         << G4BestUnit(rmsEflow,   "Energy") 
         << G4endl;

 //particles flux
 //
 G4cout << "\n List of particles emerging from the target :" << G4endl;
     
 for ( const auto& particleData : fParticleDataMap2 ) {
    G4String name = particleData.first;
    ParticleData data = particleData.second;
    G4int count = data.fCount;
    G4double eMean = data.fEmean/count;
    G4double eMin = data.fEmin;
    G4double eMax = data.fEmax;
    G4double Eflow = data.fEmean/TotNbofEvents;        
         
    G4cout << "  " << std::setw(13) << name << ": " << std::setw(7) << count
           << "  Emean = " << std::setw(wid) << G4BestUnit(eMean, "Energy")
           << "\t( "  << G4BestUnit(eMin, "Energy")
           << " --> " << G4BestUnit(eMax, "Energy") 
           << ") \tEflow/event = " << G4BestUnit(Eflow, "Energy") << G4endl;
 }


  //remove all contents in fProcCounter, fCount 
  fProcCounter.clear();
  fParticleDataMap1.clear();
  fParticleDataMap2.clear();
  fgIonMap.clear();
                          
  //restore default format         
  G4cout.precision(dfprec);   
}

