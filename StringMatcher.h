#ifndef GENOME_STRINGMATCHER_H
#define GENOME_STRINGMATCHER_H

#include <vector>
#include <string>

class StringMatcher
{
public:
	static std::vector<unsigned int> NaiveSearch(std::string sample, std::string pattern);
	static std::vector<unsigned int> BorderSearch(std::string sample, std::string pattern);
	static std::vector<unsigned int> KMPSearch(std::string sample, std::string pattern);
};


#endif //GENOME_STRINGMATCHER_H
