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