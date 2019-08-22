//
// Created by Jiahao Liu on 2019-05-28.
//
#ifndef PATH_FINDER_H
#define PATH_FINDER_H

#include "Graph.h"
#include <tuple>
#include <optional>

namespace PathFinder {

    std::tuple< unsigned long, std::vector<long>, std::optional<std::vector<long>> >        //implementation of bellman ford
    findBestPaths(const Graph &graph);                                                      //to find the shortest path

} // namespace PathFinder

#endif // ifndef PATH_FINDER_H