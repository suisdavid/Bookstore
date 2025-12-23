#pragma once
#include "main.hpp"
#include "database.hpp"
using namespace database;

namespace log_database
{
    database::MemoryRiver<action>db=database::MemoryRiver<action>("database_action.out");
    database::MemoryRiver<long double>db_deal=database::MemoryRiver<long double>("database_deal.out");
    void read_deal(long double *deal,int &deal_cnt)
    {
        db_deal.get_info(deal_cnt,0);
        db_deal.read_many(deal,0,deal_cnt);
    }
    void write_deal(long double *deal,int deal_cnt)
    {
        db_deal.write_info(deal_cnt,0);
        db_deal.update_many(deal,0,deal_cnt);
    }
    void add(action _action)
    {
        int cnt=0;db.get_info(cnt,0);
        db.write_info(++cnt,0);
        db.write(_action);
    }
    void all_out()
    {
        int cnt=0;db.get_info(cnt,0);
        action * T=new action;
        db.read_many(T,0,cnt);
        for (int i=0;i<cnt;i++)
        {
            T[i].print();
        }
        free(T);
    }
}
