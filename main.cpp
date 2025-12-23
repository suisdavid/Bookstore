#include "Database.hpp"
#include "main.hpp"
#include "module_book.hpp"
#include "module_user.hpp"
#include "module_log.hpp"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <format>
#include <time.h>
using std::string;
using std::cin;
using std::cout;
using std::endl;
const int maxn=100005;
int user_cnt,timer,deal_cnt,bookmark;
user root=user("root","root","sjtu",7);
user user_stack[maxn];
book selection[maxn];
string op;
long double deal[maxn];
void filter_line()
{
    while (cin.peek()!='\n'&&cin.peek()!='\r'&&cin.peek()!=-1){getchar();}
    if (cin.peek()=='\r'){getchar();}
    if (cin.peek()=='\n'){getchar();}
}
void add_deal(long double x)
{
    deal[++deal_cnt]=x;
}
void deal_all_out()
{
    cout<<"Finance:"<<endl;
    for (int i=1;i<=deal_cnt;i++)
    {
       if (deal[i]>0){cout<<"[+]"<<std::fixed<<std::setprecision(2)<<deal[i]<<endl;}
       else{cout<<"[-]"<<std::fixed<<std::setprecision(2)<<-deal[i]<<endl;}
    }
}
std::pair<double,double>query_deal(int cnt=0)
{
    std::pair<double,double>res=std::make_pair(0.0,0.0);
    if (cnt==0)
    {
        for (int i=1;i<=deal_cnt;i++)
        {
            if (deal[i]>0){res.first+=deal[i];}
            else{res.second+=-deal[i];}
        }
    }
    else if (cnt>deal_cnt)
    {
        res.first=-1;
    }
    else
    {
        for (int i=deal_cnt-cnt+1;i<=deal_cnt;i++)
        {
            if (deal[i]>0){res.first+=deal[i];}
            else{res.second+=-deal[i];}
        }
    }
    return res;
}
bool has(string s,char c)
{
    int len=s.length();
    for (int i=0;i<len;i++)
    {
        if (s[i]==c){return 1;}
    }
    return 0;
}

bool check_keyword(string s)
{
    string pool[61];int tot=0;
    int len=s.length();string t="";
    for (int i=0;i<len;i++)
    {
        if (s[i]=='|')
        {
            if (t.length())
            {
                pool[++tot]=t;
                t="";
            }
            else{return 0;}
        }
        else
        {
            t+=s[i];
        }
    }
    if (!t.length()){return 0;}
    pool[++tot]=t;t="";
    for (int i=1;i<=tot;i++)
    {
        for (int j=1;j<i;j++)
        {
            if (pool[i]==pool[j]){return 0;}
        }
    }
    return 1;
}

bool can_cin()
{
    while (cin.peek()==' ')
    {
        getchar();
    }
    return (cin.peek()!='\n'&&cin.peek()!='\r'&&cin.peek()!=-1);
}
bool check_input(string &s,int llim,int rlim)
{
    while (cin.peek()==' ')
    {
        getchar();
    }
    while (cin.peek()!='\n'&&cin.peek()!=' '&&cin.peek()!='\r'&&cin.peek()!=-1)
    {
        s+=getchar();
    }
    if (s.length()<llim||s.length()>rlim){return 0;}
    return 1;
}
void invalid()
{
    filter_line();
    cout<<"Invalid"<<endl;
}
bool check1(string s)//digit,letter,_
{
    int len=s.length();
    for (int i=0;i<len;i++)
    {
        if (!((s[i]>='0'&&s[i]<='9')||(s[i]>='a'&&s[i]<='z')||(s[i]>='A'&&s[i]<='Z')||s[i]=='_'))
        {
            return 0;
        }
    }
    return 1;
}
bool check2(string s)//visible characters
{
    int len=s.length();
    for (int i=0;i<len;i++)
    {
        if (s[i]<32||s[i]>126)
        {
            return 0;
        }
    }
    return 1;
}

