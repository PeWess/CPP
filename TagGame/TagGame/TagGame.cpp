#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <map>
using namespace std;

//В данном классе хранится информация о том, какая матрица на какой глубине лежит, а также, какой ход был сделан при данной матрице.
//AddMatrixToDeep - добавляет на указанную глубину матрицу.
//AddCommand - добавляет к списку ходов при данной матрице еще один вариант хода.
//AddDeepToMatrix - добавляет информацию о том, на какой глкбине находится данная матрица.
//UsedCommand - проверяет, совершался ли уже указанный ход при данной матрице. 
//UsedMatrix - проверяет, получалась ли ранее такая матрица.
//ReturnToPrevMatrix - возвращается к предыдущей по глубине матрице.

class Tree {
public:
    void AddMatrixToDeep(vector<vector<int>>& initial_matrix, int& deep, int& x, int& y) {
        deep_and_matrix[deep] = initial_matrix;

    }

    void AddCommand(vector<vector<int>>& initial_matrix, string& command) {
        matrix_and_commands[initial_matrix].push_back(command);
    }

    void AddDeepToMatrix(vector<vector<int>>& initial_matrix, int& deep) {
        matrix_and_deep[initial_matrix] = deep;
    }

    bool UsedCommand(vector<vector<int>>& initial_matrix, string& command) {
        if (matrix_and_commands.count(initial_matrix) != 0) {
            int counter = matrix_and_commands.count(initial_matrix);
            for (int i = 0; i < counter; i++) {
                if (matrix_and_commands[initial_matrix][i] == command) return true;
            }
            return false;
        }
    }

    bool UsedMatrix(vector<vector<int>>& initial_matrix) {
        if (matrix_and_deep.count(initial_matrix) == 0) {
            return false;
        }
        return true;
    }

    void ReturnToPrevMatrix(vector<vector<int>>& initial_matrix, int& deep) {
        deep -= 1;
        initial_matrix = deep_and_matrix[deep];
    }

private:
    map<int, vector<vector<int>>> deep_and_matrix;
    map<vector<vector<int>>, int> matrix_and_deep;
    map<vector<vector<int>>, vector<string>> matrix_and_commands;
};


//Функция для нахождения координат нуля в матрице
void FindZero(vector<vector<int>>& initial_matrix, int& x, int& y, int& x_cord, int& y_cord) {
    for (int i = 0; i < y; i++) {
        for (int j = 0; j < x; j++) {
            if (initial_matrix[i][j] == 0) {
                y_cord = i;
                x_cord = j;
                break;
            }
        }
    }
}

//Нуль и значение ячейки над нулем меняются местами, нуль перемещается на одну ячейку вверх.
void UpRule(vector<vector<int>>& initial_matrix, int& x_cord, int& y_cord) {
    int tmp = initial_matrix[y_cord - 1][x_cord];
    initial_matrix[y_cord - 1][x_cord] = 0;
    initial_matrix[y_cord][x_cord] = tmp;
}

//Нуль и значение ячейки под нулем меняются местами, нуль перемещается на одну ячейку вниз.
void DownRule(vector<vector<int>>& initial_matrix, int& x_cord, int& y_cord) {
    int tmp = initial_matrix[y_cord + 1][x_cord];
    initial_matrix[y_cord + 1][x_cord] = 0;
    initial_matrix[y_cord][x_cord] = tmp;
}

//Нуль и значение ячейки слева от нуля меняются местами, нуль перемещается на одну ячейку влево.
void LeftRule(vector<vector<int>>& initial_matrix, int& x_cord, int& y_cord) {
    int tmp = initial_matrix[y_cord][x_cord - 1];
    initial_matrix[y_cord][x_cord - 1] = 0;
    initial_matrix[y_cord][x_cord] = tmp;
}

//Нуль и значение ячейки справа от нуля меняются местами, нуль перемещается на одну ячейку вправо.
void RightRule(vector<vector<int>>& initial_matrix, int& x_cord, int& y_cord) {
    int tmp = initial_matrix[y_cord][x_cord + 1];
    initial_matrix[y_cord][x_cord + 1] = 0;
    initial_matrix[y_cord][x_cord] = tmp;
}

