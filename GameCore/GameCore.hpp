#pragma once
#include "RandomDevice.hpp"
#include "Field.hpp"
#include "fstream"

template<size_t FieldSize = 4>
class GameCore {
protected:
	Field<FieldSize> m_field;
	RandomDevice<short> m_random;

	bool m_isOver;
	size_t m_score;
	size_t m_record;

public:
	explicit GameCore() {
		std::ifstream f("maximum.dat");
		if (f.good())
			f >> m_record;
		else
			m_record = 0;
		restart();
	}
	~GameCore() {
		std::ofstream f("maximum.dat");
		f << m_record;
	}

	void step() {
		auto cells = m_field.getEmptyCells();
		if (cells.size() != 0) {
			*cells.at(m_random.array(cells.size())) = m_random.item();
		} else
			m_isOver = true;
	}
	bool down() {
		bool st = false;
		for (int i = 0; i < 4; i++) {
			if (m_field.at(1, i) == 0) {
				m_field.at(1, i) = m_field.at(0, i);
				m_field.at(0, i) = 0;
				st = true;
			}
			if (m_field.at(2, i) == 0) {
				m_field.at(2, i) = m_field.at(1, i);
				m_field.at(1, i) = m_field.at(0, i);
				m_field.at(0, i) = 0;
				st = true;
			}
			if (m_field.at(3, i) == 0) {
				m_field.at(3, i) = m_field.at(2, i);
				m_field.at(2, i) = m_field.at(1, i);
				m_field.at(1, i) = m_field.at(0, i);
				m_field.at(0, i) = 0;
				st = true;
			}

			if (m_field.at(3, i) == m_field.at(2, i) && m_field.at(3, i) != 0) {
				scoreInc(++m_field.at(3, i));
				m_field.at(2, i) = m_field.at(1, i);
				m_field.at(1, i) = m_field.at(0, i);
				m_field.at(0, i) = 0;
				st = true;
			}
			if (m_field.at(2, i) == m_field.at(1, i) && m_field.at(2, i) != 0) {
				scoreInc(++m_field.at(2, i));
				m_field.at(1, i) = m_field.at(0, i);
				m_field.at(0, i) = 0;
				st = true;
			}
			if (m_field.at(1, i) == m_field.at(0, i) && m_field.at(1, i) != 0) {
				scoreInc(++m_field.at(1, i));
				m_field.at(0, i) = 0;
				st = true;
			}
		}
		if (st) step();
		return st;
	}
	bool left() {
		bool st = false;
		for (int i = 0; i < 4; i++) {
			if (m_field.at(i, 2) == 0) {
				m_field.at(i, 2) = m_field.at(i, 3);
				m_field.at(i, 3) = 0;
				st = true;
			}
			if (m_field.at(i, 1) == 0) {
				m_field.at(i, 1) = m_field.at(i, 2);
				m_field.at(i, 2) = m_field.at(i, 3);
				m_field.at(i, 3) = 0;
				st = true;
			}
			if (m_field.at(i, 0) == 0) {
				m_field.at(i, 0) = m_field.at(i, 1);
				m_field.at(i, 1) = m_field.at(i, 2);
				m_field.at(i, 2) = m_field.at(i, 3);
				m_field.at(i, 3) = 0;
				st = true;
			}

			if (m_field.at(i, 0) == m_field.at(i, 1) && m_field.at(i, 0) != 0) {
				scoreInc(++m_field.at(i, 0));
				m_field.at(i, 1) = m_field.at(i, 2);
				m_field.at(i, 2) = m_field.at(i, 3);
				m_field.at(i, 3) = 0;
				st = true;
			}
			if (m_field.at(i, 1) == m_field.at(i, 2) && m_field.at(i, 1) != 0) {
				scoreInc(++m_field.at(i, 1));
				m_field.at(i, 2) = m_field.at(i, 3);
				m_field.at(i, 3) = 0;
				st = true;
			}
			if (m_field.at(i, 2) == m_field.at(i, 3) && m_field.at(i, 2) != 0) {
				scoreInc(++m_field.at(i, 2));
				m_field.at(i, 3) = 0;
				st = true;
			}
		}
		if (st) step();
		return st;
	}
	bool right() {
		bool st = false;
		for (int i = 0; i < 4; i++) {
			if (m_field.at(i, 1) == 0) {
				m_field.at(i, 1) = m_field.at(i, 0);
				m_field.at(i, 0) = 0;
				st = true;
			}
			if (m_field.at(i, 2) == 0) {
				m_field.at(i, 2) = m_field.at(i, 1);
				m_field.at(i, 1) = m_field.at(i, 0);
				m_field.at(i, 0) = 0;
				st = true;
			}
			if (m_field.at(i, 3) == 0) {
				m_field.at(i, 3) = m_field.at(i, 2);
				m_field.at(i, 2) = m_field.at(i, 1);
				m_field.at(i, 1) = m_field.at(i, 0);
				m_field.at(i, 0) = 0;
				st = true;
			}

			if (m_field.at(i, 3) == m_field.at(i, 2) && m_field.at(i, 3) != 0) {
				scoreInc(++m_field.at(i, 3));
				m_field.at(i, 2) = m_field.at(i, 1);
				m_field.at(i, 1) = m_field.at(i, 0);
				m_field.at(i, 0) = 0;
				st = true;
			}
			if (m_field.at(i, 2) == m_field.at(i, 1) && m_field.at(i, 2) != 0) {
				scoreInc(++m_field.at(i, 2));
				m_field.at(i, 1) = m_field.at(i, 0);
				m_field.at(i, 0) = 0;
				st = true;
			}
			if (m_field.at(i, 1) == m_field.at(i, 0) && m_field.at(i, 1) != 0) {
				scoreInc(++m_field.at(i, 1));
				m_field.at(i, 0) = 0;
				st = true;
			}
		}
		if (st) step();
		return st;
	}
	bool up() {
		bool st = false;
		for (int i = 0; i < 4; i++) {
			if (m_field.at(2, i) == 0) {
				m_field.at(2, i) = m_field.at(3, i);
				m_field.at(3, i) = 0;
				st = true;
			}
			if (m_field.at(1, i) == 0) {
				m_field.at(1, i) = m_field.at(2, i);
				m_field.at(2, i) = m_field.at(3, i);
				m_field.at(3, i) = 0;
				st = true;
			}
			if (m_field.at(0, i) == 0) {
				m_field.at(0, i) = m_field.at(1, i);
				m_field.at(1, i) = m_field.at(2, i);
				m_field.at(2, i) = m_field.at(3, i);
				m_field.at(3, i) = 0;
				st = true;
			}

			if (m_field.at(0, i) == m_field.at(1, i) && m_field.at(0, i) != 0) {
				scoreInc(++m_field.at(0, i));
				m_field.at(1, i) = m_field.at(2, i);
				m_field.at(2, i) = m_field.at(3, i);
				m_field.at(3, i) = 0;
				st = true;
			}
			if (m_field.at(1, i) == m_field.at(2, i) && m_field.at(1, i) != 0) {
				scoreInc(++m_field.at(1, i));
				m_field.at(2, i) = m_field.at(3, i);
				m_field.at(3, i) = 0;
				st = true;
			}
			if (m_field.at(2, i) == m_field.at(3, i) && m_field.at(2, i) != 0) {
				scoreInc(++m_field.at(2, i));
				m_field.at(3, i) = 0;
				st = true;
			}
		}
		if (st) step();
		return st;
	}