bool check3(string s)//visible characters except quotation
{
    int len=s.length();
    for (int i=0;i<len;i++)
    {
        if (s[i]<32||s[i]>126||s[i]=='"')
        {
            return 0;
        }
    }
    return 1;
}
long long mystoi(string s)
{
    long long res=0;int len=s.length();
    if (len>10||len<1){return -1;}
    for (int i=0;i<len;i++)
    {
        if (s[i]<'0'||s[i]>'9')
        {
            return -1;
        }
        res=res*10+(s[i]-'0');
    }
    if (res>2147483647){return -1;}
    return res;
}
long double mystod(string s)
{
    int len=s.length();
    if (len>13||len<1){return -1;}
    int flg=0;
    long long intpart=0;long double floatpart=0,pw=1.0;
    for (int i=0;i<len;i++)
    {
        if (s[i]=='.')
        {
            if (flg){return -1;}
            flg=1;
            continue;
        }
        if (s[i]<'0'||s[i]>'9')
        {
            return -1;
        }
        if (!flg)
        {
            intpart=intpart*10+(s[i]-'0');
        }
        else
        {
            pw/=10;floatpart+=pw*(s[i]-'0');
        }
    }
    return (long double)intpart+floatpart;
}
void solve_su()
{
    string userid="",password="";
    if (!check_input(userid,1,30)||!check1(userid)){invalid();return;}
    if(!check_input(password,0,30)||!check1(password)){invalid();return;}
    if (can_cin()){invalid();return;}
    user info=user_database::query(userid);
    if (info.privilege==-1)//用户不存在
    {
        invalid();return;
    }
    else if (password=="")
    {
        if (user_cnt==0||info.privilege>=user_stack[user_cnt].privilege)
        {
            invalid();return;
        }
    }
    else if (info.password!=password)
    {
        invalid();return;
    }
    user_stack[++user_cnt]=info;
    log_database::add(action(timer,userid,"login"));
    filter_line();
}
void solve_logout()
{
    if (!user_cnt){invalid();return;}
    if (can_cin()){invalid();return;}
    log_database::add(action(timer,user_stack[user_cnt].userid,"logout"));
    filter_line();
    user_cnt--;
}
void solve_register()
{
    string userid="",password="",username="";
    if (!check_input(userid,1,30)||!check1(userid)){invalid();return;}
    if(!check_input(password,1,30)||!check1(password)){invalid();return;}
    if (!check_input(username,1,30)||!check2(username)){invalid();return;}
    if (can_cin()){invalid();return;}
    user info=user_database::query(userid);
    if (info.privilege!=-1)//用户已存在
    {
        invalid();return;
    }
    user _user=user(userid,username,password,1);
    user_database::add(_user);
    log_database::add(action(timer,userid,"register",userid+","+password+","+username));
    filter_line();
}
void solve_passwd()
{
    if (!user_cnt){invalid();return;}
    string userid="",password1="",password2="";
    if (!check_input(userid,1,30)||!check1(userid)){invalid();return;}
    if(!check_input(password1,1,30)||!check1(password1)){invalid();return;}
    if (!check_input(password2,0,30)||!check1(password2)){invalid();return;}
    if (can_cin()){invalid();return;}
    user info=user_database::query(userid);
    if (info.privilege==-1)//用户不存在
    {
        invalid();return;
    }
    if (password2=="")
    {
        if (user_stack[user_cnt].privilege!=7)
        {
            invalid();return;
        }
        user_database::modify(userid,password1);
        log_database::add(action(timer,user_stack[user_cnt].userid,"modify_password",userid+" to "+password1));
        filter_line();
    }
    else
    {
        if (info.password!=password1)
        {
            invalid();return;
        }
        user_database::modify(userid,password2);
        log_database::add(action(timer,user_stack[user_cnt].userid,"modify_password",userid+" to "+password2));
        filter_line();
    }
}

