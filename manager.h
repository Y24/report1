//
// Created by root on 12/6/18.
//
/*
 *从文件读入一个班学生三门课（语文、数学、外语）的成绩，分别统计下列内容：
(1) 统计平均分不及格人数并打印不及格学生名单；
(2) 统计成绩在全班平均分及平均分之上的学生人数并打印其学生名单；
(3) 统计平均分的各分数段的学生人数及所占的百分比；
(4) 按总分成绩由高到低排出成绩的名次；
(5) 打印出名次表，表格内包括学生编号、各科分数、总分和平均分；
(6) 任意输入一个学号，能够查找出该学生在班级中的排名及其考试分数。
 */
#ifndef STUDENT_MANAGER_H
#define STUDENT_MANAGER_H
#define DEFAULTPASSSCORE 60
#define OBJECTNUM 3

#include <istream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <map>

class scores {
private:
    double ChineseScore;
    double MathScore;
    double EnglishScore;
public:
    scores() : ChineseScore(0), MathScore(0), EnglishScore(0) {};

    scores(double ChineseScore, double MathScore, double EnglishScore) :
            ChineseScore(ChineseScore),
            MathScore(MathScore),
            EnglishScore(EnglishScore) {}

    double getChineseScore() const {
        return ChineseScore;
    }

    double getMathScore() const {
        return MathScore;
    }

    double getEnglishScore() const {
        return EnglishScore;
    }

    double getTotal() const {
        return ChineseScore + MathScore + EnglishScore;
    }
};

using namespace std;

class manager {
private:
    int studentNum;
    string *studentName;
    string *studentID;
    scores *score;
public:
    explicit manager(const char *filename) : studentNum(0), studentName(nullptr), studentID(nullptr), score(nullptr) {
        loadInformationFromFile(filename);
    }

    virtual ~manager() {
        delete[] studentName;
        delete[] studentID;
        delete[] score;
    }

    //从文件读入一个班学生三门课（语文、数学、外语）的成绩
    //文件内容：
    // studentNum \n[姓名 学号 语文分 数学分 英语分\n]+
    void loadInformationFromFile(const char *filename) {
        ifstream information(filename, ios_base::in);
        if (!information.is_open()) {
            cout << "cannot open the file " << filename << endl;
            exit(EXIT_FAILURE);
        }
        information >> studentNum;
        studentName = new string[studentNum];
        studentID = new string[studentNum];
        score = new scores[studentNum];
        int ChineseScoreTmp = 0, MathScoreTmp = 0, EnglishScoreTmp = 0;
        for (int i = 0; i < studentNum; ++i) {
            information >> studentName[i];
            information >> studentID[i];
            information >> ChineseScoreTmp;
            information >> MathScoreTmp;
            information >> EnglishScoreTmp;
            score[i] = scores(ChineseScoreTmp, MathScoreTmp, EnglishScoreTmp);
        }
    }

    const scores calculateAverage() const {
        double ChineseScore = 0, MathScore = 0, EnglishScore = 0;
        for (int i = 0; i < studentNum; i++) {
            ChineseScore += score[i].getChineseScore();
            MathScore += score[i].getMathScore();
            EnglishScore += score[i].getEnglishScore();
        }
        return scores(ChineseScore / studentNum, MathScore / studentNum, EnglishScore / studentNum);
    }

