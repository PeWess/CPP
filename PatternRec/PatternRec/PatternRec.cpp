#include <iostream>
#include <cmath>
#include <set>
#include <map>
#include <vector>
using namespace std;

class Objects {
public:

    //???????, ????????? ???? ???????? (????????????? ?????????????? ??? ?????? ?????????? ????????)
    vector<vector<float>> FindCore(set<int>& cluster) {
        vector<vector<float>> result;
        result.assign(10, vector<float>(10));
        int counter = cluster.size();
        if (counter > 1) {
            for (int i = 0; i < counter; i++) {
                for (int j = 0; j < 10; j++) {
                    for (int k = 0; k < 10; k++) {
                        result[j][k] += cluster_and_objects[cluster][i][j][k];
                    }
                }
            }
            for (int m = 0; m < 10; m++) {
                for (int n = 0; n < 10; n++) {
                    result[m][n] = result[m][n] / counter;
                }
            }
        }
        else {
            for (int i = 0; i < 10; i++) {
                for (int j = 0; j < 10; j++) {
                    result[i][j] = cluster_and_objects[cluster][0][i][j];
                }
            }
        }
        return result;
    }

    //?????????? ??????? ???????
    float Euclid(set<int> first_cluster, set<int> second_cluster) {
        float result = 0;
        vector<vector<float>> first_core = FindCore(first_cluster);
        vector<vector<float>> second_core = FindCore(second_cluster);
        for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 10; j++) {
                result += pow(first_core[i][j] - second_core[i][j], 2);
            }
        }
        return sqrt(result);
    }

    //???????, ???????????? ??? ???????? ? ????, ? ????????? ?? ????????? ????? ?? ??????????
    void BuildNewCluster(set<int>& first_old_cluster, set<int>& second_old_cluster) {
        set<int> new_cluster;
        for (auto it : first_old_cluster) {
            new_cluster.insert(it);
        }
        for (auto its : second_old_cluster) {
            new_cluster.insert(its);
        }
        cluster_and_objects[new_cluster] = cluster_and_objects[first_old_cluster];
        for (int m = 0; m < second_old_cluster.size(); m++) {
            cluster_and_objects[new_cluster].push_back(cluster_and_objects[second_old_cluster][m]);
        }
        cluster_and_objects.erase(first_old_cluster);
        cluster_and_objects.erase(second_old_cluster);
    }

    //???????, ??????? ???????????? ??? ????????? ???????, ? ?????????? ???? ????, ????:
    //1)?????? ??????? ? ?????????? ????? ????? ??????? (????? ????????? ?? ?????????? ????)
    //2)?????????? ?? ??????? ???????? ?? ??????? ?? ????????? ?? ??????? ????? ????? ??????(???? ?? ?? ??? ?????????? ????? - ????????????? ?????? ???????? ?????), ?? ????????, ???????? ? main()
    //?????, ???? ??? ??????? ?? ???????????? ? ???? ??????? ? ??? ???? ? ????? ???? ???????????? ??????? ?????????, ?? ???? ???????????, ?????????????? ?????? ?????????? ??? ???? ?????????.
    void HierarchicalClustering(int& integrations, float& max_distance) {
        float min_metric = 99999999;
        set<int> first_cluster;
        set<int> second_cluster;
        bool was_integration = false;
        for (auto it_1 : cluster_and_objects) {
            for (auto it_2 : cluster_and_objects) {
                if (it_1.first < it_2.first) {
                    float euclid_for_comp = Euclid(it_1.first, it_2.first);
                    if (euclid_for_comp < min_metric && euclid_for_comp <= max_distance) {
                        first_cluster = it_1.first;
                        second_cluster = it_2.first;
                        min_metric = euclid_for_comp;
                        was_integration = true;
                    }
                }
            }
        }
        if (cluster_and_objects.size() != 1 && was_integration == true) {
            BuildNewCluster(first_cluster, second_cluster);
            integrations++;
            cout << "????????? ????? " << integrations << "-?? ???????????" << endl;
            PrintClusters();
            HierarchicalClustering(integrations, max_distance);
        }
    }

    //?????? ?????????? ??????? ???????? ? ????? ???????? ?????????        
    void PrintClusters() {
        for (auto it : cluster_and_objects) {
            cout << "??????? ? ?????????: {";
            for (int its : it.first) {
                cout << its << ", ";
            }
            cout << "\b\b" << "}" << endl;
            cout << "??????? ????????: " << endl;
            for (int i = 0; i < it.first.size(); i++) {
                for (int k = 0; k < 10; k++) {
                    for (int m = 0; m < 10; m++) {
                        cout << cluster_and_objects[it.first][i][k][m];
                    }
                    cout << endl;
                }
                cout << endl << endl;
            }
        }
    }
    map<set<int>, vector<vector<vector<int>>>> cluster_and_objects = {
    {{1},{{{0,0,1,1,1,1,1,1,0,0},
        {0,1,1,1,1,1,1,1,1,0},
        {1,1,0,0,0,0,0,0,1,1},
        {1,1,0,0,0,0,0,0,1,1},
        {1,1,0,0,0,0,0,0,1,1},
        {1,1,0,0,0,0,0,0,1,1},
        {1,1,0,0,0,0,0,0,1,1},
        {1,1,0,0,0,0,0,0,1,1},
        {0,1,1,1,1,1,1,1,1,0},
        {0,0,1,1,1,1,1,1,0,0}}}},

    {{2},{{{1,0,0,0,0,0,0,0,0,1},
        {0,1,0,0,0,0,0,0,1,0},
        {0,0,1,0,0,0,0,1,0,0},
        {0,0,0,1,0,0,1,0,0,0},
        {0,0,0,0,1,1,0,0,0,0},
        {0,0,0,0,1,1,0,0,0,0},
        {0,0,0,1,0,0,1,0,0,0},
        {0,0,1,0,0,0,0,1,0,0},
        {0,1,0,0,0,0,0,0,1,0},
        {1,0,0,0,0,0,0,0,0,1}}}},

    {{3},{{{0,0,0,0,1,1,0,0,0,0},
        {0,0,0,1,0,0,1,0,0,0},
        {0,0,0,1,0,0,1,0,0,0},
        {0,0,1,0,0,0,0,1,0,0},
        {0,0,1,0,0,0,0,1,0,0},
        {0,1,0,0,0,0,0,0,1,0},
        {0,1,0,0,0,0,0,0,1,0},
        {1,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,1},
        {1,1,1,1,1,1,1,1,1,1}}}},

    {{4},{{{0,0,0,0,1,1,0,0,0,0},
        {0,0,0,1,0,0,1,0,0,0},
        {0,0,1,0,0,0,0,1,0,0},
        {0,1,0,0,0,0,0,0,1,0},
        {1,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,1},
        {0,1,0,0,0,0,0,0,1,0},
        {0,0,1,0,0,0,0,1,0,0},
        {0,0,0,1,0,0,1,0,0,0},
        {0,0,0,0,1,1,0,0,0,0}}}},

    {{5},{{{0,1,1,1,1,1,1,1,1,0},
        {1,1,1,1,1,1,1,1,1,1},
        {1,1,0,0,0,0,0,0,1,1},
        {1,1,0,0,0,0,0,0,1,1},
        {1,1,0,0,0,0,0,0,1,1},
        {1,1,0,0,0,0,0,0,1,1},
        {1,1,0,0,0,0,0,0,1,1},
        {1,1,0,0,0,0,0,0,1,1},
        {1,1,1,1,1,1,1,1,1,1},
        {0,1,1,1,1,1,1,1,1,0}}}},

    {{6},{{{0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0},
        {0,0,1,0,0,0,0,1,0,0},
        {0,0,0,1,0,0,1,0,0,0},
        {0,0,0,0,1,1,0,0,0,0},
        {0,0,0,0,1,1,0,0,0,0},
        {0,0,0,1,0,0,1,0,0,0},
        {0,0,1,0,0,0,0,1,0,0},
        {0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0}}}},

    {{7},{{{0,0,0,1,1,1,1,0,0,0},
        {0,0,0,1,0,0,1,0,0,0},
        {0,0,1,1,0,0,1,1,0,0},
        {0,0,1,0,0,0,0,1,0,0},
        {0,1,1,0,0,0,0,1,1,0},
        {0,1,0,0,0,0,0,0,1,0},
        {1,1,0,0,0,0,0,0,1,1},
        {1,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,1},
        {1,1,1,1,1,1,1,1,1,1}}}},

    {{8},{{{0,0,0,0,1,1,0,0,0,0},
        {0,0,0,1,1,1,1,0,0,0},
        {0,0,1,1,0,0,1,1,0,0},
        {0,1,1,0,0,0,0,1,1,0},
        {1,1,0,0,0,0,0,0,1,1},
        {1,1,0,0,0,0,0,0,1,1},
        {0,1,1,0,0,0,0,1,1,0},
        {0,0,1,1,0,0,1,1,0,0},
        {0,0,0,1,1,1,1,0,0,0},
        {0,0,0,0,1,1,0,0,0,0}}}},

    {{9},{{{0,0,1,1,1,1,1,1,0,0},
        {0,1,0,0,0,0,0,0,1,0},
        {1,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,1},
        {0,1,0,0,0,0,0,0,1,0},
        {0,0,1,1,1,1,1,1,0,0}}}},

    {{10},{{{0,0,0,0,0,0,0,0,0,1},
        {0,0,0,0,0,0,0,0,1,0},
        {0,0,1,0,0,0,0,1,0,0},
        {0,0,0,1,0,0,1,0,0,0},
        {0,0,0,0,1,1,0,0,0,0},
        {0,0,0,0,1,1,0,0,0,0},
        {0,0,0,1,0,0,1,0,0,0},
        {0,0,1,0,0,0,0,1,0,0},
        {0,1,0,0,0,0,0,0,0,0},
        {1,0,0,0,0,0,0,0,0,0}}}},

    {{11},{{{0,0,0,0,1,1,0,0,0,0},
        {0,0,0,1,1,1,1,0,0,0},
        {0,0,0,1,0,0,1,0,0,0},
        {0,0,1,1,0,0,1,1,0,0},
        {0,0,1,0,0,0,0,1,0,0},
        {0,1,1,0,0,0,0,1,1,0},
        {0,1,0,0,0,0,0,0,1,0},
        {1,0,0,0,0,0,0,0,0,1},
        {1,1,0,0,0,0,0,0,1,1},
        {1,1,1,1,1,1,1,1,1,1}}}},

    {{12},{{{0,0,0,1,1,1,1,0,0,0},
        {0,0,1,1,0,0,1,1,0,0},
        {0,1,1,0,0,0,0,1,1,0},
        {1,1,0,0,0,0,0,0,1,1},
        {1,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,1},
        {1,1,0,0,0,0,0,0,1,1},
        {0,1,1,0,0,0,0,1,1,0},
        {0,0,1,1,0,0,1,1,0,0},
        {0,0,0,1,1,1,1,0,0,0}}}},

    {{13},{{{0,0,1,1,1,1,1,1,0,0},
        {0,1,0,1,1,1,1,0,1,0},
        {1,0,1,0,0,0,0,0,0,1},
        {1,1,0,0,0,0,0,0,1,1},
        {1,1,0,0,0,0,0,0,1,1},
        {1,1,0,0,0,0,0,0,1,1},
        {1,1,0,0,0,0,0,0,1,1},
        {1,0,1,0,0,0,0,1,0,1},
        {0,1,0,1,1,1,1,0,1,0},
        {0,0,1,1,1,1,1,1,0,0}}}},

    {{14},{{{1,0,0,0,0,0,0,0,0,1},
        {0,1,0,0,0,0,0,0,1,0},
        {0,0,1,0,0,0,0,1,0,0},
        {0,0,0,1,0,0,1,0,0,0},
        {0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0},
        {0,0,0,1,0,0,1,0,0,0},
        {0,0,1,0,0,0,0,1,0,0},
        {0,1,0,0,0,0,0,0,1,0},
        {1,0,0,0,0,0,0,0,0,1}}}},

    {{15},{{{0,0,0,0,1,1,0,0,0,0},
        {0,0,0,0,1,1,0,0,0,0},
        {0,0,0,1,0,0,1,0,0,0},
        {0,0,0,1,0,0,1,0,0,0},
        {0,0,1,0,0,0,0,1,0,0},
        {0,0,1,0,0,0,0,1,0,0},
        {0,1,0,0,0,0,0,0,1,0},
        {0,1,0,0,0,0,0,0,1,0},
        {1,0,0,0,0,0,0,0,0,1},
        {1,1,1,1,1,1,1,1,1,1}}}},

    {{16},{{{0,0,0,1,1,1,0,0,0,0},
        {0,0,1,1,0,1,1,0,0,0},
        {0,1,1,0,0,0,1,1,0,0},
        {1,1,0,0,0,0,0,1,1,0},
        {1,0,0,0,0,0,0,0,0,1},
        {1,1,0,0,0,0,0,0,0,1},
        {0,1,1,0,0,0,0,0,1,1},
        {0,0,1,1,0,0,0,1,1,0},
        {0,0,0,1,1,0,1,1,0,0},
        {0,0,0,0,1,1,1,0,0,0}}}}
    };
};

int main() {
    setlocale(LC_ALL, "Russian");
    Objects objects;
    int integrations = 0; //?????????? ???????????
    float max_distance = 4.6; //???????????? ?????????
    cout << "????????? ?????????: " << endl;
    objects.PrintClusters();
    objects.HierarchicalClustering(integrations, max_distance);
    return 0;
}