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
            cout<<"按 Y 或 N 選擇要不要做";
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
    //初始化 courseCnt 和 maxNameLen
    static void init();
};

class RequiredCourse : public Course{
private:
    const int semester;//第幾學期的必修
    static int requiredCnt;//必修課程數量
public:
    RequiredCourse(string n, int c, int sv, int s);
    int getSemester() {return semester;}
    static void init();//初始化 requiredCnt
};

/*class ElectiveCourse : public Course{
private:
    static int electiveCnt;//選修課程數量
public:
    ElectiveCourse(string n, int c, int sv, double ap);
    static void init();//初始化 electiveCnt
};
*/

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

void Course::init(){
    maxNameLen = 0;
    courseCnt = 0;
}

RequiredCourse::RequiredCourse(string name, int credit, int scoreVari, int semester):
Course(requiredCnt + 101, name, credit, scoreVari, 1, true), semester(semester){
    requiredCnt++;
}

void RequiredCourse::init(){
    Course::init();
    requiredCnt = 0;
}

/*
ElectiveCourse::ElectiveCourse(string name, int credit, int scoreVari, double assignedProb):
Course(electiveCnt + 201, name, credit, scoreVari, assignedProb, false){
    electiveCnt++;
}
*/
class Player{
    protected:
        const string name;//玩家名稱
        vector<Course*> courselist;//本學期修課列表
        vector<Course*> passcourse;//已通過課程列表
        int totalCredit;//已修學分數
        int semesterCredit;//本學期學分數
        int mood;//心情值
        int basicScore;//基本分數，由課程數量決定
        int week;//目前在第幾週
    public:
        Player(string n);
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
        //期末結算
        void countpassfail();
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
        //coursrelist 中是否有此課程 id
        bool idExist(int id) const;
        //passcourse 中是否有此課程 id
        bool isPassed(int id) const;
};