void solve_useradd()
{
    string userid="",password="",_privilege="",username="";
    if (!check_input(userid,1,30)||!check1(userid)){invalid();return;}
    if(!check_input(password,1,30)||!check1(password)){invalid();return;}
    if (!check_input(_privilege,1,1)){invalid();return;}
    if (!check_input(username,1,30)||!check2(username)){invalid();return;}
    if (_privilege[0]!='1'&&_privilege[0]!='3'&&_privilege[0]!='7')
    {
        invalid();return;
    }
    if (can_cin()){invalid();return;}
    int privilege=_privilege[0]-'0';
    if (user_cnt==0||user_stack[user_cnt].privilege<3||privilege>=user_stack[user_cnt].privilege)
    {
        invalid();return;
    }
    user info=user_database::query(userid);
    if (info.privilege!=-1)//用户已存在
    {
        invalid();return;
    }
    user _user=user(userid,username,password,privilege);
    user_database::add(_user);
    log_database::add(action(timer,user_stack[user_cnt].userid,"add_user",userid+","+password+","+std::to_string(privilege)+","+username));
    filter_line();
}
void solve_delete()
{
    if (!user_cnt||user_stack[user_cnt].privilege<7){invalid();return;}
    string userid="";
    if (!check_input(userid,1,30)||!check1(userid)){invalid();return;}
    if (can_cin()){invalid();return;}
    for (int i=1;i<=user_cnt;i++)
    {
        if (user_stack[i].userid==userid)
        {
            invalid();return;
        }
    }
    user info=user_database::query(userid);
    if (info.privilege==-1)//用户不存在
    {
        invalid();return;
    }
    user_database::del(userid);
    log_database::add(action(timer,user_stack[user_cnt].userid,"delete_user",userid));
    filter_line();
}
void solve_finance()
{
    if (!user_cnt||user_stack[user_cnt].privilege<7){invalid();return;}
    string count="";long long cnt=0;
    if (!check_input(count,0,10)){invalid();return;}
    if (can_cin()){invalid();return;}
    if (count=="")
    {
        std::pair<double,double>info=query_deal();
        cout<<std::fixed<<std::setprecision(2)<<"+ "<<info.first<<" - "<<info.second<<endl;
        filter_line();
    }
    else
    {
        cnt=mystoi(count);
        if (cnt==-1){invalid();return;}
        if (cnt==0){cout<<endl;return;}
        std::pair<double,double>info=query_deal(cnt);
        if (info.first<0)
        {
            invalid();return;
        }
        cout<<std::fixed<<std::setprecision(2)<<"+ "<<info.first<<" - "<<info.second<<endl;
        filter_line();
    }
}
void solve_show()
{
    string info="";
    if (!user_cnt||!check_input(info,0,80)){invalid();return;}
    if (info=="")
    {
        book_database::all_out();filter_line();
    }
    else if (info=="finance")//finance
    {
        solve_finance();
    }
    else if (info.length()>6&&info.substr(0,6)=="-ISBN=")//ISBN
    {
        info=info.substr(6,info.length()-6);
        if (info.length()>20||!check2(info)||can_cin()){invalid();return;}
        book_database::query_ISBN(info);
        log_database::add(action(timer,user_stack[user_cnt].userid,"show_book_info_ISBN",info));
        filter_line();
    }
    else if (info.length()>8&&info.substr(0,7)=="-name=\""&&info[info.length()-1]=='"')//name
    {
        info=info.substr(7,info.length()-8);
        if (info.length()>60||!check3(info)||can_cin()){invalid();return;}
        book_database::query_name(info);
        log_database::add(action(timer,user_stack[user_cnt].userid,"show_book_info_name",info));
        filter_line();
    }
    else if (info.length()>10&&info.substr(0,9)=="-author=\""&&info[info.length()-1]=='"')//author
    {
        info=info.substr(9,info.length()-10);
        if (info.length()>60||!check3(info)||can_cin()){invalid();return;}
        book_database::query_author(info);
        log_database::add(action(timer,user_stack[user_cnt].userid,"show_book_info_author",info));
        filter_line();
    }
    else if (info.length()>11&&info.substr(0,10)=="-keyword=\""&&info[info.length()-1]=='"')//keyword
    {
        info=info.substr(10,info.length()-11);
        if (info.length()>60||!check3(info)||can_cin()){invalid();return;}
        if (has(info,'|')){invalid();return;}
        book_database::query_keyword(info);
        log_database::add(action(timer,user_stack[user_cnt].userid,"show_book_info_keyword",info));
        filter_line();
    }
    else{
        invalid();return;
    }
}

