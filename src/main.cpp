#include <iostream>
#include <chrono>
#include <functional>
#include <numeric>
#include <array>
#include "file/file_reader.hpp"
#include "file/file_reader_buffered.hpp"


namespace quick_usage {
    void file_reader_run() {
        const std::string FILE_PATH = "../README.md";

        using file::reader::FileReaderBase;
        using file::reader::FileReader;
        using file::reader::FileReaderBuffered;

        auto fr_run = [&FILE_PATH]() {
            FileReader reader{FILE_PATH};
            reader.readFile();
            auto content = reader.get();
            //std::cout << reader.toString() << std::endl;
        };

        auto frb_run = [&FILE_PATH]() {
            FileReaderBuffered reader2{FILE_PATH};

            while (reader2.hasNextLine()) {
                auto line = reader2.getNextLine();
            }

            //std::cout << reader2.toString() << std::endl;
        };

        auto clock_run = [](const std::function<void()> &func) -> long {
            auto start = std::chrono::system_clock::now();
            func();
            auto end = std::chrono::system_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
            //std::cout << std::to_string(duration) << std::endl;
            return duration;
        };

        auto simulate = [&clock_run](const std::size_t iterations, const std::function<void()> &run, std::vector<long> &collection) {
            for (std::size_t i = 0; i < iterations; i++) {
                auto duration = clock_run(run);
                collection.emplace_back(duration);
            }
        };

        std::vector<long> fr_times{};
        std::vector<long> frb_times{};
        std::vector<long> ratios{};
        constexpr std::size_t ITERATIONS = 1'000;
        constexpr std::size_t SIMULATION_NO = 2;

        auto get_min_avg_max = [](const std::vector<long> &times) {
            auto fr_avg = static_cast<double>(std::accumulate(times.begin() + 1, times.end(), 0L)) / ITERATIONS;
            auto fr_min = *std::min_element(times.begin() + 1, times.end());
            auto fr_max = *std::max_element(times.begin() + 1, times.end());
            return std::array<double, 3>{static_cast<double>(fr_min), fr_avg, static_cast<double>(fr_max)};
        };

        for (std::size_t i = 0; i < SIMULATION_NO; i++) {
            simulate(ITERATIONS, fr_run, fr_times);
            simulate(ITERATIONS, frb_run, frb_times);
            auto frr = get_min_avg_max(fr_times);
            auto frbr = get_min_avg_max(frb_times);
            auto ratio = frr[1] / frbr[1];
            ratios.emplace_back(ratio);
            std::cout << "fr (min, avg, max) = ( " << frr[0] << ", " << frr[1] << ", " << frr[2] << " )" << std::endl;
            std::cout << "frb(min, avg, max) = ( " << frbr[0] << ", " << frbr[1] << ", " << frbr[2] << " )" << std::endl;
            std::cout << ratio << std::endl;
            std::cout << std::endl;
        }

        auto ratio3 = get_min_avg_max(ratios);
        std::cout << "ratio(min, avg, max) = ( " << ratio3[0] << ", " << ratio3[1] << ", " << ratio3[2] << " )" << std::endl;
    }
}

int main() {
    std::cout << "Hello, World!" << std::endl;
    quick_usage::file_reader_run();
    return 0;
}
