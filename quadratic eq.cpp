#include <iostream>
#include <ctime>
using namespace std;

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

int main(){
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
    else
        cout << "錯誤" << endl;
}