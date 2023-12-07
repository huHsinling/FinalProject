#include<iostream>
#include<fstream>
#include<string>
#include<ctime>
#include<vector>
using namespace std;
//1207test
//這是曾子恩打的
//I'm a good boy
//version2  
//one
//two


//branchtest!!!
class Events{
    protected:
        const string eventName; //事件名稱
        const string eventDetail; //事件敘述，可以加一些故事情節之類的？
        const int scoreChange; //此事件會改變多少分數
        const int mood; //心情值
    public:
        Events(string eventName, string eventDetail, int scoreChange, int mood): eventName(eventName), eventDetail(eventDetail), scoreChange(scoreChange), mood(mood)
        {}
        void eventHappened() const ; //執行事件
        string getEventName();
        string getEventDetail();
        ~Events(){};
};

string Events::getEventName(){
    return eventName;
}
string Events::getEventDetail(){
    return eventDetail;
}
void Events::eventHappened() const
{
    cout<<eventDetail<<endl;
    

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
    int getID()const{return id;};
    int getScore()const{return scoreVari;};
    string getName()const{return name;};
    int getCredit()const{ return credit; };
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
*/

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
};
Player::Player(string n):name(n){
    totalCredit = 0;
    mood = 100;
    basicScore = 0;
    week = 0;
}
int Player::getBasicscore(){
    return 80 - totalCredit * 5;//應該只計算這學期的吧？
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
            courselist[i]->changeScore(scorechange);
            cout << courselist[i]->getName() << scorechange << "points " << endl;
            cout << "The current score of this class is " << this->getBasicscore() + courselist[i]->getScore() << endl;
            return;
        }
    }
}
void Player::countpassfail(){
    int basicscore = getBasicscore();
    for(int i = 0; i < courseCnt; i++){
        int score = basicScore + courselist[i]->getScore();
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
    week += step;
}

void Player::moveToWeek(int week){
    this->week = week;
}

const int MAX_WEEK_NUM = 18;

class Game{
private:
    const int weekNum;//一學期有幾個禮拜
    vector<string> weeks;//每個禮拜的敘述
    const int totalSemester;//總共幾學期
    int semester;//目前是第幾學期
    vector<Events*> events;//列表所有事件
    Player player;//單人玩家
public:
    Game(int totalSemester, int weekNum);
    int getSemester() const {return semester;}
    //void Choose();//選課
    void dice();//丟骰子，移動
    void printMap();//印地圖
    void event();//執行事件
    void miniGame();//期中或期末遊戲
    void nextSemester();//進入下一個學期
    void countPassFail();//結算
    void theEnd();//最終大結算
    
};
Game::Game(int totalSemester, int weekNum): totalSemester(totalSemester), semester(1), weekNum(weekNum), player(/*待補*/){
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
    
}
void Game::nextSemester(){
    semester++;
}
void Game::miniGame(){

}

int main(){
    Course::maxNameLen = 0;
    Course::courseCnt = 0;
    srand(time(NULL));
    int totalSemester = 0;
    cout << "Customize your semesters in college" << endl;
    cin >> totalSemester;

    Game theGame(totalSemester);

    while(theGame.getSemester() <= totalSemester){
        if(theGame.getWeek() == 0){

        }
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