    const scores analyzeFlunk(vector<int> *flunks,
                              const scores averageScore = scores(DEFAULTPASSSCORE, DEFAULTPASSSCORE,
                                                                 DEFAULTPASSSCORE)) const {
        int ChineseScoreTmp = 0, MathScoreTmp = 0, EnglishScoreTmp = 0;
        for (int i = 0; i < studentNum; i++) {

            if (score[i].getChineseScore() < averageScore.getChineseScore()) {
                ChineseScoreTmp++;
                flunks[0].push_back(i);

            }
            if (score[i].getMathScore() < averageScore.getMathScore()) {
                MathScoreTmp++;
                flunks[1].push_back(i);
            }
            if (score[i].getEnglishScore() < averageScore.getEnglishScore()) {
                EnglishScoreTmp++;
                flunks[2].push_back(i);
            }

        }
        return scores(ChineseScoreTmp, MathScoreTmp, EnglishScoreTmp);
    }

//(1) 统计平均分不及格人数并打印不及格学生名单
    void function0() const {
        const scores averageScore = calculateAverage();
        auto *flunkIndex = new vector<int>[OBJECTNUM];
        const scores flunkNum = analyzeFlunk(flunkIndex);
        cout << "The average score is:" <<
             " Chinese " << averageScore.getChineseScore() <<
             " Math " << averageScore.getMathScore() <<
             " English " << averageScore.getEnglishScore()
             << endl;
        cout << "The flunk number is:" <<
             " Chinese " << flunkNum.getChineseScore() <<
             " Math " << flunkNum.getMathScore() <<
             " English " << flunkNum.getEnglishScore()
             << endl;
        string object[] = {"Chinese", "Math", "English"};
        cout << "The flunk's details are:" << endl;
        for (int i = 0; i < OBJECTNUM; ++i) {
            if (flunkIndex[i].empty())
                cout << "no " << object[i] << "\'s flunk" << endl;
            else {
                cout << object[i] << "\'s flunk details are (display by name)";
                for (auto &j:flunkIndex[i])
                    cout << " " << studentName[j];
            }
            cout << endl;
        }
        delete[] flunkIndex;
    }

//(2) 统计成绩在全班平均分及平均分之上的学生人数并打印其学生名单
    void function1() const {
        const scores averageScore = calculateAverage();
        auto *flunkIndex = new vector<int>[OBJECTNUM];
        const scores flunkNum = analyzeFlunk(flunkIndex, averageScore);
        string object[] = {"Chinese", "Math", "English"};
        cout << "The Passed details are :" << endl;
        for (int i = 0; i < OBJECTNUM; ++i) {
            if (flunkIndex[i].size() == studentNum)
                cout << "no passed students in " << object[i] << endl;
            else {
                cout << object[i] << "\'s passed details are (display by name)";
                if (flunkIndex[i].empty())
                    for (int j = 0; j < studentNum; ++j)
                        cout << " " << studentName[j];
                else
                    for (int j = 0; j < studentNum; ++j)
                        if (find(flunkIndex[i].begin(), flunkIndex[i].end(), j) != flunkIndex[i].end())
                            cout << " " << studentName[j];
            }
            cout << endl;
        }
        delete[] flunkIndex;
    }

//(3) 统计平均分的各分数段的学生人数及所占的百分比
    void function2() const {
        int section[OBJECTNUM][10];
        for (auto &i : section)
            for (int &j : i)
                j = 0;
        for (int i = 0; i < studentNum; ++i) {
            int Tmp[] = {static_cast<int>(score[i].getChineseScore() / 10),
                         static_cast<int>(score[i].getMathScore() / 10),
                         static_cast<int>(score[i].getEnglishScore() / 10)};
            for (int j = 0; j < OBJECTNUM; j++)
                section[j][Tmp[j] != 10 ? Tmp[j] : 9]++;
        }
        int total[] = {0, 0, 0};
        string object[] = {"Chinese", "Math", "English"};
        for (int i = 0; i < OBJECTNUM; ++i)
            for (int j = 0; j < 10; ++j) {
                total[i] += section[i][j];
            }
        for (int i = 0; i < OBJECTNUM; ++i) {
            cout << object[i] << "\'s details: \n";
            for (int j = 0; j < 10; ++j) {
                cout << j << "0 -" << j + 1 << "0 : total number " << section[i][j] << " percent "
                     << 100.0 * section[i][j] / total[i] << "%" << endl;
            }

        }
    }

//(4) 按总分成绩由高到低排出成绩的名次
    void function3() const {
        auto *total = new double[studentNum];
        for (int i = 0; i < studentNum; ++i)
            total[i] = score[i].getChineseScore() + score[i].getEnglishScore() + score[i].getMathScore();
        map<int, double> sortMap;
        for (int i = 0; i < studentNum; ++i)
            sortMap[i] = total[i];
        vector<pair<int, double>> vec;
        vec.reserve(sortMap.size());
        for (auto &it : sortMap)
            vec.emplace_back(it.first, it.second);
        sort(vec.begin(), vec.end(),
             [](pair<int, double> x, pair<int, double> y) -> int { return x.second > y.second; });
        cout << "sort and print the details by total scores: \n";
        for (auto &item : vec)
            cout << studentName[item.first] << ": " << item.second << endl;
        delete[] total;
    }

//(5) 打印出名次表，表格内包括学生编号、各科分数、总分和平均分
    void function4() const {
        for (int i = 0; i < studentNum; i++) {
            cout << "name: " << studentName[i]
                 << " ID: " << studentID[i]
                 << " Chinese: " << score[i].getChineseScore()
                 << " Math: " << score[i].getMathScore()
                 << " English: " << score[i].getEnglishScore()
                 << " total: " << score[i].getTotal()
                 << " average: " << score[i].getTotal() / OBJECTNUM << endl;
        }

    }

//(6) 任意输入一个学号，能够查找出该学生在班级中的排名及其考试分数
    void function5() const {
        string IDToSearch;
        cout << "Please specify the ID you want to search :";
        cin >> IDToSearch;
        for (int i = 0; i < studentNum; i++)
            if (studentID[i] == IDToSearch) {
                auto *total = new double[studentNum];
                for (int j = 0; j < studentNum; j++)
                    total[j] = score[j].getTotal();
                int rank = 1;
                for (int k = 0; k < studentNum; ++k)
                    if (k != i && total[k] > total[i])
                        rank++;
                cout << "found successfully : \nname: " << studentName[i]
                     << " ID: " << studentID[i]
                     << " rank: " << rank
                     << " Chinese: " << score[i].getChineseScore()
                     << " Math: " << score[i].getMathScore()
                     << " English: " << score[i].getEnglishScore()
                     << " total: " << score[i].getTotal()
                     << " average: " << score[i].getTotal() / OBJECTNUM << endl;
                delete[] total;
                return;
            }
        cout<<" no searching result specified by ID "<<IDToSearch<<endl;

    }

