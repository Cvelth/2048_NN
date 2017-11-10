#include <fstream>
#include <iostream>
#include <sstream>
#include <filesystem>
#include "..\GameCore\GameCore.hpp"
#include "..\GameCore\Field.hpp"
#include "..\..\MyNeuralNetwork\NetworkEvolutionManager\NetworkGenerationEvolutionManager.hpp"
#include "..\..\MyNeuralNetwork\AbstractNetworkCore\Storage.hpp"
int main(int argc, char** argv) {
	size_t generations = 10, generated_networks = 100, stored_networks = 5;
	size_t hidden_neurons = 32, hidden_layers = 2;
	float unit_mutation_chance = 0.2f, link_mutation_chance = 0.5f;
	std::string folder_name;
	std::stringstream s;
	switch (argc) {
		default:
		case 1:
			std::cout << "Incorrect parameters passed\n";
			return -1;

		case 9:
			s.str(argv[8]);
			s >> link_mutation_chance;
			s.clear();
		case 8:
			s.str(argv[7]);
			s >> unit_mutation_chance;
			s.clear();
		case 7:
			s.str(argv[6]);
			s >> hidden_layers;
			s.clear();
		case 6:
			s.str(argv[5]);
			s >> hidden_neurons;
			s.clear();
		case 5:
			s.str(argv[4]);
			s >> stored_networks;
			s.clear();
		case 4:
			s.str(argv[3]);
			s >> generated_networks;
			s.clear();
		case 3:
			s.str(argv[2]);
			s >> generations;
			s.clear();
		case 2:
			if (strcmp(argv[1], "-help") == 0) {
				std::cout << "In order to generate a set of neural networks accordingly to evolutionaly algorithm pass\n"
					<< "\tFirst parameter - folder to store resultion networks.\n" << "\t\tNo default value\n"
					<< "\tSecond parameter - number of generations.\n" << "\t\tDefault value: 10\n"
					<< "\tThird parameter - number of networks per generation.\n" << "\t\tDefault value: 100\n"
					<< "\tForth parameter - number of networks per generation to be stored.\n" << "\t\tDefault value: 5\n"
					<< "\tFifth parameter - number of hidden neurons in each network.\n" << "\t\tDefault value: 32\n"
					<< "\tSixth parameter - number of hidden layers in each network.\n" << "\t\tDefault value: 2\n"
					<< "\tSeventh parameter - chance of unit mutation.\n" << "\t\tDefault value: 0.2\n"
					<< "\tEighth parameter - chance of link mutation in mutated unit.\n" << "\t\tDefault value: 0.5\n";
				return 0;
			} else
				folder_name = argv[1];
	}

	if (folder_name != "") {
		GameCore<4> core;
		mnn::NetworkGenerationEvolutionManager em(generated_networks, [&core](std::function<NeuronContainer<Type>(NeuronContainer<Type>)> calculate) -> Type {
			core.restart();
			while (!core.isOver()) {
				auto f = core.getField()->getNormalizedCellValues();
				auto o = calculate(f);
				if (!core.make_move(o[0], o[1], o[2], o[3])) break;
			}
			return Type(core.score());
		}, 16, 4, hidden_neurons, hidden_layers);

		em.newPopulation();
		try {
			std::experimental::filesystem::remove_all("./" + folder_name);
			std::experimental::filesystem::create_directory(folder_name);
			for (size_t i = 0; i < generations; i++) {
				std::stringstream s;
				s << "./" << folder_name << "/generation_" << i;
				std::experimental::filesystem::create_directory(s.str());
				s << "/unit_";
				auto filename = s.str();
				s.str("");

				float average = em.testPopulation(true);
				size_t j = 0;
				for (auto it : *em) {
					s << j++ << "_" << it.first << ".mnn";
					mnn::save_to_file(filename + s.str(), it.second);
					s.str("");
					if (j >= stored_networks)
						break;
				}
				std::cout << "Generation_" << i << ":\t" << em->front().first << " <- " << average << " <- " << em->back().first << "\n";
				em.populationSelection();
				em.recreatePopulation();
				em.mutatePopulation(unit_mutation_chance, link_mutation_chance);
			}
		} catch (std::exception &e) {
			std::cout << e.what();
		}
	} else
		std::cout << "Unsupported parameter was passed.\n";
	return 0;
}