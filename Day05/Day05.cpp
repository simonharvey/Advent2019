//#include "pch.h"
#include <iostream>
#include <vector>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <functional>
#include <stack>
#include <iterator>
#include <assert.h>
#include <thread>
#include <queue>
#include <future>
#include "Queue.h"
#include <chrono>

using namespace std;

typedef vector<int64_t> data_t;

enum class OpCode
{
	Add = 1,
	Mult = 2,
	Input = 3,
	Output = 4,
	JIT = 5,
	JIF = 6,
	Less = 7,
	Equals = 8,
	Halt = 99,
};

int std_input()
{
	int value;
	cout << "Input:";
	cin >> value;
	return value;
}

void std_output(int value)
{
	cout << "Output: " << value << '\n';
}

enum class Mode
{
	Position = 0,
	Immediate = 1,
};

#pragma warning(push)
#pragma warning(disable:26451)
class CPU
{
	int64_t pc = 0;

public:

	void Execute(data_t& data, std::function<int64_t()> input = std_input, std::function<void(int64_t)> output = std_output)
	{
		bool done = false;

		while (!done)
		{
			auto instr = data[pc];
			auto op = (OpCode)(instr % 100);
			auto param = [&](int p) -> decltype(auto) { return GetValue(data, pc, p); };
			switch (op)
			{
			case OpCode::Add:
			{
				param(2) = param(0) + param(1);
				pc += 4;
				break;
			}
			case OpCode::Mult:
			{
				param(2) = param(0) * param(1);
				pc += 4;
				break;
			}
			case OpCode::Input:
			{
				auto value = input();
				data[data[pc+1]] = value;
				pc += 2;
				break;
			}
			case OpCode::Output:
			{
				output(param(0));
				pc += 2;
				break;
			}
			case OpCode::JIT:
			{
				if (param(0) != 0)
				{
					pc = param(1);
				}
				else
				{
					pc += 3;
				}
				break;
			}
			case OpCode::JIF:
			{
				if (param(0) == 0)
				{
					pc = param(1);
				}
				else
				{
					pc += 3;
				}
				break;
			}
			case OpCode::Less:
			{
				param(2) = param(0) < param(1) ? 1 : 0;
				pc += 4;				
				break;
			}
			case OpCode::Equals:
			{
				param(2) = param(0) == param(1) ? 1 : 0;
				pc += 4;
				break;
			}
			case OpCode::Halt:
			{
				done = true;
				//cout << "Done\n";
				break;
			}
			default:
			{
				cout << "Invalid Opcode: " << (int)op << '\n';
				exit((int)op);
				break;
			}
			}
		}
	}

private:

	int64_t& GetValue(data_t& data, int64_t pc, int paramIdx)
	{
		auto instr = data[pc++];
		for (int i = 0; i < paramIdx + 2; ++i)
		{
			instr /= 10;
		}

		switch ((Mode)(instr % 10))
		{
		case Mode::Position: return data.at(data[pc + paramIdx]);
			break;
		case Mode::Immediate: return data.at(pc + paramIdx);
			break;
		default:
			throw exception("Invalid mode");
			break;
		}
	}
};

#pragma warning(pop)

data_t Tokenize(const string& input)
{
	data_t result;
	istringstream tokenStream(input);
	string tokenStr;
	while (std::getline(tokenStream, tokenStr, ','))
	{
		result.push_back(stol(tokenStr));
	}
	return result;
}

