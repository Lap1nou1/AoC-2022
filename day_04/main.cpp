#include <iostream>
#include <fstream>
#include <utility>
#include <string>
#include <cstdint>
#include <cstdlib>
#include <chrono>

//Vrai si p2 est intégralement contenu dans p1
bool isIncluded(const std::pair<std::uint16_t, std::uint16_t>& p1, const std::pair<std::uint16_t, std::uint16_t>& p2) {
	return (p1.first <= p2.first && p1.second >= p2.second);
}

bool isOverlapping(const std::pair<std::uint16_t, std::uint16_t>& p1, const std::pair<std::uint16_t, std::uint16_t>& p2) {
	return	(p1.first >= p2.first && p1.first <= p2.second) ||
			(p1.second >= p2.first && p1.second <= p2.second) ||
			isIncluded(p1, p2);
			
}

int main(int argc, char** argv) {
	if (argc != 2) {
		std::cout << "Veuillez indiquer un nom de fichier" << std::endl;
		return -1;
	}

	std::ifstream input(argv[1]);
	if (!input.is_open()) {
		std::cout << "Le fichier \"" << argv[1] << "\" n'a pas pu etre ouvert" << std::endl;
		return -1;
	}

	auto ch_start = std::chrono::high_resolution_clock::now();

	std::uint64_t part1 = 0;
	std::uint64_t part2 = 0;
	for (std::string line; std::getline(input, line);) {
		//On récupère les paires
		std::pair<std::uint16_t, std::uint16_t> firstAssignements, secondAssignements;

		//(Posez pas de question, je sais pas coder)
		{
			const char* start = line.c_str();
			char* end = nullptr;

			firstAssignements.first = std::strtoul(start, &end, 10);
			start = end + 1;

			firstAssignements.second = std::strtoul(start, &end, 10);
			start = end + 1;

			secondAssignements.first = std::strtoul(start, &end, 10);
			start = end + 1;

			secondAssignements.second = std::strtoul(start, &end, 10);
		}

		//std::cout << "<" << firstAssignements.first << ", " << firstAssignements.second << "> ; <" 
		//			<< secondAssignements.first << ", " << secondAssignements.second << ">" << std::endl; 

		//On regarde un des deux inclus l'autre
		if (isIncluded(firstAssignements, secondAssignements) || isIncluded(secondAssignements, firstAssignements)) part1++;
		if (isOverlapping(firstAssignements, secondAssignements)) part2++;
	}
	
	auto ch_end = std::chrono::high_resolution_clock::now();

	std::cout << "Part 1: " << part1 << std::endl;
	std::cout << "Part 2: " << part2 << std::endl;
	std::cout << "Time: " << std::chrono::duration_cast<std::chrono::microseconds>(ch_end-ch_start).count() << "us" << std::endl;

	input.close();
	return 0;
}
