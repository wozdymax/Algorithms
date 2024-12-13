#include <iostream>
#include <fstream>
#include <vector>
#include <optional>
#include <utility>
#include <queue>
#include <string>
#include <sstream>
#include <climits>

void Prim(const std::vector<std::vector<int>>& graph) {
    int n = graph.size();
    std::vector<int> minDist(n, INT_MAX);
    std::vector<bool> inMST(n, false);
    minDist[0] = 0;

    int totalWeight = 0;

    std::cout << "Круг\t";
    for (int i = 1; i <= n; i++) {
        std::cout <<  i << "\t";
    }
    std::cout << "Ребро\tСумма" << std::endl;

    for (int count = 0; count < n; count++) {
        int u = -1;
        int uPrev = -1; 
        for (int i = 0; i < n; i++) {
            if (!inMST[i] && (u == -1 || minDist[i] < minDist[u])) {
                u = i;
            }
        }

        inMST[u] = true;

        int addedEdgeWeight = minDist[u];
        totalWeight += addedEdgeWeight;

        std::cout << count + 1 << "\t"; 

        for (int i = 0; i < n; i++) {
            if (inMST[i]) {
                std::cout << "*";  
            }
            if (minDist[i] == INT_MAX)
            {
                std::cout << "&" << "\t";;
            }
            else
            {
                std::cout << minDist[i] << "\t";
            }
        }

        if (u != 0) {
            for (int v = 0; v < n; v++) {
                if (graph[u][v] != INT_MAX && inMST[v]) {
                    uPrev = v;
                    break;
                }
            }
            std::cout  << uPrev + 1 << "-" << u + 1 << "\t";
        }
        else {
            std::cout << "\t"; 
        }

        std::cout << totalWeight << std::endl; 

        for (int v = 0; v < n; v++) {
            if (graph[u][v] != INT_MAX && !inMST[v]) {
                minDist[v] = std::min(minDist[v], graph[u][v]);
            }
        }
    }
}

std::vector<std::vector<int>> ReadGraph(std::ifstream& input)
{
    std::vector<std::vector<int>> graph;
    std::string line;

    while (std::getline(input, line)) {
        std::istringstream iss(line);
        int u, v, w;
        if (!(iss >> u >> v >> w)) {
            std::cout << "Ошибка чтения из файла: " << line << std::endl;
            continue; 
        }

        int maxIndex = std::max(u-1, v-1);
        if (maxIndex >= graph.size()) { 
            int newSize = maxIndex + 1;
            graph.resize(newSize);
            for (int i = 0; i < newSize; i++) {
                if (graph[i].size() < newSize) {
                    graph[i].resize(newSize, INT_MAX);
                }
            }
        }

        graph[u-1][v-1] = w;
        graph[v-1][u-1] = w; 
    }

    return graph;
}

void MakeSpanningTree(std::ifstream& input)
{
    setlocale(LC_ALL, "RU");
    std::vector<std::vector<int>> graph = ReadGraph(input);
    Prim(graph);
}

 void CreateSpanningTree(std::string& inputFile)
{
    std::ifstream inputStream;
    inputStream.open(inputFile, std::ios::binary);
    if (!inputStream.is_open())
    {
        throw std::runtime_error("Failed to open " + inputFile);
    }

    MakeSpanningTree(inputStream);
}

 std::string ParseArgs(int argc, char* argv[])
 {

     if (argc != 2)
     {
         std::cout << "Wrong command! Please use: TennisTournament.exe <input file> <output file>";
         return "fail";
     }
     return argv[1];
}

int main(int argc, char* argv[])
{
    std::string inputFile = ParseArgs(argc, argv);
    if (inputFile == "fail")
    {
        return EXIT_FAILURE;
    }

    try
    {
        CreateSpanningTree(inputFile);
    }
    catch (const std::exception e) 
    {
        std::cout << e.what() << std::endl; 
    }

    return EXIT_SUCCESS;
}

