// Compares execution speed of two loops:
// - One with a predictable branch (alternating pattern)
// - One with an unpredictable branch (random condition)
// The performance difference reflects the cost of branch misprediction,
// which disrupts CPU speculative execution and pipeline efficiency.
// Outputs average timings over multiple trials to a CSV file for further analysis.

#include <iostream>
#include <random>
#include <fstream>
#include <iomanip>
#include <vector>

#include "kaizen.h"

volatile int result = 0; // Prevent compiler optimization

void unpredictable_loop(int n, std::mt19937 &gen, std::uniform_int_distribution<> &dis)
{
    int sum = 0;
    for (int i = 0; i < n; i++)
    {
        if (dis(gen))
        {
            sum += i;
        }
    }
    result = sum;
}

void predictable_loop(int n)
{
    int sum = 0;
    for (int i = 0; i < n; i++)
    {
        if ((i % 2) == 0) // Predictable alternating pattern
        {
            sum += i;
        }
    }
    result = sum;
}

int main(int argc, char *argv[])
{
    zen::cmd_args args(argv, argc);

    if (!args.is_present("--size") || !args.is_present("--trials"))
    {
        std::cerr << "Usage: " << argv[0] << " --size <array_size> --trials <num_trials>" << std::endl;
        return 1;
    }

    int n = std::stoi(args.get_options("--size")[0]);
    int trials = std::stoi(args.get_options("--trials")[0]);

    if (n <= 0 || trials <= 0)
    {
        std::cerr << "Both array size and number of trials must be positive integers." << std::endl;
        return 1;
    }

    std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<> dis(0, 1);

    // Warm-up
    predictable_loop(n);
    unpredictable_loop(n, gen, dis);

    zen::timer timer;
    std::vector<long long> predictable_times, unpredictable_times;

    for (int i = 0; i < trials; ++i)
    {
        timer.start();
        predictable_loop(n);
        timer.stop();
        predictable_times.push_back(timer.duration<zen::timer::nsec>().count());

        timer.start();
        unpredictable_loop(n, gen, dis);
        timer.stop();
        unpredictable_times.push_back(timer.duration<zen::timer::nsec>().count());
    }

    long long total_pred = 0, total_unpred = 0;
    for (int i = 0; i < trials; ++i)
    {
        total_pred += predictable_times[i];
        total_unpred += unpredictable_times[i];
    }

    long long avg_pred = total_pred / trials;
    long long avg_unpred = total_unpred / trials;

    std::ofstream csv("branch_prediction_results.csv");
    csv << "Trial, Predictable (ns), Unpredictable (ns), Difference (ns)\n";
    for (int i = 0; i < trials; ++i)
    {
        csv << (i + 1) << ", " << predictable_times[i] << ", "
            << unpredictable_times[i] << ", "
            << (unpredictable_times[i] - predictable_times[i]) << "\n";
    }
    csv << "Average, " << avg_pred << ", " << avg_unpred << ", " << (avg_unpred - avg_pred) << "\n";
    csv.close();

    std::stringstream ss;
    ss << "| " << std::left << std::setw(30) << "Predictable Loop (avg ns)"
       << " | " << std::right << std::fixed << std::setprecision(2) << avg_pred << " |\n";

    ss << "| " << std::left << std::setw(30) << "Unpredictable Loop (avg ns)"
       << " | " << std::right << std::fixed << std::setprecision(2) << avg_unpred << " |\n";

    ss << "| " << std::left << std::setw(30) << "Difference"
       << " | " << std::right << std::fixed << std::setprecision(2) << (avg_unpred - avg_pred) << " |\n";

    zen::print(ss.str());
    return 0;
}