//Функция, считающая количество ячеек не на своих местах.
int DistanceCounter(vector<vector<int>>& initial_matrix, map<vector<int>, int>& coordinates_and_nums, int& x, int& y) {
    int distance = 0;
    for (int i = 0; i < y; i++) {
        for (int j = 0; j < x; j++) {
            if (initial_matrix[i][j] != coordinates_and_nums[{i, j}]) {
                distance += 1;
            }
        }
    }
    return distance;
}

//Эвристика, в которой учитываются:
//1)Манхэттенское расстояние (сумма расстояний для каждой ячейки до ее целевой).
//2)Линейный конфликт по рядам (если две ячейки стоят не на своих местах, но находятся в одном ряду, при этом одна из ячеек находится правее другой, но в целевой матрице должно быть наоборот, то Манхэттенскому расстоянию прибавляется 2)
//3)Линей конфликт по столбцам (То же самое, только по столбцам).
//4)Ко всему прибавляется и количество ячеек не на своих местах (не считая нуля).
int Heuristic(vector<vector<int>>& initial_matrix, map<int, vector<int>>& nums_and_coordinates, map<vector<int>, int>& coordinates_and_nums, int& x, int& y) {
    int result = 0;
    for (int i = 0; i < y; i++) {
        for (int j = 0; j < x; j++) {
            if (initial_matrix[i][j] != coordinates_and_nums[{i, j}] && initial_matrix[i][j] != 0) {
                result += abs(i - nums_and_coordinates[initial_matrix[i][j]][0]) + abs(j - nums_and_coordinates[initial_matrix[i][j]][1]);
            }
        }
    }
    for (int i = 0; i < y; i++) {
        for (int j = 0; j < x; j++) {
            if (i - nums_and_coordinates[initial_matrix[i][j]][0] == 0 && j - nums_and_coordinates[initial_matrix[i][j]][1] != 0 && initial_matrix[i][j] != 0) {
                for (int a = j + 1; a < x; a++) {
                    if (nums_and_coordinates[initial_matrix[i][a]][0] == nums_and_coordinates[initial_matrix[i][j]][0] &&
                        nums_and_coordinates[initial_matrix[i][a]][1] < nums_and_coordinates[initial_matrix[i][j]][1] && initial_matrix[i][a] != 0) {
                        result += 2;
                    }
                }
            }
        }
    }
    for (int j = 0; j < x; j++) {
        for (int i = 0; i < y; i++) {
            if (i - nums_and_coordinates[initial_matrix[i][j]][0] != 0 && j - nums_and_coordinates[initial_matrix[i][j]][1] == 0 && initial_matrix[i][j] != 0) {
                for (int b = i + 1; b < y; b++) {
                    if (nums_and_coordinates[initial_matrix[b][j]][1] == nums_and_coordinates[initial_matrix[i][j]][1] &&
                        nums_and_coordinates[initial_matrix[b][j]][0] < nums_and_coordinates[initial_matrix[i][j]][0] && initial_matrix[b][j] != 0) {
                        result += 2;
                    }
                }
            }
        }
    }
    return result + DistanceCounter(initial_matrix, coordinates_and_nums, x, y) - 1;
}


