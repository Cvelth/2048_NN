#pragma once
#include "Field.hpp"

enum class Move {
	Left = 1, Right = 2, Up = 3, Down = 4
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
		m_file << m_field << uint8_t(move);
	}
};