void solve_buy()
{
    if (!user_cnt){invalid();return;}
    string ISBN="",_quantity="";
    if (!check_input(ISBN,1,20)||!check2(ISBN)){invalid();return;}
    if (!check_input(_quantity,1,10)){invalid();return;}
    if (can_cin()){invalid();return;}
    int quantity=mystoi(_quantity);
    if (quantity<=0){invalid();return;}
    book info=book_database::find_by_ISBN(ISBN);
    if (info.quantity<quantity)//不存在则为-1
    {
        invalid();return;
    }
    info.quantity-=quantity;
    book_database::modify(info);
    add_deal(info.price*quantity);
    cout<<std::fixed<<std::setprecision(2)<<info.price*quantity<<endl;
    log_database::add(action(timer,user_stack[user_cnt].userid,"buy_book",ISBN+","+std::to_string(quantity)));
    filter_line();
}
void solve_select()
{
    if (!user_cnt||user_stack[user_cnt].privilege<3){invalid();return;}
    string ISBN="";
    if (!check_input(ISBN,1,20)||!check2(ISBN)){invalid();return;}
    if (can_cin()){invalid();return;}
    book info=book_database::find_by_ISBN(ISBN);
    if (info.quantity==-1)//不存在则为-1
    {
        info=book(ISBN);info.mark=++bookmark;info.quantity=0;info.price=0;
        book_database::add(info);
    }
    selection[user_cnt]=info;
    log_database::add(action(timer,user_stack[user_cnt].userid,"select_book",ISBN));
    filter_line();
}

void solve_modify()
{
    if (!user_cnt||selection[user_cnt].mark==0||user_stack[user_cnt].privilege<3){invalid();return;}
    bool vis[5];vis[0]=vis[1]=vis[2]=vis[3]=vis[4]=0;int flg=0;
    selection[user_cnt]=book_database::query(selection[user_cnt].mark);
    if (selection[user_cnt].quantity==-1){invalid();return;}
    book newbook=selection[user_cnt];
    while (1)
    {
        string info="";
        if (!check_input(info,0,80)){invalid();return;}
        if (info==""){break;}
        else if (info.length()>6&&info.substr(0,6)=="-ISBN=")//ISBN
        {
            info=info.substr(6,info.length()-6);
            if (vis[0]||info.length()>20||!check2(info)){invalid();return;}
            if (info==selection[user_cnt].ISBN||book_database::find_by_ISBN(info).quantity!=-1){invalid();return;}
            vis[0]=1; charmake(newbook.ISBN,info);flg=1;
        }
        else if (info.length()>8&&info.substr(0,7)=="-name=\""&&info[info.length()-1]=='"')//name
        {
            info=info.substr(7,info.length()-8);
            if (vis[1]||info.length()>60||!check3(info)){invalid();return;}
            vis[1]=1;charmake(newbook.bookname,info);flg=1;
        }
        else if (info.length()>10&&info.substr(0,9)=="-author=\""&&info[info.length()-1]=='"')//author
        {
            info=info.substr(9,info.length()-10);
            if (vis[2]||info.length()>60||!check3(info)){flg=0;continue;}
            vis[2]=1;charmake(newbook.author,info);flg=1;
        }
        else if (info.length()>11&&info.substr(0,10)=="-keyword=\""&&info[info.length()-1]=='"')//keyword
        {
            info=info.substr(10,info.length()-11);
            if (vis[3]||info.length()>60||!check_keyword(info)){invalid();return;}
            vis[3]=1;charmake(newbook.keyword,info);flg=1;
        }
        else if (info.length()>7&&info.substr(0,7)=="-price=")
        {
            info=info.substr(7,info.length()-7);
            if (vis[4]||info.length()>13){invalid();return;}
            long double _price=mystod(info);
            if (_price<0){invalid();return;}
            vis[4]=1;newbook.price=_price;flg=1;
        }
        else{invalid();return;}
    }
    if (!flg){invalid();return;}
    else
    {
        selection[user_cnt]=newbook;
        book_database::modify(selection[user_cnt]);
        log_database::add(action(timer,user_stack[user_cnt].userid,"modify_book_info",string(selection[user_cnt].ISBN)+","+string(selection[user_cnt].bookname)+","+string(selection[user_cnt].author)+","+string(selection[user_cnt].keyword)+","+std::to_string(selection[user_cnt].price)));
        filter_line();
    }
}

