#include <fstream>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <cstdlib>
#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>
#include "HMetisWrapper.hpp"

using namespace std;


vector<vector<int> > random_hypergraph(int vertex_count,
        int hyperedge_count) {
    vector<vector<int> > hedges;

    for(int hedge_id = 0; hedge_id < hyperedge_count; hedge_id++) {
        vector<int> vertex_ids;
        int count = (rand() % 7) + 2;
        for(int i = 0; i < count; i++) {
            vertex_ids.push_back(rand() % vertex_count);
        }
        hedges.push_back(vertex_ids);
    }
    return hedges;
}


int main(int argc, char **argv) {
    using boost::format;
    using boost::lexical_cast;
    using namespace hmetis;

    int partitions_count;
    int balance = 2;
    if(argc < 3 || argc > 5) {
        cerr << format("usage: %s <vertex count> <hyperedge count> "
                "[<partitions count>=vertex_count/64] [<balance>=2]")
            % argv[0] << endl;
        exit(-1);
    }
    srand(0);
    int vertex_count = lexical_cast<int>(argv[1]);
    int hyperedge_count = lexical_cast<int>(argv[2]);
    if(argc < 4) {
        partitions_count = max(2, vertex_count / 64);
    } else {
        partitions_count = lexical_cast<int>(argv[3]);
    }
    if(argc >= 5) {
        balance = lexical_cast<int>(argv[4]);
    }
    cout << partitions_count << endl;

    vector<vector<int> > hedges = random_hypergraph(vertex_count,
            hyperedge_count);
    vector<vector<int> > p = get_partition(vertex_count, hedges,
            partitions_count, balance);

    dump_partition(cout, p);

    return 0;
}
