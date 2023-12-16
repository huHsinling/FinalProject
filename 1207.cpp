#include<iostream>
#include<fstream>
#include<string>
#include<ctime>
#include<vector>
#include<cstdlib>
//#include"minigame.h"
using namespace std;
void makecopy(int answer[], int Anscopy[]);
void strToInt(int keyIn[], std::string numstr);
void makerand(int answer[]);
int findA(int Anscopy[], int keyIn[]);
int findB(int Anscopy[], int keyIn[]);

class Events{
    protected:
        const string eventName; //事件名稱
        const string eventDetail; //事件敘述，可以加一些故事情節之類的？
        const int scoreChange; //此事件會改變多少分數
        const int mood; //心情值
        const int type;// type == 0 不做任何事 ； type == 1 yes or no ； type == 2 選擇課 ； type == 3 所有課
    public:
        Events(string eventName, string eventDetail, int scoreChange, int mood, int type): eventName(eventName), eventDetail(eventDetail), scoreChange(scoreChange), mood(mood),  type(type)
        {}
        //執行事件，回傳 type
        virtual int eventHappened(int& scoreChange, int& moodChange) const; 
        string getEventName();
        string getEventDetail();
        ~Events(){}
};

string Events::getEventName(){
    return eventName;
}
string Events::getEventDetail(){
    return eventDetail;
}
int Events::eventHappened(int& scoreChange, int& moodChange) const
{
    cout << "事件發生：" << endl;
    cout << eventDetail << endl;
    scoreChange = this->scoreChange;
    moodChange = this->mood;
    return type;
}
class EventDefault: public Events
{
    protected:

    public:
        EventDefault(string eventName, string eventDetail, int scoreChange, int mood, int type): Events(eventName, eventDetail, scoreChange, mood, type)
        {}
        ~EventDefault(){}

};
class EventOne : public Events
{
    protected:
        const int scorechange1;
        const int mood1;
    public:
        EventOne(string eventName, string eventDetail, int scoreChange, int mood, int type, int scorechange1, int mood1): Events(eventName, eventDetail, scoreChange, mood, type), scorechange1(scorechange1), mood1(mood1)
        {}
        int eventHappened(int& scoreChange, int& moodChange) const
        {
            string decision;
            cout << "事件發生:" << endl;
            cout<<eventDetail<<endl;
            cout<<"按 Y 或 N 選擇要不要";
            cout<<endl;
            while(true)
            {
                cin>>decision;
                if(decision == "Y" || decision == "y")// yes 回傳
                {
                    scoreChange = this->scoreChange;
                    moodChange = this->mood;
                    break;
                }
                else if(decision == "N" || decision == "n") //no 回傳
                {
                    moodChange = this->mood1;
                    break;
                }
                else
                    cout<<"輸入錯誤，請輸入 Y 或 N"<<endl;
            }
            return type;
        }
        ~EventOne(){}
        
};

class Course{
protected:
    const int id;//課程編號
    const string name;//課程名稱
    const int credit;//學分數
    int scoreVari;//分數加減，與課程難度有關
    const double assignedProb;//選上機率
    const bool isRequired;//是否為必修
    static int maxNameLen;//課程名稱最長值
    static int courseCnt;//課程數量
public:
    Course(int id, string n, int c, int sv, double ap, bool ir);
    //選課時印出所有課程資訊
    void printAll() const;
    //事件選擇時印出 id 及 name
    void printIdName() const;
    //結算時印出 id, name, credit
    void printIdNameCredit() const;
    //選課時印出所有課程資訊的標題
    static void printTitle();
    //結算時印出 id, name, credit 的標題
    static void printINCTitle();
    void changeScore(int score);
    int getID()const{return id;};
    int getScore()const{return scoreVari;};
    string getName()const{return name;};
    int getCredit()const{ return credit; };
};

class RequiredCourse : public Course{
private:
    const int semester;//第幾學期的必修
    static int requiredCnt;//必修課程數量
public:
    RequiredCourse(string n, int c, int sv, int s);
    int getSemester() {return semester;}
};

