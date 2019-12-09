#pragma once

#include <functional>
#include <iostream>
#include <sstream>
#include <vector>

namespace cpu
{
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

	enum class Mode
	{
		Position = 0,
		Immediate = 1,
	};

	int64_t std_input()
	{
		int value;
		cout << "Input:";
		cin >> value;
		return value;
	}

	void std_output(int64_t value)
	{
		cout << "Output: " << value << '\n';
	}

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
					data[data[pc + 1]] = value;
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
					break;
				}
				default:
				{
					cout << "Invalid Opcode: " << (int)op << '\n';
					//exit((int)op);
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
}