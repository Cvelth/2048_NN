#include <fstream>
#include <iostream>
#include "..\GameCore\GameCore.hpp"
#include "..\GameCore\Field.hpp"
#include "..\..\MyNeuralNetwork\NetworkEvolutionManager\NetworkGenerationEvolutionManager.hpp"
int main(int argc, char** argv) {
	//if (argc < 2)
	//	return -1;
	//if (strcmp(argv[1], "-help") == 0)
	//	std::cout << "Use a path to the Neural Network as the first parameter.\n";
	//else if (strcmp(argv[1], "") != 0) {
		GameCore<4> core;
		mnn::NetworkGenerationEvolutionManager em(100, [&core](std::function<NeuronContainer<Type>(NeuronContainer<Type>)> calculate) -> Type {
			core.restart();
			while (!core.isOver()) {
				auto f = core.getField()->getNormalizedCellValues();
				auto o = calculate(f);
				bool turn = false;
				if (o[0] > o[1])
					if (o[0] > o[2])
						if (o[0] > o[3])
							turn = core.up();
						else
							turn = core.right();
					else
						if (o[2] > o[3])
							turn = core.left();
						else
							turn = core.right();
				else
					if (o[1] > o[2])
						if (o[1] > o[3])
							turn = core.down();
						else
							turn = core.right();
					else
						if (o[2] > o[3])
							turn = core.left();
						else
							turn = core.right();
				if (!turn) break;
			}
			return core.score();
		}, 16, 4, 32, 2);
		em.newPopulation();
		em.testPopulation(true);
	//} else
	//	std::cout << "Unsupported parameter was passed.\n";
	return 0;
}