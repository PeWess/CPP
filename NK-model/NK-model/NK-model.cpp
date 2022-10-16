#include <iostream>
#include <cmath>
#include <vector>
#include <map>
using namespace std;

//�������, ��������� �� ����� ��� ������������ ����������
void PrintAttractors(map<vector<bool>, int>& repeating_atr_and_lenght) {
	cout << "\nOriginal attractors:\n";
	for (auto& item : repeating_atr_and_lenght) {
		cout << "{";
		for (int a = 0; a < item.first.size(); a++) {
			cout << item.first[a] << ", ";
		}
		cout << '\b' << '\b';
		cout << "}\n";
	}
}

//�������, ������������ �������� �������� ���������� � ��������� ��������� ����������
void ResetToDeafult(int& counter, int& atr_ammount, float& atr_lenght, vector<bool>& start_vector, map<vector<bool>, int>& repeating_atr_and_lenght) {
	counter = 1;
	atr_ammount = 0;
	atr_lenght = 0;
	start_vector.clear();
	repeating_atr_and_lenght.clear();
}

//�������, �������� ��������� ������:
//1) ���� ������ ������, ��������� ��� ������ � ���������� N;
//2) ���� �� ������ ��������, � ��������� ����� ������� - 0, �������� �� �� 1;
//3) ���� �� ��������� ����� ������� - 1, �������� �� �� 0, ��� � ��� ����������� 1, � ��� ������ ������������� 0 �������� ��� �� 1 � ���������� ����.
void StartVectorBuilder(const int& N, vector<bool>& start_vector) {
	if (start_vector.empty() == 1) {
		for (int i = 0; i < N; i++) {
			start_vector.push_back(0);
		}
		return;
	}
	if (start_vector[N - 1] == 0) {
		start_vector[N - 1] = 1;
		return;
	}
	else {
		start_vector[N - 1] = 0;
		for (int j = 2; j <= N; j++) {
			if (start_vector[N - j] == 1) {
				start_vector[N - j] = 0;
			}
			else {
				start_vector[N - j] = 1;
				return;
			}
		}
	}
}

//�������, �������� ������� NK-��������, � ������ ����������, ���������� ��������������� ����������� � �� �����.
void AttractorFinder(const int& N, const int& K, int& atr_ammount, float& atr_lenght, map<int, vector<string>>& commands, vector<bool>& start_vector, map<vector<bool>, int>& repeating_atr_and_lenght, map<int, vector<int>>& matrix_of_relations) {
	map<vector<bool>, int> machine_state; //����� ��������� ��������
	int current_stroke = 1; //��������� ����� � ��������
	bool atr_fnd = 0; //���������� ��� ����������� ������ �����, ���� ������ ���������
	bool result; //��������� ���������� ��������
	vector<bool> current_vector; //��������� ������
	vector<bool> prev_vector; //���������� ������
	vector<int> tmp; //������������ ����� ����������
	vector<string> tmp_commands; //������������ ����� ���������� ��������

	prev_vector = start_vector;
	machine_state[prev_vector] = 0;
	//���� ��������, ���� �� ����� ������ ���������:
	//�� ���������� ��������� �� ������� ��������� (��������� � ��� ����������) � ��������������� ���������� ��������;
	//��������� ���������� � result � ��������� � current_vector
	//����� ���������, ���������� �� ���� ������ �� ��� ���� ������ ������� ��������.
	//���� ��, � � ������ (repeating_atr_and_length) ����� ��������� ����������� �� ����, �� �� ������ ������ ��������� �������� ��� �����
	//���� �� ��� � ������ ���, �� ������ ��������� � ���� ��������� ������ � ������, 
	//� ����� ���� ������������ �������� ����� ������� �������, � � �������� ������������ �������� ������������ 1
	//���� �� ������ ������ �� ���������� �� ��� ������ ������� ��������, �� ���� �����������.
	while (atr_fnd == 0) {
		for (int i = 0; i < N; i++) {
			tmp_commands = commands[i];
			tmp = matrix_of_relations[i];
			result = prev_vector[tmp[0]];
			for (int j = 1; j < K; j++) {
				if (tmp_commands[j - 1] == "AND") {
					result = result && prev_vector[tmp[j]];
				}
				else if (tmp_commands[j - 1] == "OR") {
					result = result || prev_vector[tmp[j]];
				}
				else if (tmp_commands[j - 1] == "->") {
					result = !result || prev_vector[tmp[j]];
				}
				else if (tmp_commands[j - 1] == "<->") {
					result = result == prev_vector[tmp[j]];
				}
			}
			current_vector.push_back(result);
		}
		if (machine_state.count(current_vector) == 1) {
			if (repeating_atr_and_lenght.count(current_vector) == 1) {
				atr_lenght += repeating_atr_and_lenght[current_vector];
				atr_fnd = 1;
			}
			else {
				repeating_atr_and_lenght[current_vector] = current_stroke - machine_state[current_vector];
				atr_lenght += current_stroke - machine_state[current_vector];
				atr_ammount += 1;
				atr_fnd = 1;
			}
		}
		else {
			machine_state[current_vector] = current_stroke;
			current_stroke += 1;
		}
		prev_vector = current_vector;
		current_vector.clear();
	}
	machine_state.clear();
}

