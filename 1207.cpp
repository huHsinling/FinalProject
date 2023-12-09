#include<iostream>
#include<fstream>
#include<string>
#include<ctime>
#include<vector>
using namespace std;

class Events{
    protected:
        const string eventName; //事件名稱
        const string eventDetail; //事件敘述，可以加一些故事情節之類的？
        const int scoreChange; //此事件會改變多少分數
        const int mood; //心情值
        const int type;// type == 0 不做任何事 ； type == 1 yes or no ; type == 2 選擇課
    public:
        Events(string eventName, string eventDetail, int scoreChange, int mood, int type): eventName(eventName), eventDetail(eventDetail), scoreChange(scoreChange), mood(mood),  type(type)
        {}
        virtual int eventHappened(int& scoreChange, int& moodChange) const = 0; //執行事件，回傳 type
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
        int eventHappened(int& scoreChange, int& moodChange)
        {
            string decision;
            cout<<eventDetail<<endl;
            cin>>decision;
            cout<<endl;
            while(true)
            {
                cin>>decision;
                if(decision == "Y" or decision == "y")// yes 回傳
                {
                    scoreChange = this->scoreChange;
                    moodChange = this->mood;
                    break;
                }
                else if(decision == "N" or decision == "n") //no 回傳
                {
                    scoreChange = this->scorechange1;
                    moodChange = this->mood1;
                    break;
                }
                else
                    cout<<"Input Error. Please type again"<<endl;
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
    void printAll() const;//選課時印出所有課程資訊
    void printIdName() const;//事件選擇時印出 id 及 name
    void printIdNameCredit() const;//結算時印出 id, name, credit
    static void printTitle();//選課時印出所有課程資訊的標題
    static void printINCTitle();//結算時印出 id, name, credit 的標題
    void changeScore(int score);
    int getID()const{return id;};
    int getScore()const{return scoreVari;};
    string getName()const{return name;};
    int getCredit()const{ return credit; };
    static void init();//初始化 courseCnt 和 maxNameLen
};

class RequiredCourse : public Course{
private:
    const int semester;//第幾學期的必修
    static int requiredCnt;//必修課程數量
public:
    RequiredCourse(string n, int c, int sv, int s);
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
    int n = ((maxNameLen - 8) / 8) + 1;
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
        int mood;//心情值
        int basicScore;//基本分數，由課程數量決定
        int week;//目前在第幾週
    public:
        Player(string n);
        int getBasicscore();
        int getWeek() const {return week;}
        void changeScore(int ID, int scorechange);//改變某課程的分數
        bool changeMood(int moodchange);//改變心情值
        void addcourse(Course& choosedcourse);//將課程加入 courselist
        void addPasscourse(Course& passcourse);//將課程加入 passcourse
        void countpassfail();
        void clearcourse();
        void move(int step);//丟骰後移動 step 格
        void moveToWeek(int week);//移動到第 week 格
        int randomID() const;//回傳 courselist 中隨機一個課程的 id
        void printCourse() const;//印出 courselist 所有課程的 id 及 name
        bool idExist(int id) const;//coursrelist 中是否有此課程 id
};
Player::Player(string n):name(n){
    totalCredit = 0;
    mood = 100;
    basicScore = 0;
    week = 0;
}
int Player::getBasicscore(){
    basicScore = 80 - totalCredit * 5;//應該只計算這學期的吧？
    return basicScore;
}
void Player::addcourse(Course& choosedcourse){
    Course* thecourse = new Course (choosedcourse);
    courselist.push_back(thecourse);
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
}
bool Player::changeMood(int moodchange){
    mood += moodchange;
    cout << "Your mood is now " << mood << endl;
    if(mood < 0)
        return false;
    else
        return true;
}
void Player::changeScore(int ID, int scorechange){
    for(int i = 0; i < courselist.size(); i++){
        if(courselist[i]->getID() == ID){
            courselist[i]->changeScore(scorechange);
            cout << courselist[i]->getName() << scorechange << "points " << endl;
            cout << "The current score of this class is " << this->getBasicscore() + courselist[i]->getScore() << endl;
            return;
        }
    }
}
void Player::countpassfail(){
    int basicscore = getBasicscore();
    Course::printINCTitle();
    cout << "結果";

    for(int i = 0; i < courselist.size(); i++){
        int score = basicScore + courselist[i]->getScore();
        courselist[i]->printIdNameCredit();
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

const int MAX_WEEK_NUM = 18;

class Game{
private:
    const int weekNum;//一學期有幾個禮拜
    vector<string> weeks;//每個禮拜的敘述
    const int totalSemester;//總共幾學期
    int semester;//目前是第幾學期
    vector<Events*> events;//列表所有事件
    vector<Course*> courses;//列表所有課程
    Player player;//單人玩家
public:
    Game(int totalSemester, int weekNum, string name);
    int getSemester() const {return semester;}
    //void Choose();//選課
    void dice();//丟骰子，移動
    void printMap();//印地圖
    void event();//執行事件
    void miniGame();//期中或期末遊戲
    void nextSemester();//進入下一個學期
    void countPassFail();//結算
    void theEnd();//最終大結算
    bool isWeek0() const;//玩家是否在第 0 週
    bool isMid() const;//玩家是否在期中週
    bool isFinal() const;//玩家是否在期末週
    
};
Game::Game(int totalSemester, int weekNum, string name): totalSemester(totalSemester), semester(1), weekNum(weekNum), player(name){
    for(int i = 0; i < MAX_WEEK_NUM; i++){
        weeks.push_back("week" + to_string(i));
    }
}
void Game::dice(){
    cout << "press y or Y to throw the dice" << endl;
    int step = 0;
    string throwDize;
    cin >> throwDize;
    if(throwDize == string("Y") || throwDize == string("y")){
        step = 1 + rand() % 3;
        cout << "You moved " << step << (step == 1 ? " step" : " steps");
    }  
    else
        cout << "Please type again" << endl;
    cout << endl;
    
    player.move(step);
    if(player.getWeek() >= weekNum)
        player.moveToWeek(weekNum);
}

void Game::printMap(){
    int midWeek = weekNum / 2;
    for(int i = 0; i < weekNum; i++){
        cout << (i == player.getWeek() ? "*" : "") << weeks[i]; 
        cout << (i == midWeek ? "\n" : "\t");
    }
}

void Game::event(){
    int index = rand() % events.size();
    int scoreChange = 0, moodChange = 0;
    int type = events[index]->eventHappened(scoreChange, moodChange);
    int id = 0;

    //type = 0 or 1: 隨機選一堂課
    if(type == 0 || 1){
        id = player.randomID();
    }

    //type = 2: 請玩家選擇課程
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
    }

    player.changeScore(id, scoreChange);
    player.changeMood(moodChange);
    
}
void Game::nextSemester(){
    semester++;
}
void Game::miniGame(){

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

int main(){
    Course::init();
    srand(time(NULL));
    int totalSemester = 8, weekNum = 16;
    string name;
    cout << "Enter your name" << endl;
    cin >> name;
    /*cout << "Customize your semesters in college" << endl;
    cin >> totalSemester;
    cout << "Customize your weeks per semester" << endl;
    cin >> weekNum;*/
    

    Game theGame(totalSemester, weekNum, name);

    while(theGame.getSemester() <= totalSemester){
        if(theGame.isWeek0()){

        }
        theGame.dice();
        theGame.printMap();
        if(theGame.isMid()){
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

    return 0;
}
