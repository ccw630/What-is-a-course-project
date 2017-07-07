#include<fstream>
#include<iostream>
#include<string>
#include<algorithm>
#include<map>
#include<vector>
#include<set>
#include<iomanip>
#include<sstream>
#include<conio.h>
using namespace std;
class Student {
private:
	string id, name;
	map<string, double>score;
public:
	Student() {};
	Student(string a, string b, map<string, double>c) :id(a), name(b), score(c) {}
	string getId() { return id; }
	string getName() { return name; }
	map<string, double>getScore() { return score; }
	double getScore(string s) { return score[s]; }
	int passSubject(string s) {	//passSubject -1:未选 0:挂 1:过
		if (!score.count(s))return -1;
		if (score[s] >= 60)return 1;
		return 0;
	}
	double gpa() {//绩点计算
		if (!score.size())return 0;
		double r = 0;
		for (map<string, double>::iterator i = score.begin(); i != score.end(); i++)
			if (passSubject(i->first)>0)r += (i->second - 50) / 10;
		return r / score.size();
	}

};
class Class {
private:
	vector<Student>st;
	int fileEnd = 0;
public:
	vector<Student>all() { return st; }
	void insertStudent(Student s) {
		vector<int>t = findStudentKey(s.getId());
		if (t.size() == 0) st.push_back(s);
		else if (st[t[0]].getName() == s.getName()) {
			map<string, double>m(st[t[0]].getScore()), sm(s.getScore());
			if (sm.size()>0)//防止map为空
				for (map<string, double>::iterator i = sm.begin(); i != sm.end(); i++) {
					m[i->first] = i->second;
				}
			st[t[0]] = Student(s.getId(), s.getName(), m);
		}
		else {
			cerr << "学号已存在!" << endl;
			system("pause");
		}
	}
	void getFileData() {
		ifstream f("data.txt", ios::in);
		if (!f) {
			cerr << "无此文件" << endl;
			system("pause");
			ofstream ff("data.txt", ios::trunc);//文件不存在则创建
			return;
		}
		string s1, s2;
		map<string, double>m;
		while (f >> s1 >> s2) {
			m.clear();
			string s; double d;
			while (f >> s >> d) {
				if (s == "end"&&d<0)break;
				m[s] = d;
			}
			insertStudent(Student(s1, s2, m));
		}
		fileEnd = st.size();
	}
	void saveFileData() {
		ofstream f("data.txt", ios::app);
		for (int i = fileEnd; i < st.size(); i++) {
			f << st[i].getId() << ' ' << st[i].getName() << ' ';
			map<string, double>m = st[i].getScore();
			for (map<string, double>::iterator j = m.begin(); j != m.end(); j++)
				f << j->first << ' ' << j->second << ' ';
			f << "end -1" << endl;//以end -1作为一行结束的标志
		}
	}
	vector<int> findStudentKey(string s) {
		vector<int>r;
		r.clear();
		for (int i = 0; i<st.size(); i++)
			if (st[i].getName() == s || st[i].getId() == s)r.push_back(i);
		return r;
	}
	vector<Student> findStudent(string s) {
		vector<Student>r;
		r.clear();
		for (int i = 0; i<st.size(); i++)
			if (st[i].getName() == s || st[i].getId() == s)r.push_back(st[i]);
		return r;
	}
	void deleteStudent(string s) {
		vector<int>t = findStudentKey(s);
		for (int i = t.size() - 1; i >= 0; i--)
			st.erase(st.begin() + t[i]);
	}
	double scoreAvr(string s) {//科目s平均分计算
		double r = 0; int n = 0;
		for (int i = 0; i < st.size(); i++) {
			r += st[i].getScore(s);
			if (st[i].passSubject(s) >= 0)n++;
		}
		return r / n;
	}
	double passRate(string s) {//科目s及格率计算
		int n = 0, a = 0;
		for (int i = 0; i<st.size(); i++) {
			if (st[i].passSubject(s) >= 0)a++;
			if (st[i].passSubject(s)>0)n++;
		}
		return (double)n / a;
	}

