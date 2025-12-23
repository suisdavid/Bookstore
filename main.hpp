#pragma once
#include <string.h>
#include <iostream>
#include <iomanip>
using std::cout;
using std::endl;
const int Base=257;
const int Mod=1e9+7;

int hash(std::string s)
{
    int len=s.length();long long v=0;
    for (int i=0;i<len;i++)
    {
        v=(v*Base+s[i])%Mod;
    }
    return v;
}
void charmake(char *c,std::string s)
{
    int len=s.length();
    for (int i=0;i<len;i++)
    {
        c[i]=s[i];
    }
    c[len]=0;
}
struct user
{
    char userid[31],username[31],password[31];
    int privilege;
    int hash_val;
    user(std::string _userid="",std::string _username="",std::string _password="",int _privilege=-1)
    {
        charmake(userid,_userid);charmake(username,_username);charmake(password,_password);privilege=_privilege;
        hash_val=hash(_userid);
    }
    bool const operator < (user rhs) const{
        return hash_val<rhs.hash_val;
    }
    bool const operator == (user rhs) const{
        return hash_val==rhs.hash_val;
    }
    bool const operator != (user rhs) const{
        return hash_val!=rhs.hash_val;
    }
};
struct book
{
    char ISBN[21],bookname[61],author[61],keyword[61];
    long double price;
    long long quantity;
    int mark;
    book(std::string _ISBN="",std::string _bookname="",std::string _author="",std::string _keyword="",long long _quantity=-1,long double _price=0,int _mark=0)
    {
        charmake(ISBN,_ISBN);charmake(bookname,_bookname);charmake(author,_author);charmake(keyword,_keyword);
        quantity=_quantity;price=_price;mark=_mark;
    }
    bool const operator < (book rhs) const{
        return std::string(ISBN)<std::string(rhs.ISBN);
    }
    bool const operator == (book rhs) const{
        return std::string(ISBN)==std::string(rhs.ISBN);
    }
    bool const operator != (book rhs) const{
        return std::string(ISBN)!=std::string(rhs.ISBN);
    }
    void print()
    {
        cout<<ISBN<<'\t'<<bookname<<'\t'<<author<<'\t'<<keyword<<'\t';
        cout<<std::fixed<<std::setprecision(2)<<price;
        cout<<'\t'<<quantity<<endl;
    }
};

struct action
{
    int time;
    char userid[31];
    char op[21],detail[300];
    action(int _time=0,std::string _userid="",std::string _op="",std::string _detail="")
    {
        time=_time;charmake(userid,_userid);charmake(op,_op);charmake(detail,_detail);
    }
     bool const operator < (action rhs) const{
        return time<rhs.time;
    }
    bool const operator == (action rhs) const{
        return time==rhs.time;
    }
    bool const operator != (action rhs) const{
        return time!=rhs.time;
    }
    void print()
    {
        cout<<"Operation: "<<op<<" | "<<"Operator: "<<userid;
        if (detail[0])
        {
            cout<<" | Detail: "<<detail;
        }
        cout<<endl;
    }
};