//
// Created by attila on 2021. 10. 12..
//

#include "SuffixTree.h"

SuffixTree::SuffixTree() : Root(nullptr)
{

}

SuffixTree::~SuffixTree()
{
	delete Root;
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
		TreeNode* newNode = new TreeNode(textPos.Start, textPos.End);
		startNode->Children.push_back(newNode);
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

	//Remove original child from the original parent
	std::vector<TreeNode*>::iterator it = parent->Children.begin();
	while (it != parent->Children.end())
	{
		if (*it == child)
		{
			parent->Children.erase(it);
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

