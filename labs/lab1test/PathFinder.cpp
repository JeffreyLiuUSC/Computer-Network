//
// Created by Jiahao Liu on 2019-05-28.
//
#include "PathFinder.h"
#include <limits>

namespace PathFinder {
    namespace {
        bool addWillOverflow(long x, long y)                                        //To check the next move if it is out of boundary
        {
            return ((y > 0) && (x > (std::numeric_limits<long>::max() - y)))
                   || ((y < 0) && (x < (std::numeric_limits<long>::min() - y)));
        }

        bool takeStep(long                     prevNode,                            //based on the current node, take a move
                      const std::vector<long> &stepCosts,                           //towards next nodes
                      std::vector<long>       &minCosts,
                      std::vector<long>       &bestPaths)
        {
            bool flag = false;                                                      //a flag indicating if a new short path was found
            long minCostToPrev = minCosts[prevNode];                                //read the min cost till current node
            for (long nextNode = 0; nextNode < stepCosts.size(); ++nextNode) {      //for all nodes from the current node
                long stepCostPrevNext = stepCosts[nextNode];                        //get the cost to one of the next node
                if (addWillOverflow(minCostToPrev, stepCostPrevNext)) {
                    continue;                                                       // nextNode is unreachable from prevNode
                }
                long totalCostNext = minCostToPrev + stepCostPrevNext;              //if reachable, sum up the total cost
                long &minCostNext  = minCosts[nextNode];                            //get the cost to the next from the original data
                if (totalCostNext < minCostNext) {                                  //update the min cost next if a new path has been
                    minCostNext         = totalCostNext;                            //found by the smaller path cost
                    bestPaths[nextNode] = prevNode;
                    flag             = true;
                }
            }
            return flag;
        }
    } // namespace

    std::tuple< unsigned long, std::vector<long>, std::optional<std::vector<long>> >    //return a tuple contains the iteration number
    findBestPaths(const Graph &graph)                                                   //path vector and the cost(nullptr if negative loop exists)
    {
        std::vector<long> minCosts(graph.size(), Graph::INFINITE_COST);                 //initialize a vector for storing min cost
        std::vector<long> bestPaths(graph.size(), -1);                                  //initialize a vector for storing the best path
        bool flag     = false;
        minCosts.front() = 0;                                                           // set up the first value(0 to 0)
        unsigned long iterations  = 1;                                                  // count the iteration
        do {                                                                            //bellman for implementation
            flag = false;                                                               //starting from every nodes, check
            for (long node = 0; node < graph.size(); ++node) {                          //all adjacent nodes to see if there is a less cost path
                flag |= takeStep(node, graph[node], minCosts, bestPaths);
            }
            iterations++;
        } while (flag && (iterations < graph.size()));

        if (!flag) {                                                                    //if no update, early determination
            return {                                                                    //will be triggered
                    iterations,                                                         //return the current bestPath and cost
                    std::move(bestPaths),
                    std::make_optional(std::move(minCosts))
            };
        }
        return { 0, std::move(bestPaths), std::nullopt };                                //after N-1 times of iteration, there supposed to be
    }                                                                                    //an termination, otherwise, there exist a negative loop
} // namespace PathFinder
