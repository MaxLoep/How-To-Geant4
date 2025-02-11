#include <string>
#include <vector>
typedef unsigned int uint;

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

	struct RunActionConf {
		std::vector<NTuple> tuples = std::vector<NTuple>();
		std::vector<Histogram> histograms = std::vector<Histogram>();

		void add_analysis(NTuple tuple) {
			this->tuples.push_back(tuple);
		}

		void add_analysis(Histogram hist) {
			this->histograms.push_back(hist);
		}
	};
}
