//
// Created by attila on 2021. 09. 03..
//

#ifndef GENOME_CIGAR_H
#define GENOME_CIGAR_H

#include <string>

class Cigar
{
public:
	static std::string GenerateCIGAR(std::string original, std::string found);
	static std::string GenerateExtendedCigar(std::string original, std::string found);

	static std::string GenerateDNAFromCIGAR(std::string original, std::string cigar);
	static std::string GenerateDNAFromExtendedCIGAR(std::string original, const std::string& cigar);

	static std::string CIGARLenghtener(std::string cigar);
	static std::string CIGARShortener(std::string cigar);
};


#endif //GENOME_CIGAR_H
