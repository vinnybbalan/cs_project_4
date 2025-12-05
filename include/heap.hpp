#pragma once

#include <vector>
//#include <functional>
#include <algorithm>

namespace cs251 {

    struct MinHeap {
        // Heap stores state ids; priorities are given by dist reference.
        const std::vector<double> &key;     // reference to distance array
        std::vector<int> heap;              // heap of state ids
        std::vector<int> pos;               // pos[stateId] = index in heap, -1 if not in heap

        explicit MinHeap(const std::vector<double> &key, int capacity)
            : key(key), pos(capacity, -1) {
            heap.reserve(capacity);
        }

        bool empty() const { return heap.empty(); }
        bool contains(int id) const { return pos[id] != -1; }

        void push(int id) {
            //throw std::logic_error("MinHeap::" + std::string(__FUNCTION__) + " not implemented");
            int idx = static_cast<int>(heap.size());
            heap.push_back(id);
            pos[id] = idx;
            moveUp(idx);
        }

        void updateKey(int id) {
            //throw std::logic_error("MinHeap::" + std::string(__FUNCTION__) + " not implemented");
            int idx = pos[id];
            if (idx == -1) {
                // not currently in heap -> nothing to do.
                return;
            }

            // if smaller than the parent, it should move up
            if (idx > 0) {
                int parent = (idx - 1) / 2;
                if (lessAt(idx, parent)) {
                    moveUp(idx);
                    return;
                }
            }
            // else move down.
            moveDown(idx);
        }

        int pop() {
            //throw std::logic_error("MinHeap::" + std::string(__FUNCTION__) + " not implemented");
            if (heap.empty()) {
                return -1;
            }

            int rootId = heap[0];
            int lastIndex = static_cast<int>(heap.size()) - 1;

            if (lastIndex > 0) {
                heap[0] = heap[lastIndex];
                pos[heap[0]] = 0;
            }

            heap.pop_back();
            pos[rootId] = -1;

            if (!heap.empty()) {
                moveDown(0);
            }

            return rootId;
        }

    private:
        inline bool lessAt(int i, int j) const {
            // tie-break deterministically by ids
            if (key[heap[i]] != key[heap[j]]) return key[heap[i]] < key[heap[j]];
            return heap[i] < heap[j];
        }

        void moveUp(int i) {
            //throw std::logic_error("MinHeap::" + std::string(__FUNCTION__) + " not implemented");
            while (i > 0) {
                int parent = (i - 1) / 2;
                if (lessAt(i, parent)) {
                    int idI = heap[i];
                    int idP = heap[parent];

                    std::swap(heap[i], heap[parent]);
                    pos[idI] = parent;
                    pos[idP] = i;

                    i = parent;
                } else {
                    break;
                }
            }
        }

        void moveDown(int i) {
            //throw std::logic_error("MinHeap::" + std::string(__FUNCTION__) + " not implemented");
            int n = static_cast<int>(heap.size());
            while (true) {
                int left  = 2 * i + 1;
                int right = 2 * i + 2;
                int best = i;

                if (left < n && lessAt(left, best)) {
                    best = left;
                }
                if (right < n && lessAt(right, best)) {
                    best = right;
                }

                if (best == i) {
                    break;
                }

                int idI = heap[i];
                int idB = heap[best];

                std::swap(heap[i], heap[best]);
                pos[idI] = best;
                pos[idB] = i;

                i = best;
            }
        }
    };

} // namespace cs251
