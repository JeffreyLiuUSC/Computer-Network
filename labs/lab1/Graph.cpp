//
// Created by Jiahao Liu on 2019-05-28.
//
#include "Graph.h"
#include <stdexcept>
#include <sstream>
#include <limits>
#include <algorithm>

const long Graph::INFINITE_COST = std::numeric_limits<long>::max();

Graph::Graph(std::istream &input)
{
    std::string line;
    if (!std::getline(input, line))
        throw std::invalid_argument("empty graph");

    initMatrix(line);

    std::istringstream tokens;
    std::string token;

    auto rowIter = stepCostMatrix.begin(),
            rowsEnd = stepCostMatrix.end();
    do {
        tokens.str(line);
        tokens.clear();
        for (auto &stepCostMatrix : *rowIter) {
            if (!(tokens >> token)) {
                throw std::invalid_argument("not enough graph columns");
            }
            stepCostMatrix = parseCost(token);
        }
    } while (   (++rowIter != rowsEnd)
                && std::getline(input, line));

    if (rowIter != rowsEnd) {
        throw std::invalid_argument("not enough graph rows");
    }
}

// for debugging purposes
std::ostream& operator<<(std::ostream &output, const Graph &graph)
{
    for (const auto &row : graph.stepCostMatrix) {
        for (const auto stepCostMatrix : row) {
            output << stepCostMatrix << ' ';
        }
        output << std::endl;
    }
}

void Graph::initMatrix(const std::string &firstLine)
{
    std::size_t countSpaces = std::count(firstLine.begin(),
                                         firstLine.end(),
                                         ' ');
    std::size_t countNodes = countSpaces + 1;

    stepCostMatrix.resize(countNodes);
    for (auto &row : stepCostMatrix) {
        row.resize(countNodes);
        std::fill(row.begin(),
                  row.end(),
                  INFINITE_COST);
    }
}

long Graph::parseCost(const std::string &token)
{
    if (token == "inf")
        return INFINITE_COST; // infinite cost

    if ((token.front() == '[') && (token.back() == ']')) {
        return parseMultiCost(token); // multi-cost of the form [c_0,...,c_N]
    }

    return std::stol(token, nullptr); // integer cost
}

long Graph::parseMultiCost(const std::string &token)
{
    long totalCost = 0;
    long stepCostMatrix;
    std::istringstream sequence(token);
    while (sequence.ignore() && (sequence >> stepCostMatrix)) {
        totalCost += stepCostMatrix;
    }
    return totalCost;
}
