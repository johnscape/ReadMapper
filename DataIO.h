//
// Created by attila on 2021. 09. 12..
//

#ifndef GENOME_DATAIO_H
#define GENOME_DATAIO_H

#include <string>
#include <vector>
#include <map>
#include <exception>

class NotLoadedException : public std::exception
{
	const char* what() const throw() {
		return "The class is not loaded yet!";
	}
};

class InvalidFileException : public std::exception
{
	const char* what() const throw() {
		return "File type cannot be determined.";
	}
};

/**
 * @brief A class for reading and parsing FASTA and FASTQ files
 */
class DataIO
{
public:
	explicit DataIO(bool verbose = false);

	bool Open(const char* fileName);
	bool Open(const std::string& fileName);

	bool Save(const char* fileName);
	bool Save(const std::string& fileName);


	std::string GetData(std::string id) const;
	unsigned int GetSequenceCount() const;
	std::map<std::string, std::string>::const_iterator GetSequenceStart() const;
	std::map<std::string, std::string>::const_iterator  GetSeqneceEnd() const;

private:
	static bool IsFasta(const char* fileName);
	bool ReadFASTA(const char* fastaFile);
	bool ReadFASTQ(const char* fastqFile);

	void Reset();

	bool Loaded;
	bool FastaFile;
	bool Verbose;

	std::map<std::string, std::string> FASTASequences;
	std::map<std::string, std::string> FASTQData;
};


#endif //GENOME_DATAIO_H
