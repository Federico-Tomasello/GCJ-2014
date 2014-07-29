// code.cpp : Defines the entry point for the console application.
//

#pragma warning (disable: 4786)


#include <iostream>
#include <typeinfo>
#include <string>
#include <fstream>
#include <cmath>
#include <cstdio>
#include <algorithm>
#include <map>
#include <numeric>
#include <queue>
#include <set>
#include <utility>
#include <vector>

using namespace std;

const char out_name[]= "E:\\Desktop\\gcj\\data.out";
//const char out_name[]= "E:\\Desktop\\gcj\\large.out";

const char in_name[]= "E:\\Desktop\\gcj\\data.in";
//const char in_name[]= "E:\\Desktop\\gcj\\large.in";


/* Minimum and maximum macros */

#define max(a,b)  (((a) > (b)) ? (a) : (b))
#define min(a,b)  (((a) < (b)) ? (a) : (b))

const unsigned int LINES= 1;
const bool WIN= false;

const char SEP= ' ';
//const char SEP= 0;



inline vector<unsigned int> u_convert(vector<const char*>& in)
{
	vector<unsigned int> out;
	const unsigned int max= in.size();
	for (register unsigned int i= 0; i < max; ++i) out.push_back(atoi(in[i]));
	return out;
};

inline vector<int> convert(vector<const char*>& in)
{
	vector<int> out;
	const unsigned int max= in.size();
	for (register unsigned int i= 0; i < max; ++i) out.push_back(atoi(in[i]));
	return out;
};


class general_program
{
	ofstream out;
	unsigned int max_case;

protected:

	unsigned int _case;
	vector< vector<const char*> > argv;

public:
	general_program(): out(out_name, ios::out|ios::binary), _case (0), max_case (0) {};
	virtual ~general_program()
	{
		out.close();
	};

	virtual int input(char* buffer) = 0;
	virtual int output(const char* buffer)
	{
		if (_case) out <<"Case #" << _case << ":" << buffer;
		else max_case= atoi(argv[0][0]);
//		cout <<"Case #" << _case << ": " << buffer << endl;
		return ++_case;
	};

	virtual void parse_with_spaces(char* buffer)
	{
		vector <const char*> tmp;
		tmp.push_back(buffer);
		argv.push_back(tmp);
	};
	virtual void parse(char* buffer)
	{
		vector <const char*> tmp;
		char* p= buffer;
		char* arg= buffer;
		while (*p)
		{
			if (*p == SEP)
			{
				*p= 0;
				tmp.push_back(arg);
				arg= p +1;
			}
			++p;
		}
		tmp.push_back(arg);
		argv.push_back(tmp);
	};
};

class program: public general_program
{
	unsigned int line_count;
	unsigned int lines_per_action;

	void Init();

public:

	program(): line_count(0), lines_per_action(LINES) {Init();};
	virtual ~program() {};

	int my_program();
	virtual int input(char* buffer)
	{
		parse(buffer);
//		parse_with_spaces(buffer);
		if (!_case) return output(buffer);

		++line_count;
		if (line_count == lines_per_action)
		{
			line_count= 0;
			my_program();
		}
		return 0;
	};

/*
	virtual int output() {return 0;};
*/
};

int main(int argc, const char* argv[])
{
//	if (argc == 2 && argv[1])
	{
//		ifstream file (argv[1], ios::in|ios::binary|ios::ate);
		ifstream file (in_name, ios::in|ios::binary|ios::ate);
		if (file.is_open())
		{
			streampos size = file.tellg();
			char* memblock = new char[size];

			file.seekg(0, ios::beg);
			file.read(memblock, size);
			file.close();
			
			char* line= memblock;
			char* buffer= line;
			char* last= memblock +size;

			program p;

			while (line < last)
			{
				if (WIN && *line == '\r' && line+1 < last && *(line+1) == '\n')
				{
					*line= 0;
					p.input(buffer);
					buffer= line +2;
					++line;
				}
				else if (*line == '\n')
				{
					*line= 0;
					p.input(buffer);
					buffer= line +1;
				}
				++line;
			}
			delete[] memblock;
		}
		else
		{
			cout << "Unable to open file" << endl;
			return 1;
		}
	}
	return 0;
}

map<char, const char*> seq;

void program::Init()
{
};

bool possible(unsigned int R, unsigned int C, unsigned int M, int& cas, unsigned int& holes)
{
	if (!M)
	{
		cas= -1;
		return true;
	}
	holes= (R * C) -M;
	if (holes == 1)
	{
		cas= 0;
		return true;
	}
	unsigned int dim= min(R, C);
	switch (dim)
	{
	case 1:
		cas= 1;
		return true;
		break;
	case 2:
		cas= 2;
		return (holes > 2) && (holes %2 == 0);
		break;
	default:
		cas= 3;
		return (holes == 4) || (holes == 6) || (holes > 7);
		break;
	}
	return false;
}

const char CLCK= 'c';
const char HOLE= '.';
const char MINE= '*';

