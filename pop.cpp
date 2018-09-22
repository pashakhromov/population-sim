// pop.cpp

#include "pop.h"

// === Constructors ===

Population::Population(const Parameters& p, const string& fname) {
	par = p;

	for (const auto& part : p.get_part()) {
		prob_counts[part] = 0;
	}

	if (fname == "") {
		this->rand_pop();
	}
	else {
		read(fname);
	}
}

void Population::read(const string& fname) {
	auto dataList = CSVReader(fname).getData();
	for(const auto& vec : dataList) {
		vector<int> n;
		for (const auto& v : vec) {
			n.push_back( stoi(v) );
		}
		pop.push_back( Node {n} );
	}
}

void Population::rand_pop() {
	// create a new population of size Parameters::Npop
	int Npop = par.get_Npop();
	double top_frac = par.get_top_frac();

	if (top_frac < 1e-10) {
		for (int i = 0; i < Npop; i++) {
			pop.push_back( Node {par} );
		}
	}
	else {
		int n1 = static_cast<int>( Npop * top_frac );
		int n2 = Npop - n1;
		
		// fraction n1/Npop of the population is on the higher planes
		for (int i = 0; i < n1; i++) {
			pop.push_back( Node {par, "top"} );
		}
		// fraction n2/Npop of the population is random
		for (int i = 0; i < n2; i++) {
			pop.push_back( Node {par} );
		}
	}
}



// === Functionality ===

double Population::fitness() const {
	double pop_fit = 0.0;
	for (const auto& node : pop) {
		pop_fit += node.fitness(par);
	}
	return pop_fit;
}

int Population::pick_parent() const {
	double r = this->fitness() * rand_double();
	double max = 0.0;
		
	int index = 0;
	for (const auto& node : pop) {
		max += node.fitness(par);
		if (r <= max) {
			return index;
		}
		index++;
	}
}

void Population::next_gen() {
	// updates population
	// Moran model
	
	int p_idx = this->pick_parent();
	Node offspring { pop.at(p_idx) };
	
	// recombine
	double rho = par.get_rho();
	double r_rho = rand_double();
	if (rho > 1e-10 and r_rho <= rho and par.get_net_type() == "SPM") {
		// pick 2 distinct parents
		int pp_idx = this->pick_parent();
		while (p_idx == pp_idx) {
			pp_idx = this->pick_parent();
		}
		offspring.recombine(par, pop.at(p_idx), pop.at(pp_idx));
		//cout << "recombine " << p_idx << " and " << pp_idx << endl;
	}
	
	// mutate
	double r_mu = rand_double();
	if (r_mu <= par.get_mu()) {
		offspring.mutate(par);
		//cout << "mutate " << p_idx << endl;
	}
	
	// replace
	int r_idx = rand_int( pop.size() );
	//cout << r_idx << " is replaced\n";
	pop.at(r_idx) = offspring;
}

void Population::evolve(int N) {
	for (int i = 0; i < N; i++) {
		this->next_gen();
	}
}

const Node& Population::rand_node() const {
	return pop.at( rand_int( pop.size() ) );
}

void Population::take_sample() {
	// count: Node -> ni
	map<Node, int> count;
	for (int i = 0; i < par.get_sample_size(); i++) {
		++count[ this->rand_node() ];
	}

	// n-counts
	vector<int> n_counts;
	for (const auto& c : count) {
		n_counts.push_back(c.second);
	}
	std::sort(n_counts.begin(), n_counts.end(), std::greater<int>());

	// update probability counts
	++prob_counts[n_counts];

	//cout << "Print n-counts\n" << n_counts << endl;
}

void Population::resize(int N) {
	if (pop.size() >= N) {
		pop.resize(N);
	}
	// to get pop_hi sample unifromly Npop_hi times from pop_lo
	else {
		vector<Node> new_pop(N);
		for (int i = 0; i < N; i++) {
			int p = rand_int( pop.size() );
			new_pop.at(i) = pop.at(p);
		}
		pop = new_pop;
	}
}

int Population::seg_sites(int n) const {
	int S = 0;
	int Lseq = par.get_Lseq();
	int Npop = pop.size();
	if (n > Npop or n <= 0) {
		n = Npop;
	}
	for (int pos = 0; pos < Lseq; pos++) {
		int val = pop.at(0).show().at(pos);
		for (int i = 1; i < n; i++) {
			if (pop.at(i).show().at(pos) != val) {
				S++;
				break;
			}
		}
	}
	return S;
}

double Population::Tajima(int n) const {
	int Npop = pop.size();
	if (n > Npop or n <= 0) {
		n = Npop;
	}
	double a1 = 0.0;
	for (int i = 1; i < n; i++) {
		a1 += 1.0 / i;
	}
	return seg_sites(n) / a1;
}



// === Printing an returning members ===

void Population::print(const string& msg) const {
	cout << msg << "\n";
	for (const auto& node : pop) {
		cout << node << "\n";
	}
}

void Population::write(const string& fname) const {
	std::ofstream f;
	f.open (fname);
	for (const auto& p : pop) {
		f << p << "\n";
	}
	f.close();
}

void Population::print_prob_counts(const string& msg) const {
	cout << msg << "\n";
	for (const auto& c : prob_counts) {
		cout << "{" << c.first << "}:" << c.second << "\n";
	}
}

void Population::write_prob_counts(const string& fname) const {
	std::ofstream f;
	f.open (fname);
	for (const auto& c : prob_counts) {
		f << "{" << c.first << "}:" << c.second << "\n";
	}
	f.close();
}

PopStat::PopStat(const Parameters& par) {
	epoch = 0;
	epoch_len = par.get_Ngen() / par.get_Nepoch();
	mean_fitness.assign( par.get_Nepoch(), 0.0 );
}

void PopStat::write(const Population& pop, int itr) const {
	// write to mean fitness file mean_fitness.at(epoch)
	// is a result of summation of (itr+1) terms
	// each of which is population fitness at given epoch;
	// to turn this number into mean fitness
	// we divide it by (itr+1)*Npop
	std::ofstream f;
	f.open("mean_fitness_ss.txt");
	for (const auto& mf : mean_fitness) {
		f << mf / ( pop.get_Npop() * (itr + 1) ) << "\n";
	}
	f.close();
}

void PopStat::update(const Population& pop, int gen, int itr) {
	if (gen % epoch_len == 0) {
		// population fitness at epoch is calculated and added
		// to mean_fitness.at(epoch) this happens Nitr times
		mean_fitness.at(epoch) += pop.fitness();
		++epoch;

		this->write(pop, itr);
	}	
}