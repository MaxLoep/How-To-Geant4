#include <map>
#include <mutex>
#include <string>
#include <tuple>
#include <vector>
typedef unsigned int uint;

#ifndef CONFIGSTRUCTS
#define CONFIGSTRUCTS

namespace ConfigStructs {
	struct NTuple {
		std::string name, title = "";
		std::vector<std::string> members;
	};

	struct Histogram {
		std::string name, title = "";
		uint nbins;
		double xmin, xmax;
		std::string unit_name = "none";
		std::string fcn_name = "none";
	};

	enum ParticleProperty {
		Ekin, local_pos_x,  local_pos_y,  local_pos_z, time, theta, phi
	};

	struct DetectionInfo {
		std::string particle_kind;
		int ntuple;
		std::vector<std::tuple<ParticleProperty, int>> ntuple_spec;
	};

	struct SDConfig {
		const std::string name, logical_volume;
		std::vector<DetectionInfo> log_properties = std::vector<DetectionInfo>();
	};

	enum ParticleKind {
		ion, other
	};

	struct ParticleSpec {
		std::string name;
		ParticleKind kind;
		uint protons, nucleons, electrons; //only relevant for ions

		ParticleSpec(std::string description, uint protons = 0, uint nucleons = 0){
			this->name = description;
			this->kind = nucleons == 0? ParticleKind::other: ParticleKind::ion;
			this->protons = protons;
			this->nucleons = nucleons;
		};
	};

	struct PSConf {
		const std::string name, logical_volume, quantity;
		bool filtered;
		ParticleSpec filter_for;
	};

	std::vector<std::string> property_strings(std::vector<ParticleProperty> props);

	struct RunActionConf {
		std::vector<NTuple> tuples = std::vector<NTuple>();
		std::vector<Histogram> histograms = std::vector<Histogram>();

		void add_analysis(NTuple tuple) {
			this->tuples.push_back(tuple);
		}

		void add_analysis(Histogram hist) {
			this->histograms.push_back(hist);
		}

		DetectionInfo add_detection_info(
			std::string name, std::string particle, std::vector<ParticleProperty> properties
		) {
			NTuple tuple{
				.name = name,
				.title = "",
				.members = property_strings(properties),
			};

			this->add_analysis(tuple);

			DetectionInfo det_info;
			det_info.ntuple = this->tuples.size() - 1;
			det_info.particle_kind = particle;
			int col = 0;
			for (auto p : properties) {
				det_info.ntuple_spec.push_back({p, col});
				++ col;
			}

			return det_info;
		};
	};

	struct EOEConfig {
		std::vector<std::tuple<std::string, int, int>> ps_tuple_targets
			= std::vector<std::tuple<std::string, int, int>>();
		std::vector<std::tuple<std::string, int>> ps_hist_targets
			= std::vector<std::tuple<std::string, int>>();
	};

	struct GlobalConf {
		public:
		RunActionConf ra_conf;
		EOEConfig eoe_conf;
		std::vector<SDConfig> sd_conf = std::vector<SDConfig>();
		std::vector<PSConf> ps_conf = std::vector<PSConf>();
		std::map<std::string, std::map<std::string, int>> sd_counts;
		std::lock_guard<std::mutex> lock() {
			return std::lock_guard<std::mutex>(this->m);
		}

		std::map<std::string, double> misc_doubles = std::map<std::string, double>();

		private:
		std::mutex m;
	};
}

#endif
