#include <iostream>
#include <cstdint>
#include <string>
#include <fstream>
#include <chrono>

void insert_sorter(std::uint64_t cals[3], std::uint64_t to_insert) {
	//Add to cals the value to_insert while keeping cals sorted (and containing only the greatest 3 elements)
	int pos = -1;
	for (; pos < 2 && cals[pos+1] < to_insert; pos++) if (pos != -1) cals[pos] = cals[pos + 1];
	if (pos != -1) cals[pos] = to_insert;
}

int main() {
	auto start = std::chrono::high_resolution_clock::now();

	//Read the input
	std::ifstream file("input.txt");

	std::uint64_t top_cals[3] = {0, 0, 0}; //Array of the top three calories

	std::uint64_t current_cal = 0;
	for (std::string line; std::getline(file, line);) {
		if (line.empty()) {
			insert_sorter(top_cals, current_cal);

			current_cal = 0;
		}
		else {
			current_cal += std::stoull(line);
		}
	}


	std::cout << "Part 1: " << top_cals[2] << std::endl;
	std::cout << "Part 2: " << top_cals[0] + top_cals[1] + top_cals[2] << std::endl;

	auto end = std::chrono::high_resolution_clock::now();

	std::cout << "Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count() << " ms" << std::endl;
}
