// Day04.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

bool is_match(int64_t v)
{
	bool valid = true;
	auto last = std::numeric_limits<int64_t>().max();
	auto stride = 0;
	bool has_pair = false;

	while (valid)
	{
		auto d = v % 10;
		valid = d <= last;
		if (d == last)
		{
			++stride;
		}
		else
		{
			if (stride == 1)
				has_pair = true;
			stride = 0;
		}
		v /= 10;
		last = d;
		if (v == 0)
			break;
	}

	return (valid && has_pair || (!has_pair && stride == 1));
}

int main()
{
	std::cout << is_match(112233);
	int64_t matches = 0;

	for (int64_t i = 197487; i < 673251; ++i)
	{
		if (is_match(i))
		{
			std::cout << i << '\n';
			++matches;
		}
	}

	std::cout << matches;
}
