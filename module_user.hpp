#pragma once
#include "main.hpp"
#include "Database.hpp"
using namespace database;
using std::string;

namespace user_database
{
    database::Block_Chain<user>db=database::Block_Chain<user>("database_user.out");
    void add(user _user)
    {
        db.insert(_user.userid,_user);
    }
    user query(string userid)
    {
        return db.findone(userid);
    }
    void modify(string userid,string password)
    {
        user _user= db.findone(userid);
        if (_user.privilege!=-1)
        {
            charmake(_user.password,password);
            db.modify(userid,_user);
        }
    }
    void del(string userid)
    {
        db.del(userid,user(userid));
    }
}
