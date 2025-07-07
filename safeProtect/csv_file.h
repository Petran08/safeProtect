#pragma once
#include <string>
#include<iostream>
#include <fstream>

class csv_file
{
public:
	std::string headers[100];
	std::string contents[150][100];
	std::string file_name;
	int rows_used;
	void read_file()
	{
		std::ifstream in(file_name);
		std::string head;
		in >> head;
		long long col = 0;
		for (long long i = 0; i < head.length(); i++)
		{
			if (head[i] == ',')
			{
				std::cout << headers[col] << ' ';
				col++;
			}
			else
				headers[col].push_back(head[i]);
		}
		std::cout << headers[col] << ' ';
		std::cout << '\n';
		int row = 0;
		std::string val;
		in >> val;
		while (val[0] != '[' && val[1] != '(' && val[2] != ')' && val[3] != ']' && row < 100)
		{
			long long col = 0;
			for (long long i = 0; i < val.length(); i++)
			{
				if (val[i] == ',')
				{
					std::cout << contents[row][col] << ' ';
					col++;
				}
				else
					contents[row][col].push_back(val[i]);
			}
			std::cout << contents[row][col] << ' ';
			std::cout << '\n';
			in >> val;
			row++;
		}
		rows_used = row;
	}
};

