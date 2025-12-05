#include <iostream>
#include <limits>
#include <vector>
#include <iomanip>
// added due to make command not running properly
#include <sstream>

#include "csp.hpp"

using namespace cs251;

// Template declaration for running tests with a specific type
template <typename W>
void run_test();

// Converts string input into appropriate type
template <typename W>
W parse_item(std::string itemstr);

int main() {
    try {
        std::string typestr;
        std::getline(std::cin, typestr);

        if (typestr == "double") {
            run_test<double>();
        } else {
            std::cerr << "Unsupported type: " << typestr << std::endl;
            return 1;
        }
    } catch (const std::exception& e) {
        std::cerr << "Unhandled exception: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}

template <typename W>
void run_test() {
    std::string line;

    std::cout << std::fixed << std::setprecision(2);

    std::getline(std::cin, line);
    std::stringstream graphss(line);
    int n, m;
    graphss >> n >> m;

    if (n <= 0 || m < 0) {
        throw std::runtime_error("Invalid graph dimensions.");
    }

    Adj adj(n);
    for (int i = 0; i < m; i++) {
        std::getline(std::cin, line);
        std::stringstream edgess(line);
        int u, v;
        W w, wb;
        edgess >> u >> v >> w >> wb;
        add_edge_undirected(adj, u, v, w, wb);
    }

    std::getline(std::cin, line);
    std::stringstream cpss(line);
    std::vector<int> checkpoints;
    int cp;
    while (cpss >> cp) checkpoints.push_back(cp);

    std::getline(std::cin, line);
    std::stringstream kss(line);
    int K, start;
    kss >> K >> start;

    PathResult result = constrained_shortest_path(adj, checkpoints, K, start);

    std::cout << result.reachable << "\n";

    if (result.bestTime < std::numeric_limits<double>::infinity()) {
        std::cout << result.bestTime << "\n";
        for (size_t i = 0; i < result.pathNodes.size(); i++) {
            std::cout << result.pathNodes[i] << (i + 1 < result.pathNodes.size() ? " -> " : "\n");
        }
        for (size_t i = 0; i < result.usedBoostEdge.size(); i++) {
            std::cout << result.usedBoostEdge[i] << (i + 1 < result.usedBoostEdge.size() ? " -> " : "\n");
        }
    }
}

template <>
double parse_item<double>(std::string itemstr) {
    return !itemstr.empty() ? std::stod(itemstr) : 0.0;
}
