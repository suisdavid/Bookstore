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
using std::cin;
using std::cout;
using std::endl;
using std::string;
const int maxn = 100005;
int user_cnt, timer, deal_cnt, bookmark;
user root = user("root", "root", "sjtu", 7);
user user_stack[maxn];
book selection[maxn];
string op;
long double deal[maxn];
void FilterLine()
{
    while (cin.peek() != '\n' && cin.peek() != '\r' && cin.peek() != -1)
    {
        getchar();
    }
    if (cin.peek() == '\r')
    {
        getchar();
    }
    if (cin.peek() == '\n')
    {
        getchar();
    }
}
void AddDeal(long double x)
{
    deal[++deal_cnt] = x;
}
void DealAllOut()
{
    cout << "Finance:" << endl;
    for (int i = 1; i <= deal_cnt; i++)
    {
        if (deal[i] > 0)
        {
            cout << "[+]" << std::fixed << std::setprecision(2) << deal[i] << endl;
        }
        else
        {
            cout << "[-]" << std::fixed << std::setprecision(2) << -deal[i] << endl;
        }
    }
}
std::pair<double, double> QueryDeal(int cnt = 0)
{
    std::pair<double, double> res = std::make_pair(0.0, 0.0);
    if (cnt == 0)
    {
        for (int i = 1; i <= deal_cnt; i++)
        {
            if (deal[i] > 0)
            {
                res.first += deal[i];
            }
            else
            {
                res.second += -deal[i];
            }
        }
    }
    else if (cnt > deal_cnt)
    {
        res.first = -1;
    }
    else
    {
        for (int i = deal_cnt - cnt + 1; i <= deal_cnt; i++)
        {
            if (deal[i] > 0)
            {
                res.first += deal[i];
            }
            else
            {
                res.second += -deal[i];
            }
        }
    }
    return res;
}
bool has(string s, char c)
{
    int len = s.length();
    for (int i = 0; i < len; i++)
    {
        if (s[i] == c)
        {
            return 1;
        }
    }
    return 0;
}

bool CheckKeyword(string s)
{
    string pool[61];
    int tot = 0;
    int len = s.length();
    string t = "";
    for (int i = 0; i < len; i++)
    {
        if (s[i] == '|')
        {
            if (t.length())
            {
                pool[++tot] = t;
                t = "";
            }
            else
            {
                return 0;
            }
        }
        else
        {
            t += s[i];
        }
    }
    if (!t.length())
    {
        return 0;
    }
    pool[++tot] = t;
    t = "";
    for (int i = 1; i <= tot; i++)
    {
        for (int j = 1; j < i; j++)
        {
            if (pool[i] == pool[j])
            {
                return 0;
            }
        }
    }
    return 1;
}

bool CanCin()
{
    while (cin.peek() == ' ')
    {
        getchar();
    }
    return (cin.peek() != '\n' && cin.peek() != '\r' && cin.peek() != -1);
}
bool CheckInput(string &s, int llim, int rlim)
{
    while (cin.peek() == ' ')
    {
        getchar();
    }
    while (cin.peek() != '\n' && cin.peek() != ' ' && cin.peek() != '\r' && cin.peek() != -1)
    {
        s += getchar();
    }
    if (s.length() < llim || s.length() > rlim)
    {
        return 0;
    }
    return 1;
}
void Invalid()
{
    FilterLine();
    cout << "Invalid" << endl;
}
bool check1(string s) // digit,letter,_
{
    int len = s.length();
    for (int i = 0; i < len; i++)
    {
        if (!((s[i] >= '0' && s[i] <= '9') || (s[i] >= 'a' && s[i] <= 'z') || (s[i] >= 'A' && s[i] <= 'Z') || s[i] == '_'))
        {
            return 0;
        }
    }
    return 1;
}
bool check2(string s) // visible characters
{
    int len = s.length();
    for (int i = 0; i < len; i++)
    {
        if (s[i] < 32 || s[i] > 126)
        {
            return 0;
        }
    }
    return 1;
}

