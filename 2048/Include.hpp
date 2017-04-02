#pragma once
#include <random>

template <typename Type>
class RandomDevice {
private:
	std::mt19937_64 m_generator;
	std::uniform_int_distribution<short> m_integer;
	std::exponential_distribution<float> m_exponent;
public:
	explicit RandomDevice(){
		m_generator.seed(std::random_device()());
		m_exponent = std::exponential_distribution<float>(1);
	}

	short array(const size_t& i) {
		return m_integer(m_generator) % i;
	}

	short item() {
		float t = m_exponent(m_generator);
		return Type(t) == 0 ? 1 : Type(t);
	}
};