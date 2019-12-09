#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <streambuf>
#include <iterator>
#include <sstream>
#include <algorithm>
#include <stdio.h>
#include <map>

struct Coord
{
	int x, y;
	Coord() : x(0), y(0) {}
	Coord(int x, int y) : x(x), y(y) {}
	Coord(const Coord& rhs) : x(rhs.x), y(rhs.y) {}
};

uint64_t Hash(const Coord& c) { return (uint64_t)c.x << 32 | c.y; }

Coord operator+ (const Coord& a, const Coord& b)
{
	return Coord(a.x + b.x, a.y + b.y);
}

Coord& operator+=(Coord& lhs, const Coord& rhs)
{
	lhs.x += rhs.x;
	lhs.y += rhs.y;
	return lhs;
}


enum Direction
{
	Up,
	Left,
	Down,
	Right,
	Count
};

struct Instruction
{
	Direction Direction;
	Coord Coord;
};

template <typename Out>
void split(const std::string& s, char delim, Out result) {
	std::istringstream iss(s);
	std::string item;
	while (std::getline(iss, item, delim)) {
		*result++ = item;
	}
}

std::vector<std::string> split(const std::string& s, char delim) {
	std::vector<std::string> elems;
	split(s, delim, std::back_inserter(elems));
	return elems;
}

std::string ReadFile(const std::string &path)
{
	std::ifstream t(path);
	std::string str;

	t.seekg(0, std::ios::end);
	str.reserve(t.tellg());
	t.seekg(0, std::ios::beg);

	str.assign((std::istreambuf_iterator<char>(t)),
		std::istreambuf_iterator<char>());
	return str;
}

/*std::vector<std::vector<Instruction>>*/ /*int ParseFile(const std::string& path)
{
	std::string contents = ReadFile(path);
	char cDir;
	int dist;
	for (auto& line : split(contents, '\n'))
	{
		for (auto& move : split(line, ','))
		{
			sscanf_s(move.c_str(), "%c%i", &cDir, 1, &dist);
			std::cout << dist << '\n';
		}
	}
	return 0;
}*/

int main(int argc, char* argv[])
{
	typedef std::map<Coord, int> grid_t;

	std::string contents = ReadFile(argv[1]);
	std::vector<grid_t> maps;
	
	char cDir;
	int dist;
	Coord dir;
	Coord pos;
	for (auto& line : split(contents, '\n'))
	{
		grid_t grid;
		int step = 0;

		for (auto& move : split(line, ','))
		{
			sscanf_s(move.c_str(), "%c%i", &cDir, 1, &dist);
			//std::cout << dist << '\n';
			
			switch (cDir)
			{
			case 'U': dir = Coord(0, -1); break;
			case 'D': dir = Coord(0, 1); break;
			case 'L': dir = Coord(-1, 0); break;
			case 'R': dir = Coord(1, 0); break;
			default:
				throw new std::exception();
				break;
			}

			for (int d = 0; d < dist; ++d)
			{
				pos += dir;
				++step;
				if (grid.find(pos) == grid.end())
				{
					grid[pos] = step;
				}
			}
		}
	}

	/*auto fp = argv[1];
	auto wires = ParseFile(fp);
    std::cout << "Hello World!\n" << fp;*/
}