//Функция, реализующая сборку пятнашек, с примененияем эвристик и учетом предыдущих состояний матрицы.
//Сначала программа проверяет, возможно ли вообще сделать тот или иной ход. Также учитывается, рассматривался ли вообще какой-нибудь вариант хода.
//Если ход совершить возможно, и ранее никакой другой ход не рассматривался, то в переменную command записывается ход, далее проверяется, делался ли уже данный ход при настоящей матрице.
//	Если нет, то данный ход совершается при тестовой матрице с таким же значением.
//	Если полученная после хода матрица ранее не появлялась, то вместо значения настоящей записывается уже новое значение тестовой матрицы, а в переменную для сравнения значений эвристик записывается значение при новой матрице.
//Если ход совершить возможно, но ранее рассматривался другой ход , то в переменную command_for_comparison записывается ход, далее проверяется, делался ли уже данный ход при настоящей матрице
//	Если нет, то данный ход совершается при тестовой матрице с таким же значением.
//	Если полученная после хода матрица ранее не появлялась, и результат эвристики при ней меньше результата при раннее рассматриваемой, 
//	то вместо значения настоящей записывается уже новое значение тестовой матрицы, а в переменную для сравнения значений эвристик записывается новое значение при новой матрице.
void Pyatnashki(Tree& tree, map<vector<int>, int>& coordinates_and_nums, map<int, vector<int>>& nums_and_coordinates,
    vector<vector<int>>& initial_matrix, int& x, int& y, int& x_cord, int& y_cord, int& deep, int& step) {
    vector<vector<int>> test_matrix = initial_matrix;                                                                //Матрица для сравнения, к которой будут применяться различные варианты ходов. Это сделано для вычисления эвристики при каждом варианте хода.
    vector<vector<int>> start_matrix = initial_matrix;   															//Матрица для возвращения значения тестовой матрицы к исходному.
    int heuristic_for_comparison; 																					//Результат применения эвристики для сравнения с результатами при других матрицах.
    string command_for_comparison; 																					//Переменная для сравнения двух разных вариантов хода.
    string command = "No rule"; 																					//Сделанный ход.
    if (y_cord != 0) {
        command = "up";
        if (tree.UsedCommand(test_matrix, command) == false) {
            UpRule(test_matrix, x_cord, y_cord);
            if (tree.UsedMatrix(test_matrix) == false) {
                initial_matrix = test_matrix;
                heuristic_for_comparison = Heuristic(initial_matrix, nums_and_coordinates, coordinates_and_nums, x, y);
            }
            else command = "No rule";
        }
        else command = "No rule";
    }
    if (y_cord != (y - 1)) {
        test_matrix = start_matrix;
        if (command == "No rule") {
            command = "down";
            if (tree.UsedCommand(test_matrix, command) == false) {
                DownRule(test_matrix, x_cord, y_cord);
                if (tree.UsedMatrix(test_matrix) == false) {
                    initial_matrix = test_matrix;
                    heuristic_for_comparison = Heuristic(initial_matrix, nums_and_coordinates, coordinates_and_nums, x, y);
                }
                else command = "No rule";
            }
            else command = "No rule";
        }
        else {
            command_for_comparison = "down";
            if (tree.UsedCommand(test_matrix, command_for_comparison) == false) {
                DownRule(test_matrix, x_cord, y_cord);
                if (tree.UsedMatrix(test_matrix) == false && heuristic_for_comparison > Heuristic(test_matrix, nums_and_coordinates, coordinates_and_nums, x, y)) {
                    initial_matrix = test_matrix;
                    heuristic_for_comparison = Heuristic(test_matrix, nums_and_coordinates, coordinates_and_nums, x, y);
                    command = command_for_comparison;
                }
            }
        }
    }
    if (x_cord != 0) {
        test_matrix = start_matrix;
        if (command == "No rule") {
            command = "left";
            if (tree.UsedCommand(test_matrix, command) == false) {
                LeftRule(test_matrix, x_cord, y_cord);
                if (tree.UsedMatrix(test_matrix) == false) {
                    initial_matrix = test_matrix;
                    heuristic_for_comparison = Heuristic(initial_matrix, nums_and_coordinates, coordinates_and_nums, x, y);
                }
                else command = "No rule";
            }
            else command = "No rule";
        }
        else {
            command_for_comparison = "left";
            if (tree.UsedCommand(test_matrix, command_for_comparison) == false) {
                LeftRule(test_matrix, x_cord, y_cord);
                if (tree.UsedMatrix(test_matrix) == false && heuristic_for_comparison > Heuristic(test_matrix, nums_and_coordinates, coordinates_and_nums, x, y)) {
                    initial_matrix = test_matrix;
                    heuristic_for_comparison = Heuristic(test_matrix, nums_and_coordinates, coordinates_and_nums, x, y);
                    command = command_for_comparison;
                }
            }
        }
    }
    if (x_cord != (x - 1)) {
        test_matrix = start_matrix;
        if (command == "No rule") {
            command = "right";
            if (tree.UsedCommand(test_matrix, command) == false) {
                RightRule(test_matrix, x_cord, y_cord);
                if (tree.UsedMatrix(test_matrix) == false) {
                    initial_matrix = test_matrix;
                    heuristic_for_comparison = Heuristic(initial_matrix, nums_and_coordinates, coordinates_and_nums, x, y);
                }
                else command = "No rule";
            }
            else command = "No rule";
        }
        else {
            command_for_comparison = "right";
            if (tree.UsedCommand(test_matrix, command_for_comparison) == false) {
                RightRule(test_matrix, x_cord, y_cord);
                if (tree.UsedMatrix(test_matrix) == false && heuristic_for_comparison > Heuristic(test_matrix, nums_and_coordinates, coordinates_and_nums, x, y)) {
                    initial_matrix = test_matrix;
                    heuristic_for_comparison = Heuristic(test_matrix, nums_and_coordinates, coordinates_and_nums, x, y);
                    command = command_for_comparison;
                }
            }
        }
    }

    //Если во время работы функции был сделан ход, то высчитывается новая дистанция, значения шага и глубины увеличиваются на 1, вся информация о текущем положении выводится на экран. Если хода не было, берется предыдущая матрица. 
    //Если дистанция равна нулю, выводится сообщение о том, что исходная матрица найдена и функция прекращает свою работу.
    //Если дистанция не равна нулю, то записывается, какая глубина соответствует данной матрице, какой ход был совершен при предыдущей матрице, высчитываются новые координаты нуля и снова вызывается данная функция.
    if (command != "No rule") {
        int dis = DistanceCounter(initial_matrix, coordinates_and_nums, x, y);
        step++;
        deep++;
        cout << "Step: " << step << " Deep: " << deep << " Distance: " << dis << " Matrix: {";
        for (int i = 0; i < y; i++) {
            for (int j = 0; j < x; j++) {
                cout << initial_matrix[i][j] << ", ";
            }
        }
        cout << "\b" << "\b" << "}" << endl;
        if (dis == 0) {
            cout << "Matrix is found!: " << endl;
            for (int i = 0; i < y; i++) {
                for (int j = 0; j < x; j++) {
                    cout << initial_matrix[i][j] << " ";
                }
                cout << endl;
            }
        }
        else {
            tree.AddDeepToMatrix(initial_matrix, deep);
            tree.AddMatrixToDeep(initial_matrix, deep, x, y);
            tree.AddCommand(start_matrix, command);
            FindZero(initial_matrix, x, y, x_cord, y_cord);
            Pyatnashki(tree, coordinates_and_nums, nums_and_coordinates, initial_matrix, x, y, x_cord, y_cord, deep, step);
        }
    }
    else {
        tree.ReturnToPrevMatrix(initial_matrix, deep);
        FindZero(initial_matrix, x, y, x_cord, y_cord);
        Pyatnashki(tree, coordinates_and_nums, nums_and_coordinates, initial_matrix, x, y, x_cord, y_cord, deep, step);
    }
}


