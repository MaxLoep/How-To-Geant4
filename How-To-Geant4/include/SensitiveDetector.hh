#ifndef SensitiveDetector_h
#define SensitiveDetector_h 1

#include "G4VSensitiveDetector.hh"

class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;

class SD1 : public G4VSensitiveDetector
{
  public:
    SD1(const G4String& name);
    virtual ~SD1();

    virtual void   Initialize(G4HCofThisEvent* hce);
    virtual G4bool ProcessHits(G4Step* step, G4TouchableHistory* history);
    virtual void   EndOfEvent(G4HCofThisEvent* hce);
};

class SD2 : public G4VSensitiveDetector
{
  public:
    SD2(const G4String& name);
    virtual ~SD2();

    virtual void   Initialize(G4HCofThisEvent* hce);
    virtual G4bool ProcessHits(G4Step* step, G4TouchableHistory* history);
    virtual void   EndOfEvent(G4HCofThisEvent* hce);
};

class SD3 : public G4VSensitiveDetector
{
  public:
    SD3(const G4String& name);
    virtual ~SD3();

    virtual void   Initialize(G4HCofThisEvent* hce);
    virtual G4bool ProcessHits(G4Step* step, G4TouchableHistory* history);
    virtual void   EndOfEvent(G4HCofThisEvent* hce);
};

class SD4 : public G4VSensitiveDetector
{
  public:
    SD4(const G4String& name);
    virtual ~SD4();

    virtual void   Initialize(G4HCofThisEvent* hce);
    virtual G4bool ProcessHits(G4Step* step, G4TouchableHistory* history);
    virtual void   EndOfEvent(G4HCofThisEvent* hce);
};

class SD5 : public G4VSensitiveDetector
{
  public:
    SD5(const G4String& name);
    virtual ~SD5();

    virtual void   Initialize(G4HCofThisEvent* hce);
    virtual G4bool ProcessHits(G4Step* step, G4TouchableHistory* history);
    virtual void   EndOfEvent(G4HCofThisEvent* hce);
};

class SphereSD : public G4VSensitiveDetector
{
  public:
    SphereSD(const G4String& name);
    virtual ~SphereSD();

    virtual void   Initialize(G4HCofThisEvent* hce);
    virtual G4bool ProcessHits(G4Step* step, G4TouchableHistory* history);
    virtual void   EndOfEvent(G4HCofThisEvent* hce);
};


#endif
