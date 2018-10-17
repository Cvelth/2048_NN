#pragma once
#include <vector>
#include <ostream>

template <size_t FieldSize = 4, typename Type = short>
class Field {
protected:
	Type m_cells[FieldSize * FieldSize];
public:
	explicit Field() { clear();	}
	~Field() {}
	inline void clear() {
		for (size_t i = 0; i < FieldSize * FieldSize; i++)
			m_cells[i] = Type(0);
	}
	inline size_t countEmpty() {
		size_t res = 0;
		for (size_t i = 0; i < FieldSize * FieldSize; i++)
			if (m_cells[i] == Type(0))
				res++;
		return res;
	}

	inline std::vector<Type> getCellValues() const {
		std::vector<Type> res;
		for (size_t i = 0; i < FieldSize * FieldSize; i++)
			res.push_back(m_cells[i]);
		return res;
	}
	inline std::vector<double> getNormalizedCellValues() const {
		std::vector<double> res;
		for (size_t i = 0; i < FieldSize * FieldSize; i++)
			res.push_back(m_cells[i] == 0.f ? 0.f : 1.f / m_cells[i]);
		return res;
	}
	inline std::vector<Type*> getEmptyCells() {
		std::vector<Type*> res;
		for (size_t i = 0; i < FieldSize * FieldSize; i++)
			if (m_cells[i] == Type(0))
				res.push_back(&m_cells[i]);
		return res;
	}

	inline Type& operator()(const size_t& i, const size_t& j) {
		return m_cells[i * FieldSize + j];
	}
	inline Type& at(const size_t& i, const size_t& j) {
		return (*this)(i, j);
	}
	inline const Type& operator()(const size_t& i, const size_t& j) const {
		return m_cells[i * FieldSize + j];
	}
	inline const Type& at(const size_t& i, const size_t& j) const {
		return (*this)(i, j);
	}

	friend std::ostream& operator<<(std::ostream& os, const Field<FieldSize, Type>& dt);
};

template <size_t FieldSize = 4, typename Type = short>
std::ostream& operator<<(std::ostream& os, const Field<FieldSize, Type>& f) {
	for (size_t i = 0; i < FieldSize * FieldSize; i++)
		os << f.m_cells[i];
	return os;
}