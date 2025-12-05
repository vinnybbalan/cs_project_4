#pragma once

#include <vector>
#include <functional>
#include <algorithm>

#include "heap.hpp"

namespace cs251 {

    struct Edge {
        int to;
        double timeRegular;     // required (non-negative)
        double timeBoosted;     // if < 0, booster is NOT available on this edge

        Edge(int to, double w, double wb = -1.0)
            : to(to), timeRegular(w), timeBoosted(wb) {}

        bool hasBoost() const { return timeBoosted >= 0.0; }
    };

    using Adj = std::vector<std::vector<Edge>>; // adjacency list

    struct PathResult {
        double bestTime = std::numeric_limits<double>::infinity();
        std::vector<int> pathNodes;         // sequence of node ids from start -> ... -> last checkpoint
        std::vector<int> usedBoostEdge;    // per-edge flag aligned with pathNodes transitions
        bool reachable = false;
    };

    PathResult constrained_shortest_path(
        const Adj &adj,
        const std::vector<int> &checkpoints,
        int K,
        int start
    ) {
        //throw std::logic_error("CSP::" + std::string(__FUNCTION__) + " not implemented");
        PathResult result;

        int n = static_cast<int>(adj.size());
        int numCheckpoints = static_cast<int>(checkpoints.size());

        if (n == 0) {
            return result;
        }

        // SPECIAL CASE: no checkpoints -> stay at start (cost 0)
        if (numCheckpoints == 0) {
            result.bestTime = 0.0;
            result.reachable = true;
            result.pathNodes.push_back(start);
            result.usedBoostEdge.push_back(0);
            return result;
        }

        if (K < 0) {
            K = 0;
        }
        int maxBoosts = K;

        int statesPerLayer = (maxBoosts + 1) * n;
        int layers = numCheckpoints + 1;
        int totalStates = layers * statesPerLayer;

        double INF = std::numeric_limits<double>::infinity();

        std::vector<double> dist(totalStates, INF);
        std::vector<int> parent(totalStates, -1);
        std::vector<int> parentBoost(totalStates, 0);

        // if start is already the first checkpoint.
        int j0 = 0;
        if (numCheckpoints > 0 && start == checkpoints[0]) {
            j0 = 1;
        }

        int startState = ((j0 * (maxBoosts + 1)) * n) + start;
        dist[startState] = 0.0;
        parent[startState] = -1;
        parentBoost[startState] = 0;

        MinHeap heap(dist, totalStates);
        heap.push(startState);

        double bestGoalDist = INF;
        int bestGoalState = -1;

        // Dijkstra
        while (!heap.empty()) {
            int state = heap.pop();

            if (dist[state] >= bestGoalDist) {
                continue;
            }

            // eecode state -> (v, j, k).
            int tmp = state;
            int v = tmp % n;
            tmp /= n;
            int k = tmp % (maxBoosts + 1);
            int j = tmp / (maxBoosts + 1);

            if (j == numCheckpoints && v == checkpoints[numCheckpoints - 1]) {
                bestGoalDist = dist[state];
                bestGoalState = state;
                break;
            }

            const std::vector<Edge> &edges = adj[v];
            for (size_t ei = 0; ei < edges.size(); ei++) {
                const Edge &e = edges[ei];
                int u = e.to;

                // if we arrive at next checkpoint, update progress
                int jNext = j;
                if (jNext < numCheckpoints && u == checkpoints[jNext]) {
                    jNext++;
                }

                // traverse without booster
                {
                    double newCost = dist[state] + e.timeRegular;
                    int kNext = k;
                    int nextState = (((jNext * (maxBoosts + 1)) + kNext) * n) + u;
                    if (newCost < dist[nextState]) {
                        dist[nextState] = newCost;
                        parent[nextState] = state;
                        parentBoost[nextState] = 0;
                        if (heap.contains(nextState)) {
                            heap.updateKey(nextState);
                        } else {
                            heap.push(nextState);
                        }
                    }
                }

                // if allowed/available, traver using booster
                if (k < maxBoosts && e.hasBoost()) {
                    double newCost = dist[state] + e.timeBoosted;
                    int kNext = k + 1;
                    int nextState = (((jNext * (maxBoosts + 1)) + kNext) * n) + u;
                    if (newCost < dist[nextState]) {
                        dist[nextState] = newCost;
                        parent[nextState] = state;
                        parentBoost[nextState] = 1;
                        if (heap.contains(nextState)) {
                            heap.updateKey(nextState);
                        } else {
                            heap.push(nextState);
                        }
                    }
                }
            }
        }

        // no valid path that visits all checkpoints
        if (bestGoalState == -1) {
            result.bestTime = INF;
            result.reachable = false;
            result.pathNodes.clear();
            result.usedBoostEdge.clear();
            return result;
        }

        std::vector<int> nodesRev;
        std::vector<int> boostsRev;

        int cur = bestGoalState;
        while (cur != -1) {
            int tmp = cur;
            int v = tmp % n;
            tmp /= n;
            int k = tmp % (maxBoosts + 1);
            int j = tmp / (maxBoosts + 1);

            (void)k; // k not needed during reconstruction
            (void)j; //j not needed during reconstruction

            nodesRev.push_back(v);
            boostsRev.push_back(parentBoost[cur]);
            cur = parent[cur];
        }

        int pathLen = static_cast<int>(nodesRev.size());
        result.pathNodes.resize(pathLen);
        result.usedBoostEdge.resize(pathLen);

        for (int i = 0; i < pathLen; i++) {
            result.pathNodes[i] = nodesRev[pathLen - 1 - i];
            result.usedBoostEdge[i] = boostsRev[pathLen - 1 - i];
        }

        // first node is the start node -> no edge used to reach
        if (pathLen > 0) {
            result.usedBoostEdge[0] = 0;
        }

        result.bestTime = bestGoalDist;
        result.reachable = true;

        return result;
    }

    void add_edge_undirected(Adj &adj, int u, int v, double w, double wBoost = -1.0) {
        adj[u].emplace_back(v, w, wBoost);
        adj[v].emplace_back(u, w, wBoost);
    }
} // namespace cs251