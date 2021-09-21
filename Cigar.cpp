//
// Created by attila on 2021. 09. 03..
//

// NOLINT(cert-err34-c)

#include "Cigar.h"
#include <cstdlib>
#include <utility>

std::string Cigar::GenerateCIGAR(std::string original, std::string found)
{
	return {};
}

std::string Cigar::GenerateExtendedCigar(std::string original, std::string found)
{
	return {};
}

std::string Cigar::GenerateDNAFromCIGAR(std::string original, std::string cigar)
{
	std::string result;
	std::string longCigar = CIGARLenghtener(std::move(cigar));
	unsigned int pointer = 0;
	for (char i : longCigar)
	{
		if (i == 'M')
			result += original[pointer];
		else if (i == 'I')
		{
			result += 'A';
			result += original[pointer];
		}
		pointer++;
	}


	return result;
}

std::string Cigar::GenerateDNAFromExtendedCIGAR(std::string original, const std::string& cigar)
{
	std::string result;
	unsigned int pointer = 0;
	for (char i : cigar)
	{
		if (i == '=')
			result += original[pointer];
		else if (i == 'I')
		{
			result += 'A';
			result += original[pointer];
		}
		else if (i == 'X')
			result += 'D'; //TODO: implement sample generator
		pointer++;
	}


	return result;
}

std::string Cigar::CIGARLenghtener(std::string cigar)
{
	std::string result;

	while (!cigar.empty())
	{
		//get number from beginning
		unsigned int count = 0;
		while (std::isdigit(cigar[count]))
			count++;
		unsigned int cig = std::atoi(cigar.substr(0, count).c_str());
		char cigType = cigar[count];
		cigar = cigar.substr(count + 1);
		for (int i = 0; i < cig; ++i)
			result += cigType;
	}

	return result;
}
