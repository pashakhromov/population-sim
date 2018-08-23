// simulation-contd.cpp

#include "pop.h"
#include "clock.h"

int main()
{
    Clock clock {};
    Parameters par {};
	PopStat pop_stat {par};

    int Nitr = 200000; // double check it with value in run.py as they should sum to 10^6
	int Npop = par.get_Npop();

    Population pop {par, "pop.txt"};

    // time between samples
    int t_btw_smpl = static_cast<int>( 1.0 / par.get_mu() );
    
    // iteration 1
    pop.take_sample();

    // the rest Nitr-1 iterations
    for (int itr = 1; itr < Nitr; itr++) {
        clock.start();
    
        pop.evolve(t_btw_smpl);
        pop.take_sample();
        pop.write_prob_counts("prob_counts-contd.txt");
    
        clock.stop();
        clock.write_stat("progress-contd.txt");
    }

	clock.write_data("times-contd.txt");

	return 0;
}