Player::Player(string n):name(n){
    semesterCredit = 0;
    totalCredit = 0;
    mood = 100;
    basicScore = 0;
    week = 0;
}
int Player::getBasicscore(){
    basicScore = 80 - semesterCredit * 5;//應該只計算這學期的吧？
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
void Player::clearcourse(){
    courselist.clear();
    mood = 100;
    basicScore = 0;
    week = 0;
    semesterCredit = 0;
}
bool Player::changeMood(int moodchange){
    mood += moodchange;
    cout << "你的心情 " << (moodchange >= 0? "+":"") << moodchange << " 分 ";
    cout << "你的心情值現在是 " << mood << endl << endl;
    if(mood < 0)
        return false;
    else
        return true;
}
void Player::changeScore(int ID, int scorechange){
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
    for(int i = 0; i < courselist.size(); i++){
        courselist[i]->changeScore(scorechange);
        cout << courselist[i]->getName() << " " << (scorechange >= 0? "+":"") << scorechange << " 分" << endl;
        cout << courselist[i]->getName() << " 目前的分數為 " << this->getBasicscore() + courselist[i]->getScore() << endl;
        return;
    }
}

void Player::countpassfail(){
    int basicScore = getBasicscore();
    Course::printINCTitle();
    cout << "分數" << "\t" << "結果";

    for(int i = 0; i < courselist.size(); i++){
        int score = basicScore + courselist[i]->getScore();
        courselist[i]->printIdNameCredit();
        cout << score << "\t";
        if(score < 60){
            cout << "Fail" << endl;
        }
        else{
            cout << "Pass" << endl;
            totalCredit += courselist[i]->getCredit();
            addPasscourse(*courselist[i]);
        }
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
    /*vector<ElectiveCourse*> electiveCourses;//列表所有選修課程*/
    Player player;//單人玩家
public:
    Game(int totalSemester, int weekNum, string name, int goalCredit);
    int getSemester() const {return semester;}
    //印出本學期課程，並加入player course
    void printCourse();
    /*//選課
    void chooseCourse();*/
    //丟骰子，移動
    void dice();
    //印地圖
    void printMap();
    //執行事件
    void event();
    //期中或期末遊戲
    void miniGame();
    //進入下一個學期
    void nextSemester();
    //結算
    void countPassFail();
    //最終大結算
    void theEnd();
    //玩家是否在第 0 週
    bool isWeek0() const;
    //玩家是否在期中週
    bool isMid() const;
    //玩家是否在期末週
    bool isFinal() const;
    //輸入event_default
    void addEventsDefault(string eventName, string eventdetail, int scorechange, int mood, int type);
    //輸入event_one
    void addEventsOne(string eventName, string eventdetail, int scorechange, int mood, int type, int scorechange1, int mood1);
    //輸入必修課程
    void addRequiredCourse(string name, int credit, int scoreVari, int semester);
    /*//輸入選修課程
    void addElectiveCourse(string name, int credit, int scoreVari, double assignedProb);*/
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
}

void Game::dice(){
    cout << "按 y 或 Y 來擲骰子" << endl;
    int step = 0;
    string throwDice;
    while(cin >> throwDice){
        if(throwDice == string("Y") || throwDice == string("y")){
            step = 1 + rand() % 3;
            cout << "你移動了 " << step << " 格" << endl;
            break;
        }  
        else
            cout << "按 y 或 Y 來擲骰子" << endl;
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

void Game::event(){
    int index = rand() % events.size();
    int scoreChange = 0, moodChange = 0;
    /*0 or 1: 隨機選一堂課
    2: 請玩家選擇課程
    3: 所有課程*/
    int type = events[index]->eventHappened(scoreChange, moodChange);
    int id = 0;

    if(type == 0 || 1){
        id = player.randomID();
        player.changeScore(id, scoreChange);
        player.changeMood(moodChange);
    }
    else if(type == 2){
        cout << "請選擇一堂課，輸入課程代號" << endl;
        player.printCourse();
        while(true){
            cin >> id;
            if(player.idExist(id))
                break;
            else
                cout << "課程不存在，請再輸入一次" << endl;
        }
        player.changeScore(id, scoreChange);
        player.changeMood(moodChange);
    }
    else if(type == 3){
        player.changeAllScore(scoreChange);
        player.changeMood(moodChange);
    }
}
void Game::nextSemester(){
    semester++;
}
void Game::miniGame(){
    cout << "輸入四個數字" << endl;
    srand(time(NULL));
    string numstr;
    int A = 0;
    int B = 0;
    int answer[4] = {0};
    int keyIn[4] = {0};
    int Anscopy[4] = {0};
    int tryCnt = 0;
    makerand(answer);
    while(A != 4){   
        tryCnt++;
        if(tryCnt>20){
            cout<<"你失敗了，所有課程被扣10分";
            player.changeAllScore(-10);
            break;
        }
        cin >> numstr;
        if(numstr.length() != 4){
            cout << "請輸入四個數字";
            tryCnt--;
            continue;
        }
        strToInt(keyIn, numstr);
        makecopy( answer, Anscopy);
        A = findA( Anscopy, keyIn);
        B = findB( Anscopy , keyIn);
        if( A != 4){
            cout << "<" << A << "A" << B << "B" << ">" << endl;
            cout << "剩餘次數: " << 20 - tryCnt <<" 請繼續嘗試"<<endl;
        }
        else if( A==4)
          cout<<"恭喜你成功!"<<endl;
    }
    return;
}


void Game::countPassFail(){
    cout << "期末結算" << endl;
    player.countpassfail();
}

void Game::theEnd(){
    cout << "畢業結算" << endl;
    cout << "目標學分：" << goalCredit << endl;
    cout << "總學分：" << player.getTotalCredit() << endl;
    if(player.getTotalCredit() >= goalCredit){
        //cout << "恭喜你成功畢業！" << endl;
    }
    else{
        cout << "畢業失敗，請再加油！" << endl;
    }
}

bool Game::isWeek0() const{
    return (player.getWeek() == 0 ? true : false);
}

bool Game::isMid() const{
    int mid = weekNum / 2;
    return (player.getWeek() == mid ? true : false);
}

bool Game::isFinal() const{
    return (player.getWeek() == weekNum ? true : false);
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
/*void Game::addElectiveCourse(string name, int credit, int scoreVari, double assignedProb){
    ElectiveCourse* coursePtr = new RequiredCourse(name, credit, scoreVari, assignedProb);
    requiredCourses.push_back(coursePtr);
}*/


//initialize static member
int Course::maxNameLen = 0;
int Course::courseCnt = 0;
int RequiredCourse::requiredCnt = 0;

int main(){
    Course::init();
    srand(time(NULL));
    int totalSemester = 8, weekNum = 16, goalCredit = 64;
    string name;
    cout << "請輸入你的姓名: " << endl;
    cin >> name;
    Game theGame(totalSemester, weekNum, name, goalCredit);

    ifstream Event_default;
    ifstream Event_one;
    ifstream Required_Course;
    Event_default.open("eventdefault.txt");
    if(Event_default){
        string eventName, eventDetail;
        int scoreChange, mood, type;
        while(Event_default >> eventName >> eventDetail >> scoreChange >> mood >> type){
            theGame.addEventsDefault(eventName, eventDetail, scoreChange, mood, type);
        }
    }
    Event_one.open("event_one.txt");
    if(Event_one){
        string eventName, eventDetail;
        int scoreChange, scoreChange1, mood, mood1, type;
        while(Event_one >> eventName >> eventDetail >> scoreChange >> mood >> type >> scoreChange1 >> mood1){
            theGame.addEventsOne(eventName, eventDetail, scoreChange, mood, type, scoreChange1, mood1);
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
    /*cout << "Customize your semesters in college" << endl;
    cin >> totalSemester;
    cout << "Customize your weeks per semester" << endl;
    cin >> weekNum;//需小於 MAX_WEEK_NUM = 18
    cout << "Customize your goal of credits" << endl;
    cin >> goalCredit;*/
    cout << "遊戲規則:" << endl << "規則";
    while(theGame.getSemester() <= totalSemester){
        if(theGame.isWeek0()){
            theGame.printCourse();
        }
        theGame.dice();
        theGame.printMap();
        if(theGame.isMid()){
            cout << "你進入了期中周，現在要玩幾A幾B。如果輸了所有課程將扣10分，贏了則順利度過期中周。";
            theGame.miniGame();
        }
        else if(theGame.isFinal()){
            theGame.miniGame();
            theGame.countPassFail();
            theGame.nextSemester();
        }
        else{
            theGame.event();
        }
    }
    theGame.theEnd();
    Event_default.close();
    Event_one.close();
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
