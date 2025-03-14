#ifndef SensitiveDetector_h
#define SensitiveDetector_h

#include "G4VSensitiveDetector.hh"
#include "ConfigStructs.hh"

class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;

class GenericSD : public G4VSensitiveDetector {
	public:
    GenericSD(ConfigStructs::SDConfig conf);
    virtual ~GenericSD();

    virtual void   Initialize(G4HCofThisEvent* hce);
    virtual G4bool ProcessHits(G4Step* step, G4TouchableHistory* history);
    virtual void   EndOfEvent(G4HCofThisEvent* hce);

    std::map<std::string, int> particle_map = std::map<std::string, int>();
    std::vector<ConfigStructs::DetectionInfo> log_properties;
    G4String name;
	G4int oldTrackId;
    G4int currentTrackId;
    std::thread::id thread_id;
};

#endif
