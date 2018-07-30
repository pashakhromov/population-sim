// node.h

#ifndef NODE_H
#define NODE_H	

#include "params.h"
#include <random>
#include <unordered_set>
#include <utility>

using namespace std::rel_ops;

// ===============================================================
// ================= necessary functions =========================
// ===============================================================

// print std::vector
template<class T>
std::ostream& operator<<(std::ostream& stream, const vector<T>& vec);

// random numbers
int rand_int(const int min_val, const int max_val);
int rand_int(const int val);
double rand_double();


// ===============================================================
// ================= Node class ==================================
// ===============================================================

class Node
{
	private:
		vector<int> node;
	
	public:
		// Constructors
		explicit Node(const vector<int>& v) : node {v} {}
		
		// Random node. Initialize as: Node n {par};
		explicit Node(const Parameters& par);
		
		// Random node with the highest fitness. Initialize as: Node n {par, "top"};
		explicit Node(const Parameters& par, const string& msg);
		
		explicit Node() { node.push_back(0); }
		
		// Copy constructor. Given Node m, initialized as: Node n {m};
		explicit Node(const Node& m) { node = m.show(); }

		// Show the node
		const vector<int>& show() const { return node; }
		
		// compare
		bool operator==(const Node& nd) const { return node == nd.show(); }
		bool operator<(const Node& nd) const { return node < nd.show(); }
		
		// assignment operator
		Node& operator=(const Node& nd) {
			node = nd.show();
			return *this;
		}
		
		void mutate(const Parameters& par);
		
		void recombine(const Parameters& par, const Node& p1, const Node& p2);
			
		double fitness(const Parameters& par) const;
};

// print Node object
std::ostream& operator<<(std::ostream& stream, const Node& node);

#endif