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
	int passSubject(string s) {	//passSubject -1:δѡ 0:�� 1:��
		if (!score.count(s))return -1;
		if (score[s] >= 60)return 1;
		return 0;
	}
	double gpa() {//�������
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
			if (sm.size()>0)//��ֹmapΪ��
				for (map<string, double>::iterator i = sm.begin(); i != sm.end(); i++) {
					m[i->first] = i->second;
				}
			st[t[0]] = Student(s.getId(), s.getName(), m);
		}
		else {
			cerr << "ѧ���Ѵ���!" << endl;
			system("pause");
		}
	}
	void getFileData() {
		ifstream f("data.txt", ios::in);
		if (!f) {
			cerr << "�޴��ļ�" << endl;
			system("pause");
			ofstream ff("data.txt", ios::trunc);//�ļ��������򴴽�
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
			f << "end -1" << endl;//��end -1��Ϊһ�н����ı�־
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
	double scoreAvr(string s) {//��Ŀsƽ���ּ���
		double r = 0; int n = 0;
		for (int i = 0; i < st.size(); i++) {
			r += st[i].getScore(s);
			if (st[i].passSubject(s) >= 0)n++;
		}
		return r / n;
	}
	double passRate(string s) {//��Ŀs�����ʼ���
		int n = 0, a = 0;
		for (int i = 0; i<st.size(); i++) {
			if (st[i].passSubject(s) >= 0)a++;
			if (st[i].passSubject(s)>0)n++;
		}
		return (double)n / a;
	}

	int getRank(Student t) {//��������
		int n = 1;
		for (int i = 0; i<st.size(); i++)
			if (st[i].gpa()>t.gpa())n++;
		return n;
	}
	~Class() {
		saveFileData();
	}
};
string toString(double n) {//ʵ��תΪ�ַ��� ����3λС��
	ostringstream s;
	s.setf(ios::fixed);
	s << fixed << setprecision(3) << n;
	return s.str();
}
void listDisplay(vector<vector<string> >table, int col, string info = "", string title = "") {//ʵ���б��鹦�� table:ԭʼ��� col:��,��ѡ2 3 6 info:���ڵײ���ʾ��Ϣ title:���ϽǱ���
	int x = 0, y = 0, xm = table.size() - 9, ym = table[0].size() - col;
	while (1) {
		system("cls");
		printf("\t+-------------------������ѧ��ѧ������ϵͳ-------------------+\n");
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
		printf("\t|   (I)���Ϸ� (K)���·� (J)���� (L)���ҷ�       (ESC)����  |\n");
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
void showStudentList(vector<Student>t, string title = "") {//ѧ����Ϣ(��ѯ)�б�
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
	listDisplay(table, 6, t.size() ? "" : "��ʾ: δ�ҵ�ѧ��", title);
}
void showSubjectList(Class c, int mode, string title = "") {//ѧ����Ϣ(��ѯ)�б� mode 1:������ 0:ƽ����
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
	listDisplay(table, 2, "", mode ? "������" : "ƽ����");
}
int main() {
	system("cls");
	printf("\t+-------------------������ѧ��ѧ������ϵͳ-------------------+\n");
	printf("\t|                                                            |\n");
	printf("\t|                                                            |\n");
	printf("\t|                                                            |\n");
	printf("\t|                                                            |\n");
	printf("\t|                                                            |\n");
	printf("\t|                          ������                            |\n");
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
		printf("\t+-------------------������ѧ��ѧ������ϵͳ-------------------+\n");
		printf("\t|                                                            |\n");
		printf("\t|                          1605112��                         |\n");
		printf("\t|                                                            |\n");
		printf("\t|         (I) ����ѧ��ѧ����Ϣ   (D) ɾ��ѧ��ѧ����Ϣ        |\n");
		printf("\t|                                                            |\n");
		printf("\t|         (S) ����ѧ��ѧ����Ϣ   (R) ��ѯѧ��������Ϣ        |\n");
		printf("\t|                                                            |\n");
		printf("\t|         (P)   ���༰����       (A)   �༶ƽ����            |\n");
		printf("\t|                                                            |\n");
		printf("\t|         (L)   �г�����ѧ��ѧ����Ϣ                         |\n");
		printf("\t|                                                            |\n");
		printf("\t|                                             (ESC)�˳�ϵͳ  |\n");
		printf("\t+------------------------------------------------------------+\n");
		char c = getch();
		if (c == 'i' || c == 'I') {
			cout << "������ѧ��ѧ�ż�����:";
			string s1, s2; cin >> s1 >> s2; cin.ignore(1000, '\n');
			map<string, double>m;
			int n;
			cout << "�����뽫Ҫ�ǼǵĿ�Ŀ����: ";
			cin >> n;
			while (cin.fail()) {
				cin.clear();
				cerr << "��������ȷ�Ŀ�Ŀ����" << endl;
				cin.ignore(1000, '\n');
				cin >> n;
			}
			cin.ignore(1000, '\n');
			for (int i = 0; i < n; i++) {
				cout << "��Ŀ���Ƽ�����: ";
				string s; double d;
				cin >> s;
				cin >> d;
				while (cin.fail() || d<0 || d>100) {
					cin.clear();
					cerr << "��������ȷ�ķ���" << endl;
					cin.ignore(1000, '\n');
					cin >> d;
				}
				cin.ignore(1000, '\n');
				m[s] = d;
			}
			s112.insertStudent(Student(s1, s2, m));
			s112.saveFileData();//�������ݺ󱣴�
		}
		if (c == 'd' || c == 'D') {
			cout << "������ѧ��ѧ�Ż�����:";
			string s; cin >> s; cin.ignore(1000, '\n');
			showStudentList(s112.findStudent(s), "����ѧ����Ϣ����ɾ��(ȷ�Ϻ�esc)");
			cout << "ȷ��Ҫɾ��������?(Y/N)" << endl;
			cin >> c; cin.ignore(1000, '\n');
			if (c == 'Y' || c == 'y')s112.deleteStudent(s);
			s112.saveFileData();//�������ݺ󱣴�
		}
		if (c == 'l' || c == 'L')showStudentList(s112.all(), "ȫ��ѧ��");
		if (c == 's' || c == 'S') {
			cout << "������ѧ��ѧ�Ż�����:";
			string s; cin >> s; cin.ignore(1000, '\n');
			showStudentList(s112.findStudent(s), "����ѧ��:" + s);
		}
		if (c == 'p' || c == 'P')showSubjectList(s112, 1);
		if (c == 'a' || c == 'A')showSubjectList(s112, 0);
		if (c == 'r' || c == 'R') {
			cout << "������ѧ��ѧ��:";
			string s; cin >> s; cin.ignore(1000, '\n');
			Student t = s112.findStudent(s).size() ? s112.findStudent(s)[0] : Student("�����ڵ�", "�����ڵ�", map<string, double>());
			string id = t.getId(), name = t.getName();
			double gpa = t.gpa();
			int rank = t.getScore().size() ? s112.getRank(t) : 0;
			system("cls");
			printf("\t+-------------------������ѧ��ѧ������ϵͳ-------------------+\n");
			printf("\t|                                                            |\n");
			printf("\t|         *****                                              |\n");
			printf("\t|        *     *                                             |\n");
			printf("\t|             *                  �ܼ���: %.3f               |\n", gpa);
			printf("\t|           *                                                |\n");
			printf("\t|           *                                                |\n");
			printf("\t|                                                            |\n");
			printf("\t|           *                    �༶����: %3d               |\n", rank);
			printf("\t|                                                            |\n");
			printf("\t|   ����:  %10s                                        |\n", name.c_str());
			printf("\t|   ѧ��:  %10s                                        |\n", id.c_str());
			printf("\t|                                                            |\n");
			printf("\t+------------------------------------------------------------+\n");
			system("pause");
		}
		if (c == 27)break;
	}
	return 0;
}