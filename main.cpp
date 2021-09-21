#include <iostream>
#include "DataIO.h"
#include "StringMatcher.h"

#define METHOD 0 //Use 0 for naive, 1 for border search, and 2 for KHM

int main(int argc, char** argv)
{
	if (argc < 3)
	{
		std::cout << "Missing arguments!" << std::endl << "Use the following: readmapper.exe fastaFile.fa fastqFile.fq"
		<< std::endl;
		return 1;
	}

	DataIO fastaReader, fastqReader;

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
			if (METHOD == 0)
				matches = StringMatcher::NaiveSearch(text, pattern);
			else if (METHOD == 1)
				matches = StringMatcher::BorderSearch(text, pattern);
			else
				matches = StringMatcher::KMPSearch(text, pattern);
			for (int i = 0; i < matches.size(); ++i)
			{
				std::cout << outer->first << '\t' << '0' << '\t' << inner->first << '\t' << matches[i] << '\t' << '0'
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


	return 0;
}
