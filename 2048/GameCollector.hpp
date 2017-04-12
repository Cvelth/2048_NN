#pragma once
#include "Field.hpp"

enum class Move {
	Left, Right, Up, Down
};

template<size_t FieldSize = 4>
class GameCollector {
	Field<FieldSize>* m_field;
	std::ofstream m_file;
public:
	GameCollector(char* fileName, Field<FieldSize>* field) : GameCollector(std::string(fileName), field) {}
	GameCollector(std::string fileName, Field<FieldSize>* field)
		: m_field(field) {
		m_file.open(fileName, std::ios_base::ate | std::ios::binary);
	}
	~GameCollector() {
		m_file.close();
	}

	void saveMove(Move move) {
		m_file << m_field << convertMove(move);
	}
	size_t convertMove(Move move) {
		switch (move) {
			case Move::Left: return 1;
			case Move::Right: return 2;
			case Move::Up: return 3;
			case Move::Down: return 4;
		}
	}
};