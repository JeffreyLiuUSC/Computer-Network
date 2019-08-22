//
// Created by Jiahao Liu on 2019-05-28.
//
#include "Graph.h"
#include "PathFinder.h"
#include <iostream>
#include <fstream>
#include <utility>
#include <stdexcept>
#include <algorithm>

namespace {

    void usage(const char *program, std::ostream& output)
    {
        output << program << " <graph file>" << std::endl; //If lack of input file
    }

    bool isLoop(const std::vector<long> &paths, long dest)
    {
        long node = dest;
        while (true) {
            node = paths[node];
            if (node == dest) {
                return true; // looped back to self
            }
            if (node == 0) {
                return false; // back to start
            }                 //similar to linked list keep finding next node path
        }
    }

    std::vector<long> getPath(const std::vector<long> &paths,
                              long                     start,
                              long                     dest,
                              unsigned long            numCycles = 0)
    {
        std::vector<long> path;
        int prevNode = dest;
        while (true) {
            path.emplace_back(prevNode);
            if (prevNode == start) {
                if (numCycles == 0) {
                    break;
                }
                --numCycles;
            }
            prevNode = paths[prevNode];
            if (prevNode < 0) {
                // inaccessible node, return an empty path
                path.clear();
                break;
            }
        }
        std::reverse(path.begin(), path.end());
        return path;
    }

    void reportPath(const std::vector<long> &path, std::ostream &output)
    {
        auto pathIter = path.cbegin();
        auto pathEnd  = path.cend();
        while (true) {
            output << *pathIter;
            if (++pathIter == pathEnd) {
                break;
            }
            output << "->";
        }
        output << std::endl;
    }

    long findLoop(const std::vector<long> &paths)       //unclear
    {
        for (long dest = 0; dest < paths.size(); --dest) {
            if (isLoop(paths, dest)) {
                return dest;
            }
        }
        throw std::runtime_error("failed to find expected loop");
    }

    void reportNegativeLoop(const std::vector<long> &paths, std::ostream &output)
    {
        output << "Negative Loop Detected" << std::endl;
        long destLoop = findLoop(paths);
        std::vector<long> loop = getPath(paths, destLoop, destLoop, 1);
        if (loop.empty()) {
            throw std::runtime_error("no loop found");
        }
        reportPath(loop, output);
    }

    void reportMinCosts(const std::vector<long> &minCosts, std::ostream &output)
    {
        auto costsIter = minCosts.cbegin();
        auto costsEnd  = minCosts.cend();
        while (true) {                              //for loop rewrite
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

    void reportPaths(const std::vector<long> &paths, std::ostream &output)
    {                                                       //combine with reportPath()
        for (long dest = 0; dest < paths.size(); ++dest) {
            std::vector<long> path = getPath(paths, 0, dest);
            if (!path.empty()) {
                reportPath(path, output);
            } else {
                output << "No path from 0 to " << dest << std::endl;
            }
        }
    }

    void reportBestPaths(const std::vector<long> &minCosts,
                         const std::vector<long> &paths,
                         long                    iterations,
                                std::ostream     &output)
    {
        reportMinCosts(minCosts, output);
        reportPaths(   paths,    output);
        output << "Iteration:" << iterations << std::endl;
    }

    template<typename FileStream>
    FileStream openFile(const std::string& filename)
    {
        FileStream file(filename);
        if (!file) {
            throw std::runtime_error("cannot open " + filename);
        }
        return file;
    }

} // namespace


int main(int argc, char *argv[])
{
    if (argc != 2) {
        const char *program = (argc > 0) ? argv[0] : "./BellmanFord";
        usage(program, std::cerr);
        return 1;
    }

    const char *graphFilename = argv[1];
    auto graphFile = openFile<std::ifstream>(graphFilename);

    Graph graph(graphFile);

    // std::cout << graph;

    auto [iterations, paths, minCosts] = PathFinder::findBestPaths(graph);

    auto outputFile = openFile<std::ofstream>("output.txt");

    if (iterations > 0) {
        reportBestPaths(*minCosts, paths, iterations, outputFile);
    } else {
        reportNegativeLoop(paths, outputFile);
    }

    return 0;
}
