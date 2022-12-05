#include <iostream>
#include <cstdint>
#include <fstream>
#include <string>
#include <stack>
#include <array>
#include <chrono>

//Renverse la pile st
void inverse_pile(std::stack<char>& st) {
	std::stack<char> acc;

	while (!st.empty()) {
		acc.push(st.top());
		st.pop();
	}

	st.swap(acc);
}

//Déplace nbOfCrates boîtes de inCrate à outCrate (part 1)
void move_crates(std::stack<char>& inCrate, std::stack<char>& outCrate, std::uint8_t nbOfCrates) {
	while (nbOfCrates-- != 0) {
		outCrate.push(inCrate.top());
		inCrate.pop();
	}
}

//Déplace nbOfCrates boîtes de inCrate à outCrate (part 2)
void move_crates_sec(std::stack<char>& inCrate, std::stack<char>& outCrate, std::uint8_t nbOfCrates) {
	//Pareil que la p1 sauf qu'on déplace d'abord dans une pile temporaire
	std::stack<char> temp;

	move_crates(inCrate, temp, nbOfCrates);
	move_crates(temp, outCrate, nbOfCrates);
}

//Affiche les crates
void affiche_piles(std::array<std::stack<char>, 9> cratesStack) {
	for (std::size_t i = 0; i < 9; i++) {
		std::cout << i << ": ";

		while (!cratesStack[i].empty()) {
			std::cout << cratesStack[i].top() << " ";
			cratesStack[i].pop();
		}

		std::cout << std::endl;
	}

	std::cout << "##############" << std::endl;
}

std::string topString(std::array<std::stack<char>, 9>& cratesStack) {
	std::string res;

	for (auto it = cratesStack.begin(); it != cratesStack.end(); ++it) {
		if (!it->empty())
			res += it->top();
	}

	return res;
}

int main(int argc, char** argv) {
	if (argc < 2) {
		std::cout << "Veuillez indiquer une entrée" << std::endl;
		return -1;
	}

	std::ifstream input(argv[1]);
	if (!input.is_open()) {
		std::cout << "Le fichier \"" << argv[1] << "\" n'a pas pu etre ouvert" << std::endl;
		return -1;
	}
	
	auto ch_start = std::chrono::high_resolution_clock::now();

	//On part du principe qu'on a toujours 9 piles
	std::array<std::stack<char>, 9> cratesStackPart1;

	//On lit les boîtes
	for (std::string line; std::getline(input, line);) {

		//On regarde si on a lu tout le fichier (donc si on a un «1»)
		if (line[1] == '1') break;

		//On va de pile en pile et on regarde si on a une boîte
		//L'indice de la boîte dans la ligne est 1 + ind_pile * 4
		for (std::size_t ind_pile = 0; 1 + ind_pile*4 < line.size(); ind_pile++) {
			const std::size_t ind_line = ind_pile * 4 + 1;
			
			if (line[ind_line] != ' ') {
				cratesStackPart1[ind_pile].push(line[ind_line]);
			}
		}
	}

	//On inverse les piles
	for (auto it = cratesStackPart1.begin(); it != cratesStackPart1.end(); ++it) inverse_pile(*it);

	//affiche_piles(cratesStack);
	std::array<std::stack<char>, 9> cratesStackPart2 = cratesStackPart1;

	//On lit les mouvements et on les process
	for (std::string line; std::getline(input, line);) {
		//Si jamais la ligne est vide on fait rine
		if (line.empty()) continue;

		const char* start = line.c_str() + 5;	//On va après «move»
		char* end = nullptr;

		std::uint8_t nbOfCrates = std::strtoul(start, &end, 10);
		start = end + 5;	//On va après «from»
		
		std::uint8_t inCrateInd = std::strtoul(start, &end, 10) - 1;
		start = end + 3;	//On va après le «to»
							
		std::uint8_t outCrateInd = std::strtoul(start, &end, 10) - 1;

		//On déplace
		move_crates(cratesStackPart1[inCrateInd], cratesStackPart1[outCrateInd], nbOfCrates);
		move_crates_sec(cratesStackPart2[inCrateInd], cratesStackPart2[outCrateInd], nbOfCrates);

		//affiche_piles(cratesStack);
	}

	std::string part1 = topString(cratesStackPart1);
	std::string part2 = topString(cratesStackPart2);

	auto ch_end = std::chrono::high_resolution_clock::now();

	std::cout << "Part 1: " << part1 << std::endl;
	std::cout << "Part 2: " << part2 << std::endl;
	std::cout << "Time: " << std::chrono::duration_cast<std::chrono::microseconds>(ch_end-ch_start).count() << "us" << std::endl; 

	return 0;
}
