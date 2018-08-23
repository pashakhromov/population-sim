// params.cpp

#include "params.h"


Parameters::Parameters(const string& paramFile, const string& partFile)
{
    auto dataList = CSVReader(paramFile).getData();
    for(const auto& vec : dataList)
	{
        // string
		if (vec[0] == "net_type") {
            network_type = vec[1];
        }
        if (vec[0] == "scenario") {
            scenario = vec[1];
        }
        // bool
        if (vec[0] == "write_pop") {
            if (vec[1] == "1") {
                write_pop = true;
            }
            if (vec[1] == "0") {
                write_pop = false;
            }
        }
        // int
        if (vec[0] == "sample_size") {
            sample_size = stoi(vec[1]);
        }
        if (vec[0] == "Lalp") {
            Lalp = stoi(vec[1]);
        }
        if (vec[0] == "Lseq") {
            Lseq = stoi(vec[1]);
        }
        if (vec[0] == "Npop") {
            Npop = stoi(vec[1]);
        }
        if (vec[0] == "Npop_lo") {
            Npop_lo = stoi(vec[1]);
        }
        if (vec[0] == "Npop_hi") {
            Npop_hi = stoi(vec[1]);
        }
        if (vec[0] == "Ngen") {
            Ngen = stoi(vec[1]);
        }
        if (vec[0] == "Ngen_lo") {
            Ngen_lo = stoi(vec[1]);
        }
        if (vec[0] == "Ngen_hi") {
            Ngen_hi = stoi(vec[1]);
        }
        if (vec[0] == "Nitr") {
            Nitr = stoi(vec[1]);
        }
        if (vec[0] == "Npart") {
            Npart = stoi(vec[1]);
        }
        if (vec[0] == "Nepoch") {
            Nepoch = stoi(vec[1]);
        }
        // double
        if (vec[0] == "mu") {
            mutation_rate = stod(vec[1]);
        }
        if (vec[0] == "rho") {
            recombination_rate = stod(vec[1]);
        }
        if (vec[0] == "top_frac") {
            top_frac = stod(vec[1]);
        }
        // vector
        if (vec[0] == "fitness") {
            for (int i = 1; i < vec.size(); i++) {
                fitness.push_back( stod(vec[i]) );
            }
        }
        if (vec[0] == "gamma") {
            for (int i = 1; i < vec.size(); i++) {
                fractions.push_back( stod(vec[i]) );
            }
        }
	}

	// network size (Nnet)
	Nnet = static_cast<int>( std::round( std::pow(Lalp, Lseq) ) );
	
	// cutoff
	// SPM
	if (network_type == "SPM") {
		cutoff.push_back( (Lalp-1)*Lseq - 2*(Lalp-1) - 7 );
		cutoff.push_back( (Lalp-1)*Lseq - 1*(Lalp-1) - 6 );
		cutoff.push_back( (Lalp-1)*Lseq );
    }
	// FC
	else {
		try {
			// cutoff
			int cumulant = 0;
            for (const auto& f : fractions) {
                cumulant += static_cast<int>( std::round(Nnet * f) );
                cutoff.push_back(cumulant);
            }
			// check whether cutoffs sum up to Nnet
			if (cumulant != Nnet) {
				throw	"ERROR when initializing Network object:\n"
						"Cutoffs don't sum up to Nnet.";
            }
        }
		catch(const char* message){
			cout << message << endl;
			std::exit(1);
        }
    }

    auto partList = CSVReader(partFile).getData();
	for(const auto& vec : partList) {
        vector<int> p;
        for (const auto& v : vec) {
            p.push_back( stoi(v) );
        }
        part.push_back(p);
    }
}

// print Network object
std::ostream& operator<<(std::ostream& stream, const Parameters& param)
{
    cout << "net_type " << param.get_net_type() << "\n";
    cout << "scenario " << param.get_scenario() << "\n";

    cout << "Lalp = " << param.get_Lalp() << "\n";
    cout << "Lseq = " << param.get_Lseq() << "\n";

    cout << "Npop = " << param.get_Npop() << "\n";
    cout << "Npop_lo = " << param.get_Npop_lo() << "\n";
    cout << "Npop_hi = " << param.get_Npop_hi() << "\n";

    cout << "Ngen = " << param.get_Ngen() << "\n";
    cout << "Ngen_lo = " << param.get_Ngen_lo() << "\n";
    cout << "Ngen_hi = " << param.get_Ngen_hi() << "\n";

    cout << "Nitr = " << param.get_Nitr() << "\n";

    cout << "Npart = " << param.get_Npart() << "\n";
    cout << "Nepoch = " << param.get_Nepoch() << "\n";

    cout << "mu = " << param.get_mu() << "\n";
    cout << "rho = " << param.get_rho() << "\n";
    cout << "top_frac = " << param.get_top_frac() << "\n";

    cout << "fitness\n";
    for (const auto& f : param.get_fitness()) {
        cout << f << " ";
    }

    cout << "\nfractions\n";
    for (const auto& g : param.get_frac()) {
        cout << g << " ";
    }
    
    cout << "\nPartitions\n";
    for (const auto& p : param.get_part()) {
        for (const auto& i : p) {
            cout << i << ", ";
        }
        cout << "\n";
    }
	return stream;
}