const auto DAY_5 = "3,225,1,225,6,6,1100,1,238,225,104,0,1102,7,85,225,1102,67,12,225,102,36,65,224,1001,224,-3096,224,4,224,1002,223,8,223,101,4,224,224,1,224,223,223,1001,17,31,224,1001,224,-98,224,4,224,1002,223,8,223,101,5,224,224,1,223,224,223,1101,86,19,225,1101,5,27,225,1102,18,37,225,2,125,74,224,1001,224,-1406,224,4,224,102,8,223,223,101,2,224,224,1,224,223,223,1102,13,47,225,1,99,14,224,1001,224,-98,224,4,224,102,8,223,223,1001,224,2,224,1,224,223,223,1101,38,88,225,1102,91,36,224,101,-3276,224,224,4,224,1002,223,8,223,101,3,224,224,1,224,223,223,1101,59,76,224,1001,224,-135,224,4,224,102,8,223,223,1001,224,6,224,1,223,224,223,101,90,195,224,1001,224,-112,224,4,224,102,8,223,223,1001,224,7,224,1,224,223,223,1102,22,28,225,1002,69,47,224,1001,224,-235,224,4,224,1002,223,8,223,101,5,224,224,1,223,224,223,4,223,99,0,0,0,677,0,0,0,0,0,0,0,0,0,0,0,1105,0,99999,1105,227,247,1105,1,99999,1005,227,99999,1005,0,256,1105,1,99999,1106,227,99999,1106,0,265,1105,1,99999,1006,0,99999,1006,227,274,1105,1,99999,1105,1,280,1105,1,99999,1,225,225,225,1101,294,0,0,105,1,0,1105,1,99999,1106,0,300,1105,1,99999,1,225,225,225,1101,314,0,0,106,0,0,1105,1,99999,107,226,226,224,102,2,223,223,1006,224,329,1001,223,1,223,1107,677,226,224,1002,223,2,223,1005,224,344,101,1,223,223,108,677,226,224,102,2,223,223,1006,224,359,101,1,223,223,7,677,226,224,102,2,223,223,1005,224,374,101,1,223,223,1008,677,226,224,1002,223,2,223,1006,224,389,1001,223,1,223,7,226,677,224,102,2,223,223,1005,224,404,101,1,223,223,1007,226,226,224,102,2,223,223,1006,224,419,101,1,223,223,7,226,226,224,102,2,223,223,1005,224,434,1001,223,1,223,8,226,226,224,1002,223,2,223,1006,224,449,101,1,223,223,1007,677,677,224,102,2,223,223,1006,224,464,101,1,223,223,1007,226,677,224,1002,223,2,223,1006,224,479,101,1,223,223,108,226,226,224,102,2,223,223,1005,224,494,1001,223,1,223,1108,677,677,224,102,2,223,223,1005,224,509,1001,223,1,223,107,226,677,224,1002,223,2,223,1005,224,524,101,1,223,223,1108,677,226,224,1002,223,2,223,1005,224,539,1001,223,1,223,1008,677,677,224,1002,223,2,223,1006,224,554,101,1,223,223,1008,226,226,224,102,2,223,223,1005,224,569,1001,223,1,223,8,677,226,224,102,2,223,223,1006,224,584,101,1,223,223,107,677,677,224,102,2,223,223,1006,224,599,101,1,223,223,8,226,677,224,102,2,223,223,1006,224,614,101,1,223,223,1107,226,677,224,102,2,223,223,1006,224,629,101,1,223,223,108,677,677,224,1002,223,2,223,1005,224,644,1001,223,1,223,1107,226,226,224,102,2,223,223,1005,224,659,101,1,223,223,1108,226,677,224,102,2,223,223,1005,224,674,101,1,223,223,4,223,99,226";
const auto DAY_7 = "3,8,1001,8,10,8,105,1,0,0,21,34,51,76,101,126,207,288,369,450,99999,3,9,102,4,9,9,1001,9,2,9,4,9,99,3,9,1001,9,2,9,1002,9,3,9,101,3,9,9,4,9,99,3,9,102,5,9,9,1001,9,2,9,102,2,9,9,101,3,9,9,1002,9,2,9,4,9,99,3,9,101,5,9,9,102,5,9,9,1001,9,2,9,102,3,9,9,1001,9,3,9,4,9,99,3,9,101,2,9,9,1002,9,5,9,1001,9,5,9,1002,9,4,9,101,5,9,9,4,9,99,3,9,1002,9,2,9,4,9,3,9,102,2,9,9,4,9,3,9,1001,9,1,9,4,9,3,9,102,2,9,9,4,9,3,9,1001,9,1,9,4,9,3,9,101,1,9,9,4,9,3,9,1001,9,2,9,4,9,3,9,1002,9,2,9,4,9,3,9,1001,9,1,9,4,9,3,9,1001,9,1,9,4,9,99,3,9,102,2,9,9,4,9,3,9,101,2,9,9,4,9,3,9,102,2,9,9,4,9,3,9,1002,9,2,9,4,9,3,9,1001,9,2,9,4,9,3,9,101,1,9,9,4,9,3,9,1001,9,2,9,4,9,3,9,102,2,9,9,4,9,3,9,101,1,9,9,4,9,3,9,102,2,9,9,4,9,99,3,9,101,2,9,9,4,9,3,9,101,2,9,9,4,9,3,9,101,1,9,9,4,9,3,9,1001,9,2,9,4,9,3,9,101,2,9,9,4,9,3,9,101,2,9,9,4,9,3,9,102,2,9,9,4,9,3,9,102,2,9,9,4,9,3,9,1001,9,2,9,4,9,3,9,1001,9,1,9,4,9,99,3,9,1001,9,2,9,4,9,3,9,101,1,9,9,4,9,3,9,102,2,9,9,4,9,3,9,1001,9,1,9,4,9,3,9,101,1,9,9,4,9,3,9,101,2,9,9,4,9,3,9,1001,9,1,9,4,9,3,9,101,2,9,9,4,9,3,9,102,2,9,9,4,9,3,9,102,2,9,9,4,9,99,3,9,1001,9,2,9,4,9,3,9,102,2,9,9,4,9,3,9,1002,9,2,9,4,9,3,9,102,2,9,9,4,9,3,9,102,2,9,9,4,9,3,9,1001,9,1,9,4,9,3,9,101,2,9,9,4,9,3,9,102,2,9,9,4,9,3,9,102,2,9,9,4,9,3,9,102,2,9,9,4,9,99";
const auto DAY_7_T1 = "3,15,3,16,1002,16,10,16,1,16,15,15,4,15,99,0,0";
const auto DAY_7_2_T1 = "3,26,1001,26,-4,26,3,27,1002,27,2,27,1,27,26,27,4,27,1001,28,-1,28,1005,28,6,99,0,0,5";

