// simulation.cpp

#include "pop.h"
#include "clock.h"

int main()
{
    Clock clock {};
    Parameters par {};
	PopStat pop_stat {par};

    int Nitr = par.get_Nitr();
    int Ngen = par.get_Ngen();
	int Ngen_lo = par.get_Ngen_lo();
	int Ngen_hi = par.get_Ngen_hi();
    int Npop = par.get_Npop();
	int Npop_lo = par.get_Npop_lo();
	int Npop_hi = par.get_Npop_hi();

	if (par.get_scenario() == "time_ss") {
		for (int itr = 0; itr < Nitr; itr++) {
			clock.start();

			Population pop {par};
			pop_stat.reset();
			for (int gen = 0; gen < Ngen; gen++) {
				pop_stat.update(pop, gen, itr);
				pop.next_gen();
			}

			clock.stop();
			clock.write_stat("progress.txt");
		}
	}
	if (par.get_scenario() == "const") {
		Population pop {par};
		pop.evolve(Ngen);

		// time between samples
		int t_btw_smpl = static_cast<int>( 1.0 / par.get_mu() );
		
		// iteration 1
		pop.take_sample();

		// the rest Nitr-1 iterations
		for (int itr = 1; itr < Nitr; itr++) {
			clock.start();
		
			pop.evolve(t_btw_smpl);
			pop.take_sample();
			pop.write_prob_counts();
		
			clock.stop();
			clock.write_stat("progress.txt");
		}
	}
	if (par.get_scenario() == "bottleneck") {
		Population pop {par};

		for (int itr = 0; itr < Nitr; itr++) {
			clock.start();

			pop.rand_pop();
			pop.evolve(Ngen);

			pop.resize(Npop_lo);
			pop.evolve(Ngen_lo);

			pop.resize(Npop_hi);
			pop.evolve(Ngen_hi);
			
			pop.take_sample();
			pop.write_prob_counts();

			clock.stop();
			clock.write_stat("progress.txt");
		}
	}
	clock.write_data("times.txt");

	return 0;
}