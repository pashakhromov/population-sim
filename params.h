// params.h

#ifndef PARAMS_H
#define PARAMS_H

#include "csv_reader.h"

class Parameters
{
	protected:
		// Network
		// network type: FC or SPM
		string network_type;
		// sequence length
		int Lseq;
		// alphabet size
		int Lalp;
		// network size
		int Nnet;
		// mutation rate
		double mutation_rate;
		// recombination rate
		double recombination_rate;
		// fitness landscape
		vector<double> fitness;
		// fitness cutoff
		// if function(node) <= cutoff.at(i)
		// then node's fitness is fitness.at(i)
		vector<int> cutoff;
		// plane fractions
		vector<double> fractions;
		
		// Population
        // population size: const or bottleneck
		string scenario;
		int Npop;
        int Npop_lo;
        int Npop_hi;
		bool write_pop;

		// Sampling
		int sample_size;
		int Npart;
		vector<vector<int>> part;

		// Evolution
		// number of generations to steady state
		int Ngen;
        int Ngen_lo;
        int Ngen_hi;
		// number of iterations
		int Nitr;
		int Nepoch;
		int epoch_len;
		double top_frac;
	
	public:
		explicit Parameters(const string& paramFile = "param.txt", const string& partFile = "partitions.txt");

		const string& get_net_type() const {return network_type;}
		int get_Lseq() const {return Lseq;}
		int get_Lalp() const {return Lalp;}
		int get_Nnet() const {return Nnet;}
		double get_mu() const {return mutation_rate;}
		double get_rho() const {return recombination_rate;}
		const vector<double>& get_fitness() const {return fitness;}
		const vector<int>& get_cutoff() const {return cutoff;}
		const vector<double>& get_frac() const {return fractions;}

		const string& get_scenario() const {return scenario;}
		int get_Npop() const {return Npop;}
		int get_Npop_lo() const {return Npop_lo;}
		int get_Npop_hi() const {return Npop_hi;}

		int get_sample_size() const {return sample_size;}
		int get_Npart() const {return Npart;}
		const vector<vector<int>>& get_part() const {return part;}

		int get_Ngen() const {return Ngen;}
		int get_Ngen_lo() const {return Ngen_lo;}
		int get_Ngen_hi() const {return Ngen_hi;}

		int get_Nitr() const { return Nitr; }
		int get_Nepoch() const { return Nepoch; }
		double get_top_frac() const { return top_frac; }

		bool get_write_pop() const { return write_pop; }
};

// print Parameters object
std::ostream& operator<<(std::ostream& stream, const Parameters& par);

#endif