#pragma once
#include "main.hpp"
#include "Database.hpp"
using namespace database;
using std::string;

namespace book_database
{
    database::Block_Chain<book> db = database::Block_Chain<book>("database_book.out");
    database::Block_Chain<book> db_ISBN = database::Block_Chain<book>("database_book_ISBN.out");
    database::Block_Chain<book> db_bookname = database::Block_Chain<book>("database_book_bookname.out");
    database::Block_Chain<book> db_author = database::Block_Chain<book>("database_book_author.out");
    database::Block_Chain<book> db_keyword = database::Block_Chain<book>("database_book_keyword.out");
    database::Block_Chain<book> db_allout = database::Block_Chain<book>("database_book_allout.out");
    void AllOut()
    {
        db_allout.AllOut();
    }
    void QueryISBN(string ISBN)
    {
        db_ISBN.find(ISBN);
    }
    void QueryName(string name)
    {
        db_bookname.find(name);
    }
    void QueryAuthor(string author)
    {
        db_author.find(author);
    }
    void QueryKeyword(string keyword)
    {
        db_keyword.find(keyword);
    }
    book FindByISBN(string ISBN)
    {
        return db_ISBN.findone(ISBN);
    }
    book query(int mark)
    {
        return db.findone(std::to_string(mark));
    }
    void DelKeyword(book _book)
    {
        std::string s = _book.keyword, t = "";
        int len = s.length();
        for (int i = 0; i < len; i++)
        {
            if (s[i] == '|')
            {
                if (t.length())
                {
                    db_keyword.del(t, _book);
                    t = "";
                }
            }
            else
            {
                t += s[i];
            }
        }
        if (t.length())
        {
            db_keyword.del(t, _book);
        }
    }
    void AddKeyword(book _book)
    {
        std::string s = _book.keyword, t = "";
        int len = s.length();
        for (int i = 0; i < len; i++)
        {
            if (s[i] == '|')
            {
                if (t.length())
                {
                    db_keyword.insert(t, _book);
                    t = "";
                }
            }
            else
            {
                t += s[i];
            }
        }
        if (t.length())
        {
            db_keyword.insert(t, _book);
        }
    }
    void ModifyKeyword(book _book)
    {
        std::string s = _book.keyword, t = "";
        int len = s.length();
        for (int i = 0; i < len; i++)
        {
            if (s[i] == '|')
            {
                if (t.length())
                {
                    db_keyword.modify(t, _book);
                    t = "";
                }
            }
            else
            {
                t += s[i];
            }
        }
        if (t.length())
        {
            db_keyword.modify(t, _book);
        }
    }
    void add(book _book)
    {
        db_allout.insert("", _book);
        db.insert(std::to_string(_book.mark), _book);
        db_ISBN.insert(_book.ISBN, _book);
        db_bookname.insert(_book.bookname, _book);
        db_author.insert(_book.author, _book);
        AddKeyword(_book);
    }
    void del(book _book)
    {
        db_allout.del("", _book);
        db.del(std::to_string(_book.mark), _book);
        db_ISBN.del(_book.ISBN, _book);
        db_bookname.del(_book.bookname, _book);
        db_author.del(_book.author, _book);
        DelKeyword(_book);
    }
    void modify(book _book)
    {
        book origin = db.findone(std::to_string(_book.mark));
        if (origin.quantity == -1)
        {
            return;
        }
        del(origin);
        add(_book);
    }
}