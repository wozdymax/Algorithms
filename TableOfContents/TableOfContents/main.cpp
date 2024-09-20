//13. Имеется   текст   документа, состоящий  из  нескольких
//параграфов. Каждый параграф начинается с заголовка и  отделен
//пустой  строкой  от  предыдущего параграфа. Текст разделен на
//страницы. Номер страницы проставлен в ее начале и  выделен  с
//обеих  сторон  знаком  '-'. Сформировать  файл  с оглавлением
//документа(7).
//Яндыганов Игорь
//ПС-24
//Visual Studio 2022 C++20

#include <string>
#include <fstream>
#include <optional>
#include <iostream>
#include <stack>
#include <vector>

struct Args
{
	std::string inputFileName;
	std::string outputFileName;
};

std::optional<Args> ParseArgs(int argc, char* argv[])
{

	if (argc != 3)
	{
		std::cout << "Wrong command! Please use: TableOfContent.exe <input file> <output file>"; 
		return std::nullopt;
	}

	Args result;

	result.inputFileName = argv[1];
	result.outputFileName = argv[2];

	return result;
}

void MakeFileHeader(std::istream& input, std::ostream& output)
{
	const int lengthStrInOutFile {40}; 
	std::string currentStr;
	std::string pageNumber;
	std::string possiblePageNumber;
	std::string currentHeader;
	std::string outStr;
	int i;
	int lengthHeader;

	while (std::getline(input, currentStr))
	{
		i = 0;
		while (currentStr[i] == ' ')
		{
			i++;
		}
		if (currentStr[i] == '\r' && currentHeader != "")
		{
			currentHeader = "";
			continue;
		}
		if (currentStr[i] == '-')
		{
			possiblePageNumber = "";
			i++; 
			while (currentStr[i] >= '0' && currentStr[i] <= '9') 
			{
				possiblePageNumber = possiblePageNumber + currentStr[i]; 
				i++; 
			}
			if (currentStr[i] == '-') 
			{
				pageNumber = possiblePageNumber; 
			}
		}
		if (currentHeader == "") 
		{
			lengthHeader = 0;
			if (currentStr[i] != '-') 
			{
				i = 0;
			}
			else
			{
				i++;
			}
			while (currentStr[i] != '\r')
			{
				currentHeader = currentHeader + currentStr[i];
				lengthHeader++;
				i++;
			}
			outStr = currentHeader;
			while (lengthHeader < lengthStrInOutFile)
			{
				outStr = outStr + '_';
				lengthHeader++;
			}
			output << outStr << pageNumber << std::endl;
		}
	}
}

void MakeHeader(Args& args)
{
	std::ifstream inputStream;
	inputStream.open(args.inputFileName, std::ios::binary); 
	if (!inputStream.is_open())
	{
		throw std::runtime_error("Failed to open " + args.inputFileName);
	}

	std::ofstream outputStream;
	outputStream.open(args.outputFileName, std::ios::binary);
	if (!inputStream.is_open())
	{
		throw std::runtime_error("Failed to open " + args.outputFileName);
	}

	MakeFileHeader(inputStream, outputStream);

	if (!outputStream.flush())
	{
		throw std::runtime_error("Failed to save data");
	}
}

int main(int argc, char* argv[])
{
	std::optional<Args> args = ParseArgs(argc, argv);
	if (!args)
	{
		return EXIT_FAILURE;
	}

	try
	{
		MakeHeader(*args);
	}
	catch (const std::exception e)
	{
		std::cout << e.what() << std::endl;
	}

	return EXIT_SUCCESS;
}


