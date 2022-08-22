#pragma once

#ifndef SIMPLELOGGING_HPP
#define SIMPLELOGGING_HPP

#include <chrono>
#include <iostream>
#include <string>
#include <unordered_map>


namespace sl
{
	double GetTime(const std::string& benchmarkName);
	
	void BeginBenchmark(const std::string& benchmarkName);
	double EndBenchmark(const std::string& benchmarkName);

}



#endif // !SIMPLELOGGING_HPP
