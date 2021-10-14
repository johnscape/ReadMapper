#ifndef GENOME_STRINGMATCHER_H
#define GENOME_STRINGMATCHER_H

#include <vector>
#include <string>

namespace StringMatcher
{
	void NaiveSearch(std::string sample, std::string pattern, std::vector<unsigned int>& results);
	void BorderSearch(std::string sample, std::string pattern, std::vector<unsigned int>& results);
	void KMPSearch(std::string sample, std::string pattern, std::vector<unsigned int>& results);
}


#endif //GENOME_STRINGMATCHER_H