template <typename T>
T pop_stack(stack<T> &s)
{
	auto v = s.top();
	s.pop();
	return v;
}

int64_t Day7(const data_t& data)
{
	std::vector<int> v = { 0,1,2,3,4 };
	std::stack<int64_t> stack;
	int64_t max = std::numeric_limits<int64_t>().min();

	do 
	{
		stack.push(0); // inital signal
		
		for (auto& i : v)
		{
			assert(stack.size() == 1);
			auto datac = data_t(data);
			CPU cpu;
			stack.push(i);
			cpu.Execute(datac, 
				[&]() { /*cout << "pop: " << stack.top() << "\n";*/ return pop_stack(stack); },
				[&](int64_t v) { /*cout << "push\n";*/ stack.push(v); }
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
		//cout << stack.top() << '\n';
		stack.pop();
	} 
	while (std::next_permutation(v.begin(), v.end()));

	return max;
}

int64_t Day7_2(const data_t& data)
{
	using namespace std::chrono_literals;

	std::vector<int> v = { 5, 6, 7, 8, 9 };
	int64_t max = std::numeric_limits<int64_t>().min();

	do
	{
		CPU amps[5];
		Queue<int64_t> inputQueue[5];
		thread threads[5];

		for (int i=0; i<5; ++i)
		{
			CPU &cpu = amps[i];
			inputQueue[i].push(v[i]);
			threads[i] = thread([&, i]() {
				auto datac = data_t(data);
				cpu.Execute(datac,
					[&]() { return inputQueue[i].pop(); },
					[&](int64_t v) 
					{ 
						//std::this_thread::sleep_for(.01s);
						inputQueue[(i+1)%5].push(v); 
					}
				);
			});
		}

		inputQueue[0].push(0);

		for (int i = 0; i < 5; ++i)
		{
			threads[i].join();
		}

		auto output = inputQueue[0].pop();
		if (output > max)
		{
			max = output;
		}

		std::stringstream result;
		std::copy(v.begin(), v.end(), std::ostream_iterator<int>(result, ""));

		cout << result.str().c_str() << ": " << output << '\n';
	} 
	while (std::next_permutation(v.begin(), v.end()));

	return max;
}

int main()
{
	//cout << "Max: " << Day7(Tokenize(DAY_7));
	cout << "Max: " << Day7_2(Tokenize(DAY_7));
	/*auto data = Tokenize(DAY_5);
	CPU cpu;
	cpu.Execute(data);*/
 }