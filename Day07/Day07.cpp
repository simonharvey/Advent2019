// Day07.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <algorithm>
#include <iostream>
#include <iterator>
#include <stack>
#include <cpu.h>

using namespace cpu;

int64_t part_1(const data_t& data)
{
	std::vector<int> v = { 0,1,2,3,4 };
	std::stack<int64_t> stack;
	int64_t max = std::numeric_limits<int64_t>().min();

	do
	{
		stack.push(0); // inital signal

		for (auto& i : v)
		{
			auto datac = data;
			CPU cpu;
			stack.push(i);
			cpu.Execute(datac,
				[&]() 
				{
					auto v = stack.top();
					stack.pop();
					return v; 
				},
				[&](int64_t v) 
				{ 
					stack.push(v); 
				}
			);
		}

		auto output = stack.top();
		if (output > max)
		{
			max = output;
		}

		std::stringstream result;
		std::copy(v.begin(), v.end(), std::ostream_iterator<int>(result, ""));

		cout << result.str().c_str() << ": " << output << '\n';
		stack.pop();
	} while (std::next_permutation(v.begin(), v.end()));

	return max;
}

int main()
{
    //std::cout << ;
}
