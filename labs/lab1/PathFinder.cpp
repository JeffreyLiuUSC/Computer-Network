//
// Created by Jiahao Liu on 2019-05-28.
//
#include "PathFinder.h"

namespace PathFinder {
    namespace {
        bool addWillOverflow(long x, long y)
        {
            return ((y > 0) && (x > (std::numeric_limits<long>::max() - y)))
                   || ((y < 0) && (x < (std::numeric_limits<long>::min() - y)));
        }

        bool takeStep(long                     prevNode,
                      const std::vector<long> &stepCosts,
                      std::vector<long>       &minCosts,
                      std::vector<long>       &bestPaths)
        {
            bool updated = false;
            long minCostToPrev = minCosts[prevNode];
            for (long nextNode = 0; nextNode < stepCosts.size(); ++nextNode) {
                long stepCostPrevNext = stepCosts[nextNode];
                if (addWillOverflow(minCostToPrev, stepCostPrevNext)) {
                    continue; // nextNode is inaccessible from prevNode
                }
                long totalCostNext = minCostToPrev + stepCostPrevNext;
                long &minCostNext  = minCosts[nextNode];
                if (totalCostNext < minCostNext) {
                    minCostNext         = totalCostNext;
                    bestPaths[nextNode] = prevNode;
                    updated             = true;
                }
            }
            return updated;
        }

    } // namespace

    std::tuple< unsigned long, std::vector<long>, std::optional<std::vector<long>> >
    findBestPaths(const Graph &graph)
    {
        std::vector<long> minCosts(graph.size(), Graph::INFINITE_COST);
        std::vector<long> bestPaths(graph.size(), -1);
        bool updated     = false;
        minCosts.front() = 0; // set the initial
        unsigned long iterations  = 1; // count the
        do {
            updated = false;
            // for every node, take a step to all accessible nodes
            // if a cheaper path to a new node is found, update minCosts and
            // bestPaths
            for (long node = 0; node < graph.size(); ++node) {              ///？？？？？？？？
                updated |= takeStep(node,
                                    graph[node],
                                    minCosts,
                                    bestPaths);
            }
            ++iterations;
        } while (updated && (iterations < graph.size()));

        if (!updated) {
            // early termination
            // => minCosts and bestPaths are correct
            return {
                    iterations,
                    std::move(bestPaths),
                    std::make_optional(std::move(minCosts))
            };
        }

        // continued updating past maximum N-1 iterations
        // => must have encountered a negative loop
        // => return '0' iterations to signal this condition
        return { 0, std::move(bestPaths), std::nullopt };
    }


} // namespace PathFinder