	enum MoveID {
		Move_up = 0,
		Move_down = 1,
		Move_left = 2,
		Move_right = 3
	};
	bool make_move(MoveID id) {
		switch (id) {
			case Move_up: return up();
			case Move_down: return down();
			case Move_left: return left();
			case Move_right: return right();
			default: return false;
		}
	}
	bool make_move(float up, float down, float left, float right) {
		size_t rank[4];
		if (up > down) {
			rank[0] = Move_up;
			rank[1] = Move_down;
		} else {
			rank[0] = Move_down;
			rank[1] = Move_up;
		}
		if (left > rank[1]) {
			rank[2] = rank[1];
			if (left > rank[0]) {
				rank[1] = rank[0];
				rank[0] = Move_left;
			} else
				rank[1] = Move_left;
		} else
			rank[2] = Move_left;
		if (right > rank[2]) {
			rank[3] = rank[2];
			if (right > rank[1]) {
				rank[2] = rank[1];
				if (right > rank[0]) {
					rank[1] = rank[0];
					rank[0] = Move_right;
				} else
					rank[1] = Move_right;
			} else
				rank[2] = Move_right;
		} else
			rank[3] = Move_right;

		if (!make_move((MoveID) rank[0]))
			if (!make_move((MoveID) rank[1]))
				if (!make_move((MoveID) rank[2]))
					if (!make_move((MoveID) rank[3]))
						return false;
		return true;
	}

	bool isOver() {
		return m_isOver;
	}
	size_t score() {
		return m_score;
	}
	size_t record() {
		return m_record;
	}

	void scoreInc(size_t i) {
		m_score += (size_t) pow(2, i);
		if (m_score > m_record)
			m_record = m_score;
	}

	void restart() {
		m_isOver = false;
		m_score = 0;
		m_field.clear();
		step();
		step();
	}

	Field<FieldSize>* getField() {
		return &m_field;
	}
};