int main() {
	int counter = 1, N, K; //������� ��� ������������ ���������� ���� ��������� ���������� � �������� N � K
	int atr_ammount = 0; //���������� ������������ �����������
	float atr_lenght = 0; //��������� ����� �����������
	map<int, vector<string>> commands; //����� ������
	map<int, vector<int>> matrix_of_relations; //��������� � ����������� �������� ������ � ������� ������� ������
	map<vector<bool>, int> repeating_atr_and_lenght; //������������� ���������� � �� �����
	vector<bool> start_vector; //��������� ������

	//1)���������� ���������� N � K � ���������� ����������� commands � matrix_of_relations
	//2)����, ������������ ��� ��������� (2^N) �������� ���������� ������� (���� counter �� ����� ������ 2^N)
	//3)���������� ���������� ������� � ������� StartVectorBuilder
	//4)������� ���� ����������� ������ � ������� AttractorFinder � ����� �� �� ����� (�� ���������� �������� PrintAttractors)
	//5)����� ������ ��� ������ ResetToDeafult
	N = 6;
	K = 2;
	cout << "N=6, K=2\n";
	matrix_of_relations = { {0, {2,5}}, {1, {0,4}}, {2, {1,2}}, {3, {1,5}}, {4, {3,5}}, {5, {0,5}} };
	cout << "n1 = {0, 0, 1, 0, 0, 1} \nn2 = {1, 0, 0, 0, 1, 0} \nn3 = {0, 1, 1, 0, 0, 0} \nn4 = {0, 1, 0, 0, 0, 1}"
		"\nn5 = {0, 0, 0, 1, 0, 1} \nn6 = {1, 0, 0, 0, 0, 1}\n";
	commands = { {0, {"AND"}}, {1, {"<->"}}, {2, {"AND"}}, {3, {"->"}}, {4, {"OR"}}, {5, {"OR"}} };
	cout << "1)AND; 2)<->; 3)AND; 4)->; 5)OR; 6)OR\n";
	while (counter <= pow(2, N)) {
		StartVectorBuilder(N, start_vector);
		AttractorFinder(N, K, atr_ammount, atr_lenght, commands, start_vector, repeating_atr_and_lenght, matrix_of_relations);
		counter += 1;
	}
	PrintAttractors(repeating_atr_and_lenght);
	cout << "Average length of attractors = " << (atr_lenght / (counter - 1)) << ", number of original attractors = " << atr_ammount << endl;

	ResetToDeafult(counter, atr_ammount, atr_lenght, start_vector, repeating_atr_and_lenght);
	N = 8;
	K = 3;
	cout << "\nN=8, K=3\n";
	matrix_of_relations = { {0, {0, 2, 4}}, {1, {0, 2, 6}}, {2, {1, 3, 5}}, {3, {1, 3, 7}}, {4, {1, 2, 4}},
	{5, {0, 3, 7}}, {6, {0, 1, 2}}, {7, {5, 6, 7}} };
	cout << "n1 = {1, 0, 1, 0, 1, 0, 0, 0} \nn2 = {1, 0, 1, 0, 0, 0, 1, 0} \nn3 = {0, 1, 0, 1, 0, 1, 0, 0} \nn4 = {0, 1, 0, 1, 0, 0, 0, 1}"
		"\nn5 = {0, 1, 1, 0, 1, 0, 0, 0} \nn6 = {1, 0, 0, 1, 0, 0, 0, 1} \nn7 = {1, 1, 1, 0, 0, 0, 0, 0} \nn8 = {0, 0, 0, 0, 0, 1, 1, 1}\n";
	commands = { {0, {"AND", "<->"}}, {1, {"<->", "OR"}}, {2, {"AND", "OR"}}, {3, {"->", "AND"}}, {4, {"OR", "<->"}}, {5, {"OR", "->"}}, {6, {"->", "<->"}}, {7, {"OR", "AND"}} };
	cout << "1)AND, <->; 2)<->, OR; 3)AND, OR; 4)->, AND; 5)OR, <->; 6)OR, ->; 7)->, <->; 8)OR, AND\n";
	while (counter <= pow(2, N)) {
		StartVectorBuilder(N, start_vector);
		AttractorFinder(N, K, atr_ammount, atr_lenght, commands, start_vector, repeating_atr_and_lenght, matrix_of_relations);
		counter += 1;
	}
	PrintAttractors(repeating_atr_and_lenght);
	cout << "Average length of attractors = " << (atr_lenght / (counter - 1)) << ", number of original attractors = " << atr_ammount << endl;

	ResetToDeafult(counter, atr_ammount, atr_lenght, start_vector, repeating_atr_and_lenght);
	N = 9;
	K = 5;
	cout << "\nN=9, K=5\n";
	matrix_of_relations = { {0, {1, 4, 6, 7, 8}}, {1, {0, 2, 5, 6, 7}}, {2, {3, 5, 6, 7, 8}}, {3, {2, 3, 5, 6, 8}}, {4, {0, 1, 3, 5, 7}},
	{5, {1, 3, 5, 7, 8}}, {6, {0, 2, 4, 6, 8}}, {7, {0, 1, 2, 3, 4}}, {8, {4, 5, 6, 7, 8}} };
	cout << "n1 = {0, 1, 0, 0, 1, 0, 1, 1, 1} \nn2 = {1, 0, 1, 0, 0, 1, 1, 1, 0} \nn3 = {0, 0, 0, 1, 0, 1, 1, 1, 1} \nn4 = {0, 0, 1, 1, 0, 1, 1, 0, 1}"
		"\nn5 = {1, 1, 0, 1, 0, 1, 0, 1, 0} \nn6 = {0, 1, 0, 1, 0, 1, 0, 1, 1} \nn7 = {1, 0, 1, 0, 1, 0, 1, 0, 1} \nn8 = {1, 1, 1, 1, 1, 0, 0, 0, 0}"
		"\nn9 = {0, 0, 0, 0, 1, 1, 1, 1, 1}\n";
	commands = { {0, {"AND", "<->", "OR", "->"}}, {1, {"<->", "OR", "->", "AND"}}, {2, {"AND", "OR", "->", "<->"}}, {3, {"->", "AND", "<->", "OR"}}, {4, {"OR", "<->", "->", "AND"}},
	 {5, {"OR", "->", "<->", "AND"}}, {6, {"->", "<->", "OR", "AND"}}, {7, {"OR", "AND", "<->", "->"}}, {8, {"AND", "->", "<->", "OR"}} };
	cout << "1)AND, <->, OR, ->; 2)<->, OR, ->, AND; 3)AND, OR, ->, <->; 4)->, AND, <->, OR; 5)OR, <->, ->, AND; 6)OR, ->, <->, AND; 7)->, <->, OR, AND; 8)OR, AND, <->, ->; 9)AND, ->, <->, OR\n";
	while (counter <= pow(2, N)) {
		StartVectorBuilder(N, start_vector);
		AttractorFinder(N, K, atr_ammount, atr_lenght, commands, start_vector, repeating_atr_and_lenght, matrix_of_relations);
		counter += 1;
	}
	PrintAttractors(repeating_atr_and_lenght);
	cout << "Average length of attractors = " << (atr_lenght / (counter - 1)) << ", number of original attractors = " << atr_ammount << endl;
	return 0;
}