int main() {
    Tree tree;
    int x = 4;
    int y = 4;
    int x_cord, y_cord; //Координаты нуля
    int deep = 0, step = 0;
    map<vector<int>, int> coordinates_and_nums = { {{0, 0}, 1}, {{0, 1}, 2}, {{0, 2}, 3}, {{0, 3}, 4},      //Каким координатам какое значение ячейки соответствует в целевой матрице
    {{1, 0}, 5}, {{1, 1}, 6}, {{1, 2}, 7}, {{1, 3}, 8},
    {{2, 0}, 9}, {{2, 1}, 10}, {{2, 2}, 11}, {{2, 3}, 12},
    {{3, 0}, 13}, {{3, 1}, 14}, {{3, 2}, 15}, {{3, 3}, 0} };
    map<int, vector<int>> nums_and_coordinates = { {1, {0, 0}}, {2, {0, 1}}, {3, {0, 2}}, {4, {0, 3}},      //Каким значениям ячеек какие координаты соответствуют в целевой матрице
    {5, {1, 0}}, {6, {1, 1}}, {7, {1, 2}}, {8, {1, 3}},
    {9, {2, 0}}, {10, {2, 1}}, {11, {2, 2}}, {12, {2, 3}},
    {13, {3, 0}}, {14, {3, 1}}, {15, {3, 2}}, {0, {3, 3}} };
    vector<vector<int>> initial_matrix = { {8, 14, 11, 4}, {2, 13, 0, 15}, {9, 1, 12, 5}, {10, 3, 6, 7} };   //Начальная матрица
    FindZero(initial_matrix, x, y, x_cord, y_cord);
    Pyatnashki(tree, coordinates_and_nums, nums_and_coordinates, initial_matrix, x, y, x_cord, y_cord, deep, step);
    return 0;
}
