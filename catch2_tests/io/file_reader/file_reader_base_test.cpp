#include <iostream>
#include <iomanip>
#include <vector>
#include <string>

#include "../../../externs/catch.hpp"
#include "../../../src/io/file_reader/file_reader_base.hpp"
#include "../../../src/io/file_reader//file_reader.hpp"
#include "../../../src/io/file_reader/file_reader_buffered.hpp"


SCENARIO("Comparing FileReader's speed", "[file][file_readers][!mayfail]")
{
    using libs::io::file_reader::FileReader;
    using libs::io::file_reader::FileReaderBuffered;


    struct Tasks {
        using Runner = std::function<const std::string(const std::string&)>;

        static auto RunFileReader(const std::string& path)
        {
            FileReader reader{path};

            if (!reader.isOpened()) {
                throw std::ios::failure("!FileReader.isOpened()");
            }

            reader.readFile();

            if (!reader.isReadClean()) {
                throw std::ios::failure("!FileReader.isReadClean()");
            }

            return reader.toString();
        }

        static auto RunFileReaderBuffered(const std::string& path)
        {
            std::vector<std::string> lines{};
            FileReaderBuffered reader{path};

            if (!reader.isOpened()) {
                throw std::ios::failure("!FileReaderBuffered.isOpened()");
            }

            while (reader.hasNextLine()) {
                auto line = reader.getNextLine();
                lines.push_back(line);
            }

            return reader.toString();
        }

        static auto CountDuration(const Runner& procedure, const std::string& path)
        {
            auto start = std::chrono::system_clock::now();
            procedure(path);
            auto end = std::chrono::system_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
            return duration;
        }

        static void RunSimulationStep(const std::size_t& it, const Runner& runner, std::vector<long>& times, const std::string& path)
        {
            for (std::size_t i = 0; i < it; i++) {
                auto duration = CountDuration(runner, path);
                times.push_back(duration);
            }
        }

        static auto ComputeMeanTimes(const std::vector<long>& times, const std::size_t& it)
        {
            auto avg = static_cast<double>(std::accumulate(times.begin() + 1, times.end(), 0L)) / static_cast<double>(it);
            auto min = *std::min_element(times.begin() + 1, times.end());
            auto max = *std::max_element(times.begin() + 1, times.end());
            return std::array<double, 3>{static_cast<double>(min), avg, static_cast<double>(max)};
        }

        static auto RunMonteCarloSimulation(const std::size_t& sim, const std::size_t& it, const std::string& path)
        {
            std::vector<long> file_reader_times{};
            std::vector<long> file_reader_buffered_times{};
            std::vector<long> time_ratios{};

            for (std::size_t i = 0; i < sim; i++) {
                RunSimulationStep(it, RunFileReader, file_reader_times, path);
                RunSimulationStep(it, RunFileReaderBuffered, file_reader_buffered_times, path);
                auto result_whole = ComputeMeanTimes(file_reader_times, it);
                auto result_buffered = ComputeMeanTimes(file_reader_buffered_times, it);
                auto ratio = static_cast<long>(result_whole[1] / result_buffered[1]);
                time_ratios.push_back(ratio);
            }

            auto ratio = ComputeMeanTimes(time_ratios, it);
            return ratio[1];
        }
    };

    GIVEN("N simulations per M iterations") {
        constexpr auto FILE_PATH = "../../UNLICENSE.md";
        constexpr std::size_t ITERATIONS = 1'000;
        constexpr std::size_t SIMULATIONS = 4;

        WHEN("computing the speed ratio") {
            const auto speedRatio = Tasks::RunMonteCarloSimulation(SIMULATIONS, ITERATIONS, FILE_PATH);

            THEN("ratio is in range of (1.45, 1.65)") {
                const auto speed = speedRatio * 100;
                const bool isInRange = speed > 1.45 && speed < 1.65;
                std::cout << std::setprecision(4) << speed << std::endl;
                CHECK(isInRange);
            }
        }
    }

}