bool check3(string s) // visible characters except quotation
{
    int len = s.length();
    for (int i = 0; i < len; i++)
    {
        if (s[i] < 32 || s[i] > 126 || s[i] == '"')
        {
            return 0;
        }
    }
    return 1;
}
long long MyStoi(string s)
{
    long long res = 0;
    int len = s.length();
    if (len > 10 || len < 1)
    {
        return -1;
    }
    for (int i = 0; i < len; i++)
    {
        if (s[i] < '0' || s[i] > '9')
        {
            return -1;
        }
        res = res * 10 + (s[i] - '0');
    }
    if (res > 2147483647)
    {
        return -1;
    }
    return res;
}
long double MyStod(string s)
{
    int len = s.length();
    if (len > 13 || len < 1)
    {
        return -1;
    }
    int flg = 0;
    long long intpart = 0;
    long double floatpart = 0, pw = 1.0;
    for (int i = 0; i < len; i++)
    {
        if (s[i] == '.')
        {
            if (flg)
            {
                return -1;
            }
            flg = 1;
            continue;
        }
        if (s[i] < '0' || s[i] > '9')
        {
            return -1;
        }
        if (!flg)
        {
            intpart = intpart * 10 + (s[i] - '0');
        }
        else
        {
            pw /= 10;
            floatpart += pw * (s[i] - '0');
        }
    }
    return (long double)intpart + floatpart;
}
void SolveSu()
{
    string userid = "", password = "";
    if (!CheckInput(userid, 1, 30) || !check1(userid))
    {
        Invalid();
        return;
    }
    if (!CheckInput(password, 0, 30) || !check1(password))
    {
        Invalid();
        return;
    }
    if (CanCin())
    {
        Invalid();
        return;
    }
    user info = user_database::query(userid);
    if (info.privilege == -1) // 用户不存在
    {
        Invalid();
        return;
    }
    else if (password == "")
    {
        if (user_cnt == 0 || info.privilege >= user_stack[user_cnt].privilege)
        {
            Invalid();
            return;
        }
    }
    else if (info.password != password)
    {
        Invalid();
        return;
    }
    user_stack[++user_cnt] = info;
    log_database::add(action(timer, userid, "login"));
    FilterLine();
}
void SolveLogout()
{
    if (!user_cnt)
    {
        Invalid();
        return;
    }
    if (CanCin())
    {
        Invalid();
        return;
    }
    log_database::add(action(timer, user_stack[user_cnt].userid, "logout"));
    FilterLine();
    user_cnt--;
}
void SolveRegister()
{
    string userid = "", password = "", username = "";
    if (!CheckInput(userid, 1, 30) || !check1(userid))
    {
        Invalid();
        return;
    }
    if (!CheckInput(password, 1, 30) || !check1(password))
    {
        Invalid();
        return;
    }
    if (!CheckInput(username, 1, 30) || !check2(username))
    {
        Invalid();
        return;
    }
    if (CanCin())
    {
        Invalid();
        return;
    }
    user info = user_database::query(userid);
    if (info.privilege != -1) // 用户已存在
    {
        Invalid();
        return;
    }
    user _user = user(userid, username, password, 1);
    user_database::add(_user);
    log_database::add(action(timer, userid, "register", userid + "," + password + "," + username));
    FilterLine();
}
void SolvePasswd()
{
    if (!user_cnt)
    {
        Invalid();
        return;
    }
    string userid = "", password1 = "", password2 = "";
    if (!CheckInput(userid, 1, 30) || !check1(userid))
    {
        Invalid();
        return;
    }
    if (!CheckInput(password1, 1, 30) || !check1(password1))
    {
        Invalid();
        return;
    }
    if (!CheckInput(password2, 0, 30) || !check1(password2))
    {
        Invalid();
        return;
    }
    if (CanCin())
    {
        Invalid();
        return;
    }
    user info = user_database::query(userid);
    if (info.privilege == -1) // 用户不存在
    {
        Invalid();
        return;
    }
    if (password2 == "")
    {
        if (user_stack[user_cnt].privilege != 7)
        {
            Invalid();
            return;
        }
        user_database::modify(userid, password1);
        log_database::add(action(timer, user_stack[user_cnt].userid, "modify_password", userid + " to " + password1));
        FilterLine();
    }
    else
    {
        if (info.password != password1)
        {
            Invalid();
            return;
        }
        user_database::modify(userid, password2);
        log_database::add(action(timer, user_stack[user_cnt].userid, "modify_password", userid + " to " + password2));
        FilterLine();
    }
}

