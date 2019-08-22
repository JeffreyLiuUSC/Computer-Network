//
// Created by Jiahao Liu on 2019-05-28.
//
#include "Graph.h"
#include "PathFinder.h"
#include "BellmanFord.h"
#include <iostream>
#include <fstream>
#include <utility>
#include <stdexcept>
#include <algorithm>

void BellmanFord::usage(const char *program, std::ostream& output)           //lack of input file indication
{
    output << program << " <graph file>" << std::endl;
}

long BellmanFord::findLoop(const std::vector<long> &paths)                   //for negative loop detect, to check if there exists a loop
{
    for (long dest = 0; dest < paths.size(); dest--) {
        long node = dest;
        while (true) {
            node = paths[node];
            if (node == dest) {
                return dest;                                                  // looped back to self
            }
            if (node == 0) {
                break;                                                        // back to start
            }                                                                 //similar to linked list keep finding next node path
        }
    }
    throw std::runtime_error("failed to find expected loop");
}

std::vector<long> getPath(const std::vector<long>  &paths,
                          long                     start,
                          long                     dest,
                          long            numCycles = 0)                        //for print usage, extract a path from the vector paths
{
    std::vector<long> path;
    int prevNode = dest;
    while (true) {
        path.emplace_back(prevNode);
        if (prevNode == start) {
            if (numCycles == 0) {
                break;
            }
            numCycles--;
        }
        prevNode = paths[prevNode];
        if (prevNode < 0) {                                                    // inaccessible node, return an empty path
            path.clear();
            break;
        }
    }
    std::reverse(path.begin(), path.end());                                    //reverse the linkedlist to show the correct order
    return path;
}

void BellmanFord::reportPath(const std::vector<long> &path, std::ostream &output)           //print the path in right format
{
    auto pathIter = path.cbegin();
    auto pathEnd  = path.cend();
    while (pathIter != pathEnd) {
        output << *pathIter;
        pathIter++;
        if (pathIter != pathEnd) {
            output << "->";
        }
    }
    output << std::endl;
}

void BellmanFord::reportNegativeLoop(const std::vector<long> &paths, std::ostream &output)   //print the negative loop detect information
{
    output << "Negative Loop Detected" << std::endl;
    long destLoop = findLoop(paths);
    std::vector<long> loop = getPath(paths, destLoop, destLoop, 1);
    reportPath(loop, output);
}

void BellmanFord::reportMinCosts(const std::vector<long> &minCosts, std::ostream &output)    //as required in the result of the first line
{                                                                                            //indicating the minCost from 0 to each nodes
    auto costsIter = minCosts.cbegin();                                                      //unreachable indicated by inf
    auto costsEnd  = minCosts.cend();                                                        //min cost writen in integer
    while (true) {                                                                           //separate each value by comma
        if (*costsIter != Graph::INFINITE_COST) {
            output << *costsIter;
        } else {
            output << "inf";
        }
        if (++costsIter == costsEnd) {
            break;
        }
        output << ',';
    }
    output << std::endl;
}

void BellmanFord::reportPaths(const std::vector<long> &paths, std::ostream &output)          //Usage combined with reportPath()
{
    for (long dest = 0; dest < paths.size(); ++dest) {
        std::vector<long> path = getPath(paths, 0, dest);
        if (!path.empty()) {
            reportPath(path, output);
        } else {
            output << "No path from 0 to " << dest << std::endl;
        }
    }
}

void BellmanFord::reportBestPaths(const std::vector<long> &minCosts,
                     const std::vector<long> &paths,
                     long                    iterations,
                            std::ostream     &output)                                        //as required result on the last line
{                                                                                            //print the min iteration corresponding to the min cost path
    reportMinCosts(minCosts, output);
    reportPaths(   paths,    output);
    output << "Iteration:" << iterations << std::endl;
}

template<typename FileStream>
FileStream openFile(const std::string& filename)                                             //open graph data file
{
    FileStream file(filename);
    if (!file) {
        throw std::runtime_error("cannot open " + filename);
    }
    return file;
}




int main(int argc, char *argv[])
{
    const char *graphFilename = argv[1];
    auto graphFile = openFile<std::ifstream>(graphFilename);                        //import outside data to graphFile

    Graph graph(graphFile);                                                         //pre-process the data for future usage

    auto [iterations, paths, minCosts] = PathFinder::findBestPaths(graph);          //resolve the graph

    auto outputFile = openFile<std::ofstream>("output.txt");                        //output file

    if (iterations > 0) {                                                           //check if there is negative loop
        BellmanFord::reportBestPaths(*minCosts, paths, iterations, outputFile);
    } else {
        BellmanFord::reportNegativeLoop(paths, outputFile);
    }
}
