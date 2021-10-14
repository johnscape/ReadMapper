#ifndef READMAPPER_SUFFIXTREE_H
#define READMAPPER_SUFFIXTREE_H

#include <string>
#include <vector>

class SuffixTree
{
public:
	SuffixTree();
	~SuffixTree();

	void BuildNaiveTree(const char* text, unsigned int textLen);
	void BuildNaiveTree(const std::string& text);

private:
	struct Range
	{
		Range() : Start(0), End(0) {}
		Range(unsigned int start, unsigned int end) : Start(start), End(end) {}
		unsigned int Start;
		unsigned int End;
	};

	struct TreeNode
	{
		TreeNode() : FirstChar(0), LastChar(0) {}
		TreeNode(unsigned int first, unsigned int last) : FirstChar(first), LastChar(last) {}
		TreeNode(Range textPos) : FirstChar(textPos.Start), LastChar(textPos.End) {}
		std::vector<TreeNode*> Children;
		unsigned int FirstChar;
		unsigned int LastChar;

		~TreeNode() {
			for (TreeNode* i : Children)
				delete i;
			Children.clear();
		}

		TreeNode* IsStringInChildren(unsigned int pos, const char* text)
		{
			for (auto & i : Children)
			{
				if (text[i->FirstChar] == text[pos])
					return i;
			}
			return nullptr;
		}
	};

	TreeNode* Root;

	//Naive methods
	void InsertNodeNaive(const char* text, Range TextPos, TreeNode* startNode);

	//Common methods
	void SplitEdge(TreeNode* parent, TreeNode* child, unsigned int splitPos, Range insertText);
	void InsertChild(TreeNode* parent, Range textRange);


};


#endif //READMAPPER_SUFFIXTREE_H