class plane
{
	char** p;
	unsigned int row;
	unsigned int column;

public:

	plane(unsigned int R, unsigned int C): p (0), row(R), column(C)
	{
		p= new char*[row];
		for (register unsigned int i= 0; i<row; ++i)
		{
			p[i]= new char[column +1];
			memset(p[i], MINE, column);
			p[i][column]= 0;
		}
	}
	virtual ~plane()
	{
		for (register unsigned int i= 0; i<row; ++i)
			delete [] p[i];
		delete [] p;
	};
	inline void SetState(unsigned int r, unsigned int c, char state) {p[r][c]= state;};
	inline char GetState(unsigned int r, unsigned int c) {return p[r][c];};
	inline char* GetRow(unsigned int r) {return p[r];};

	inline void SetHole4()
	{
		p[0][0]= CLCK;
		p[0][1]= HOLE;
		p[1][0]= HOLE;
		p[1][1]= HOLE;
	};
	inline void SetHole9()
	{
		p[0][0]= CLCK;
		p[0][1]= HOLE;
		p[0][2]= HOLE;
		p[1][0]= HOLE;
		p[1][1]= HOLE;
		p[1][2]= HOLE;
		p[2][0]= HOLE;
		p[2][1]= HOLE;
		p[2][2]= HOLE;
	};

	inline void SetLateralC(unsigned int c) {p[0][c]= HOLE; p[1][c]= HOLE;};
	inline void SetLateralR(unsigned int r) {p[r][0]= HOLE; p[r][1]= HOLE;};
};

int program::my_program()
{
	const unsigned int vi= 1+ (_case -1) * lines_per_action;

	vector<const char*>& P= argv[vi];
	const unsigned int max= P.size();

	unsigned int R= atoi(P[0]);
	unsigned int C= atoi(P[1]);
	unsigned int M= atoi(P[2]);

	int cas= 0;
	unsigned int holes= 0;
	string out;
	if (!possible(R, C, M, cas, holes)) return output("\nImpossible\n");

	out += "\n";

	plane p(R, C);

	register unsigned int i= 0;
	register unsigned int j= 0;

	switch(cas)
	{
	case -1:
		out += "c";
		for (i= 0; i<R; ++i)
		{
			for (j= 0; j <C; ++j)
			{
				if (i != 0 || j != 0) out += ".";
			}
			out += "\n";
		}
		break;
	case 0:
		out += "c";
		for (i= 0; i<R; ++i)
		{
			for (j= 0; j <C; ++j)
			{
				if (i != 0 || j != 0) out += "*";
			}
			out += "\n";
		}
		break;
	case 1:
		out += "c";
		for (i= 0; i<R; ++i)
		{
			for (j= 0; j <C; ++j)
			{
				if (i != 0 || j != 0)
				{
					if (holes != 0) --holes;
					if (holes != 0) out += ".";
					else out += "*";
				}
			}
			out += "\n";
		}
		break;
	case 2:
/*		out += "c";
		for (i= 0; i<R; ++i)
		{
			for (j= 0; j <C; ++j)
			{
				if (i != 0 || j != 0)
				{
					if (--holes)
					{
						if (C == 2)
						{
							out += ".";
						}
						else
						{
							static ndx= (holes +1) /2;
							if (j <ndx) out += ".";
							else out += "*";
						}						
					}
					else out += "*";
				}
			}
			out += "\n";
		}
		break;
*/	case 3:
		{
			bool holes_are_even= holes %2 == 0;
			if (holes_are_even)
			{
				p.SetHole4();
				holes -=4;

				unsigned int lateral= holes /2;
				unsigned int c_av= C -2;
				unsigned int r_av= R -2;

				while (holes && c_av)
				{
					p.SetLateralC(C -c_av);
					--c_av;
					holes -=2;
				}
				while (holes && r_av)
				{
					p.SetLateralR(R -r_av);
					--r_av;
					holes -=2;
				}
				for (unsigned int r= 2; holes && r <R; ++r)
					for (unsigned int c=2; holes && c <C; ++c)
					{
						p.SetState(r, c, HOLE);
						--holes;
					}
			}
			else
			{
				p.SetHole9();
				holes -=9;

				unsigned int lateral= holes /2;
				unsigned int c_av= C -3;
				unsigned int r_av= R -3;

				while (holes && c_av)
				{
					p.SetLateralC(C -c_av);
					--c_av;
					holes -=2;
				}
				while (holes && r_av)
				{
					p.SetLateralR(R -r_av);
					--r_av;
					holes -=2;
				}
				for (unsigned int r= 2; holes && r <R; ++r)
					for (unsigned int c=2; holes && c <C; ++c)
					{
						if (r != 2 || c != 2)
						{
							p.SetState(r, c, HOLE);
							--holes;
						}
					}
			}
		}

		for (i= 0; i<R; ++i)
		{
			out += p.GetRow(i);
			out += "\n";
		}
		break;
	}
	return output(out.c_str());
}

