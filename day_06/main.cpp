#include <iostream>
#include <fstream>
#include <string>
#include <cstdint>
#include <array>
#include <chrono>

bool isFound(std::array<std::uint8_t, 26>& charCounts) {
	for (auto it = charCounts.begin(); it != charCounts.end(); ++it) {
		if (*it >= 2) return false;
	}

	return true;
}

std::size_t find_message(const std::string& line, const std::size_t sizeMessage) {
	//Cherche le premier bout de texte a avoir sizeMessage en taille et sans doublon
	//On utilise un array comportant le nombre pour chacun des caractères dans la séquence des trois derniers
	std::array<std::uint8_t, 26> charCounts;
	charCounts.fill(0);

	std::uint16_t pos = -1;
	while (++pos < sizeMessage) charCounts[line[pos] - 'a']++;

	for (; !isFound(charCounts); pos++) {
		//On ajoute la lettre qu'on lit et on retire la 4ème
		charCounts[line[pos] - 'a']++;
		charCounts[line[pos - sizeMessage] - 'a']--;
	}

	return pos;
}

int main(int argc, char** argv) {
	if (argc == 1) {
		std::cout << "Veuillez indiquer un fichier" << std::endl;
		return -1;
	}
	
	std::ifstream input(argv[1]);
	if (!input.is_open()) {
		std::cout << "Le fichier \"" << argv[1] << "\" n'a pas pu etre ouvert" << std::endl;
		return -1;
	}

	auto ch_start = std::chrono::high_resolution_clock::now();

	//On récupère la ligne
	std::string line;
	std::getline(input, line);


	//for (auto it = charCounts.begin(); it != charCounts.end(); ++it) {
	//	std::cout << int(*it) << std::endl;
	//}
	
	std::size_t part1 = find_message(line, 4);
	std::size_t part2 = find_message(line, 14);

	auto ch_end = std::chrono::high_resolution_clock::now();

	std::cout << "Part 1: " << part1 << std::endl;
	std::cout << "Part 2: " << part2 << std::endl;
	std::cout << "Time: " << std::chrono::duration_cast<std::chrono::microseconds>(ch_end-ch_start).count() << "us" << std::endl;

	return 0;
}
