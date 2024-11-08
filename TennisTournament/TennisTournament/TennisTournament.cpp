//19. Теннисный турнир проходит по олимпийской системе с
//выбываниями. В турнире участвуют 2^n игроков. Известен рейтинг
//каждого игрока. Чем больше рейтинг, тем выше мастерство игрока.
//В первом туре сильнейший игрок встречается со слабейшим, второй
//по рейтингу с предпоследним и т.д. Организаторы собираются и в
//дальнейшем придерживаться этого принципа, предполагая, что в
//очередной тур будет выходить сильнейшая половина игроков
//предыдущего тура. Известно, что турнир прошел в полном
//соответствии с рейтингом игроков. Требуется показать в наглядном
//виде дерево проведенного турнира(11).
//Яндыганов Игорь
//ПС-24
//Visual Studio 2022 C++

#include <string>
#include <fstream>
#include <optional>
#include <iostream>
#include <vector>
#include <cmath>

struct Args
{
	std::string inputFile;
	std::string outputFile;
};

struct Node {
	std::string name;
	Node* left = 0;
	Node* right = 0;
};

struct Player
{
	std::string name;
	std::string rating;
};

Node* buildTree(const std::vector<std::string>& elements, int num, int k) {
	k++;
	if (elements.size() < pow(2, k)) {
		Node* root = new Node;
		root->name = elements[num];
		return root;
	}

	Node* root = new Node; 
	root->name = elements[num];

	int pos = pow(2, k) - 1;

	root->left = buildTree(elements, num, k);
	k--;
	root->right = buildTree(elements, pos - num, k);
	k--;

	return root;
}

void deleteTree(Node* root) {
	if (root) {
		deleteTree(root->left);
		deleteTree(root->right);
		delete root;
	}
}

void displayTree(std::ostream& outFile, Node* root, int depth = 0) {
	if (!root) return;

	displayTree(outFile, root->right, depth + 1); 
	outFile << std::string(depth * 4, ' ') << root->name << "\n"; 
	displayTree(outFile, root->left, depth + 1); 
}

void MakeFileTournamentTree(std::istream& input, std::ostream& output)
{
	std::vector<std::string> players = {};
	std::vector<Player> startPlayers = {};
	std::string player = "";
	std::string name = "";
	int i = 0;
	std::string rating = "";

	while (std::getline(input, player))
	{
		while (player[i] != ' ')
		{
			name = name + player[i];
			i++;
		}
		i++;
		while (i < player.length())
		{
			rating = rating + player[i];
			i++;
		}
		startPlayers.push_back({ name, rating });
		players.push_back(name);
		name = "";
		rating = "";
		i = 0;
	}

	Node* root = buildTree(players, 0, 0);

	displayTree(output, root);

	deleteTree(root);
}

void CreateTournamentTree(Args& args)
{
	std::ifstream inputStream;
	inputStream.open(args.inputFile, std::ios::binary);
	if (!inputStream.is_open())
	{
		throw std::runtime_error("Failed to open " + args.inputFile);
	}

	std::ofstream outputStream;
	outputStream.open(args.outputFile, std::ios::binary);
	if (!outputStream.is_open())
	{
		throw std::runtime_error("Failed to open " + args.outputFile);
	}

	MakeFileTournamentTree(inputStream, outputStream);

	if (!outputStream.flush())
	{
		throw std::runtime_error("Failed to save data");
	}
}

std::optional<Args> ParseArgs(int argc, char* argv[])
{

	if (argc != 3)
	{
		std::cout << "Wrong command! Please use: TennisTournament.exe <input file> <output file>";
		return std::nullopt;
	}

	Args result;

	result.inputFile = argv[1];
	result.outputFile = argv[2];

	return result;
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
		CreateTournamentTree(*args);
	}
	catch (const std::exception e)
	{
		std::cout << e.what() << std::endl;
	}

	return EXIT_SUCCESS;
}