class ElectiveCourse : public Course{
private:
    static int electiveCnt;//選修課程數量
public:
    ElectiveCourse(string n, int c, int sv, double ap);
    double getAssignedProb() {return assignedProb;}
    static void init();//初始化 electiveCnt
};


Course::Course(int id, string name, int credit, int scoreVari, double assignedProb, bool isRequired):
id(id), name(name), credit(credit), scoreVari(scoreVari), assignedProb(assignedProb), isRequired(isRequired){
    if(name.length() > maxNameLen)
        maxNameLen = name.length();
    courseCnt++;
}

void Course::printAll() const{
    printIdNameCredit();
    if(scoreVari >= 0)
        cout << "+" << scoreVari;
    else
        cout << scoreVari;
    cout << "\t" << assignedProb * 100 << "%";
    cout << endl;
}

void Course::printIdName() const{
    cout << id << ". " << name << endl;
}

void Course::printIdNameCredit() const{
    cout << id << ". " << name;
    int n = ((maxNameLen - name.length()) / 8) + 1;
    for(int i = 0; i < n; i++)
        cout << "\t";
    cout << credit << "\t";
}

void Course::printTitle(){
    printINCTitle();
    cout << "難度" << "\t" << "選上機率";
    cout << endl;
}

void Course::printINCTitle(){
    cout << "編號 課程名稱";
    int n = ((maxNameLen - 8) / 8);
    for(int i = 0; i < n; i++)
        cout << "\t";
    cout << "學分" << "\t";
}

void Course::changeScore(int score){
    scoreVari += score;
}

RequiredCourse::RequiredCourse(string name, int credit, int scoreVari, int semester):
Course(requiredCnt + 101, name, credit, scoreVari, 1, true), semester(semester){
    requiredCnt++;
}


ElectiveCourse::ElectiveCourse(string name, int credit, int scoreVari, double assignedProb):
Course(electiveCnt + 201, name, credit, scoreVari, assignedProb, false){
    electiveCnt++;
}

class Player{
    protected:
        const string name;//玩家名稱
        vector<Course*> courselist;//本學期修課列表
        vector<Course*> passcourse;//已通過課程列表
        vector<ElectiveCourse*> chooseCourse;//選課暫存列表
        int totalCredit;//已修學分數
        int semesterCredit;//本學期學分數
        int mood;//心情值
        int basicScore;//基本分數，由課程數量決定
        int week;//目前在第幾週
    public:
        Player(string n);
        ~Player();
        int getBasicscore();
        int getWeek() const {return week;}
        int getTotalCredit() {return totalCredit;}
        //改變某課程的分數
        void changeScore(int ID, int scorechange);
        //改變所有課程的分數
        void changeAllScore(int scorechange);
        //改變心情值
        bool changeMood(int moodchange);
        //將課程加入 courselist
        void addcourse(Course& choosedcourse);
        //將課程加入 passcourse
        void addPasscourse(Course& passcourse);
        //將課程加入 chooseCourse
        void addChooseCourse(ElectiveCourse& chooseCourse);
        //期末結算，被二一則回傳 false
        bool countpassfail();
        //回到學期初始狀態(passcourse還在)
        void clearcourse();
        //丟骰後移動 step 格
        void move(int step);
        //移動到第 week 格
        void moveToWeek(int week);
        //回傳 courselist 中隨機一個課程的 id
        int randomID() const;
        //印出 courselist 所有課程的 id 及 name
        void printCourse() const;
        //印出選課結果
        void printChooseResult();
        //coursrelist 中是否有此課程 id
        bool idExist(int id) const;
        //passcourse 中是否有此課程 id
        bool isPassed(int id) const;
        //chooseCourse 中是否有此課程 id
        bool isChoosed(int id) const;
        //課程分發
        void assignCourse();
};

