#include "StatsAccumulator.h"
#include "StatUtils.h"

StatsAccumulator::StatsAccumulator(const size_t n, const double m1, const double m2, const double m3, const double m4,
	const bool isIntegerDistribution):
	n(n), m1(m1), m2(m2), m3(m3), m4(m4), isIntegerDistribution(isIntegerDistribution) {}

void StatsAccumulator::push(const double x) {
	if (!StatUtils::valueNormalOrZero(x)) {
		return;
	}
		
	// Check if x is an integer
	isIntegerDistribution = isIntegerDistribution && StatUtils::isValueInteger(x);

	const auto n1 = n;
	n += 1;

	const auto delta = x - m1;
	const auto deltaN = delta / n;
	const auto deltaNSquared = deltaN * deltaN;
	const auto term1 = delta * deltaN * n1;
	m1 += deltaN;
	m4 += term1 * deltaNSquared * (n * n - 3 * n + 3) + 6 * deltaNSquared * m2 - 4 * deltaN * m3;
	m3 += term1 * deltaN * (n - 2) - 3 * deltaN * m2;
	m2 += term1;
}

size_t StatsAccumulator::getN() const {
	return n;
}

double StatsAccumulator::getMean() const {
	return m1;
}

bool StatsAccumulator::valid() const {
	return StatUtils::valueNormalOrZero(m1) && StatUtils::valueNormalOrZero(m2) && StatUtils::valueNormalOrZero(m3)
		&& StatUtils::valueNormalOrZero(m4);
}

void StatsAccumulator::debugPrint() const {
	std::cout << "StatsAccumulator" << std::endl;
	std::cout << "M1: " << m1 << std::endl;
	std::cout << "M2: " << m2 << std::endl;
	std::cout << "M3: " << m3 << std::endl;
	std::cout << "M4: " << m4 << std::endl;
	std::cout << "n: " << n << std::endl;

	std::cout << "Mean: " << getMean() << std::endl;
	std::cout << "Skewness: " << getSkewness() << std::endl;
	std::cout << "Kurtosis: " << getKurtosis() << std::endl;
	std::cout << std::endl;
}

StatsAccumulator& StatsAccumulator::operator+=(const StatsAccumulator& rhs) {
	const auto combined = *this + rhs;
	*this = combined;
	return *this;
}

StatsAccumulator StatsAccumulator::operator+(const StatsAccumulator& other) const {
	if (other.n == 0 || !other.valid()) {
		std::cout << "discarded other" << std::endl;
		return *this;
	}

	if (n == 0 || !this->valid()) {
		std::cout << "discarded this" << std::endl;
		return other;
	}

	auto result = StatsAccumulator();
	result.n = n + other.n;

	const auto delta = other.m1 - m1;
	const auto delta2 = delta * delta;
	const auto delta3 = delta2 * delta;
	const auto delta4 = delta2 * delta2;
	const auto mean = (m1 * n + other.m1 * other.n) / result.n;
	result.m1 = mean;
	result.m2 = m2 + other.m2 + delta2 * n * other.n / result.n;
	result.m3 = m3 + other.m3 + delta3 * n * other.n * (n - other.n) / (result.n * result.n) +
		3.0 * delta * (n * other.m2 - other.n * m2) / result.n;
	result.m4 = m4 + other.m4 + delta4 * n * other.n * (n * n - n * other.n + other.n * other.n) /
		(result.n * result.n * result.n) +
		6.0 * delta2 * (n * n * other.m2 + other.n * other.n * m2) / (result.n * result.n) +
		4.0 * delta * (n * other.m3 - other.n * m3) / result.n;
		
	result.isIntegerDistribution = isIntegerDistribution && other.isIntegerDistribution;

	// debugPrint();

	return result;
}
