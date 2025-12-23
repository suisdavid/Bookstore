#pragma once
#include "main.hpp"
#include "Database.hpp"
using namespace database;

namespace log_database
{
    database::MemoryRiver<action> db = database::MemoryRiver<action>("database_action.out");
    database::MemoryRiver<long double, 4> db_deal = database::MemoryRiver<long double, 4>("database_deal.out");
    void ReadDeal(long double *deal, int &deal_cnt, int &bookmark)
    {
        db_deal.GetInfo(deal_cnt, 0);
        db_deal.GetInfo(bookmark, 1);
        db_deal.ReadMany(deal + 1, 0, deal_cnt);
    }
    void WriteDeal(long double *deal, int deal_cnt, int bookmark)
    {
        int pre_cnt = 0;
        db_deal.GetInfo(pre_cnt, 0);
        db_deal.WriteInfo(deal_cnt, 0);
        db_deal.WriteInfo(bookmark, 1);
        db_deal.UpdateMany(deal + 1, 0, pre_cnt);
        if (pre_cnt < deal_cnt)
        {
            db_deal.WriteMany(deal + pre_cnt + 1, deal_cnt - pre_cnt);
        }
    }
    void add(action _action)
    {
        int cnt = 0;
        db.GetInfo(cnt, 0);
        db.WriteInfo(++cnt, 0);
        db.write(_action);
    }
    void AllOut()
    {
        int cnt = 0;
        db.GetInfo(cnt, 0);
        action T;
        for (int i = 0; i < cnt; i++)
        {
            db.read(T, i);
            T.print();
        }
    }
}
