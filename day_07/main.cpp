#include <iostream>
#include <fstream>
#include <cstdint>
#include <string>
#include <chrono>
#include <map>

struct System;
struct SystemComp;

struct System {
	bool isFile;	//True si le noeud est un fichier
	std::map<std::string, System*> children;	//Bon c'est redondant mais je savais pas comment définit le comparateur pour le set sans créer un header
	System* father;

	std::string name;
	std::size_t size;	//Taille du fichier ou taille totale du dossier

	System(std::string _name, std::size_t _size = 0, bool _isFile = false, System*_father = nullptr) : isFile(_isFile), father(_father), name(_name), size(_size) {};

	~System() {
		for (auto it = children.begin(); it != children.end(); ++it) delete it->second;
	}

	//Ajoute un enfant
	void add_child(std::string _name, std::size_t _size = 0, bool _isFile = false) {
		if (this->children.find(_name) == this->children.end()) {
			System* newChild = new System(_name, _size, _isFile, this);

			this->children[_name] = newChild;
		}
	}

	//Calcule de la taille par un parcours postfixe
	void calculate_size() {
		if (!isFile) {
			for (auto it = children.begin(); it != children.end(); ++it) {
				it->second->calculate_size();

				this->size += it->second->size;
			}
		}
	}

	//Cherche l'enfant ayant dirName pour nom
	System* find_child(std::string dirName) {
		for (auto it = children.begin(); it != children.end(); ++it) {
			if (it->second->name == dirName) return it->second;
		}

		return nullptr;
	}
};

//Renvoie le résultat de la p1, parcours postfixe
std::size_t smallest_directories_size(System* current) {
	if (current->isFile) return 0;

	std::size_t res = current->size < 100000 ? current->size : 0;

	for (auto it = current->children.begin(); it != current->children.end(); ++it) {
		res += smallest_directories_size(it->second);
	}

	return res;
}

//P2
std::size_t find_directory_to_delete(System* current, const std::size_t toFree) {
	//La taille des dossiers ne peut aller qu'en baissant
	if (current->isFile || current->size < toFree) return -1;

	std::uint64_t res = current->size;
	for (auto it = current->children.begin(); it != current->children.end(); ++it) {
		std::size_t returned = find_directory_to_delete(it->second, toFree);

		if (returned < res) res = returned; 
	}

	return res;
}

int main(int argc, char** argv) {
	if (argc == 1) {
		std::cout << "Veuillez indiquer un input" << std::endl;
		return -1;
	}

	std::ifstream input(argv[1]);
	if (!input.is_open()) {
		std::cout << "Le fichier \"" << argv[1] << "\" n'a pas pu etre ouvert" << std::endl;
		return -1;
	}

	auto ch_start = std::chrono::high_resolution_clock::now();

	constexpr std::size_t diskSize = 70000000;
	constexpr std::size_t spaceNeeded = 30000000;

	System racine("/");
	System* current = &racine;

	for (std::string line; std::getline(input, line);) {
		//On regarde si c'est une commande
		if (line[0] == '$') {
			//On fait un truc que si on a un cd (dans le cas d'un ls, on ne fait rien
			if (line[2] == 'c') {
				std::string dirName = line.substr(5);

				//Racine
				if (dirName == "/") current = &racine;
				//Out
				else if (dirName == "..") current = current->father;
				//In
				else current = current->find_child(dirName);
			}
		} else {
			std::size_t pos = line.find(" ") + 1;
			std::string name = line.substr(pos);

			//Si on est ici, on a eu un ls et on ajoute le fichier/dossier aux documents
			if (line[0] == 'd') {
				//Dossier
				current->add_child(name);
			} else {
				//Fichier
				current->add_child(name, std::stoi(line), true);
			}
		}
	}

	//On calcule les taille
	racine.calculate_size();

	std::size_t part1 = smallest_directories_size(&racine);
	std::size_t part2 = find_directory_to_delete(&racine, spaceNeeded - (diskSize - racine.size));

	auto ch_end = std::chrono::high_resolution_clock::now();

	std::cout << "Part 1: " << part1 << std::endl;
	std::cout << "Part 2: " << part2 << std::endl;
	std::cout << "Time: " << std::chrono::duration_cast<std::chrono::microseconds>(ch_end-ch_start).count() << "us" << std::endl;

	input.close();

	return 0;
}
