#include <fstream>
#include <iostream>
#include <sstream>
#include <filesystem>
#include <random>
#include "../GameCore/GameCore.hpp"
#include "../GameCore/Field.hpp"
#include "mnn/NeuralNetworkEvolutionManager/NeuralNetworkEvolutionManager.hpp"
#include "mnn/MatrixLayeredNeuralNetwork/NeuralNetwork.hpp"
#include "mnn/storage/storage.hpp"
int main(int argc, char** argv) {
	size_t generations = 10, generated_networks = 100;
	size_t hidden_neurons = 64, hidden_layers = 64;
	double survivability_ratio = 0.5;
	double unit_mutation_chance = 0.2, link_mutation_chance = 0.5;
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
			s >> survivability_ratio;
			s.clear();
		case 6:
			s.str(argv[5]);
			s >> hidden_layers;
			s.clear();
		case 5:
			s.str(argv[4]);
			s >> hidden_neurons;
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
					<< "\tFourth parameter - maximum number of hidden neurons in each layer of a network.\n" << "\t\tDefault value: 64\n"
					<< "\tFifth parameter - maximum number of hidden layers in each network.\n" << "\t\tDefault value: 64\n"
					<< "\tSixth parameter - chance of unit mutation.\n" << "\t\tDefault value: 0.2\n"
					<< "\tSeventh parameter - chance of link mutation in mutated unit.\n" << "\t\tDefault value: 0.5\n";
				return 0;
			} else
				folder_name = argv[1];
	}

	if (folder_name != "") {

		std::mt19937_64 g(std::random_device{}());
		std::uniform_int_distribution<size_t> d_l(1, hidden_layers);
		std::uniform_int_distribution<size_t> d_n(1, hidden_neurons);

		mnn::NeuralNetworkEvolutionManager em {
			generated_networks, 16, 4,
			[](auto n) -> auto {
				GameCore<4> core(0);
				while (!core.isOver()) {
					auto f = core.getField()->getNormalizedCellValues();
					n->process(f);
					auto o = n->outputs();
					if (!core.make_move(float(o[0]), float(o[1]), float(o[2]), float(o[3]))) break;
				}
				return mnn::Value(core.score());
			},
			[&g, &d_l, &d_n]() -> auto { 
				auto ret = std::make_shared<mnn::MatrixLayeredNeuralNetwork>(16, 4);
				auto layer_counter = d_l(g);
				while (--layer_counter)
					ret->add_layer(d_n(g));
				ret->add_layer(4);
				return ret;
			},
			[](auto n1, auto n2) -> auto { 
				return mnn::MatrixLayeredNeuralNetwork::generate(
					*std::dynamic_pointer_cast<mnn::MatrixLayeredNeuralNetwork>(n1), 
					*std::dynamic_pointer_cast<mnn::MatrixLayeredNeuralNetwork>(n2)
				);
			}
		};

		em.fill();
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

				em.select();
				//float average = em.select(true);
				size_t j = 0;
				for (auto it : *em) {
					s << j++ << "_" << it.second << ".mnn";
					mnn::save_to_file(filename + s.str(), *it.first);
					s.str("");
				}
				std::cout << "Generation_" << i << ":\t" << em->front().second << " <- " << /*average << " <- " <<*/ em->back().second << "\n";
				em.mutate(unit_mutation_chance, link_mutation_chance);
			}
		} catch (std::exception &e) {
			std::cout << e.what();
		}
	} else
		std::cout << "Unsupported parameter was passed.\n";
	return 0;
}