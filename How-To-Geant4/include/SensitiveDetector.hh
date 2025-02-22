#ifndef SensitiveDetector_h
#define SensitiveDetector_h 1

#include "G4VSensitiveDetector.hh"
#include "ConfigStructs.hh"

class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;

// Does every Sensitive Detector needs its own class? -> Simplify!
class SD : public G4VSensitiveDetector
{
  public:
    SD(const G4String& name);
    virtual ~SD();

    virtual void   Initialize(G4HCofThisEvent* hce);
    virtual G4bool ProcessHits(G4Step* step, G4TouchableHistory* history);
    virtual void   EndOfEvent(G4HCofThisEvent* hce);

    G4int oldTrackId;
    G4int currentTrackId;
    std::thread::id thread_id;  //does this needs to be here? can it be defined in SDX.cc? what is nicer coding?
};

class GenericSD : public G4VSensitiveDetector {
	public:
    GenericSD(ConfigStructs::SDConfig conf);
    virtual ~GenericSD();

    virtual void   Initialize(G4HCofThisEvent* hce);
    virtual G4bool ProcessHits(G4Step* step, G4TouchableHistory* history);
    virtual void   EndOfEvent(G4HCofThisEvent* hce);

    std::map<std::string, int> particle_map = {{"test", 1}};
    std::vector<ConfigStructs::DetectionInfo> log_properties;
    G4String name;
	G4int oldTrackId;
    G4int currentTrackId;
    std::thread::id thread_id;
};

class SD1 : public G4VSensitiveDetector
{
  public:
    SD1(const G4String& name);
    virtual ~SD1();

    virtual void   Initialize(G4HCofThisEvent* hce);
    virtual G4bool ProcessHits(G4Step* step, G4TouchableHistory* history);
    virtual void   EndOfEvent(G4HCofThisEvent* hce);

    G4int oldTrackId;
    G4int currentTrackId;
    std::thread::id thread_id;  //does this needs to be here? can it be defined in SDX.cc? what is nicer coding?
};

class SD2 : public G4VSensitiveDetector
{
  public:
    SD2(const G4String& name);
    virtual ~SD2();

    virtual void   Initialize(G4HCofThisEvent* hce);
    virtual G4bool ProcessHits(G4Step* step, G4TouchableHistory* history);
    virtual void   EndOfEvent(G4HCofThisEvent* hce);

    G4int oldTrackId;
    G4int currentTrackId;
    std::thread::id thread_id;  //does this needs to be here? can it be defined in SDX.cc? what is nicer coding?
};

class SD3 : public G4VSensitiveDetector
{
  public:
    SD3(const G4String& name);
    virtual ~SD3();

    virtual void   Initialize(G4HCofThisEvent* hce);
    virtual G4bool ProcessHits(G4Step* step, G4TouchableHistory* history);
    virtual void   EndOfEvent(G4HCofThisEvent* hce);

    G4int oldTrackId;
    G4int currentTrackId;
    std::thread::id thread_id;  //does this needs to be here? can it be defined in SDX.cc? what is nicer coding?
};

class SD4 : public G4VSensitiveDetector
{
  public:
    SD4(const G4String& name);
    virtual ~SD4();

    virtual void   Initialize(G4HCofThisEvent* hce);
    virtual G4bool ProcessHits(G4Step* step, G4TouchableHistory* history);
    virtual void   EndOfEvent(G4HCofThisEvent* hce);

    G4int oldTrackId;
    G4int currentTrackId;
    std::thread::id thread_id;  //does this needs to be here? can it be defined in SDX.cc? what is nicer coding?
};

class SD5 : public G4VSensitiveDetector
{
  public:
    SD5(const G4String& name);
    virtual ~SD5();

    virtual void   Initialize(G4HCofThisEvent* hce);
    virtual G4bool ProcessHits(G4Step* step, G4TouchableHistory* history);
    virtual void   EndOfEvent(G4HCofThisEvent* hce);

    G4int oldTrackId;
    G4int currentTrackId;
    std::thread::id thread_id;  //does this needs to be here? can it be defined in SDX.cc? what is nicer coding?
};

class SphereSD : public G4VSensitiveDetector
{
  public:
    SphereSD(const G4String& name);
    virtual ~SphereSD();

    virtual void   Initialize(G4HCofThisEvent* hce);
    virtual G4bool ProcessHits(G4Step* step, G4TouchableHistory* history);
    virtual void   EndOfEvent(G4HCofThisEvent* hce);

    G4int oldTrackId;
    G4int currentTrackId;
    std::thread::id thread_id;  //does this needs to be here? can it be defined in SDX.cc? what is nicer coding?
};


#endif
