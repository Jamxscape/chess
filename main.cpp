//
//  main.cpp
//  chess
//
//  Created by 马元 on BE 2561/12/5.
//  Copyright © 2561 MaYuan. All rights reserved.
//

//main.cpp
/*
 定义三种状态，为O X 和space
 默认用户为O 电脑输入为X 空格状态下为space
 space ASCLL码为32
 */
#include <iostream>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <unistd.h>
using namespace std;
int b[3][3];
void printMap(string a){//打印棋盘
    for(int i=0;i<9;i++){
        if(i==0)
            cout<<"-------"<<endl;
        else if(i==3)
            cout<<"-------"<<endl;
        else if(i==6)
            cout<<"-------"<<endl;
        cout<<"|"<<a[i];
        if(i!=0&&i%3==2)
            cout<<"|"<<endl;
        if(i==8)
            cout<<"-------"<<endl;
    }
}
int find(int ar[], int element)//查找元素并返回位置下标,find(数组，长度，元素)
{
    int i = 0;
    int index=-1;//原始下标，没找到元素返回-1
    for (i = 0; i <9; i++)
    {
        if (element == ar[i])
        {
            index=i;//记录元素下标
        }
    }
    return index;//返回下标
}
string input(string a){
    cout<<"输入棋子的位置";
    int x;//x 是要下的棋子位于第几个位置
    bool flag=1;
    cin>>x;
    while(flag)
    {
        if(cin.fail())
        {// 或者if(!cin)
            cout<<"输入棋子的位置是数字哦!请重新输入";
            cin.clear();            //再次修复输入流
            cin.ignore();            //取走刚才流中的字符
            cin>>x;
        }
        else if(a[x-1]!=' ')
        {
            cout<<"你输入的位置已经有棋子了！请重新输入";
            cin>>x;
        }
        else
        {
            a[x-1]='O';
            flag=0;
        }
    }
    return a;
}
bool referee(string a,bool flag){//裁判函数
    for(int i=0;i<9;i++)
    {
        if(a[i]==' ')
            a[i]+=i;
    }//去掉空格
    if(a[0]==a[1]&&a[1]==a[2])
    {flag = 1;}
    else if(a[0]==a[1]&&a[1]==a[2])
    {flag = 1;}
    else if(a[0]==a[4]&&a[0]==a[8])
    {flag = 1;}
    else if(a[1]==a[4]&&a[1]==a[7])
    {flag = 1;}
    else if(a[2]==a[5]&&a[2]==a[8])
    {flag = 1;}
    else if(a[2]==a[4]&&a[2]==a[6])
    {flag = 1;}
    else if(a[3]==a[4]&&a[3]==a[5])
    {flag = 1;}
    else if(a[0]==a[1]&&a[1]==a[2])
    {flag = 1;}
    else if(a[6]==a[7]&&a[6]==a[8])
    {flag = 1;}
    return flag;
}
void search(string a){
    int c=0;
    for(int i=0;i<3;i++)
        for(int j=0;j<3;j++)
        {
            if(a[c]==' ')
            {b[i][j]=0;}
            else if(a[c] == 'O')
            {b[i][j]=1;}//人下的是1
            else if(a[c] == 'X')
            {b[i][j]=-1;}//电脑是2
            c++;
        }
}
int scoring(int sum,bool chess)
{ // 评估函数的子函数
    int score=0;
    if(chess)
    { //CHESS是使用何种棋子的标志（X 棋或O 棋）
        if(sum==-2) //若是一条直线上有两个一样的棋子和一个空位
            score+=300;
        else if(sum==2)//人 一行上有两个棋子
            score-=500;
        else if(sum==-3)//若是一条直线上有三个一样的棋子
            score+=1500;
        else if(sum==3)
            score-=1000;
    }
    else
    {
        if(sum==-2)
            score-=30;
        else if(sum==2)
            score+=50;
        else if(sum==-3)
            score-=150;
        else if(sum==3)
            score+=100;
    }
    return score;
}
int evaluate(int b[3][3],bool chess){//评估函数
    int sum=0,score=0;// sum 为求和结果的存储变量，score为该棋局的最终得分
    for(int i=0;i<3;i++)
    {
        sum = b[i][0] + b[i][1] + b[i][2]; //该循环计算每一行
        score+=scoring(sum,chess);
        sum=0;
    }
    for(int i=0;i<3;i++)
    {
        sum = b[0][i] + b[1][i] + b[2][i]; //该循环计算每一列
        score+=scoring(sum,chess);
        sum=0;
    }
    int temp=0;
    for (int i=0; i<3; i++)
    {
        sum += b[i][i];
        temp += b[i][2-i];
    }
    score+=scoring(sum,chess);
    score+=scoring(temp,chess); //该循环计算两条对角线
    return score; //返回分值
}
string minmax(string a){
    int best[9],*best_max,best_chess=0,worst[9],*worst_min=0,worst_chess=0;
    search(a);
    bool chess=1;//chess为一的时候就是机器人下，0的是人在下
    //max 部分 先搜索出当前状态的最佳状态 给机器人最大
    
   /* for(int cir=0;cir<2;cir++)
    {
        */
        int temp=0;
    for(int i=0;i<3;i++)
    {
        for(int j=0;j<3;j++)
        {
            if(b[i][j]==0)
            {
                b[i][j]=-1;
                best[temp]=evaluate(b, chess);
                //min 部分 搜索出人下的最佳位置
                int temp1=0;
                chess=0;//代表是人下的
                for(int i1=0;i1<3;i1++)
                {
                    for(int j1=0;j1<3;j1++)
                    {
                        if(b[i1][j1]==0)
                        {
                            b[i1][j1]=1;
                            worst[temp1]=evaluate(b, chess);
                            temp1++;
                            b[i1][j1]=0;
                        }
                        else
                        {
                            worst[temp1]=2000;
                            temp1++;
                        }
                    }
                }
                worst_min=min_element(worst, worst+9);
                worst_chess=find(worst_min,*worst_min);
                best[temp]=worst[worst_chess]+best[temp];//找到对电脑最不有利的棋子
                temp++;
                b[i][j]=0;
                chess=1;
            }
            else
            {
                best[temp]=-2000;
                temp++;
            }
        }
    }
        best_max=max_element(best, best+9);
        best_chess=find(best,*best_max);
    a[best_chess]='X';
    return a;
}
int main()
{
    bool flag = 0;
    string a="         ";
    int counter=0;
    while(!flag){
        cout<<"轮到你了"<<endl;
    a=input(a);
    printMap(a);
    flag=referee(a,flag);
        if(flag==1)
        {cout<<"你赢了"<<endl;break;}
        counter++;
        if(counter==9)
        {cout<<"平局了"<<endl;break;}
    search(a);
    a=minmax(a);
        cout<<"电脑正在思考中哦！"<<endl;
        sleep(1);
    printMap(a);
        cout<<"电脑下完了"<<endl;
    flag=referee(a,flag);
        if(flag==1)
        {cout<<"电脑赢了"<<endl;break;}
        counter++;
        if(counter==9)
        {cout<<"平局了"<<endl;break;}
    }
    return 0;
}

