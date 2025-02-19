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
		Ekin, local_pos_x,  local_pos_y,  local_pos_z, global_pos_x, global_pos_y, global_pos_z, time,
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

	struct GlobalConf {
		RunActionConf ra_conf;
		std::vector<SDConfig> sd_conf = std::vector<SDConfig>();
	};
}

#endif
