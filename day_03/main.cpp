#include <iostream>
#include <fstream>
#include <string>
#include <cstdint>
#include <array>
#include <chrono>

int main(int argc, char** argv) {
	if (argc != 2) {
		std::cout << "Veuillez indiquer le fichier à ouvrir" << std::endl;
		return -1;
	}

	std::ifstream file(argv[1]);

	if (!file.is_open()) {
		std::cout << "Le fichier \"" << argv[1] << "\" n'existe pas" << std::endl;
		return -1;
	}

	auto ch_start = std::chrono::high_resolution_clock::now();

	std::uint64_t part1 = 0;
	std::uint64_t part2 = 0;

	std::array<std::uint8_t, 52> carac_occ_group; //Nombre d'occurence des caractères (reset toutes les 3 lignes)
	carac_occ_group.fill(0);
												  
	std::size_t nb_lines = 1;	//Nombre de ligne lues (pour grouper par trois)
	for (std::string line; std::getline(file, line); nb_lines++) {

		std::array<std::uint8_t, 52> carac_pres_line;	//Présence d'un caractère dans la ligne (
														//0: non présent, 1: première partie, 2: les deux parties, -1 présent quelconque)
		carac_pres_line.fill(0);

		std::size_t taille_compart = line.size()/2;
		for (std::size_t i = 0; i < line.size(); i++) {
			std::size_t car_ind = line[i] - (line[i] >= 'a' ? 'a' : 'A' - 26);  

			//On regarde si on a déjà croisé ce caractère
			if (carac_pres_line[car_ind] == 0) {
				carac_occ_group[car_ind]++;
				
				carac_pres_line[car_ind] = -1;

				//std::cout << line[i] << " ";
			}
			//std::cout << line[i] << " (" << car_ind << "; " << car_ind + 1 << ")" << std::endl;
			//On regarde si on est dans le premier compartiment ou le second
			if (i < taille_compart) {
				carac_pres_line[car_ind] = 1;
			} 
			else {
				if (carac_pres_line[car_ind] == 1) {
					part1 += car_ind + 1; 
					carac_pres_line[car_ind] = 2;
				}
			}
		}
		
		if (nb_lines%3 == 0) {
			//On regarde le type du groupe
			for (int i = 0; i < 52; i++) {
				if (carac_occ_group[i] == 3) {
					part2 += i+1;

					//std::cout << i << std::endl;
				}
			}

			carac_occ_group.fill(0);
		}

		//std::cout << std::endl;
	}

	auto ch_end = std::chrono::high_resolution_clock::now();

	std::cout << "Part 1: " << part1 << std::endl;
	std::cout << "Part 2: " << part2 << std::endl;
	std::cout << "Time: " << std::chrono::duration_cast<std::chrono::microseconds>(ch_end - ch_start).count() << "us" << std::endl;

	return 0;
}
