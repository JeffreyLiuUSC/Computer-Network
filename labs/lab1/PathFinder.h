//
// Created by Jiahao Liu on 2019-05-28.
//
#ifndef PATH_FINDER_H
#define PATH_FINDER_H

#include "Graph.h"
#include <tuple>
#include <optional>

namespace PathFinder {

/**
 * @brief Finds the lowest-cost path from node 0 to node N-1 using the
 *     Bellman–Ford algorithm.
 * @return { <iteration count> >= 1, <path vector>, <cost vector> } if the
 *     algorithm terminates without encountering a negative loop.  If a
 *     negative loop is encountered, return { 0, <path vector>, nullopt }.
 */
    std::tuple< unsigned long, std::vector<long>, std::optional<std::vector<long>> >
    findBestPaths(const Graph &graph);

} // namespace PathFinder

#endif // ifndef PATH_FINDER_H