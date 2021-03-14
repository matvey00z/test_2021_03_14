#include <cstdint>
#include <cstdio>

#include <vector>

constexpr int halflen = 6;
constexpr int base = 13;

constexpr uint64_t factorial(int n)
{
	int ret = 1;
	for(int i = 2; i <= n; ++i)
		ret *= i;
	return ret;
}

void add_sums5(std::vector<uint64_t> &sums_count, int digit, int sum, int len, uint64_t repetition_factor)
{
	if(digit == 0)
	{
		repetition_factor *= factorial(len);
		static constexpr uint64_t comb_count = factorial(halflen);
		sums_count[sum] += comb_count / repetition_factor;
	}
	else
	{
		for(int i = 0; i <= len; ++i)
		{
			add_sums5(sums_count, digit - 1, sum + digit * i, len - i,
				       	repetition_factor * factorial(i));
		}
	}
}

uint64_t opt5()
{
	constexpr int max_sum = halflen * (base - 1);
	std::vector<uint64_t> sums_count(max_sum + 1, 0);
	add_sums5(sums_count, base - 1, 0, halflen, 1);
	uint64_t count = 0;
	for(auto &c: sums_count)
	{
		count += c * c;
	}
	return count * base;
}

int main(int, char**)
{
	printf("%lu\n", opt5());
	return 0;
}
