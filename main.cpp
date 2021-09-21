#include <iostream>
#include <cstring>
#include "DataIO.h"
#include "StringMatcher.h"
#include <chrono>

#define MEASURETIME true

int main(int argc, char** argv)
{
	//Parsing parameters
	if (argc == 2 && (strcmp(argv[1], "help") != 0 || strcmp(argv[1], "--help") != 0))
	{
		std::cout << "Usage: readmapper fastaFile.fa fastqFile.fq" << std::endl;
		return 0;
	}

	if (argc < 3)
	{
		std::cout << "Missing arguments!" << std::endl << "Use the following: readmapper fastaFile.fa fastqFile.fq"
				  << std::endl;
		return 1;
	}

	//Program settings - will be moved to parameters later
	unsigned int UsedMethod = 1; //0 - naive search, 1 - border search, 2 - KMP
	bool verbose = false; //should the program provide additional info
	bool measureRuntime = true; //should the program measure the average runtime of the searches

	float runtime = 0;
	unsigned int searches = 0;


	DataIO fastaReader(verbose);
	DataIO fastqReader(verbose);

	if (!fastaReader.Open(argv[1]))
		return 1;
	if (!fastqReader.Open(argv[2]))
		return 1;

	std::map<std::string, std::string>::const_iterator fastaStart, fastaEnd, fastqStart, fastqEnd;

	fastaStart = fastaReader.GetSequenceStart();
	fastaEnd = fastaReader.GetSeqneceEnd();

	fastqStart = fastqReader.GetSequenceStart();
	fastqEnd = fastqReader.GetSeqneceEnd();

	std::map<std::string, std::string>::const_iterator inner, outer;


	for (outer = fastqStart ;outer != fastqEnd; ++outer)
	{
		for(inner = fastaStart ;inner != fastaEnd; ++inner)
		{
			std::string text = inner->second;
			std::string pattern = outer->second;

			std::vector<unsigned int> matches;

#if MEASURETIME
			std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
#endif

			if (UsedMethod == 0)
				matches = StringMatcher::NaiveSearch(text, pattern);
			else if (UsedMethod == 1)
				matches = StringMatcher::BorderSearch(text, pattern);
			else if (UsedMethod == 2)
				matches = StringMatcher::KMPSearch(text, pattern);

#if MEASURETIME
			std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
			runtime += std::chrono::duration<float, std::milli>(t2 - t1).count();
#endif
			for (int i = 0; i < matches.size(); ++i)
			{
				std::cout << outer->first << '\t' << '0' << '\t' << inner->first << '\t' << (matches[i] + 1) << '\t' << '0'
				<< '\t' << pattern.size() << 'M' << '\t' << '*' << '\t' << '0' << '\t' << '0' << '\t' << outer->second
				<< '\t';
				for (int j = 0; j < pattern.size(); ++j)
				{
					std::cout << '~';
				}
				std::cout << std::endl;
			}
		}
	}

#if MEASURETIME
	runtime /= fastaReader.GetSequenceCount() * fastqReader.GetSequenceCount();
	std::cout << "Average search time is " << runtime << std::endl;
#endif

	return 0;
}
