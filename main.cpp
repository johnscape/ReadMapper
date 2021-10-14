#include <iostream>
#include <cstring>
#include "SuffixTree.h"

int main(int argc, char** argv)
{
	SuffixTree suffixTree;
	suffixTree.BuildNaiveTree("mississippi$");
	suffixTree.PrintTree();

	return 0;
}
