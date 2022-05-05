#include <iostream>
#include <random>
#include <map>
#include <set>

using namespace std;

int m, s, a;
string fileoutput;
mt19937 mt;
map<pair<int, int>, int> graph;
map<pair<int, int>, int> graphWithRes;
int seed;

int generate_true_or_false() {
    int next = ((int) mt()) % 2;
    if (next % 2 == 0) {
        return 0;
    } else {
        return 1;
    }
}

int main(int argc, char **argv) {

//    freopen("~/untitled/automata/cmake-build-debug/input.in", "r", stdin);
//    freopen("input.out", "w", stdout);

//    cin >> m >> s;

    m = atoi(argv[1]);
    s = atoi(argv[2]);
    fileoutput = argv[3];
    if (argc > 4) {
        seed = atoi(argv[4]);
        mt = mt19937(seed);
    } else {
        random_device rd;
        mt = mt19937(rd());
    }
    a = 2;
    vector<int> v;
    vector<int> is_true;
    vector<set<int>> amount;
    vector<pair<int, int>> paths;
    v.resize(m);
    is_true.resize(m);
    amount.resize(m);

    for (int i = 0; i < m; ++i) {
        is_true[i] = generate_true_or_false();
    }

    for (int i = 2; i < m; ++i) {
        int index = -1;
        int res;
        do {
            index = abs((int) mt()) % (i);
            res = generate_true_or_false();

        } while (amount[index].size() > a || amount[index].find(res) != amount[index].end());
        amount[index].insert(res);
        graph.insert({{index, i}, res});
        graphWithRes.insert({{index, res}, i});
    }

    for (int i = 0; i < m; ++i) {
        int size = amount[i].size();
        for (int j = size; j < 2; ++j) {
            int index;
            int res;
            do {
                index = abs((int) mt()) % (m);
                res = generate_true_or_false();
            } while (graph.find({i, index}) != graph.end() || amount[i].find(res) != amount[i].end());
            amount[i].insert(res);
            graph.insert({{i, index}, res});
//            graphWithRes.insert({{index, i}, res});
            graphWithRes.insert({{index, res}, i});
        }
    }
    fileoutput = fileoutput + ".txt";
    freopen(fileoutput.c_str(), "w", stdout);
    cout << s << " " << 2 << endl;

    int res;
    int length;
    vector<int> row;
    for (int i = 0; i < s; ++i) {
        length = abs(((int) mt()) % (4 * m)) + m;
        row.resize(length);
        int cur_place = 0;
//        cout << res << " " << length << " ";
        for (int j = 0; j < length; ++j) {
            res = generate_true_or_false();
            row[j] = res;
            cur_place = graphWithRes[{cur_place, res}];
//            cout << res << " ";
        }
        cout << is_true[cur_place] << " " << length << " ";
        for (int k = 0; k < row.size(); ++k) {
            cout << row[k] << " ";
        }
        cout << endl;
    }


    freopen("input.txt", "w", stdout);
    cout << "digraph G { \n";

    for (int i = 0; i < m; ++i) {
        cout << i << " " << "[label = " << "\"" << i << "\"" << "];\n";
    }

    for (auto it = graph.begin(); it != graph.end(); it++) {
        cout << it->first.first << " -> " << it->first.second << " [label = " << "\"" << it->second << "\"" << "];\n";
    }

    cout << "}";

//    dot -Tsvg input.txt > output.svg


    return 0;
}