// clock.h

#ifndef CLOCK_H
#define CLOCK_H

#include "params.h"
#include <chrono>
#include <ctime>

class Clock {
    private:
        std::chrono::high_resolution_clock::time_point T0;
        std::chrono::high_resolution_clock::time_point t0;
        vector<double> intervals;

    public:
        Clock();

        void reset();

        void start();

        void stop();

        double get_runtime() const;

        const vector<double>& get_intervals() const { return intervals; }

        void write_stat(const string& fname) const;

        void write_data(const string& fname) const;
};

#endif