void solve_import()
{
    if (!user_cnt||selection[user_cnt].mark==0||user_stack[user_cnt].privilege<3){invalid();return;}
    string _quantity="",_totalcost="";
    if (!check_input(_quantity,1,10)){invalid();return;}
    if (!check_input(_totalcost,1,13)){invalid();return;}
    if (can_cin()){invalid();return;}
    int quantity=mystoi(_quantity);long double totalcost=mystod(_totalcost);
    if (quantity<=0||totalcost<=0){invalid();return;} 
    selection[user_cnt]=book_database::query(selection[user_cnt].mark);
    if (selection[user_cnt].quantity==-1){invalid();return;}
    selection[user_cnt].quantity+=quantity;
   book_database::modify(selection[user_cnt]);
    add_deal(-totalcost);
    log_database::add(action(timer,user_stack[user_cnt].userid,"import_book",std::to_string(quantity)+","+std::to_string(totalcost)));
    filter_line();
}
void solve_report_employee()
{
    if (!user_cnt||user_stack[user_cnt].privilege<7){invalid();return;}
    if (can_cin()){invalid();return;}
    cout<<"Employee Behavior: "<<endl;
    log_database::all_out();
    filter_line();
}
void solve_report_finance()
{
    if (!user_cnt||user_stack[user_cnt].privilege<7){invalid();return;}
    if (can_cin()){invalid();return;}
    deal_all_out();filter_line();
}
void solve_log()
{
    if (!user_cnt||user_stack[user_cnt].privilege<7){invalid();return;}
    if (can_cin()){invalid();return;}
    cout<<"Employee Behavior: "<<endl;
    log_database::all_out();
    deal_all_out();
    filter_line();
}

bool get_EOF()
{
    while (cin.peek()==' '){getchar();}
    if (cin.peek()==-1){return 1;}
    return 0;
}
int main()
{
    int begin=clock();
    user_database::add(root);
    log_database::read_deal(deal,deal_cnt,bookmark);
    while (1)
    {
        timer++;
        if (get_EOF()){break;}
        if (!can_cin())
        {
            filter_line();continue;
        }
        cin>>op;
        if (op=="quit"||op=="exit")
        {
            if (can_cin()){invalid();}
            else{break;}
        }
        else if (op=="su")
        {
            solve_su();
        }
        else if (op=="logout")
        {
           solve_logout();
        }
        else if (op=="register")
        {
            solve_register();
        }
        else if (op=="passwd")
        {
            solve_passwd();
        }
        else if (op=="useradd")
        {
            solve_useradd();
        }
        else if (op=="delete")
        {
            solve_delete();
        }
        else if (op=="show")
        {
            solve_show();
        }
        else if (op=="buy")
        {
            solve_buy();
        }
        else if (op=="select")
        {
            solve_select();
        }
        else if (op=="modify")
        {
            solve_modify();
        }
        else if (op=="import")
        {
            solve_import();
        }
        else if (op=="report")
        {
            if (!can_cin()){invalid();continue;}
            cin>>op;
            if (op=="employee")
            {
                solve_report_employee();
            }
            else if (op=="finance")
            {
                solve_report_finance();
            }
            else{invalid();}
        }
        else if (op=="log")
        {
            solve_log();
        }
        else
        {
            invalid();
        }
    }
    log_database::write_deal(deal,deal_cnt,bookmark);
   // cout<<clock()-begin<<endl;
}