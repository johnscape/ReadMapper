#include "SuffixTree.h"
#include <iostream>

SuffixTree::SuffixTree() : Root(nullptr)
{

}

SuffixTree::~SuffixTree()
{
	delete Root;
}

void SuffixTree::PrintTree()
{
	if (!Root)
		return;
	PrintNode(Root);
}

void SuffixTree::BuildNaiveTree(const char *text, unsigned int textLen)
{
	//Delete root if it already exists
	delete Root;
	Root = new TreeNode();

	for (int i = 0; i < textLen; ++i)
	{
		InsertNodeNaive(text, Range(i, textLen), Root);
	}

}

void SuffixTree::BuildNaiveTree(const std::string& text)
{
	BuildNaiveTree(text.c_str(), text.size());
}

void SuffixTree::InsertNodeNaive(const char* text, Range textPos, TreeNode* startNode)
{
	TreeNode* childToFollow = startNode->IsStringInChildren(textPos.Start, text);
	if (!childToFollow)
	{
		InsertChild(startNode, textPos);
		return;
	}
	else
	{
		unsigned int currentStep = childToFollow->FirstChar;
		while (currentStep < childToFollow->LastChar)
		{
			if (text[currentStep] != text[textPos.Start])
			{
				SplitEdge(startNode, childToFollow, currentStep, textPos);
				return;
			}

			currentStep++;
			textPos.Start++;
		}
		InsertNodeNaive(text, textPos, childToFollow);
	}
}

void SuffixTree::SplitEdge(SuffixTree::TreeNode *parent, SuffixTree::TreeNode *child, unsigned int splitPos, Range insertText)
{
	//Create new node at splitting position
	TreeNode* inserted = new TreeNode(child->FirstChar, splitPos);

	//Add the previous child as the children of the new node
	inserted->Children.push_back(child);

	//Modify starting position of the original child
	child->FirstChar = splitPos;

	//Replace child with the inserted node
	for (int i = 0; i < parent->Children.size(); ++i)
	{
		if (parent->Children[i] == child)
		{
			parent->Children[i] = inserted;
			break;
		}
	}

	//Add new node to the inserted with the remaining text
	InsertChild(inserted, insertText);
}

void SuffixTree::InsertChild(SuffixTree::TreeNode *parent, SuffixTree::Range textRange)
{
	TreeNode* node = new TreeNode(textRange);
	parent->Children.push_back(node);
}

void SuffixTree::PrintNode(SuffixTree::TreeNode *node, unsigned int depth)
{
	for (int i = 0; i < depth; ++i)
		std::cout << '\t';
	std::cout << "Node range: \t[" << node->FirstChar << ", " << node->LastChar << "[";
	std::cout << "\tNode children count: " << node->Children.size() << std::endl;
	for (TreeNode* i : node->Children)
	{
		PrintNode(i, depth + 1);
	}
}

