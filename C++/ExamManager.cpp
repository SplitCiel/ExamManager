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
	error = "入力が不正です。";

	if (str.size() > 8) { //8文字を超えるならErrorとする
		cout << error << endl;
		return Inf;
	}

	for (unsigned i = 0; i < str.size(); i++) {

		if (str.size() != 1 && i == 0) { //入力が1文字でないかつ1文字目のとき
			if (score) { //得点入力時
				if ((str.at(i) < '0' || str.at(i) > '9') && str.at(i) != '+' && str.at(i) != '-') { //符号でも数でもないならErrorとする
					cout << error << endl;
					return Inf;
				}
			}
			else { //人数入力時
				if ((str.at(i) < '0' || str.at(i) > '9') && str.at(i) != '+') { //正符号でも数でもないならErrorとする
					cout << error << endl;
					return Inf;
				}
			}
		}

		else { //2文字目以降あるいは入力が1文字のとき
			if (str.at(i) < '0' || str.at(i) > '9') {
				cout << error << endl;
				return Inf;
			}
		}
	}

	int num = stoi(str);

	if (score) { //得点入力時
		if (abs(num) > 1e6) { //±100万の範囲外ならErrorとする
			cout << error << endl;
			return Inf;
		}
	}
	else { //人数入力時
		if (num < 1 || num > 1e6) { //1～100万の範囲外ならErrorとする
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

double originalcalc(double input) { //小数第1位までを丸める
	double output;

	output = input * 10;
	output = round(output);
	output /= 10;

	return output;
}

string eliminate(string input) { //小数第2位以降を排除する
	bool ex = false;
	string output;
	output.clear();

	for (unsigned i = 0; i < input.size(); i++) {
		if (input.at(i) == '.') {
			output += input.at(i);
			ex = true; //小数点が出たらExがTrueに
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

	if (input.size() > 10) return input; //10文字を超えるならInputをそのまま返す

	unsigned s = 10 - input.size(); //埋めるべき空白の数

	for (unsigned i = 0; i < s; i++) {
		output += ' ';
	}

	output += input;

	return output;
}

bool comparescore(const examine& a, const examine& b) {
	return a.score > b.score; //スコアの大きいものから並べる
}

bool compareid(const examine& a, const examine& b) {
	return a.id < b.id; //入力されたものから並べる
}

int main(void) {

	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	string strfirst, strwaitforexam, strwaitforscore, strinputted;

	strfirst = "受験者数と得点から、受験者の偏差値と順位を返します。得点差が小さい場合、正しい結果が得られないことがあります。";
	strwaitforexam = "受験者数（100万までの自然数）を入力してください。";
	strwaitforscore = "得点（±100万までの整数）を入力してください。";
	strinputted = "正しく入力されました。";

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

	sum = 0, variance = 0, maxi = -1e6 - 1, mini = 1e6 + 1; //範囲は±10^6までであるため、Maxには範囲外の小さな数を、Minには範囲外の大きな数を入れる

	for (unsigned i = 0; i < exam; i++) {
		result.at(i).id = i + 1;
		
		do {
			result.at(i).score = input(true); //符号の入力もOK
		} while (result.at(i).score == Inf);

		cout << strinputted << '(' << result.at(i).id << ')' << endl;
		cout << endl;

		sum += result.at(i).score;
		mini = min(mini, result.at(i).score);
		maxi = max(maxi, result.at(i).score);
	}

	avg2 = sum; //Avg変数にSumを渡す
	avg2 /= exam; //平均点を求める

	avg1 = round(avg2);
	avg2 = originalcalc(avg2);

	for (unsigned i = 0; i < exam; i++) {
		gap = result.at(i).score - avg1;

		variance += gap * gap; //Variance変数に得点と平均点の差の2乗を足す
	}

	variance /= exam; //分散を求める
	sd = originalcalc(sqrt((double)variance));

	sort(result.begin(), result.end(), comparescore); //得点の大きいものから並べる

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
			result.at(i).t = 50; //ゼロ除算を防ぐ
		}

	}

	sort(result.begin(), result.end(), compareid); //入力されたものから並べる

	cout << "---------------------------------------------" << endl;
	cout << "|        ID|      得点|    偏差値|      順位|" << endl;
	cout << "---------------------------------------------" << endl;

	string strid, strscore, strt, strrank;

	for (unsigned i = 0; i < exam; i++) {

		strid = to_string(result.at(i).id);
		strscore = to_string(result.at(i).score);
		strt = to_string(result.at(i).t);
		strrank = to_string(result.at(i).rank);

		strt = eliminate(strt); //小数第2位未満を排除する

		strid = addspace(strid);
		strscore = addspace(strscore);
		strt = addspace(strt);
		strrank = addspace(strrank);

		cout << '|' << strid << '|' << strscore << '|' << strt << '|' << strrank << '|' << endl;

		cout << "---------------------------------------------" << endl;
	}

	cout << endl;

	cout << "平均:" << fixed << setprecision(1) << avg2 << ' ' << "最大:" << mini << ' ' << "最小:" << maxi << endl; //小数第1位まで出力
	cout << "標準偏差:" << fixed << setprecision(1) << sd << endl;

	cout << endl;

	cout << "30秒後に終了" << endl;
	this_thread::sleep_for(std::chrono::seconds(30));

	return 0;

}