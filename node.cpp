// node.cpp

#include "node.h"

// ===============================================================
// ================= Node class ==================================
// ===============================================================

Node::Node(const Parameters& par) {
	// both FC and SPM
	int Lalp = par.get_Lalp();
	int Lseq = par.get_Lseq();

	for (int i = 0; i < Lseq; i++) {
		node.push_back( rand_int(Lalp) );
	}
}



Node::Node(const Parameters& par, const string& msg) {
	// SPM
	if (par.get_net_type() == "SPM") {
		int Lalp = par.get_Lalp();
		int Lseq = par.get_Lseq();
		
		int lo = Lalp - 2;
		int hi = Lalp - 1;
	
		for (int i = 0; i < Lseq; i++) {
			node.push_back( rand_int(lo, hi) );
		}
	}
	// FC
	else {
		int Nnet = par.get_Nnet();
		int Lseq = par.get_Lseq();
		
		int lo = par.get_cutoff().at(1);
		int hi = Nnet - 1;
		
		for (int i = 0; i < Lseq; i++) {
			node.push_back( rand_int(lo, hi) );
		}
	}
}



void Node::mutate(const Parameters& par) {
	int Lseq = par.get_Lseq();
	int Lalp = par.get_Lalp();

	int position = rand_int(Lseq);
	int letter = rand_int(Lalp);
	while ( letter == node.at(position) ) {
		letter = rand_int(Lalp);
	}
	node.at(position) = letter;
}



void Node::recombine(const Parameters& par, const Node& p1, const Node& p2) {
	// SPM only
	int Lseq = par.get_Lseq();
	
	// (0, 1, 2, ..., L-2, L-1)
	int breakpoint = rand_int(1, Lseq-1);
	
	for (int i = 0; i < breakpoint; i++) {
	    node.at(i) = p1.show().at(i);
	}
	for (int i = breakpoint; i < Lseq; i++) {
	    node.at(i) = p2.show().at(i);
	}
}


double Node::fitness(const Parameters& par) const {
	// score = sum of all letters
	int node_score = 0;
	for (auto letter : node) {
		node_score += letter;
	}
	// compare score to cutoffs to assign fitness
	int count = 0;
	for (auto cutoff : par.get_cutoff()) {
		if (node_score <= cutoff) {
			return par.get_fitness().at(count);
		}
		count++;
	}
}


// print Node object
std::ostream& operator<<(std::ostream& stream, const Node& node) {	
	stream << node.show();
	return stream;
}





// ===============================================================
// ================= necessary functions =========================
// ===============================================================

/*
template<class T>
std::ostream& operator<<(std::ostream& stream, const vector<T>& vec) {
	// print vector
	int last = vec.size() - 1;
	for (int i = 0; i < last; i++) {
		stream << vec.at(i) << ",";
	}
	stream << vec.at(last);
	return stream;
}
*/

int rand_int(const int min_val, const int max_val) {
	// returns random integer from closed interval [min_val, max_val]
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(min_val, max_val);
	return dis(gen);
}

int rand_int(const int val) {
	// returns random integer from closed interval [0, val-1]
	return rand_int(0, val-1);
}


double rand_double() {
	// returns random double from [0, 1)
	std::random_device rd;
	std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0, 1);
    return dis(gen);
}