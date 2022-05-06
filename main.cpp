#include <iostream>
#include <random>
#include <map>
#include <set>

using namespace std;

int m, s, a;
string fileoutput;
mt19937 mt;
multimap<pair<int, int>, int> graph;
map<pair<int, int>, int> graph_with_res;
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

    // количество состояний
    m = atoi(argv[1]);

    // количество примеров поведения
    s = atoi(argv[2]);

    // куда запишем ответ
    fileoutput = argv[3];

    // random seed либо передаем параметром либо сам выбирается
    if (argc > 4) {
        seed = atoi(argv[4]);
        mt = mt19937(seed);
    } else {
        random_device rd;
        mt = mt19937(rd());
    }

    // размер алфавита
    a = 2;

    // 1 - cостояние принимающее
    vector<int> is_true;

    // по каким значениям из алфавита есть переход из состояния i
    // amount[1] = {0} - из состояния 1 есть переход по 0
    vector<set<int>> amount;

    is_true.resize(m);
    amount.resize(m);

    // рандомно назначаем состояниям принимающее и нет.
    for (int i = 0; i < m; ++i) {
        is_true[i] = generate_true_or_false();
    }

    // первый шаг алгоритма - для каждой вершины i находим вершину index рандомно от 0 до i и проводим переход из index в i по 0 или 1
    for (int i = 1; i < m; ++i) {
        int index = -1;
        int res;

        // пока не нашли такую вершину index до i что перехода из index по res еще нет, пытаемся еще раз
        do {
            index = abs((int) mt()) % (i);
            res = generate_true_or_false();
            // amount[index].size() >= a - значит уже и по 0 и по 1 есть переход из index
            // amount[index].find(res) != amount[index].end() - есть переход по сгенеренному res
        } while (amount[index].size() >= a || amount[index].find(res) != amount[index].end());

        amount[index].insert(res);

        // в graph по двум упорядоченным вершинам хранится значение перехода (тк multimap, ничего не мешает переход между одинаковой парой вершин по двум символам)
        graph.insert({{index, i}, res});

        // в graph_with_res храним куда ведет переход из index по res in {0, 1} есть он есть
        graph_with_res.insert({{index, res}, i});
    }

    for (int i = 0; i < m; ++i) {
        int size = amount[i].size();

        // если еще нет перехода по какому-то из 0, 1
        for (int j = size; j < 2; ++j) {
            int index;
            int res;
            do {
                // рандомная вершина в которую будет переход из i
                index = abs((int) mt()) % (m);
                res = generate_true_or_false();

                // нет ли еще перехода по res из i
            } while (amount[i].find(res) != amount[i].end());
            amount[i].insert(res);

            graph.insert({{i, index}, res});

            graph_with_res.insert({{i, res}, index});

        }
    }
    fileoutput = fileoutput + ".txt";
    freopen(fileoutput.c_str(), "w", stdout);
    cout << s << " " << 2 << endl;

    int res;
    int length;
    vector<int> row;
    for (int i = 0; i < s; ++i) {

        // длина примера поведения (может быть очень большая, не знаю)
        length = abs(((int) mt()) % (4 * m)) + m;
        row.resize(length);

        // начинаем с вершины 0 (все состояния достижимы из нее)
        int cur_place = 0;

        for (int j = 0; j < length; ++j) {
            res = generate_true_or_false();
            row[j] = res;

            // переходим в новое состояние по сгенеренному символу перехода
            cur_place = graph_with_res[{cur_place, res}];
        }

        // принимает ли автомат пример поведения
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