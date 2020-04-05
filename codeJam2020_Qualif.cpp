#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <cstring>
#include <algorithm>

using i64 = long long;
using u64 = unsigned long long;
using i32 = int;
using u32 = unsigned int;
using i16 = short;
using u16 = unsigned short;
using u8  = unsigned char;
using i8  = char;
using r32 = float;
using r64 = double;

void flip(u32* sArray, u32 index, u32 bits)
{
	for(u32 ind = 1; ind < index; ++ind)
	{
		u32 value = sArray[ind - 1];
		u32 valueEnd = sArray[bits - ind];
		sArray[ind - 1] = value == 0 ? 1 : 0; 
		sArray[bits - ind] = valueEnd == 0 ? 1 : 0; 
	}
}

void reverse(u32* sArray, u32 index, u32 bits)
{
	for(u32 ind = 1; ind < index; ++ind)
	{
		u32 value = sArray[ind - 1];
		u32 valueEnd = sArray[bits - ind];
		sArray[ind - 1] = valueEnd; 
		sArray[bits - ind] = value; 
	}
}

void printResult(u32* sArray, u32 bits, std::ostream& out)
{
	std::string result;
	for (u32 x = 0; x < bits; ++x)
	{
		result += ((sArray[x] == 0) ? '0' : '1');
	}
	out << result << std::endl;
}

#define check(c)    \
	if ((c) == 'N') \
		return;

