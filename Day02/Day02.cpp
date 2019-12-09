// Day02.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <cpu.h>

void part_1(const cpu::data_t &input_data)
{
	cpu::CPU cpu;

	auto data = input_data;

	data[1] = 12;
	data[2] = 2;

	cpu.Execute(data);
	std::cout << data[0] << '\n';
}

void part_2(const cpu::data_t &input_data)
{
	for (auto noun = 0; noun < 99; ++noun)
	{
		for (auto verb = 0; verb < 99; ++verb)
		{
			cpu::CPU cpu;
			auto data = input_data;

			data[1] = noun;
			data[2] = verb;

			cpu.Execute(data);
			if (data[0] == 19690720)
			{
				std::cout << verb << " " << noun << " " << (100 * noun) + verb << '\n';
				return;
			}
		}
	}
}

int main()
{
	const auto raw_data = cpu::Tokenize("1,0,0,3,1,1,2,3,1,3,4,3,1,5,0,3,2,6,1,19,1,19,10,23,2,13,23,27,1,5,27,31,2,6,31,35,1,6,35,39,2,39,9,43,1,5,43,47,1,13,47,51,1,10,51,55,2,55,10,59,2,10,59,63,1,9,63,67,2,67,13,71,1,71,6,75,2,6,75,79,1,5,79,83,2,83,9,87,1,6,87,91,2,91,6,95,1,95,6,99,2,99,13,103,1,6,103,107,1,2,107,111,1,111,9,0,99,2,14,0,0");
	
	part_1(raw_data);
	part_2(raw_data);
}