void SolveUseradd()
{
    string userid = "", password = "", _privilege = "", username = "";
    if (!CheckInput(userid, 1, 30) || !check1(userid))
    {
        Invalid();
        return;
    }
    if (!CheckInput(password, 1, 30) || !check1(password))
    {
        Invalid();
        return;
    }
    if (!CheckInput(_privilege, 1, 1))
    {
        Invalid();
        return;
    }
    if (!CheckInput(username, 1, 30) || !check2(username))
    {
        Invalid();
        return;
    }
    if (_privilege[0] != '1' && _privilege[0] != '3' && _privilege[0] != '7')
    {
        Invalid();
        return;
    }
    if (CanCin())
    {
        Invalid();
        return;
    }
    int privilege = _privilege[0] - '0';
    if (user_cnt == 0 || user_stack[user_cnt].privilege < 3 || privilege >= user_stack[user_cnt].privilege)
    {
        Invalid();
        return;
    }
    user info = user_database::query(userid);
    if (info.privilege != -1) // 用户已存在
    {
        Invalid();
        return;
    }
    user _user = user(userid, username, password, privilege);
    user_database::add(_user);
    log_database::add(action(timer, user_stack[user_cnt].userid, "add_user", userid + "," + password + "," + std::to_string(privilege) + "," + username));
    FilterLine();
}
void SolveDelete()
{
    if (!user_cnt || user_stack[user_cnt].privilege < 7)
    {
        Invalid();
        return;
    }
    string userid = "";
    if (!CheckInput(userid, 1, 30) || !check1(userid))
    {
        Invalid();
        return;
    }
    if (CanCin())
    {
        Invalid();
        return;
    }
    for (int i = 1; i <= user_cnt; i++)
    {
        if (user_stack[i].userid == userid)
        {
            Invalid();
            return;
        }
    }
    user info = user_database::query(userid);
    if (info.privilege == -1) // 用户不存在
    {
        Invalid();
        return;
    }
    user_database::del(userid);
    log_database::add(action(timer, user_stack[user_cnt].userid, "delete_user", userid));
    FilterLine();
}
void SolveFinance()
{
    if (!user_cnt || user_stack[user_cnt].privilege < 7)
    {
        Invalid();
        return;
    }
    string count = "";
    long long cnt = 0;
    if (!CheckInput(count, 0, 10))
    {
        Invalid();
        return;
    }
    if (CanCin())
    {
        Invalid();
        return;
    }
    if (count == "")
    {
        std::pair<double, double> info = QueryDeal();
        cout << std::fixed << std::setprecision(2) << "+ " << info.first << " - " << info.second << endl;
        FilterLine();
    }
    else
    {
        cnt = MyStoi(count);
        if (cnt == -1)
        {
            Invalid();
            return;
        }
        if (cnt == 0)
        {
            cout << endl;
            return;
        }
        std::pair<double, double> info = QueryDeal(cnt);
        if (info.first < 0)
        {
            Invalid();
            return;
        }
        cout << std::fixed << std::setprecision(2) << "+ " << info.first << " - " << info.second << endl;
        FilterLine();
    }
}
void SolveShow()
{
    string info = "";
    if (!user_cnt || !CheckInput(info, 0, 80))
    {
        Invalid();
        return;
    }
    if (info == "")
    {
        book_database::AllOut();
        FilterLine();
    }
    else if (info == "finance") // finance
    {
        SolveFinance();
    }
    else if (info.length() > 6 && info.substr(0, 6) == "-ISBN=") // ISBN
    {
        info = info.substr(6, info.length() - 6);
        if (info.length() > 20 || !check2(info) || CanCin())
        {
            Invalid();
            return;
        }
        book_database::QueryISBN(info);
        log_database::add(action(timer, user_stack[user_cnt].userid, "show_bookInfo_ISBN", info));
        FilterLine();
    }
    else if (info.length() > 8 && info.substr(0, 7) == "-name=\"" && info[info.length() - 1] == '"') // name
    {
        info = info.substr(7, info.length() - 8);
        if (info.length() > 60 || !check3(info) || CanCin())
        {
            Invalid();
            return;
        }
        book_database::QueryName(info);
        log_database::add(action(timer, user_stack[user_cnt].userid, "show_bookInfo_name", info));
        FilterLine();
    }
    else if (info.length() > 10 && info.substr(0, 9) == "-author=\"" && info[info.length() - 1] == '"') // author
    {
        info = info.substr(9, info.length() - 10);
        if (info.length() > 60 || !check3(info) || CanCin())
        {
            Invalid();
            return;
        }
        book_database::QueryAuthor(info);
        log_database::add(action(timer, user_stack[user_cnt].userid, "show_bookInfo_author", info));
        FilterLine();
    }
    else if (info.length() > 11 && info.substr(0, 10) == "-keyword=\"" && info[info.length() - 1] == '"') // keyword
    {
        info = info.substr(10, info.length() - 11);
        if (info.length() > 60 || !check3(info) || CanCin())
        {
            Invalid();
            return;
        }
        if (has(info, '|'))
        {
            Invalid();
            return;
        }
        book_database::QueryKeyword(info);
        log_database::add(action(timer, user_stack[user_cnt].userid, "show_bookInfo_keyword", info));
        FilterLine();
    }
    else
    {
        Invalid();
        return;
    }
}

