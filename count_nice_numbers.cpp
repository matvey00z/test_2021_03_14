#include <cstdint>
#include <cstdio>

#include <vector>

constexpr uint64_t factorial(int n)
{
	int ret = 1;
	for(int i = 2; i <= n; ++i)
		ret *= i;
	return ret;
}

// Counter of the repetitions of digit sums in the number given
// number length and base
class DigitSumsCounter
{
	// The idea here is the following. We can construct different numbers
	// with a fixed collection of digits; e.g. with digits [1, 2, 3] we
	// can have 123, 132, ... The total number of such numbers is length!.
	// Then we consider the repetitions: for digits  [x, y, z] we can have
	// xyz, xzy, ... but if x=y then xyz = yxz; thus we have to divide our
	// total number (length!) by (repetition_count!) for each repetition.
	public:
		DigitSumsCounter(int len, int base)
			: number_length(len)
			, base(base)
			, max_digits_sum(len * (base - 1))
			, sums_count(max_digits_sum + 1, 0)
		{}
		// Return vector with digit sums as indicies and a count of
		// such numbers as values
		std::vector<uint64_t> count()
		{
			add_digit(base - 1, 0, number_length, 1);
			return sums_count;
		}
	private:
		const int number_length;
		const int base;
		const int max_digits_sum;
		std::vector<uint64_t> sums_count;

		// Add digit 0 to len times to the number, and then proceed
		// with (digit - 1)
		void add_digit(int digit, int prefix_sum, int len, uint64_t repetition_factor)
		{
			if(digit == 0)
			{
				repetition_factor *= factorial(len);
				static uint64_t permutations_count = factorial(number_length);
				sums_count.at(prefix_sum) += permutations_count / repetition_factor;
			}
			else
			{
				for(int i = 0; i <= len; ++i)
				{
					add_digit(digit - 1, prefix_sum + digit * i, len - i,
							repetition_factor * factorial(i));
				}
			}
		}
};

uint64_t count_nice_numbers(int number_length, int compare_length, int base)
{
	auto sums_count = DigitSumsCounter(compare_length, base).count();

	uint64_t count = 0;
	for(auto &c: sums_count)
		count += c * c;

	int ignored_numbers_count = number_length - 2 * compare_length;
	while(ignored_numbers_count-- > 0)
		count *= base;

	return count;
}

int main(int, char**)
{
	constexpr int number_length = 13;
	constexpr int compare_length = 6;
	constexpr int base = 13;
	printf("%lu\n", count_nice_numbers(
				number_length, compare_length, base));
	return 0;
}
