#include "SimpleLogging.hpp"

std::unordered_map<std::string, std::chrono::high_resolution_clock::time_point> workingBenchmarks;

double sl::GetTime(const std::string& benchmarkName)
{
	if (workingBenchmarks.contains(benchmarkName))
	{
		std::chrono::duration<double> time = std::chrono::high_resolution_clock::now() - workingBenchmarks[benchmarkName];
		return time.count();
	}
	else
	{
		std::cout << "Benchmark with that name does not exist\n";
		return 0;
	}
}

void sl::BeginBenchmark(const std::string& benchmarkName)
{
	if (!workingBenchmarks.contains(benchmarkName))
		workingBenchmarks[benchmarkName] = std::chrono::high_resolution_clock::now();
	else
		std::cout << "there's already running benchmark with that name\n";
}

double sl::EndBenchmark(const std::string& benchmarkName)
{
	if (workingBenchmarks.contains(benchmarkName))
	{	
		double time = GetTime(benchmarkName);
		workingBenchmarks.erase(benchmarkName);
		std::cout << "Benchmark took: " << time << "s\n";
		return time;
	}
	else
	{
		std::cout << "Benchmark with that name does not exist\n";		
	}


}

