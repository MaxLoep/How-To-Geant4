/*
Understand what this does and comment it
Primitive Scorer data are saved here
*/

#include "EventAction.hh"

#include "Run.hh"
#include "Analysis.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"

#include "RunAction.hh"

#include "Analysis.hh"
#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"
#include <iomanip>

#include "G4Track.hh"
#include "G4StepStatus.hh"
#include "G4ParticleTypes.hh"

#include "ConfigStructs.hh"

EventAction::EventAction():G4UserEventAction()
{ }

EventAction::~EventAction()
{ }

//PRIMITIVE SCORERS
//From example B4d

G4THitsMap<G4double>* EventAction::GetHitsCollection(G4int hcID, const G4Event* event) const {
	auto hitsCollection = static_cast<G4THitsMap<G4double>*>(event->GetHCofThisEvent()->GetHC(hcID));

	if ( ! hitsCollection ) {
		G4ExceptionDescription msg;
		msg << "Cannot access hitsCollection ID " << hcID;
		G4Exception("B4dEventAction::GetHitsCollection()",
			"MyCode0003", FatalException, msg);
	}

	return hitsCollection;
}

G4double EventAction::GetSum(G4THitsMap<G4double>* hitsMap) const
{
	G4double sumValue = 0.;
	for ( auto it : *hitsMap->GetMap() ) {
		// hitsMap->GetMap() returns the map of std::map<G4int, G4double*>
		sumValue += *(it.second);
	}
	return sumValue;
}

void EventAction::BeginOfEventAction(const G4Event*)
{ }

void EventAction::EndOfEventAction(const G4Event* event)
{
	static std::vector<std::tuple<int, int, int>> ps_tuple_targets;
	static std::vector<std::tuple<int, int>> ps_hist_targets;
	static bool eoe_conf_set = false;

	// fetch end of event config only once per thread for better performance
	if (not eoe_conf_set) {
		extern ConfigStructs::GlobalConf global_conf;
		auto _ = global_conf.lock();

		for (auto [ps, tuple, col] : global_conf.eoe_conf.ps_tuple_targets) {
			ps_tuple_targets.push_back({G4SDManager::GetSDMpointer()->GetCollectionID(ps), tuple, col});
		}

		for (auto [ps, hist] : global_conf.eoe_conf.ps_hist_targets) {
			ps_hist_targets.push_back({G4SDManager::GetSDMpointer()->GetCollectionID(ps), hist});
		}

		eoe_conf_set = true;
	}

	// get analysis manager
	auto analysisManager = G4AnalysisManager::Instance();

	for (auto [id, tuple, col] : ps_tuple_targets) {
		auto value = GetSum(GetHitsCollection(id, event));
		analysisManager->FillNtupleDColumn(tuple, col, value);
		analysisManager->AddNtupleRow(tuple);
	}

	for (auto [id, hist] : ps_hist_targets) {
		auto value = GetSum(GetHitsCollection(id, event));
		analysisManager->FillH1(hist, value);
	}

}