Player::Player(string n):name(n){
    semesterCredit = 0;
    totalCredit = 0;
    mood = 100;
    basicScore = 0;
    week = 0;
}
Player::~Player(){
    for(int i = 0; i < courselist.size(); i++){
        delete courselist[i];
        courselist[i] = nullptr;
    } 
    for(int i = 0; i < passcourse.size(); i++){
        delete passcourse[i];
        passcourse[i] = nullptr;
    }
}
int Player::getBasicscore(){
    basicScore = 80 - semesterCredit * 0.5;//應該只計算這學期的吧？
    return basicScore;
}
void Player::addcourse(Course& choosedcourse){
    Course* thecourse = new Course (choosedcourse);
    courselist.push_back(thecourse);
    semesterCredit += choosedcourse.getCredit();
}
void Player::addPasscourse(Course& choosedcourse){
    Course* thecourse = new Course (choosedcourse);
    this->passcourse.push_back(thecourse);
}
void Player::addChooseCourse(ElectiveCourse& choosedcourse){
    ElectiveCourse* thecourse = new ElectiveCourse (choosedcourse);
    this->chooseCourse.push_back(thecourse);
}
void Player::clearcourse(){
    for(int i = 0; i < courselist.size(); i++)
        delete courselist[i];
    courselist.clear();
    mood = 100;
    basicScore = 0;
    week = 0;
    semesterCredit = 0;
}
bool Player::changeMood(int moodchange){   
    if(moodchange == 0){
        cout << "沒有心情改變" << endl;
        return true;
    }
    mood += moodchange;
    cout << "你的心情 " << (moodchange > 0? "+":"") << moodchange << " 分 ";
    cout << "你的心情值現在是 " << mood << " 分 " << endl << endl;
    if(mood < 0)
        return false;
    else
        return true;
}
void Player::changeScore(int ID, int scorechange){
    if(scorechange == 0){
        cout << "沒有分數改變" << endl;
        return;
    }

    for(int i = 0; i < courselist.size(); i++){
        if(courselist[i]->getID() == ID){
            courselist[i]->changeScore(scorechange);
            cout << "分數改變:" << endl;
            cout << courselist[i]->getName() << " " << (scorechange >= 0? "+":"") << scorechange << " 分" << endl;
            cout << courselist[i]->getName() << " 目前的分數為 " << this->getBasicscore() + courselist[i]->getScore() << " 分" << endl;
            return;
        }
    }
}

void Player::changeAllScore(int scorechange){
    if(scorechange == 0){
        cout << "沒有分數改變" << endl;
        return;
    }
    
    cout << "所有課程 " << (scorechange >= 0? "+":"") << scorechange << " 分" << endl;
    for(int i = 0; i < courselist.size(); i++){
        courselist[i]->changeScore(scorechange);
        cout << courselist[i]->getName() << " 分數為 " << this->getBasicscore() + courselist[i]->getScore() << endl;
    }
}

bool Player::countpassfail(){
    int basicScore = getBasicscore();
    Course::printINCTitle();
    cout << "分數" << "\t" << "結果" << endl;
    int passCredit = 0;

    for(int i = 0; i < courselist.size(); i++){
        int score = basicScore + courselist[i]->getScore();
        courselist[i]->printIdNameCredit();
        cout << score << "\t";
        if(score < 60){
            cout << "Fail" << endl;
        }
        else{
            cout << "Pass" << endl;
            passCredit += courselist[i]->getCredit();
            totalCredit += courselist[i]->getCredit();
            addPasscourse(*courselist[i]);
        }
    }
    cout << endl;
    if(passCredit < semesterCredit / 2){
        cout << "本學期通過學分數/本學期總學分數：" << passCredit << "/" << semesterCredit << endl;
        cout << "你被二一了，遊戲結束" << endl;
        return false;
    }
    else{
        cout << "目前通過總學分數為: " << totalCredit;
        cout << endl;
        this->clearcourse();
        return true;
    }
}

void Player::move(int step){
    week += step;
}

void Player::moveToWeek(int week){
    this->week = week;
}

int Player::randomID() const{
    int index = rand() % courselist.size();
    int id = courselist[index]->getID();
    return id;
}

