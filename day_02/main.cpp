#include <iostream>
#include <fstream>
#include <string>
#include <cstdint>

void choose_winner(std::int64_t p1, std::int64_t p2, std::uint64_t& score) {
	score += p2 + 1;

	//std::cout << "\e[1;31m";
	//Si jamais p2 bat p1, alors p2 = p1 + 1 [3]
	//Gagne
	if ((p1+1)%3 == p2) {
		score += 6;
		//std::cout << "\e[1;32m";
	}
	else if (p1 == p2) {
		score += 3;
		//std::cout << "\e[1;33m";
	}
	
	//std::cout << p1 << " " << p2 << "\e[1;37m" << std::endl;
}

int main(int argc, char** argv) {
	if (argc != 2) {
		std::cout << "Veuillez indiquer le fichier input" << std::endl;

		return -1;
	}

	std::ifstream input(argv[1]);

	if (!input.is_open()) {
		std::cout << "Le fichier \"" << argv[1] << "\" n'existe pas" << std::endl;

		return -1;
	}

	std::uint64_t part_1 = 0;
	std::uint64_t part_2 = 0;
	for (std::string line; std::getline(input, line);) {
		std::int64_t p1 = line[0] - 'A', p2 = line[2] - 'X';

		choose_winner(p1, p2, part_1);
		choose_winner(p1, (p1 + p2 + 2)%3, part_2);
		
	}

	std::cout << "Part 1: " << part_1 << std::endl;
	std::cout << "Part 2: " << part_2 << std::endl;

	input.close();
	return 0;
}
