//
// Created by Jiahao Liu on 2019-05-28.
//
#include "Graph.h"
#include <stdexcept>
#include <sstream>
#include <limits>
#include <algorithm>

const long Graph::INFINITE_COST = std::numeric_limits<long>::max(); //initial definition of all costs and final result for unreachable node.

Graph::Graph(std::istream &input)
{
    std::string line;
    if (!std::getline(input, line))
    {
        throw std::invalid_argument("empty graph");                  //if not valid format, throw exception
    }
    bool getInitialed = initMatrix(line);                            //start to convert input line to 2-d vector matrix

    std::istringstream tokens;                                       //all elements in a row
    std::string token;                                               //for each element in the row

    auto rowIter = stepCostMatrix.begin(),
            rowsEnd = stepCostMatrix.end();
    do {
        tokens.str(line);                                            //read one line in string
        tokens.clear();
        for (auto &stepCostMatrix : *rowIter) {
            if (!(tokens >> token)) {                                 //throw exception if there is missing data in a column
                throw std::invalid_argument("lack of columns");
            }
            stepCostMatrix = parseCost(token);                        //parse the value in the row to check if it is infinity of integer of invalid data
        }
    } while (   (++rowIter != rowsEnd)                                //iterator for traverse each line that read
                && std::getline(input, line));                        //keep reading every lines

    if (rowIter != rowsEnd) {                                         //row incomplete check
        throw std::invalid_argument("lack of  rows");
    }
}

bool Graph::initMatrix(const std::string &firstLine)                     //initialize a dummy matrix for future fulfilled
{
    long spacesNumber = count(firstLine.begin(), firstLine.end(), ' ');  //count the lines between each nodes
    long nodesNumber = spacesNumber + 1;                                 //the number of nodes in each row is one more than the lines
    stepCostMatrix.resize(nodesNumber);                                  //construct a dummy matrix
    for (auto &row : stepCostMatrix) {                                   //fill in the value of infinite_cost to the matrix with the
        row.resize(nodesNumber);                                         //same size as the graph read from out side.
        fill(row.begin(), row.end(), std::numeric_limits<long>::max());
    }
}

long Graph::parseCost(const std::string &token)                         //handle the situation of inf value, brackets
{
    if (token == "inf")                                                 //if the value is infinity which means it is unreachable
        return std::numeric_limits<long>::max();                        // infinite cost
    if ((token.front() == '[') && (token.back() == ']')) {              //deliver it to another helper to handle
        long totalCost = 0;
        long stepCostMatrix;
        std::istringstream sequence(token);
        while (sequence.ignore() && (sequence >> stepCostMatrix)) {      //go through every parts in the token
            totalCost += stepCostMatrix;
        }
        return totalCost;
    }
    return std::stol(token, nullptr);                                   //integer value
}