void ESAbATAd(std::istream& in, std::ostream& out)
{
	u32 testCases = 0;

	in >> testCases;

	u32 bits = 0;

	in >> bits;

	u32* sArray = (u32*)malloc(bits * sizeof(u32));

	for (u32 i = 1; i <= testCases; ++i)
	{
		u32 index = 1;
		char c = 0;
		char cEnd = 0;
		bool atEnd = false;
		i32 lastDifferent = -1;
		i32 lastSame      = -1;

		u32 submissionAllowed = 5;

		while (index <= (bits / 2))
		{
			bool nextShuffle = (submissionAllowed == 0);

			if(nextShuffle) 
			{
				//@Note did we reversed, flip, both or none ?
				if(lastDifferent != -1 && lastSame != -1)
				{
					out << lastDifferent << std::endl;
					in >> c;
					check(c);

					out << lastSame << std::endl;
					in >> cEnd;
					check(cEnd);

					u32 lastDifferentValue =  (c == '0' ? 0 : 1);
					u32 lastSameValue =  (cEnd == '0' ? 0 : 1);

					if(lastDifferentValue == sArray[lastDifferent - 1])
					{
						//@Note could be Nothing, or reversed flip
						if(lastSameValue == sArray[lastSame - 1])
						{
							//@Note could be reverse, or nothing
							//@Note only nothing intersects, so do nothing
						}
						else
						{
							//@Note could be flip, or reversed flip
							//@Note reversed flip
							reverse(sArray, index, bits);
							flip(sArray, index, bits);
						}
					}
					else
					{
						//@Note could be flip or reverse
						if(lastSameValue == sArray[lastSame - 1])
						{
							//@Note could be reverse, or nothing
							//@Note only reverse intersects
							reverse(sArray, index, bits);
						}
						else
						{
							//@Note could be flip, or reversed flip
							//@Note flip
							flip(sArray, index, bits);
						}
					}

				}
				else if(lastDifferent == -1) //@Note No different, reverse won't affect us, just need to know if we flipped
				{
					out << 1 << std::endl;
					in >> c;
					check(c);
					u32 value = (c == '0' ? 0 : 1);
					if(value != sArray[0])
					{
						flip(sArray, index, bits);
					}

					out << 1 << std::endl; // Unused, just to have paired submissions allowed
					in >> c;
					check(c);
				}
				else
				{
					out << 1 << std::endl;
					in >> c;
					check(c);
					u32 value = (c == '0' ? 0 : 1);
					if(value != sArray[0])
					{
						reverse(sArray, index, bits);
					}

					out << 1 << std::endl; // Unused, just to have paired submissions allowed
					in >> c;
					check(c);
				}
				submissionAllowed = 4;
			}

			out << index << std::endl;
			in >> c;

			check(c);

			out << (bits - index + 1) << std::endl;
			in >> cEnd;

			check(cEnd);

			sArray[index - 1] = (c == '0' ? 0 : 1);
			sArray[bits - index] = (cEnd == '0' ? 0 : 1);

			if(c != cEnd)
			{
				lastDifferent = index;
			}
			else
			{
				lastSame = index;
			}

			--submissionAllowed;
			++index;
		}

		printResult(sArray, bits, out);
		in >> c;

		check(c);
	}

	free(sArray);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

struct Interval
{
	u32 begin = 0;
	u32 end = 0;
	u32 index = 0;
};

struct Track
{
	Interval t[1000];
	u32 count  = 0;
};

bool overlaps(Interval& a, Interval& b)
{
	return (a.begin <= b.begin && b.begin < a.end)
	    || (a.begin < b.end && b.end <= a.end)
		|| (a.begin >= b.begin && a.end <= b.end);
}

void insert(Track& a, Interval b)
{
	a.t[a.count] = b;
	++a.count;
}

bool overlaps(Track& a, Interval& b)
{
	bool result = false;
	for(u32 i = 0; i < a.count; ++i)
	{
		Interval* in = a.t + i;
		bool overlap = overlaps(b, *in);
		if(overlap)
		{
			result = true;
			break;
		}
	}
	return result;
}

void ParentingPartneringReturns(std::istream& in, std::ostream& out)
{
	u32 testCases = 0;

	in >> testCases;

	for (u32 i = 1; i <= testCases; ++i)
	{
		Track one = {};
		Track two = {};
		Track all = {};

		u32 n;
		in >> n;

		for(u32 a = 0; a < n; ++a)
		{
			Interval interval = {};

			in >> interval.begin;
			in >> interval.end;
			interval.index = a;

			insert(all, interval);
		}

		std::sort(all.t, all.t + all.count, [](const Interval& i, const Interval& s)
		{
			return (i.begin) < (s.begin); 
		});

		std::string result(n, ' ');

		for(u32 ind = 0; ind < n; ++ind)
		{
			Interval* interval = all.t + ind;
			if(overlaps(one, *interval))
			{
				if(overlaps(two, *interval))
				{
					result = "IMPOSSIBLE";
					break;
				}
				else
				{
					insert(two, *interval);
					result[interval->index] = 'J';
				}
			}
			else
			{
				insert(one, *interval);
				result[interval->index] = 'C';
			}
		}

		out << "Case #" << i << ": " << result << std::endl;
	}
}


// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

struct set
{
	u32 values[100];
	u32 size = 0;
};

void insert(set& s, u32 value)
{
	if(s.values[value - 1] == 0)
	{
		s.values[value - 1] = 1;
		++s.size;
	}
}

void Vestigium(std::istream& in, std::ostream& out)
{
	u32 testCases = 0;

	in >> testCases;

	u32 size = sizeof(set) * 100;
	set *columns = (set*)malloc(size);

	for (u32 i = 1; i <= testCases; ++i)
	{
		u32 n;

		in >> n;

		u32 diagonal = 0;
		u32 rows = 0;
		u32 cols = 0;

		std::memset(columns, 0, size);

		for (u32 y = 0; y < n; ++y)
		{
			set row = {};
			for (u32 x = 0; x < n; ++x)
			{
				u32 value;
				in >> value;
				insert(row, value);
				if(x == y)
				{
					diagonal += value;
				}
				set* col = columns + x;
				insert(*col, value);
			}

			rows += row.size == n ? 0 : 1;
		}

		for(u32 x = 0; x < n; ++x)
		{
			set* col = columns + x;
			cols += col->size == n ? 0 : 1;
		}

		out << "Case #" << i << ": " << diagonal << ' ' << rows << ' ' << cols << std::endl;
	}

	free(columns);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void nestingDepth(std::istream& in, std::ostream& out)
{
    u32 testCases = 0;

	in >> testCases;

	for (u32 i = 1; i <= testCases; ++i)
	{
		u32 currentDepth = 0;
		std::string result;

		std::string s;
		in >> s;

		for(char c : s)
		{
			u32 value = c - '0';
			i32 difference = (i32)currentDepth - (i32)value;
			u32 toPrint = 0;

			if (difference < 0)
			{
				toPrint = -difference;
				for (u32 index = 0; index < toPrint; ++index)
				{
					result += '(';
				}
				currentDepth += -difference;
			}
			else if (difference > 0)
			{
				toPrint = (u32)difference;
				for (u32 index = 0; index < toPrint; ++index)
				{
					result += ')';
				}
				currentDepth -= difference;
			}

			result += c;
		}

		for (u32 index = 0; index < currentDepth; ++index)
		{
			result += ')';
		}

		out << "Case #" << i << ": " << result << std::endl;

    }
}

int main()
{
	// std::ifstream inputFile("input.in");
	// std::ofstream outputFile("output_2020.txt");

	// ESAbATAd(inputFile, outputFile);
	ESAbATAd(std::cin, std::cout);

	return 0;
}