	int getRank(Student t) {//排名计算
		int n = 1;
		for (int i = 0; i<st.size(); i++)
			if (st[i].gpa()>t.gpa())n++;
		return n;
	}
	~Class() {
		saveFileData();
	}
};
string toString(double n) {//实数转为字符串 保留3位小数
	ostringstream s;
	s.setf(ios::fixed);
	s << fixed << setprecision(3) << n;
	return s.str();
}
void listDisplay(vector<vector<string> >table, int col, string info = "", string title = "") {//实现列表翻查功能 table:原始表格 col:列,可选2 3 6 info:窗口底部提示信息 title:右上角标题
	int x = 0, y = 0, xm = table.size() - 9, ym = table[0].size() - col;
	while (1) {
		system("cls");
		printf("\t+-------------------问问问学生学籍管理系统-------------------+\n");
		printf("\t| %58s |\n", title.c_str());
		for (int i = x; i < x + 9; i++) {
			cout << "\t|";
			for (int j = y; j < y + col; j++) {
				cout << setw(60 / col - 1) << setiosflags(ios::left) << table[i][j];
				if (j == y + col - 1) cout << ' ';
				else cout << '|';
			}
			cout << "|\n";
		}
		printf("\t|                                                            |\n");
		printf("\t|   (I)向上翻 (K)向下翻 (J)向左翻 (L)向右翻       (ESC)返回  |\n");
		printf("\t+------------------------------------------------------------+\n");
		cout << info << endl;
		char c = getch();
		if (c == 27) break;
		if ((c == 'i' || c == 'I') && x > 0)x--;
		if ((c == 'k' || c == 'K') && x < xm)x++;
		if ((c == 'j' || c == 'J') && y > 0)y--;
		if ((c == 'l' || c == 'L') && y < ym)y++;
	}
}
void showStudentList(vector<Student>t, string title = "") {//学生信息(查询)列表
	set<string>subject;
	for (int ii = 0; ii < t.size(); ii++) {
		map<string, double>m = t[ii].getScore();
		for (map<string, double>::iterator j = m.begin(); j != m.end(); j++)
			subject.insert(j->first);
	}
	vector<string>subjects;
	for (set<string>::iterator it = subject.begin(); it != subject.end(); it++)
		subjects.push_back(*it);
	string spaces = "";
	while (subjects.size() < 2) {
		spaces += " ";
		subjects.push_back(spaces);
	}
	vector<vector<string> >table;
	for (int i = 0; i < t.size(); i++) {
		vector<string>row;
		row.push_back(t[i].getId()); row.push_back(t[i].getName());
		for (vector<string>::iterator j = subjects.begin(); j != subjects.end(); j++) {
			row.push_back(*j);
			if (t[i].getScore().count(*j)) {
				string s = toString(t[i].getScore()[*j]);
				row.push_back(s);
			}
			else
				row.push_back(" ");
		}
		table.push_back(row);
	}
	while (table.size() < 9)table.push_back(vector<string>(2 + subjects.size() * 2, " "));
	listDisplay(table, 6, t.size() ? "" : "提示: 未找到学生", title);
}
void showSubjectList(Class c, int mode, string title = "") {//学生信息(查询)列表 mode 1:及格率 0:平均分
	set<string>subject;
	vector<Student>t(c.all());
	for (int ii = 0; ii < t.size(); ii++) {
		map<string, double>m = t[ii].getScore();
		for (map<string, double>::iterator j = m.begin(); j != m.end(); j++)
			subject.insert(j->first);
	}
	vector<vector<string> >table;
	for (set<string>::iterator i = subject.begin(); i != subject.end(); i++) {
		vector<string>row;
		row.push_back(*i);
		string s = toString(mode ? c.passRate(*i) * 100 : c.scoreAvr(*i));
		if (mode)s += "%";
		row.push_back(s);
		table.push_back(row);
	}
	while (table.size() < 9)table.push_back(vector<string>(2, " "));
	listDisplay(table, 2, "", mode ? "及格率" : "平均分");
}
int main() {
	system("cls");
	printf("\t+-------------------问问问学生学籍管理系统-------------------+\n");
	printf("\t|                                                            |\n");
	printf("\t|                                                            |\n");
	printf("\t|                                                            |\n");
	printf("\t|                                                            |\n");
	printf("\t|                                                            |\n");
	printf("\t|                          加载中                            |\n");
	printf("\t|                     ................                       |\n");
	printf("\t|                                                            |\n");
	printf("\t|                                                            |\n");
	printf("\t|                                                            |\n");
	printf("\t|                                                            |\n");
	printf("\t|                                                            |\n");
	printf("\t+------------------------------------------------------------+\n");
	Class s112;
	s112.getFileData();
	while (1) {
		system("cls");
		printf("\t+-------------------问问问学生学籍管理系统-------------------+\n");
		printf("\t|                                                            |\n");
		printf("\t|                          1605112班                         |\n");
		printf("\t|                                                            |\n");
		printf("\t|         (I) 插入学生学籍信息   (D) 删除学生学籍信息        |\n");
		printf("\t|                                                            |\n");
		printf("\t|         (S) 检索学生学籍信息   (R) 查询学生排名信息        |\n");
		printf("\t|                                                            |\n");
		printf("\t|         (P)   本班及格率       (A)   班级平均分            |\n");
		printf("\t|                                                            |\n");
		printf("\t|         (L)   列出所有学生学籍信息                         |\n");
		printf("\t|                                                            |\n");
		printf("\t|                                             (ESC)退出系统  |\n");
		printf("\t+------------------------------------------------------------+\n");
		char c = getch();
		if (c == 'i' || c == 'I') {
			cout << "请输入学生学号及姓名:";
			string s1, s2; cin >> s1 >> s2; cin.ignore(1000, '\n');
			map<string, double>m;
			int n;
			cout << "请输入将要登记的科目数量: ";
			cin >> n;
			while (cin.fail()) {
				cin.clear();
				cerr << "请输入正确的科目数量" << endl;
				cin.ignore(1000, '\n');
				cin >> n;
			}
			cin.ignore(1000, '\n');
			for (int i = 0; i < n; i++) {
				cout << "科目名称及分数: ";
				string s; double d;
				cin >> s;
				cin >> d;
				while (cin.fail() || d<0 || d>100) {
					cin.clear();
					cerr << "请输入正确的分数" << endl;
					cin.ignore(1000, '\n');
					cin >> d;
				}
				cin.ignore(1000, '\n');
				m[s] = d;
			}
			s112.insertStudent(Student(s1, s2, m));
			s112.saveFileData();//更改数据后保存
		}
		if (c == 'd' || c == 'D') {
			cout << "请输入学生学号或姓名:";
			string s; cin >> s; cin.ignore(1000, '\n');
			showStudentList(s112.findStudent(s), "以下学生信息将被删除(确认后按esc)");
			cout << "确定要删除他们吗?(Y/N)" << endl;
			cin >> c; cin.ignore(1000, '\n');
			if (c == 'Y' || c == 'y')s112.deleteStudent(s);
			s112.saveFileData();//更改数据后保存
		}
		if (c == 'l' || c == 'L')showStudentList(s112.all(), "全部学生");
		if (c == 's' || c == 'S') {
			cout << "请输入学生学号或姓名:";
			string s; cin >> s; cin.ignore(1000, '\n');
			showStudentList(s112.findStudent(s), "查找学生:" + s);
		}
		if (c == 'p' || c == 'P')showSubjectList(s112, 1);
		if (c == 'a' || c == 'A')showSubjectList(s112, 0);
		if (c == 'r' || c == 'R') {
			cout << "请输入学生学号:";
			string s; cin >> s; cin.ignore(1000, '\n');
			Student t = s112.findStudent(s).size() ? s112.findStudent(s)[0] : Student("不存在的", "不存在的", map<string, double>());
			string id = t.getId(), name = t.getName();
			double gpa = t.gpa();
			int rank = t.getScore().size() ? s112.getRank(t) : 0;
			system("cls");
			printf("\t+-------------------问问问学生学籍管理系统-------------------+\n");
			printf("\t|                                                            |\n");
			printf("\t|         *****                                              |\n");
			printf("\t|        *     *                                             |\n");
			printf("\t|             *                  总绩点: %.3f               |\n", gpa);
			printf("\t|           *                                                |\n");
			printf("\t|           *                                                |\n");
			printf("\t|                                                            |\n");
			printf("\t|           *                    班级排名: %3d               |\n", rank);
			printf("\t|                                                            |\n");
			printf("\t|   姓名:  %10s                                        |\n", name.c_str());
			printf("\t|   学号:  %10s                                        |\n", id.c_str());
			printf("\t|                                                            |\n");
			printf("\t+------------------------------------------------------------+\n");
			system("pause");
		}
		if (c == 27)break;
	}
	return 0;
}