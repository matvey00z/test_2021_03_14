#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <vector>
#include <string>
#include <chrono>
#include <functional>

constexpr int halflen = 6;
constexpr int base = 13;


uint64_t count(int leftsum, int rightsum, int leftlen, int rightlen)
{
	if(leftlen == 0)
	{
		if(rightlen == 0)
			return leftsum == rightsum ? 1 : 0;
		uint64_t ret = 0;
		for(int d = 0; d < base; ++d)
			ret += count(leftsum, rightsum + d, leftlen, rightlen - 1);
		return ret;
	}
	uint64_t ret = 0;
	for(int d = 0; d < base; ++d)
		ret += count(leftsum + d, rightsum, leftlen - 1, rightlen);
	return ret;
}

uint64_t recoursive()
{
	return count(0, 0, halflen, halflen) * base;
}

class Number
{
	public:
		Number()
		{
			memset(number, 0, len);
		}
		static Number max()
		{
			Number m;
			memset(m.number, base - 1, len);
			return m;
		}
		bool is_nice()
		{
			int left = 0;
			int right = 0;
			for(int i = 0; i < halflen; ++i)
			{
				left += number[i];
				right += number[halflen + i];
			}
			return left == right;
		}
		friend bool operator!=(const Number &l, const Number &r)
		{
			return memcmp(l.number, r.number, len);
		}
		Number &operator++()
		{
			int add = 1;
			for(int i = len - 1; i >= 0; --i)
			{
				number[i] += add;
				add = number[i] / base;
				number[i] %= base;
			}
			return *this;
		}
	private:
		static constexpr int len{2*halflen};
		uint8_t number[len];
};

uint64_t bruteforce()
{
	Number max = Number::max();
	uint64_t count = 0;
	for(Number n; n != max; ++n)
	{
		if(n.is_nice())
			++count;
	}
	if(max.is_nice())
		++count;
	return count * base;
}


void add_sums(std::vector<uint64_t> &sums_count, int prefix_sum, int len)
{
	if(len == 0)
		sums_count[prefix_sum]++;
	else
	{
		for(int d = 0; d < base; ++d)
			add_sums(sums_count, prefix_sum + d, len - 1);
	}
}

uint64_t opt3()
{
	constexpr int max_sum = halflen * (base - 1);
	std::vector<uint64_t> sums_count(max_sum + 1, 0);
	add_sums(sums_count, 0, halflen);
	uint64_t count = 0;
	for(auto &c: sums_count)
	{
		count += c * c;
	}
	return count * base;
}

int sum_digits(int num)
{
	int sum = 0;
	while(num)
	{
		sum += num % base;
		num /= base;
	}
	return sum;
}

constexpr int get_max_half()
{
	int ret = 1;
	for(int i = 0; i < halflen; ++i)
		ret *= base;
	return ret;
}

uint64_t opt4()
{
	int max_half = get_max_half();
	uint64_t count = 0;
	for(int left = 0; left < max_half; ++left)
	{
		for(int right = 0; right < max_half; ++right)
			if(sum_digits(left) == sum_digits(right))
				++count;
	}
	return count * base;
}


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

void profile(const std::string &name, const std::function<uint64_t()> &f)
{
	auto before = std::chrono::steady_clock::now();
	auto ret = f();
	auto after = std::chrono::steady_clock::now();
	printf("%s: %lu %.3lf s.\n", name.c_str(), ret,
		       	std::chrono::duration_cast<std::chrono::milliseconds>(after - before).count()/1e3);
}

#define PROFILE(f) do { profile(#f, f); } while(0)
int main(int, char**)
{
	PROFILE(opt5);
	PROFILE(opt3);
	PROFILE(recoursive);
	PROFILE(bruteforce);
	return 0;
}
