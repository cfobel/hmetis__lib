#ifndef ___H_METIS_WRAPPER__HPP___
#define ___H_METIS_WRAPPER__HPP___

#include <vector>
#include <boost/format.hpp>

namespace hmetis {
using namespace std;

extern "C" void HMETIS_PartRecursive(int nvtxs, int nhedges,
        int *vwgts,
        int *eptr, int *eind,
        int *hewgts, int nparts,
        int ubfactor, int *options, int *part, int *edgecut);

vector<vector<int> > get_partition(int vertex_count,
        vector<vector<int> > &hedges,
        int partitions_count,
        int balance) {
    using boost::format;
    int hyperedge_count = hedges.size();
    int total_count = 0;
    for(int i = 0; i < hedges.size(); i++) {
        total_count += hedges[i].size();
    }

    vector<int> edge_pointers(hedges.size() + 1);
    vector<int> edge_vertices(total_count);
    vector<int> set_ids(vertex_count);

    int index = 0;
    edge_pointers[0] = 0;
    for(int i = 0; i < hedges.size(); i++) {
        vector<int> const &h = hedges[i];
        copy(h.begin(), h.end(), edge_vertices.begin() + index);
        index += h.size();
        cout << format("[%d] ") % i;
        copy(h.begin(), h.end(),
                ostream_iterator<int>(cout, ", "));
        cout << endl;
        edge_pointers[i + 1] = index;
    }
    copy(edge_pointers.begin(), edge_pointers.end(),
            ostream_iterator<int>(cout, ", "));
    cout << endl;
    copy(edge_vertices.begin(), edge_vertices.end(),
            ostream_iterator<int>(cout, ", "));
    cout << endl;
    int options = 0;
    int cut_edges_count;
    HMETIS_PartRecursive(vertex_count, hyperedge_count,
            NULL, &edge_pointers[0], &edge_vertices[0],
            NULL, partitions_count, balance,
            &options, &set_ids[0], &cut_edges_count);
    copy(set_ids.begin(), set_ids.end(),
            ostream_iterator<int>(cout, ", "));
    cout << endl;
    vector<vector<int> > partition(partitions_count);
    for(int vertex_id = 0; vertex_id < vertex_count; vertex_id++) {
        int set_id = set_ids[vertex_id];
        partition[set_id].push_back(vertex_id);
    }
    return partition;
}

void dump_partition(ostream &output,
        vector<vector<int> > const &partition) {
    for(int set_id = 0; set_id < partition.size(); set_id++) {
        vector<int> const &vertices = partition[set_id];
        output << boost::format("[%d] ") % set_id;
        copy(vertices.begin(), vertices.end(),
                ostream_iterator<int>(output, ", "));
        output << endl;
    }

}

}

#endif
