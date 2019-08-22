//
// Created by Jiahao Liu on 2019-05-31.
//

#ifndef LAB1TEST_BELLMANFORD_H
#define LAB1TEST_BELLMANFORD_H

namespace BellmanFord {

    void usage(const char *program, std::ostream& output);                          //lack of input file indication

    void reportPath(const std::vector<long> &path, std::ostream &output);           //print the path in right format

    long findLoop(const std::vector<long> &paths);                                  //for negative loop detect, to check if there exists a loop

    void reportNegativeLoop(const std::vector<long> &paths, std::ostream &output);   //print the negative loop detect information

    void reportMinCosts(const std::vector<long> &minCosts, std::ostream &output);    //as required in the result of the first line

    void reportPaths(const std::vector<long> &paths, std::ostream &output);          //Usage combined with reportPath()

    void reportBestPaths(const std::vector<long> &minCosts,
                         const std::vector<long> &paths,
                         long                    iterations,
                         std::ostream     &output);                                  //as required result on the last line
}







#endif //LAB1TEST_BELLMANFORD_H
