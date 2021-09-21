#include "DataIO.h"
#include <cstring>
#include <iostream>
#include <fstream>
#include <random>

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

bool DataIO::Save(const char *fileName)
{
	if (!Loaded)
		return false;
	std::ofstream writer(fileName);
	if (FastaFile)
	{
		std::map<std::string, std::string>::iterator it = FASTASequences.begin();
		while (it != FASTASequences.end())
		{
			writer << ">" << it->first << std::endl << it->second << std::endl;
			it++;
		}
	}
	else
	{
		std::map<std::string, std::string>::iterator it = FASTQData.begin();
		while (it != FASTQData.end())
		{
			writer << "@" << it->first << std::endl << it->second << std::endl << "+" << std::endl;
			for (unsigned int i = 0; i < it->second.size(); ++i)
			{
				writer << "~";
			}
			writer << std::endl;
			it++;
		}
	}
	writer.close();
	if (Verbose)
		std::cout << "Data saved as " << fileName << std::endl;
	return true;
}

bool DataIO::Save(const std::string &fileName)
{
	return Save(fileName.c_str());
}

void DataIO::GenerateData(bool fasta, unsigned int count, int minLength, int maxLength)
{
	//We don't want to override actual data with testing
	if (Loaded)
		return;

	//Due to the parameters of the built-in distribution, negative numbers are allowed, but since negative length does
	//not have any sense, we return it
	if (minLength <= 0 || maxLength <= 0)
		return;
	if (minLength > maxLength) //the min length cannot be larger than the max
		return;

	std::random_device rd; // obtain a random number from hardware
	std::mt19937 gen(rd()); // seed the generator
	std::uniform_int_distribution<> distr(minLength, maxLength); // define the range
	std::uniform_int_distribution<> charDist(0, 3); //for the acid selection

	FastaFile = fasta;

	for (unsigned int c = 0; c < count; ++c)
	{
		std::string name, data;
		name = std::to_string(c);

		int chosenLen = distr(gen);
		for (int d = 0; d < chosenLen; ++d)
		{
			int chosen = charDist(gen);

			switch (chosen)
			{
				case 0:
					data += "C";
					break;
				case 1:
					data += "A";
					break;
				case 2:
					data += "G";
					break;
				case 3:
					data += "T";
					break;
				default:
					data += "A";
					break;
			}
		}

		if (FastaFile)
			FASTASequences.insert(std::pair<std::string, std::string>(name, data));
		else
			FASTQData.insert(std::pair<std::string, std::string>(name, data));
	}

	Loaded = true;
}