void SolveBuy()
{
    if (!user_cnt)
    {
        Invalid();
        return;
    }
    string ISBN = "", _quantity = "";
    if (!CheckInput(ISBN, 1, 20) || !check2(ISBN))
    {
        Invalid();
        return;
    }
    if (!CheckInput(_quantity, 1, 10))
    {
        Invalid();
        return;
    }
    if (CanCin())
    {
        Invalid();
        return;
    }
    int quantity = MyStoi(_quantity);
    if (quantity <= 0)
    {
        Invalid();
        return;
    }
    book info = book_database::FindByISBN(ISBN);
    if (info.quantity < quantity) // 不存在则为-1
    {
        Invalid();
        return;
    }
    info.quantity -= quantity;
    book_database::modify(info);
    AddDeal(info.price * quantity);
    cout << std::fixed << std::setprecision(2) << info.price * quantity << endl;
    log_database::add(action(timer, user_stack[user_cnt].userid, "buy_book", ISBN + "," + std::to_string(quantity)));
    FilterLine();
}
void SolveSelect()
{
    if (!user_cnt || user_stack[user_cnt].privilege < 3)
    {
        Invalid();
        return;
    }
    string ISBN = "";
    if (!CheckInput(ISBN, 1, 20) || !check2(ISBN))
    {
        Invalid();
        return;
    }
    if (CanCin())
    {
        Invalid();
        return;
    }
    book info = book_database::FindByISBN(ISBN);
    if (info.quantity == -1) // 不存在则为-1
    {
        info = book(ISBN);
        info.mark = ++bookmark;
        info.quantity = 0;
        info.price = 0;
        book_database::add(info);
    }
    selection[user_cnt] = info;
    log_database::add(action(timer, user_stack[user_cnt].userid, "select_book", ISBN));
    FilterLine();
}

void SolveModify()
{
    if (!user_cnt || selection[user_cnt].mark == 0 || user_stack[user_cnt].privilege < 3)
    {
        Invalid();
        return;
    }
    bool vis[5];
    vis[0] = vis[1] = vis[2] = vis[3] = vis[4] = 0;
    int flg = 0;
    selection[user_cnt] = book_database::query(selection[user_cnt].mark);
    if (selection[user_cnt].quantity == -1)
    {
        Invalid();
        return;
    }
    book newbook = selection[user_cnt];
    while (1)
    {
        string info = "";
        if (!CheckInput(info, 0, 80))
        {
            Invalid();
            return;
        }
        if (info == "")
        {
            break;
        }
        else if (info.length() > 6 && info.substr(0, 6) == "-ISBN=") // ISBN
        {
            info = info.substr(6, info.length() - 6);
            if (vis[0] || info.length() > 20 || !check2(info))
            {
                Invalid();
                return;
            }
            if (info == selection[user_cnt].ISBN || book_database::FindByISBN(info).quantity != -1)
            {
                Invalid();
                return;
            }
            vis[0] = 1;
            charmake(newbook.ISBN, info);
            flg = 1;
        }
        else if (info.length() > 8 && info.substr(0, 7) == "-name=\"" && info[info.length() - 1] == '"') // name
        {
            info = info.substr(7, info.length() - 8);
            if (vis[1] || info.length() > 60 || !check3(info))
            {
                Invalid();
                return;
            }
            vis[1] = 1;
            charmake(newbook.bookname, info);
            flg = 1;
        }
        else if (info.length() > 10 && info.substr(0, 9) == "-author=\"" && info[info.length() - 1] == '"') // author
        {
            info = info.substr(9, info.length() - 10);
            if (vis[2] || info.length() > 60 || !check3(info))
            {
                flg = 0;
                continue;
            }
            vis[2] = 1;
            charmake(newbook.author, info);
            flg = 1;
        }
        else if (info.length() > 11 && info.substr(0, 10) == "-keyword=\"" && info[info.length() - 1] == '"') // keyword
        {
            info = info.substr(10, info.length() - 11);
            if (vis[3] || info.length() > 60 || !CheckKeyword(info))
            {
                Invalid();
                return;
            }
            vis[3] = 1;
            charmake(newbook.keyword, info);
            flg = 1;
        }
        else if (info.length() > 7 && info.substr(0, 7) == "-price=")
        {
            info = info.substr(7, info.length() - 7);
            if (vis[4] || info.length() > 13)
            {
                Invalid();
                return;
            }
            long double _price = MyStod(info);
            if (_price < 0)
            {
                Invalid();
                return;
            }
            vis[4] = 1;
            newbook.price = _price;
            flg = 1;
        }
        else
        {
            Invalid();
            return;
        }
    }
    if (!flg)
    {
        Invalid();
        return;
    }
    else
    {
        selection[user_cnt] = newbook;
        book_database::modify(selection[user_cnt]);
        log_database::add(action(timer, user_stack[user_cnt].userid, "modify_bookInfo", string(selection[user_cnt].ISBN) + "," + string(selection[user_cnt].bookname) + "," + string(selection[user_cnt].author) + "," + string(selection[user_cnt].keyword) + "," + std::to_string(selection[user_cnt].price)));
        FilterLine();
    }
}

