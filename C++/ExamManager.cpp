#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iomanip>
#include <thread>
#include <chrono>

using namespace std;
const unsigned Inf = 1e9;

int input(bool score) {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	string str, error;
	cin >> str;
	error = "���͂��s���ł��B";

	if (str.size() > 8) { //8�����𒴂���Ȃ�Error�Ƃ���
		cout << error << endl;
		return Inf;
	}

	for (unsigned i = 0; i < str.size(); i++) {

		if (str.size() != 1 && i == 0) { //���͂�1�����łȂ�����1�����ڂ̂Ƃ�
			if (score) { //���_���͎�
				if ((str.at(i) < '0' || str.at(i) > '9') && str.at(i) != '+' && str.at(i) != '-') { //�����ł����ł��Ȃ��Ȃ�Error�Ƃ���
					cout << error << endl;
					return Inf;
				}
			}
			else { //�l�����͎�
				if ((str.at(i) < '0' || str.at(i) > '9') && str.at(i) != '+') { //�������ł����ł��Ȃ��Ȃ�Error�Ƃ���
					cout << error << endl;
					return Inf;
				}
			}
		}

		else { //2�����ڈȍ~���邢�͓��͂�1�����̂Ƃ�
			if (str.at(i) < '0' || str.at(i) > '9') {
				cout << error << endl;
				return Inf;
			}
		}
	}

	int num = stoi(str);

	if (score) { //���_���͎�
		if (abs(num) > 1e6) { //�}100���͈̔͊O�Ȃ�Error�Ƃ���
			cout << error << endl;
			return Inf;
		}
	}
	else { //�l�����͎�
		if (num < 1 || num > 1e6) { //1�`100���͈̔͊O�Ȃ�Error�Ƃ���
			cout << error << endl;
			return Inf;
		}
	}

	return num;
}

struct examine {
	int id;
	int score;
	double t;
	int rank;
};

double originalcalc(double input) { //������1�ʂ܂ł��ۂ߂�
	double output;

	output = input * 10;
	output = round(output);
	output /= 10;

	return output;
}

string eliminate(string input) { //������2�ʈȍ~��r������
	bool ex = false;
	string output;
	output.clear();

	for (unsigned i = 0; i < input.size(); i++) {
		if (input.at(i) == '.') {
			output += input.at(i);
			ex = true; //�����_���o����Ex��True��
		}
		else if (ex) {
			output += input.at(i);
			break;
		}
		else {
			output += input.at(i);
		}
	}
	return output;
}

string addspace(string input) {
	string output;
	output.clear();

	if (input.size() > 10) return input; //10�����𒴂���Ȃ�Input�����̂܂ܕԂ�

	unsigned s = 10 - input.size(); //���߂�ׂ��󔒂̐�

	for (unsigned i = 0; i < s; i++) {
		output += ' ';
	}

	output += input;

	return output;
}

bool comparescore(const examine& a, const examine& b) {
	return a.score > b.score; //�X�R�A�̑傫�����̂�����ׂ�
}

bool compareid(const examine& a, const examine& b) {
	return a.id < b.id; //���͂��ꂽ���̂�����ׂ�
}

int main(void) {

	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	string strfirst, strwaitforexam, strwaitforscore, strinputted;

	strfirst = "�󌱎Ґ��Ɠ��_����A�󌱎҂̕΍��l�Ə��ʂ�Ԃ��܂��B���_�����������ꍇ�A���������ʂ������Ȃ����Ƃ�����܂��B";
	strwaitforexam = "�󌱎Ґ��i100���܂ł̎��R���j����͂��Ă��������B";
	strwaitforscore = "���_�i�}100���܂ł̐����j����͂��Ă��������B";
	strinputted = "���������͂���܂����B";

	cout << strfirst << endl;
	cout << endl;
	cout << strwaitforexam << endl;

	int exam;

	do {
		exam = input(false);
	} while (exam == Inf);

	cout << strinputted << endl;
	cout << endl;
	cout << strwaitforscore << endl;

	vector<examine> result(exam);

	int maxi, mini, avg1;
	long long sum, gap, variance;
	double avg2, sd;

	sum = 0, variance = 0, maxi = -1e6 - 1, mini = 1e6 + 1; //�͈͂́}10^6�܂łł��邽�߁AMax�ɂ͔͈͊O�̏����Ȑ����AMin�ɂ͔͈͊O�̑傫�Ȑ�������

	for (unsigned i = 0; i < exam; i++) {
		result.at(i).id = i + 1;
		
		do {
			result.at(i).score = input(true); //�����̓��͂�OK
		} while (result.at(i).score == Inf);

		cout << strinputted << '(' << result.at(i).id << ')' << endl;
		cout << endl;

		sum += result.at(i).score;
		mini = min(mini, result.at(i).score);
		maxi = max(maxi, result.at(i).score);
	}

	avg2 = sum; //Avg�ϐ���Sum��n��
	avg2 /= exam; //���ϓ_�����߂�

	avg1 = round(avg2);
	avg2 = originalcalc(avg2);

	for (unsigned i = 0; i < exam; i++) {
		gap = result.at(i).score - avg1;

		variance += gap * gap; //Variance�ϐ��ɓ��_�ƕ��ϓ_�̍���2��𑫂�
	}

	variance /= exam; //���U�����߂�
	sd = originalcalc(sqrt((double)variance));

	sort(result.begin(), result.end(), comparescore); //���_�̑傫�����̂�����ׂ�

	for (unsigned i = 0; i < exam; i++) {
		if (i != 0 && result.at(i).score == result.at(i - 1).score) {
			result.at(i).rank = result.at(i - 1).rank; //Draw
		}
		else {
			result.at(i).rank = i + 1;
		}

		if (sd) {
			result.at(i).t = ((double)(result.at(i).score) - avg2) * 10 / sd + 50;
		}
		else {
			result.at(i).t = 50; //�[�����Z��h��
		}

	}

	sort(result.begin(), result.end(), compareid); //���͂��ꂽ���̂�����ׂ�

	cout << "---------------------------------------------" << endl;
	cout << "|        ID|      ���_|    �΍��l|      ����|" << endl;
	cout << "---------------------------------------------" << endl;

	string strid, strscore, strt, strrank;

	for (unsigned i = 0; i < exam; i++) {

		strid = to_string(result.at(i).id);
		strscore = to_string(result.at(i).score);
		strt = to_string(result.at(i).t);
		strrank = to_string(result.at(i).rank);

		strt = eliminate(strt); //������2�ʖ�����r������

		strid = addspace(strid);
		strscore = addspace(strscore);
		strt = addspace(strt);
		strrank = addspace(strrank);

		cout << '|' << strid << '|' << strscore << '|' << strt << '|' << strrank << '|' << endl;

		cout << "---------------------------------------------" << endl;
	}

	cout << endl;

	cout << "����:" << fixed << setprecision(1) << avg2 << ' ' << "�ő�:" << mini << ' ' << "�ŏ�:" << maxi << endl; //������1�ʂ܂ŏo��
	cout << "�W���΍�:" << fixed << setprecision(1) << sd << endl;

	cout << endl;

	cout << "30�b��ɏI��" << endl;
	this_thread::sleep_for(std::chrono::seconds(30));

	return 0;

}