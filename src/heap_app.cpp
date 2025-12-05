#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "heap.hpp"

using namespace cs251;

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <input_file>\n";
        return 1;
    }

    std::ifstream fin(argv[1]);
    if (!fin) {
        std::cerr << "Error: Cannot open input file " << argv[1] << "\n";
        return 1;
    }

    int capacity;
    fin >> capacity; // read number of elements
    std::vector<double> keys(capacity, 1e9);

    MinHeap heap(keys, capacity);

    std::string line;
    std::getline(fin, line); // consume remainder of first line

    while (std::getline(fin, line)) {
        if (line.empty()) continue;

        std::istringstream iss(line);
        std::string cmd;
        iss >> cmd;

        if (cmd == "push") {
            int id;
            double val;
            iss >> id >> val;
            keys[id] = val;
            if (!heap.contains(id)) {
                heap.push(id);
            } else {
                heap.updateKey(id);
            }
        }
        else if (cmd == "update") {
            int id;
            double val;
            iss >> id >> val;
            keys[id] = val;
            heap.updateKey(id);
        }
        else if (cmd == "pop") {
            if (!heap.empty()) {
                int top = heap.pop();
            } else {
                std::cout << "Heap empty\n";
            }
        }
        else if (cmd == "print") {
            std::cout << "Heap contents: ";
            for (int id : heap.heap) {
                std::cout << "(" << id << "," << keys[id] << ") ";
            }
            std::cout << "\n";
        }
    }

    return 0;
}