void Player::printCourse() const{
    for(int i = 0; i < courselist.size(); i++){
        courselist[i]->printIdName();
    }
}

void Player::printChooseResult(){
    Course::printINCTitle();
    cout << "分數" << endl;
    for(int i = 0; i < courselist.size(); i++){
        courselist[i]->printIdNameCredit();
        cout << getBasicscore() + courselist[i]->getScore() << endl;
    }
}

bool Player::idExist(int id) const{
    for(int i = 0; i < courselist.size(); i++){
        if(id == courselist[i]->getID())
            return true;
    }
    return false;
}

bool Player::isPassed(int id) const{
    for(int i = 0; i < passcourse.size(); i++){
        if(id == passcourse[i]->getID())
            return true;
    }
    return false;
}

bool Player::isChoosed(int id) const{
    for(int i = 0; i < chooseCourse.size(); i++){
        if(id == chooseCourse[i]->getID())
            return true;
    }
    return false;
}

void Player::assignCourse(){
    for(int i = 0; i < chooseCourse.size(); i++){
        int prob = rand() % 10;
        if(prob <= chooseCourse[i]->getAssignedProb() * 10)
            addcourse(*chooseCourse[i]);
    }
    for(int i = 0; i < chooseCourse.size(); i++){
        delete chooseCourse[i];
        chooseCourse[i] = nullptr;
    }
    chooseCourse.clear();
}

const int MAX_WEEK_NUM = 18;

class Game{
private:
    const int weekNum;//一學期有幾個禮拜
    string weeks[MAX_WEEK_NUM][2];//每個禮拜的敘述
    const int totalSemester;//總共幾學期
    int semester;//目前是第幾學期
    const int goalCredit;//目標學分數
    vector<Events*> events;//列表所有事件
    vector<RequiredCourse*> requiredCourses;//列表所有必修課程
    vector<ElectiveCourse*> electiveCourses;//列表所有選修課程
    Player player;//單人玩家
public:
    Game(int totalSemester, int weekNum, string name, int goalCredit);
    int getSemester() const {return semester;}
    ~Game();
    //印出本學期課程，並加入player course
    void printCourse();
    //選課
    void chooseCourse();
    //丟骰子，移動
    void dice();
    //印地圖
    void printMap();
    //執行事件，心情低於 0 則回傳 false
    bool event();
    //期中遊戲
    void miniGame1();
    //期末遊戲
    void miniGame2();
    //進入下一個學期
    bool nextSemester();
    //結算，被二一則回傳 false
    bool countPassFail();
    //最終大結算
    void theEnd();
    //玩家是否在第 week 週
    bool isWeek(int week) const;
    //輸入event_default
    void addEventsDefault(string eventName, string eventdetail, int scorechange, int mood, int type);
    //輸入event_one
    void addEventsOne(string eventName, string eventdetail, int scorechange, int mood, int type, int scorechange1, int mood1);
    //輸入必修課程
    void addRequiredCourse(string name, int credit, int scoreVari, int semester);
    //輸入選修課程
    void addElectiveCourse(string name, int credit, int scoreVari, double assignedProb);
};
Game::Game(int totalSemester, int weekNum, string name, int goalCredit):
totalSemester(totalSemester), semester(1), weekNum(weekNum), player(name), goalCredit(goalCredit){
    for(int i = 0; i <= weekNum; i++){
        weeks[i][0] = "第"+ to_string(i) + "週";
        weeks[i][1] = "事件";
    }
    weeks[0][1] = "選課";
    //weeks[1][1] = weeks[2][1] = weeks[3][1] = "加退選";
    weeks[weekNum/2][1] = "期中週";
    //weeks[weekNum/2 + 1][1] = weeks[weekNum/2 + 2][1] = "停修";
    weeks[weekNum][1] = "期末週";
}