void SolveImport()
{
    if (!user_cnt || selection[user_cnt].mark == 0 || user_stack[user_cnt].privilege < 3)
    {
        Invalid();
        return;
    }
    string _quantity = "", _totalcost = "";
    if (!CheckInput(_quantity, 1, 10))
    {
        Invalid();
        return;
    }
    if (!CheckInput(_totalcost, 1, 13))
    {
        Invalid();
        return;
    }
    if (CanCin())
    {
        Invalid();
        return;
    }
    int quantity = MyStoi(_quantity);
    long double totalcost = MyStod(_totalcost);
    if (quantity <= 0 || totalcost <= 0)
    {
        Invalid();
        return;
    }
    selection[user_cnt] = book_database::query(selection[user_cnt].mark);
    if (selection[user_cnt].quantity == -1)
    {
        Invalid();
        return;
    }
    selection[user_cnt].quantity += quantity;
    book_database::modify(selection[user_cnt]);
    AddDeal(-totalcost);
    log_database::add(action(timer, user_stack[user_cnt].userid, "import_book", std::to_string(quantity) + "," + std::to_string(totalcost)));
    FilterLine();
}
void SolveReportEmployee()
{
    if (!user_cnt || user_stack[user_cnt].privilege < 7)
    {
        Invalid();
        return;
    }
    if (CanCin())
    {
        Invalid();
        return;
    }
    cout << "Employee Behavior: " << endl;
    log_database::AllOut();
    FilterLine();
}
void solve_report_finance()
{
    if (!user_cnt || user_stack[user_cnt].privilege < 7)
    {
        Invalid();
        return;
    }
    if (CanCin())
    {
        Invalid();
        return;
    }
    DealAllOut();
    FilterLine();
}
void SolveLog()
{
    if (!user_cnt || user_stack[user_cnt].privilege < 7)
    {
        Invalid();
        return;
    }
    if (CanCin())
    {
        Invalid();
        return;
    }
    cout << "Employee Behavior: " << endl;
    log_database::AllOut();
    DealAllOut();
    FilterLine();
}

bool GetEOF()
{
    while (cin.peek() == ' ')
    {
        getchar();
    }
    if (cin.peek() == -1)
    {
        return 1;
    }
    return 0;
}
int main()
{
    user_database::add(root);
    log_database::ReadDeal(deal, deal_cnt, bookmark);
    while (1)
    {
        timer++;
        if (GetEOF())
        {
            break;
        }
        if (!CanCin())
        {
            FilterLine();
            continue;
        }
        cin >> op;
        if (op == "quit" || op == "exit")
        {
            if (CanCin())
            {
                Invalid();
            }
            else
            {
                break;
            }
        }
        else if (op == "su")
        {
            SolveSu();
        }
        else if (op == "logout")
        {
            SolveLogout();
        }
        else if (op == "register")
        {
            SolveRegister();
        }
        else if (op == "passwd")
        {
            SolvePasswd();
        }
        else if (op == "useradd")
        {
            SolveUseradd();
        }
        else if (op == "delete")
        {
            SolveDelete();
        }
        else if (op == "show")
        {
            SolveShow();
        }
        else if (op == "buy")
        {
            SolveBuy();
        }
        else if (op == "select")
        {
            SolveSelect();
        }
        else if (op == "modify")
        {
            SolveModify();
        }
        else if (op == "import")
        {
            SolveImport();
        }
        else if (op == "report")
        {
            if (!CanCin())
            {
                Invalid();
                continue;
            }
            cin >> op;
            if (op == "employee")
            {
                SolveReportEmployee();
            }
            else if (op == "finance")
            {
                solve_report_finance();
            }
            else
            {
                Invalid();
            }
        }
        else if (op == "log")
        {
            SolveLog();
        }
        else
        {
            Invalid();
        }
    }
    log_database::WriteDeal(deal, deal_cnt, bookmark);
}