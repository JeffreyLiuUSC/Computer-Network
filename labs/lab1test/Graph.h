//
// Created by Jiahao Liu on 2019-05-28.
//

#ifndef GRAPH_H
#define GRAPH_H

#include <istream>
#include <ostream>
#include <vector>

class Graph
{
    public:

        Graph(std::istream &input); //rephrase the input data to be a graph

        friend std::ostream& operator<<(std::ostream &output, const Graph &graph); //output the writen graph

        auto size() const { return stepCostMatrix.size(); } //return the size of the Costs Matrix

        const std::vector<long>&operator[](long node) const { return stepCostMatrix[node]; } //return the cost of given node

        static const long INFINITE_COST; //initial definition of all costs and final result for unreachable node.

    private:
        std::vector<std::vector<long>> stepCostMatrix; //2-D vector for storing the costs of graph

        static long parseCost(const std::string &token);

        static long parseMultiCost(const std::string &token);

        bool initMatrix(const std::string &firstLine);

        static bool addWillOverflow(long x, long y); //To check if the next move is out of boundary

        static bool takeStep(long                       prevNode,
                             const std::vector<long>    &stepCosts,
                             std::vector<long>          &minCosts,
                             std::vector<long>          &bestPath); //To take a move towards next reachable node
};

#endif // GRAPH_H