Game::~Game(){
    for(int i = 0; i < events.size(); i++){
        delete events[i];
        events[i] = nullptr;
    }
    events.clear();
    for(int i = 0; i < requiredCourses.size(); i++){
        delete requiredCourses[i];
        requiredCourses[i] = nullptr;
    }
    requiredCourses.clear();
    for(int i = 0; i < electiveCourses.size(); i++){
        delete electiveCourses[i];
        electiveCourses[i] = nullptr;
    }
    electiveCourses.clear();
}

void Game::printCourse(){
    cout << "<本學期必修課程>" << endl;
    Course::printTitle();
    for(int i = 0; i < requiredCourses.size(); i++){
        if(requiredCourses[i]->getSemester() <= this->semester){
            if(!player.isPassed(requiredCourses[i]->getID())){
                requiredCourses[i]->printAll();
                player.addcourse(*requiredCourses[i]);
            }
        }
    }
    cout << endl;
    cout << "<選修課程>" << endl;
    Course::printTitle();
    for(int i = 0; i < electiveCourses.size(); i++){
        if(!player.isPassed(electiveCourses[i]->getID())){
            electiveCourses[i]->printAll();
        }
    }
}

void Game::chooseCourse(){;
    cout << "請輸入要選的課程的編號，若不再選課請輸入0：";
    int id;
    while(cin >> id){
        bool idExist = false, isChoosed = false;
        if(id == 0)
            break;
        if(player.isChoosed(id)){
            isChoosed = true;
        }
        else{
            for(int i = 0; i < electiveCourses.size(); i++){
                if(electiveCourses[i]->getID() == id){
                    player.addChooseCourse(*electiveCourses[i]);
                    cout << "已成功選課 " << electiveCourses[i]->getName() << endl;
                    idExist = true;
                    break;
                }
            }
        }
        
        if(isChoosed)
            cout << "課程已選擇，請輸入其他課程，若不再選課請輸入0：";
        else if(!idExist)
            cout << "課程不存在，請輸入正確編號，若不再選課請輸入0：";
        else
            cout << "請輸入要選的課程的編號，若不再選課請輸入0：";
        
    }
    //由機率決定是否選到課
    player.assignCourse();
    cout << endl;
    //選課結果
    cout << "<選課結果與初始分數>" << endl;
    player.printChooseResult();
}

void Game::dice(){
    cout << "按 d 或 D 來擲骰子" << endl;
    int step = 0;
    string throwDice;
    while(cin >> throwDice){
        if(throwDice == string("D") || throwDice == string("d")){
            step = 1 + rand() % 3;
            cout << "你移動了 " << step << " 格" << endl;
            break;
        }  
        else
            cout << "按 d 或 D 來擲骰子" << endl;
    }
    player.move(step);
    if(player.getWeek() >= weekNum)
        player.moveToWeek(weekNum);
}

void Game::printMap(){
    int midWeek = weekNum / 2;
    for(int i = 0; i <= midWeek; i++){
        cout << (i == player.getWeek() ? "*" : "") << weeks[i][0] << "\t";
    }
    cout << endl;
    for(int i = 0; i <= midWeek; i++){
        cout <<  weeks[i][1] << "\t";
    }
    cout << endl;
    for(int i = midWeek + 1; i <= weekNum; i++){
        cout << (i == player.getWeek() ? "*" : "") << weeks[i][0] << "\t";
    }
    cout << endl;
    for(int i = midWeek + 1; i <= weekNum; i++){
        cout <<  weeks[i][1] << "\t";
    }
    cout << endl;
    cout << endl;
}

