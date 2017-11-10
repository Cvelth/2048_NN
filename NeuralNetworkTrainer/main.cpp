#include <fstream>
#include <iostream>
#include <sstream>
#include <filesystem>
#include "..\GameCore\GameCore.hpp"
#include "..\GameCore\Field.hpp"
#include "..\..\MyNeuralNetwork\NetworkEvolutionManager\NetworkGenerationEvolutionManager.hpp"
#include "..\..\MyNeuralNetwork\AbstractNetworkCore\Storage.hpp"
int main(int argc, char** argv) {
	if (argc < 2)
		return -1;
	if (strcmp(argv[1], "-help") == 0) {
		std::cout << "In order to generate a set of neural networks accordingly to evolutionaly algorithm pass\n"
			<< "\tFirst parameter - folder to store resultion networks.\n"
			<< "\tSecond parameter - number of generations.\n"
			<< "\tThird parameter - number of networks per generation.\n";
		return 0;
	}
	std::string folder_name = argv[1];
	size_t generations = 10, networks_per_generation = 100;
	if (argc >= 3) {
		std::stringstream s(argv[2]);
		s >> generations;
	}
	if (argc >= 4) {
		std::stringstream s(argv[3]);
		s >> networks_per_generation;
	}

	if (folder_name != "") {
		GameCore<4> core;
		mnn::NetworkGenerationEvolutionManager em(networks_per_generation, 
												  [&core](std::function<NeuronContainer<Type>(NeuronContainer<Type>)> calculate) -> Type {
			core.restart();
			while (!core.isOver()) {
				auto f = core.getField()->getNormalizedCellValues();
				auto o = calculate(f);
				if (!core.make_move(o[0], o[1], o[2], o[3])) break;
			}
			return Type(core.score());
		}, 16, 4, 32, 2);

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

				em.testPopulation(true);
				size_t j = 0;
				for (auto it : *em) {
					s << j++ << "_" << it.first << ".mnn";
					mnn::save_to_file(filename + s.str(), it.second);
					s.str("");
				}
				em.populationSelection();
				em.recreatePopulation();
				em.mutatePopulation(.2f, .5f);
			}
		} catch (std::exception &e) {
			std::cout << e.what();
		}
	} else
		std::cout << "Unsupported parameter was passed.\n";
	return 0;
}