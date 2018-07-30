// clock.cpp

#include "clock.h"

Clock::Clock() {
    T0 = std::chrono::high_resolution_clock::now();
}

void Clock::reset() {
    intervals.clear();
}

void Clock::start() {
    t0 = std::chrono::high_resolution_clock::now();
}

void Clock::stop() {
    auto t = std::chrono::high_resolution_clock::now();
    auto dt = std::chrono::duration<double>(t - t0).count();
    intervals.push_back(dt);
}

double Clock::get_runtime() const {
    auto T = std::chrono::high_resolution_clock::now();
    return std::chrono::duration<double>(T - T0).count();
}

void Clock::write_stat(const string& fname) const {
        std::ofstream f;
        f.open(fname);
        f << "itrN," << intervals.size()
          << "\nitrT," << intervals.back()
          << "\navgT," << std::accumulate( intervals.begin(), intervals.end(), 0.0 ) / intervals.size()
          << "\nmaxT," << *std::max_element( intervals.begin(), intervals.end() )
          << "\nrunT," << this->get_runtime()
          << endl;
        f.close();
}

void Clock::write_data(const string& fname) const {
    std::ofstream f;
    f.open(fname);
    for (const auto& t : intervals) {
        f << t << "\n";
    }
    f.close();
}