bool Game::event(){
    int index = rand() % events.size();
    int scoreChange = 0, moodChange = 0;
    /*0 or 1: 隨機選一堂課
    2: 請玩家選擇課程
    3: 所有課程*/
    int type = events[index]->eventHappened(scoreChange, moodChange);
    int id = 0;

    if(type == 0 || type == 1){
        id = player.randomID();
        player.changeScore(id, scoreChange);
    }
    else if(type == 2){
        cout << "請選擇一堂課改變分數，輸入課程代號" << endl;
        player.printCourse();
        while(true){
            cin >> id;
            if(player.idExist(id))
                break;
            else
                cout << "課程不存在，請再輸入一次：";
        }
        player.changeScore(id, scoreChange);
    }
    else if(type == 3){
        player.changeAllScore(scoreChange);
    }

    if(player.changeMood(moodChange)){
        return true;
    }
    else{
        cout << "心情太差，導致想不開，遊戲結束" << endl;
        return false;
    }
}
bool Game::nextSemester(){
    semester++;
    player.clearcourse();
    if(semester > totalSemester){
        cout << "恭喜你結束大學生活! 接下來要進行畢業結算" << endl;
        return false;
    }
    cout << "恭喜你結束本學期!" << endl << "請按 Y 或 N 決定是否要進入下學期" << endl;
    while(true){
        string decision;
        cin>> decision;
        if(decision == "Y" || decision == "y")// yes 回傳
        {
            cout << "現在開始第" << semester << "學期 " << endl;
            return true;
        }
        else if(decision == "N" || decision == "n") //no 回傳
        {
            return false;
        }
        else
            cout<<"輸入錯誤，請輸入 Y 或 N"<<endl;
    }
}

void Game::miniGame1(){
    int gcd(int, int);
    //決定數字
    srand(time(NULL));
    int sol1Num = 0, sol1Den = 0, sol2Num = 0, sol2Den = 0;
    sol1Num = 1 + rand() % 20;
    sol1Den = 1 + rand() % 10;
    sol2Num = 1 + rand() % 20;
    sol2Den = 1 + rand() % 10;
    //將答案約成最簡分數
    int gcdSol1 = gcd(sol1Den, sol1Num);
    int gcdSol2 = gcd(sol2Den, sol2Num);
    sol1Den /= gcdSol1;
    sol1Num /= gcdSol1;
    sol2Den /= gcdSol2;
    sol2Num /= gcdSol2;
    //一半的機率改變正負
    if(rand() % 2 == 0){
        sol1Num = -sol1Num;
    }
    if(rand() % 2 == 0){
        sol2Num = -sol2Num;
    }
    //輸出方程式
    int a = 0, b = 0, c = 0;
    a = sol1Den * sol2Den;
    b = -(sol1Den * sol2Num + sol2Den * sol1Num);
    c = sol1Num * sol2Num;
    cout << "提示：必為有理根，輸入格式為 a/b c/d（例如答案為 -3 或 5，則輸入 -3/1 5/1）\n";
    cout << a << "x^2 ";
    if(b > 0)
        cout << "+" << b;
    else if(b < 0)
        cout << b;
    cout << "x ";
    if(c > 0)
        cout << "+" << c;
    else if(c < 0)
        cout << c;
    cout << " = 0" << endl;
    //判斷答案
    int ans1Num = 0, ans1Den = 0, ans2Num = 0, ans2Den = 0;
    cin >> ans1Num;
    cin.ignore();
    cin >> ans1Den >> ans2Num;
    cin.ignore();
    cin >> ans2Den;
    if(ans1Num == sol1Num && ans1Den == sol1Den && ans2Num == sol2Num && ans2Den == sol2Den)
        cout << "正確" << endl;
    else if(ans1Num == sol2Num && ans1Den == sol2Den && ans2Num == sol1Num && ans2Den == sol1Den)
        cout << "正確" << endl;
    else{
        cout << "錯誤" << endl;
        player.changeAllScore(-10);
    }
        
}

