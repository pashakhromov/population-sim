// pop.h

#ifndef POP_H
#define POP_H	

#include "node.h"
#include <map>
using std::map;

class Population
{
	private:
		vector<Node> pop;
		Parameters par;
		map<vector<int>, int> prob_counts;
	
	public:
		/* It's a good idea to NOT create a new Population
		object every iteration cycle but to
		renew the Population object instad while keeping prob_counts.
		New population should be created with Parameters::Npop
		rather than actual population size. It is relevant
		for bottleneck scenario. */

		// === Constructors ===
		explicit Population(const Parameters& p, const string& fname = "");

		// === Functionality ===
		// returns fitness of the population
		void rand_pop();
		double fitness() const;
		int pick_parent() const;
		void next_gen();
		void evolve(int N);
		const Node& rand_node() const;
		void take_sample();
		void resize(int N);

		// === Printing an returning members ===
		// returns Population
		const vector<Node>& show() const { return pop; }
		int get_Npop() const { return pop.size(); }
		// prints all Nodes in Population
		void print(const string& msg = "Population") const;
		void write(const string& fname = "pop.txt") const;
		void read(const string& fname);
		void print_prob_counts(const string& msg = "Probability counts") const;
		void write_prob_counts(const string& fname = "prob_counts.txt") const;
};

class PopStat {
	private:
		int epoch;
		int epoch_len;
		vector<double> mean_fitness;

	public:
		explicit PopStat(const Parameters& par);
		void write(const Population& pop, int itr) const;
		void update(const Population& pop, int gen, int itr);
		void reset() { epoch = 0; }
};

#endif