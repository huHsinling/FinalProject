#include<iostream>
#include<fstream>
#include<string>
#include<ctime>
#include<vector>
using namespace std;
//1207test
//這是曾子恩打的
//I'm a good boy
//曾子恩建立了一個分支叫bug1然後打了這句話

class Events{
    protected:
        const string eventName; //事件名稱
        const string eventDetail; //事件敘述，可以加一些故事情節之類的？
        const int scoreChange; //此事件會改變多少分數
        const int mood; //心情值
    public:
        Events(string eventName, string eventDetail, int scoreChange, int mood);
        virtual void eventHappened() const = 0; //執行事件
        string getEventName();
        string getEventDetail();
        ~Events(){};
};
Events::Events(string eventName, string eventDetail, int scoreChange, int mood):
eventName(eventName), eventDetail(eventDetail), scoreChange(scoreChange), mood(mood){

}

string Events::getEventName(){
    return eventName;
}
string Events::getEventDetail(){
    return eventDetail;
}

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
    void print() const;
    static void printTitle();
    void changeScore(int score);
    int getID(){return id;};
    string getName(){return name;};
};

/*class RequiredCourse : public Course{
private:
    const int semester;//第幾學期的必修
    static int requiredCnt;//必修課程數量
public:
    RequiredCourse(string n, int c, int sv, int s);
};

class ElectiveCourse : public Course{
private:
    static int electiveCnt;//選修課程數量
public:
    ElectiveCourse(string n, int c, int sv, double ap);
};
放到main function
Course::maxNameLen = 0;
Course::courseCnt = 0;
RequiredCourse::requiredCnt = 0;
ElectiveCourse::electiveCnt = 0;*/
Course::Course(int id, string name, int credit, int scoreVari, double assignedProb, bool isRequired):
id(id), name(name), credit(credit), scoreVari(scoreVari), assignedProb(assignedProb), isRequired(isRequired){
    if(name.length() > maxNameLen)
        maxNameLen = name.length();
    courseCnt++;
}
void Course::print() const{
    cout << id << ". " << name;
    int n = ((maxNameLen - name.length()) / 8) + 1;
    for(int i = 0; i < n; i++)
        cout << "\t";
    cout << credit << "\t";
    if(scoreVari >= 0)
        cout << "+" << scoreVari;
    else
        cout << scoreVari;
    cout << "\t" << assignedProb * 100 << "%";
    cout << endl;
}
void Course::printTitle(){
    cout << "編號 課程名稱";
    int n = ((maxNameLen - 8) / 8) + 1;
    for(int i = 0; i < n; i++)
        cout << "\t";
    cout << "學分" << "\t" << "難度" << "\t" << "選上機率";
    cout << endl;
}
void Course::changeScore(int score){
    scoreVari += score;
}
/*
RequiredCourse::RequiredCourse(string name, int credit, int scoreVari, int semester):
Course(requiredCnt + 101, name, credit, scoreVari, 1, true, semester){
    requiredCnt++;
}

ElectiveCourse::ElectiveCourse(string name, int credit, int scoreVari, double assignedProb):
Course(electiveCnt + 201, name, credit, scoreVari, assignedProb, false, 0){
    electiveCnt++;
}
*/
class Player{
    protected:
        string name;
        vector<Course*> courselist;
        vector<Course*> passcourse;
        int courseCnt;
        int totalCredit;
        int mood;
        int basicScore;
        int week;//目前在第幾週
    public:
        Player(string n);
        int getBasicscore();
        void changeScore(int ID, int scorechange);
        bool changeMood(int moodchange);
        void addcourse(Course& choosedcourse);
        void addPasscourse(Course& passcourse);
        void countpassfail();
        void clearcourse();
        void move(int step);//丟骰後移動到第幾格
};
Player::Player(string n):name(n){
    mood = 100;
}
int Player::getBasicscore(){
    return 80 - totalCredit * 5;
}
void Player::addcourse(Course& choosedcourse){
    Course* thecourse = new Course (choosedcourse);
    courselist.push_back(thecourse);
    courseCnt++;
}
void Player::addPasscourse(Course& choosedcourse){
    Course* thecourse = new Course (choosedcourse);
    this->passcourse.push_back(thecourse);
}
void Player::clearcourse(){
    courselist.clear();
    int courseCnt = 0;
    int mood = 100;
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
    for(int i = 0; i < courseCnt; i++){
        if(courselist[i]->getID() == ID){
            cout << courselist[i]->getName() << scorechange << "points " << endl;
            cout << "The current score of this class is " << this->getBasicscore() - scorechange[i] >> endl;
            return;
        }
    }
    int random = rand() % (courseCnt);
    scorechange[random] += scorechange;
    cout >> coureselist[random].name >> scorechange >> "points" >> endl;
    return;
}
void Player::countpassfail(){
    int basicscore = getBasicscore();
    for(int i = 0; i < courseCnt; i++){
        int score = basicScore + scorechange[i];
        cout << courselist[i]->getName() << " Credit: " << courselist[i]->getCredit() << " Score: " << score;
        if(score < 60){
            cout << " Fail" << endl;
        }
        else{
            cout << " Pass" << endl;
            totalCredit += courselist[i]->getCredit();
            addPasscourse(*courselist[i]);
        }
    }
}