void Game::miniGame2(){
    cout << "輸入四個數字" << endl;
    string numstr;
    int A = 0;
    int B = 0;
    int answer[4] = {0};
    int keyIn[4] = {0};
    int Anscopy[4] = {0};
    int tryCnt = 0;
    int max = 10+rand()%11;
    makerand(answer);
    while(A != 4){   
        tryCnt++;
        cin >> numstr;
        if(tryCnt>max || numstr == "Q" || numstr == "q"){
            cout << endl;
            cout<<"你失敗了 ";
            player.changeAllScore(-10);
            break;
        }
        if(numstr.length() != 4){
            cout << "請輸入四個數字" << endl;
            tryCnt--;
            continue;
        }
        if(numstr.length() == 4){
            bool wronginput = false;
            for(int i = 0; i < 4; i++){
                if(!isdigit(numstr[i])){
                    cout << "請輸入四個數字" << endl;
                    wronginput = true;
                    tryCnt--;
                    break;
                }
            }
            if(wronginput)
                continue;
        }
        strToInt(keyIn, numstr);
        makecopy( answer, Anscopy);
        A = findA( Anscopy, keyIn);
        B = findB( Anscopy , keyIn);
        if( A != 4){
            cout << "<" << A << "A" << B << "B" << ">" << endl;
            cout << "剩餘次數: " << max - tryCnt;
            if(tryCnt > max-2)
                cout << " 沒救了...";
            else if (tryCnt > max-6)
                cout << " 請加油好嘛";  
            else
                cout << " 請繼續嘗試";
            cout << "，按 Q 可放棄" << endl;   
        }
        else if( A==4)
          cout<<"恭喜你成功!"<<endl;
    }
    return;
}


bool Game::countPassFail(){
    cout << endl;
    cout << "<期末結算>" << endl;
    return player.countpassfail();
}

void Game::theEnd(){
    bool allRequiredPass = true;
    for(int i = 0; i < requiredCourses.size(); i++){
        if(requiredCourses[i]->getSemester() < this->semester){
            if(!player.isPassed(requiredCourses[i]->getID())){
                player.addcourse(*requiredCourses[i]);
                allRequiredPass = false;
            }
        }
    }
    
    cout << "<畢業結算>" << endl;
    cout << "目標學分：" << goalCredit << endl;
    cout << "總學分：" << player.getTotalCredit() << endl;
    if(player.getTotalCredit() < goalCredit){
        cout << "學分不足，畢業失敗！" << endl;
    }
    else if(!allRequiredPass){
        cout << "未通過必修：" << endl;
        player.printCourse();
        cout << "必修未通過，畢業失敗！" << endl;
    }
    else{
        cout << "未通過必修：無" << endl;
        cout << "恭喜你成功畢業！" << endl;
    }
}

bool Game::isWeek(int week) const{
    return (player.getWeek() == week ? true : false);
}

void Game::addEventsDefault(string eventName, string eventdetail, int scorechange, int mood, int type){
    EventDefault* EvnPtr = new EventDefault(eventName, eventdetail, scorechange, mood, type);
    events.push_back(EvnPtr);
}
void Game::addEventsOne(string eventName, string eventdetail, int scorechange, int mood, int type, int scorechange1, int mood1){
    EventOne* EvnPtr = new EventOne(eventName, eventdetail, scorechange, mood, type, scorechange1, mood1);
    events.push_back(EvnPtr);
}
void Game::addRequiredCourse(string name, int credit, int scoreVari, int semester){
    RequiredCourse* coursePtr = new RequiredCourse(name, credit, scoreVari, semester);
    requiredCourses.push_back(coursePtr);
}
void Game::addElectiveCourse(string name, int credit, int scoreVari, double assignedProb){
    ElectiveCourse* coursePtr = new ElectiveCourse(name, credit, scoreVari, assignedProb);
    electiveCourses.push_back(coursePtr);
}


//initialize static member
int Course::maxNameLen = 0;
int Course::courseCnt = 0;
int RequiredCourse::requiredCnt = 0;
int ElectiveCourse::electiveCnt = 0;