    void firstPrint() const {
        cout << "Welcome to my student's score manager\n";
        cout << "Preface: \n";
        cout << "read data from the specified file,"
             << "The first data is studentNum which is an integer refers to the total number of the following students\n"
             << "and the following data is studentNum lines,each line contains the name , ID, Chinese score, Math's score, English score"
             << endl;
    }

    void printHelp() const {
        cout << "function introduction: \n";
        cout << "(1) calculate and print the average score,flunk index\n";
        cout << "(2) calculate and print the passed student's number and details\n";
        cout << "(3) calculate and print the students' details in each score section\n";
        cout << "(4) sort and print the message by total scores\n";
        cout << "(5) print the roster which includes ID,each scores,total score, and average score\n";
        cout << "(6) input an ID,find and print the specified student's rank and each score\n";
        cout << "key introduction: \n";
        cout << "1-6 refers to the corresponding function\n";
        cout << "q to quit the manager\n";
        cout << "h or H to display this help and exit\n";
    }

    void goodbye() const {
        cout << "code happily.\nenjoy your life.\nsee you!\n";
    }

    void invalidKey(const char key) const {
        cout << "cannot recognize the input key: " << key << endl;
    }


    void mainLoop() {
        firstPrint();
        printHelp();
        char currentChar;
        while (true) {
            currentChar = static_cast<char>(getchar());
            if (isspace(currentChar)) continue;
            if (currentChar == 'q') {
                goodbye();
                break;
            }
            if (currentChar == 'H' || currentChar == 'h') {
                printHelp();
                cout << "input your next choice :";
                continue;
            }
            if (isdigit(currentChar)) {
                switch (currentChar) {
                    case '1':
                        function0();
                        break;
                    case '2':
                        function1();
                        break;
                    case '3':
                        function2();
                        break;
                    case '4':
                        function3();
                        break;
                    case '5':
                        function4();
                        break;
                    case '6':
                        function5();
                        break;
                    default:
                        invalidKey(currentChar);
                        break;
                }
            } else
                invalidKey(currentChar);
            cout << "input your next choice :";
        }
    }
};

#endif //STUDENT_MANAGER_H
