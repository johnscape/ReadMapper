//
// Created by attila on 2021. 09. 12..
//

#include "DataIO.h"
#include <cstring>
#include <iostream>
#include <fstream>

DataIO::DataIO(bool verbose) : Loaded(false), FastaFile(false), Verbose(verbose)
{
}

bool DataIO::Open(const char *fileName)
{
	if (Loaded)
		return true;
	if (IsFasta(fileName))
		return ReadFASTA(fileName);
	else
		return ReadFASTQ(fileName);
}

bool DataIO::Open(const std::string& fileName)
{
	return Open(fileName.c_str());
}

bool DataIO::IsFasta(const char *fileName)
{
	//find the final dot location
	unsigned int dotLoc = 0;
	for (int i = 0; i < strlen(fileName); ++i)
	{
		if (fileName[i] == '.')
			dotLoc = i;
	}

	if (dotLoc + 2 >= strlen(fileName))
		throw InvalidFileException();

	if (fileName[dotLoc + 1] == 'f')
	{
		if (fileName[dotLoc + 2] == 'a')
			return true;
		else if (fileName[dotLoc + 2] == 'q')
			return false;
		throw InvalidFileException();
	}

	return false;
}

bool DataIO::ReadFASTA(const char *fastaFile)
{
	std::ifstream reader(fastaFile);
	if (!reader.good())
	{
		if (Verbose)
			std::cout << "Cannot open file " + std::string(fastaFile) << std::endl;
		return false;
	}

	FASTASequences.clear();

	while (reader.good())
	{
		std::string name, data;
		std::getline(reader, name);
		std::getline(reader, data);
		if (name.empty() || data.empty())
			break;
		name = name.substr(1);
		FASTASequences.insert(std::pair<std::string, std::string>(name, data));
	}

	if (Verbose)
		std::cout << fastaFile << " has been read successfully!" << std::endl;
	Loaded = true;
	FastaFile = true;
	return true;
}

bool DataIO::ReadFASTQ(const char *fastqFile)
{
	std::ifstream reader(fastqFile);
	if (!reader.good())
	{
		if (Verbose)
			std::cout << "Could not open " << fastqFile << "!" << std::endl;
		return false;
	}


	while (reader.good())
	{
		std::string name, data;
		std::getline(reader, name);
		std::getline(reader, data);
		if (name.empty() || data.empty())
			break;
		name = name.substr(1);
		FASTQData.insert(std::pair<std::string, std::string>(name, data));
		for (int i = 0; i < 2; ++i)
			reader.ignore(2048, '\n');
	}

	if (Verbose)
		std::cout << fastqFile << " has been read successfully!" << std::endl;
	Loaded = true;
	FastaFile = false;
	return true;
}

void DataIO::Reset()
{
	Loaded = false;
	FastaFile = false;
	FASTASequences.clear();
	FASTQData.clear();
}

std::string DataIO::GetData(std::string id) const
{
	if (!Loaded)
		throw NotLoadedException();
	if (FastaFile)
	{
		if (FASTASequences.find(id) == FASTASequences.end())
			return "";
		return FASTASequences.find(id)->second;
	}

	if (FASTQData.find(id) == FASTQData.end())
		return "";
	return FASTQData.find(id)->second;
}

unsigned int DataIO::GetSequenceCount() const
{
	if (!Loaded)
		throw NotLoadedException();
	if (FastaFile)
		return FASTASequences.size();
	return FASTQData.size();
}

std::map<std::string, std::string>::const_iterator DataIO::GetSequenceStart() const
{
	if (!Loaded)
		throw NotLoadedException();
	std::map<std::string, std::string>::const_iterator it;
	if (FastaFile)
		it = FASTASequences.begin();
	else
		it = FASTQData.begin();

	return it;
}

std::map<std::string, std::string>::const_iterator DataIO::GetSeqneceEnd() const
{
	if (!Loaded)
		throw NotLoadedException();
	std::map<std::string, std::string>::const_iterator it;
	if (FastaFile)
		it = FASTASequences.end();
	else
		it = FASTQData.end();

	return it;
}