int main(){
    srand(time(NULL));
    int totalSemester = 8, weekNum = 16, goalCredit = totalSemester * 8;
    string name;
    cout << "遊戲規則:" << endl << "規則" << endl;
    cout << "請輸入你的姓名: " << endl;
    cin >> name;
    Game theGame(totalSemester, weekNum, name, goalCredit);

    ifstream Event_default;
    ifstream Event_one;
    ifstream Required_Course;
    ifstream Elective_Course;
    Event_one.open("event_one.txt");
    if(Event_one){
        string eventName, eventDetail;
        int scoreChange, scoreChange1, mood, mood1, type;
        while(Event_one >> eventName >> eventDetail >> scoreChange >> mood >> type >> scoreChange1 >> mood1){
            theGame.addEventsOne(eventName, eventDetail, scoreChange, mood, type, scoreChange1, mood1);
        }
    }
    Event_default.open("eventdefault.txt");
    if(Event_default){
        string eventName, eventDetail;
        int scoreChange, mood, type;
        while(Event_default >> eventName >> eventDetail >> scoreChange >> mood >> type){
            theGame.addEventsDefault(eventName, eventDetail, scoreChange, mood, type);
        }
    }

    Required_Course.open("RequiredCourses.txt");
    if(Required_Course){
        string name;
        int credit = 0, scoreVari = 0, semester = 0;
        string ignore;
        Required_Course >> ignore;
        while(Required_Course >> semester >> credit >> scoreVari >> name){
            theGame.addRequiredCourse(name, credit, scoreVari, semester);
        }
    }

    Elective_Course.open("ElectiveCourses.txt");
    if(Elective_Course){
        string name;
        int credit = 0, scoreVari = 0;
        double assignProb = 0;
        string ignore;
        Elective_Course >> ignore;
        while(Elective_Course >> credit >> scoreVari >> assignProb >> name){
            theGame.addElectiveCourse(name, credit, scoreVari, assignProb);
        }
    }

    /*cout << "Customize your semesters in college" << endl;
    cin >> totalSemester;
    cout << "Customize your weeks per semester" << endl;
    cin >> weekNum;//需小於 MAX_WEEK_NUM = 18
    goalCredit = totalSemester * 8*/

    while(theGame.getSemester() <= totalSemester){
        if(theGame.isWeek(0)){
            theGame.printCourse();
            theGame.chooseCourse();
        }
        theGame.dice();
        theGame.printMap();
        if(theGame.isWeek(totalSemester / 2)){
            cout << "你進入了期中周，現在要解一元二次方程式。如果答錯，所有課程將扣 10 分，答對則順利度過期中周。" << endl;
            theGame.miniGame1();
        }
        else if(theGame.isWeek(totalSemester)){
            cout << "你進入了期末周，現在要玩幾A幾B(可能有重複數字)。如果輸了所有課程將扣 10 分。" << endl;
            theGame.miniGame2();
            if(!theGame.countPassFail())
                break;
            if(!theGame.nextSemester())
                break;            
        }
        else{
            if(!theGame.event())
                break;
        }
    }
    theGame.theEnd();
    Event_default.close();
    Event_one.close();
    Required_Course.close();
    Elective_Course.close();
    return 0;
}

void makecopy(int answer[], int Anscopy[])
{
    for(int i = 0; i < 4; i++)
        Anscopy[i] = answer[i];
}
void strToInt(int keyIn[], string numstr)
{
    string a;
    for(int i = 0; i < 4; i++)
    {
       a = numstr[i];
       keyIn[i] = stoi(a);
    }
}
void makerand(int answer[])
{
    for(int i = 0; i < 4; i++)
        answer[i] = rand()%10;
}
int findA(int Anscopy[], int keyIn[])
{
   int A = 0;
   for(int i = 0; i < 4; i++)
   {
      if(Anscopy[i] == keyIn[i])
      {
         A++;
         Anscopy[i] = -1;
         keyIn[i] = -2;
      }
   }
   return A;
}
int findB(int Anscopy[], int keyIn[])
{
    int B = 0;
    for(int i = 0 ; i < 4; i++)
    {
        for(int j=0; j < 4; j++)
        {
            if(Anscopy[i] == keyIn[j])
            {
                B++;
                keyIn[j] = -2;
                Anscopy[i] = -1;
            }
        }
    }
    return B;
}

int gcd(int a, int b){
    if(a >= b){
        if(b == 0)
            return a;
        else
            return gcd(b, a % b);
    }
    else{
        if(a == 0)
            return b;
        else
            return gcd(a, b % a);
    }
}