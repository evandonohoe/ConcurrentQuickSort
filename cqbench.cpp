#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <random>
//#include <limits>
#include <tuple>
#include <iomanip>
#include <climits>
#include "cqsort.hpp"

int main(int argc, char *argv[])
{
    std::ofstream outputFile;
    outputFile.open("output_file.txt", std::ios::out);

    if (outputFile.is_open()) {
    outputFile << "List Size\t\tSequential Sort Time (s)\t\t\tConcurrent Sort Time (s)\n";
    outputFile << "\t\tmin\t\tmax\t\taverage\t\tmin\t\tmax\t\taverage\n";
    outputFile << "---------\t--------\t--------\t--------\t--------\t--------\t--------\n";


    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(std::numeric_limits<int>::min(), std::numeric_limits<int>::max());


        for (int n : {1, 10, 100, 1000, 10000, 100000, 1000000}) {
            int min_seq_time = INT_MAX;
            int min_conc_time = INT_MAX;

            int max_seq_time = 0;
            int max_conc_time = 0;

            int total_seq_time = 0;
            int total_conc_time = 0;

            for (int i = 0; i < 10; i++) {


                std::vector<int> v(n);

                for (auto& i : v) {
                    i = dis(gen);
                }

                // Sort using the sequential version
                CQSort<std::vector<int>> qs_sequential(0);

                auto start_sequential = std::chrono::high_resolution_clock::now();
                qs_sequential(v);
                auto end_sequential = std::chrono::high_resolution_clock::now();
                auto duration_sequential = std::chrono::duration_cast<std::chrono::milliseconds>(end_sequential - start_sequential);

                //std::cout << "Sequential sort took " << duration_sequential.count() << " ms." << std::endl;
                int seq_ms = duration_sequential.count();

                if (seq_ms < min_seq_time) {
                    min_seq_time = seq_ms;
                } 
                if (seq_ms > max_seq_time) {
                    max_seq_time = seq_ms;
                }
                total_seq_time += seq_ms;

                //Sort using the concurrent version
                CQSort<std::vector<int>> qs_concurrent(4);
                auto start_concurrent = std::chrono::high_resolution_clock::now();


                qs_concurrent(v);
                auto end_concurrent = std::chrono::high_resolution_clock::now();
                auto duration_concurrent = std::chrono::duration_cast<std::chrono::milliseconds>(end_concurrent - start_concurrent);

                int conc_ms = duration_sequential.count();
                if (conc_ms < min_conc_time) {
                    min_conc_time = conc_ms;
                }
                if (conc_ms > max_conc_time) {
                    max_conc_time = conc_ms;
                }
                total_conc_time += conc_ms;

            }

            int avg_seq_time = total_seq_time/10;
            int avg_conc_time = total_conc_time/10;

            double min_seq_time_s = static_cast<double>(min_seq_time) / 1000.0;
            double max_seq_time_s = static_cast<double>(max_seq_time) / 1000.0;
            double avg_seq_time_s = static_cast<double>(avg_seq_time) / 1000.0;

            double min_conc_time_s = static_cast<double>(min_conc_time) / 1000.0;
            double max_conc_time_s = static_cast<double>(max_conc_time) / 1000.0;
            double avg_conc_time_s = static_cast<double>(avg_conc_time) / 1000.0;

            
            outputFile << std::to_string(n) << "\t\t" << std::scientific << std::setprecision(3) << min_seq_time_s << "\t"
                << std::scientific << std::setprecision(3) << max_seq_time_s << "\t"
                << std::scientific << std::setprecision(3) << avg_seq_time_s << "\t"
                << std::scientific << std::setprecision(3) << min_conc_time_s << "\t"
                << std::scientific << std::setprecision(3) << max_conc_time_s << "\t"
                << std::scientific << std::setprecision(3) << avg_conc_time_s << "\n";
        }

    }
    else {
        std::cout << "Could not open file." << std::endl;
    }
    outputFile.close();


    //}


	return EXIT_SUCCESS;
}
