#include "StringMatcher.h"

void StringMatcher::NaiveSearch(std::string sample, std::string pattern, std::vector<unsigned int>& results)
{
	for (int i = 0; i < sample.size() - pattern.size(); ++i)
	{
		bool match = true;
		for (int j = 0; j < pattern.size(); ++j)
		{
			if (sample[i + j] != pattern[j])
			{
				match = false;
				break;
			}
		}
		if (match)
			results.push_back(i);
	}
}

void StringMatcher::BorderSearch(std::string sample, std::string pattern, std::vector<unsigned int>& results)
{
	sample = pattern + '$' + sample;
	std::vector<unsigned int> borderArray(sample.size(), 0);

	unsigned int n = pattern.size();

	//First we have to build the border array
	for (int i = 1; i < sample.size(); ++i)
	{
		//We need to check the previous borders, to see if we can expand any of them
		//The last border is always i-1
		unsigned int lastBorder = borderArray[i - 1];
		//We can get the previous border by b^k[i - 1]
		//Since b^k is not efficient,  we can put it into a loop
		//The loop must break if:
		//1) The border pointer becomes 0 -> we might need to expand an empty border, but there is no previous
		//2) the current character is the same as the next character after the border -> we can expand it
		while (lastBorder > 0 && sample[i] != sample[lastBorder])
		{
			//if neither of the conditions are met, then we take a look at the previous border
			lastBorder = borderArray[lastBorder - 1];
		}
		//Now we take a look at the solution, there are a few possible outcomes:
		//- We found a non-empty border which we can expand (the current character is the same as next after the border)
		//-- In this case we simply increment the last border's length
		//- We can expand an empty border (e.g.: the first and second characters are the same)
		//-- We set the border's size to 1 (or increment again, since 0+1 is 1)
		//- We did not find a border, the border size is 0
		// We can see that in both scenarios, we increase the border size if the current character and the next
		// character after the border are the same. We can use this to our advantage:

		unsigned int result = (sample[i] == sample[lastBorder]) * (lastBorder + 1);

		//This is a bit hacky, but I like it. If the characters are the same, the equation will return 1, and the
		//next border size will be 1 * (lastBorder + 1). If they are not the same, the multiplication will 0 out
		//everything.

		//We store the result in the border array
		borderArray[i] = result;

		//Now, we make a check: If the result is as long as the pattern, then we have a mach.
		//This happens because our string has the pattern at the beginning and a $ character after it, which
		//shouldn't appear anywhere else in the text. Because of if, if the border is as long as the pattern,
		//then the border IS the pattern, therefore we found a match:
		if (result == n)
			results.push_back(i - n + 1);
	}
}

void StringMatcher::KMPSearch(std::string sample, std::string pattern, std::vector<unsigned int>& results)
{
	//TODO: Implement KMP
}