void Player::move(int step){
    week = 
}

class TimeMap
{
    private:
        string semester[17];
        int stepCnt;
        int dizeTime;
        const int semesterNum;
        int process;
    public:
        TimeMap(int PlayNum);
        void print();
        void printForm();
        int getstepCnt();
        int getsemesterNum() const;
        int getprocess();
        ~TimeMap();
};
TimeMap:: TimeMap(int PlayNum) : semesterNum(PlayNum)
{
    stepCnt = 0;
    dizeTime = 0;
    process = 1;
    for(int i=0; i<17; i++)
    {
           semester[i] = "week"+to_string(i);
    }
}
TimeMap::~TimeMap()
{}
int TimeMap::getstepCnt()
{
    return stepCnt;
}
int TimeMap::getsemesterNum() const
{
    return semesterNum;
}
int TimeMap::getprocess()
{
    return process;
}
void TimeMap::printForm()
{
    for(int i = 0; i<8; i++)
        cout<<semester[i]<<" ";
    cout<<endl;
    for(int i = 8; i<17; i++)
        cout<<semester[i]<<" ";
}
void TimeMap::print()
{
    cout<<"press y or Y to throw the dice"<<endl;
    int step = 0;
    string throwDize;
    cin>>throwDize;
    if(process<=this->getsemesterNum())
    {
        if(throwDize== string("Y") or throwDize == string("y"))
        {
            if(dizeTime!=0)
            {
                semester[stepCnt].erase(0,1);
                step = 1 + rand()%3;
                stepCnt += step;
                if(stepCnt>16)
                    stepCnt = 16;
                semester[stepCnt] = "*" + semester[stepCnt];
            }
            else
                semester[stepCnt] = "*" + semester[stepCnt];
            printForm();
            dizeTime++;
            if(stepCnt==16)
            {   
                semester[16].erase(0,1);
                dizeTime = 0;
                stepCnt = 0;
                process++;
            }

        }
        else
            cout<<"Please type again"<<endl;
    }
    cout<<endl;
}

class Game{
private:
    int totalSemester;//總共幾學期
    int semester;//目前是第幾學期
    
    vector<Events> events;//列表所有事件
    Player player;//單人玩家
    TimeMap map;//地圖
public:
    Game(int totalSemester);
    int getSemester() const {return semester;}
    int getWeek() const {return week;}
    //void Choose();//選課
    void dice();//丟骰子
    void event();//執行事件
    void miniGame();//期中或期末遊戲
    void nextSemester();//進入下一個學期
    void countPassFail();//結算
    void theEnd();//最終大結算
    
};
Game::Game(int totalSemester): totalSemester(totalSemester), semester(1), map(totalSemester), player(/*待補*/){
}
void Game::dice(){
    map.print();
    player.move(get);
}
void Game::event(){
    
}
void Game::nextSemester(){
    semester++;
}
void Game::miniGame(){

}

int main(){
    
    srand(time(NULL));
    int totalSemester = 0;
    cout << "Customize your semesters in college" << endl;
    cin >> totalSemester;

    Game theGame(totalSemester);

    while(theGame.getSemester() <= totalSemester){
        theGame.dice();
        if(theGame.getWeek() == 8){
            theGame.miniGame();
        }
        else if(theGame.getWeek() == 16){
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
