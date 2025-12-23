#pragma once
#include <iostream>
#include <fstream>
#include <string.h>
#include <unistd.h>
#include <algorithm>
using std::string;
using std::fstream;
using std::ifstream;
using std::ofstream;

namespace database
{
template<class T, int info_len=2>
class MemoryRiver {
private:
    /* your code here */
    fstream file;
    string file_name;
    int sizeofT = sizeof(T);
    int T_cnt=0;
public:
    MemoryRiver() = default;
    void initialise(string FN = "") {
        if (FN != "") file_name = FN;
        file.open(file_name, std::ios::out);
        int tmp = 0;
        for (int i = 0; i <=info_len; ++i)
            file.write(reinterpret_cast<char *>(&tmp), sizeof(int));
        file.close();
    }
    MemoryRiver(const char * filename)
    {
        file_name=filename;
        if (access(filename,F_OK)!=0)
        {
           initialise(filename);
        }
    }
    void name(string FN)
    {
        file_name = FN;
    }

    //读出第n个int的值赋给tmp，0_base
    void get_info(int &tmp, int n) {
        if (n >= info_len) return;
        /* your code here */
        file.open(file_name, std::ios::in);
        file.seekp(n*sizeof(int));
        file.read(reinterpret_cast<char *>(&tmp), sizeof(int));
        file.close();
    }
    void get_many_info(int *tmp,int l,int r)
    {
        if (r >= info_len||r<l) return;
        file.open(file_name, std::ios::in);
        file.seekp(l*sizeof(int));
        file.read(reinterpret_cast<char *>(tmp), sizeof(int)*(r-l+1));
        file.close();
    }
    //将tmp写入第n个int的位置，0_base
    void write_info(int tmp, int n) {
        if (n >= info_len) return;
        /* your code here */
        file.open(file_name, std::ios::out|std::ios::in);
        file.seekp(n*sizeof(int));
        file.write(reinterpret_cast<char *>(&tmp), sizeof(int));
        file.close();
    }
    void write_many_info(int *tmp, int l,int r) {
        if (r >= info_len||r<l) return;
        /* your code here */
        file.open(file_name, std::ios::out|std::ios::in);
        file.seekp(l*sizeof(int));
        file.write(reinterpret_cast<char *>(tmp), sizeof(int)*(r-l+1));
        file.close();
    }

    //在文件合适位置写入类对象t，并返回写入的位置索引index
    //位置索引意味着当输入正确的位置索引index，在以下三个函数中都能顺利的找到目标对象进行操作
    //位置索引index可以取为对象写入的起始位置
    int write(T &t) {
        /* your code here */
        file.open(file_name, std::ios::out|std::ios::in);
        file.seekp(info_len*sizeof(int)+T_cnt*sizeofT);
        file.write(reinterpret_cast<char *>(&t), sizeofT);
        file.close();
        return T_cnt++;
    }
    int write_many(T *t,int len) {
        /* your code here */
        file.open(file_name, std::ios::out|std::ios::in);
        file.seekp(info_len*sizeof(int)+T_cnt*sizeofT);
        file.write(reinterpret_cast<char *>(t), sizeofT*len);
        file.close();
        T_cnt+=len;
        return T_cnt-len;
    }
    //用t的值更新位置索引index对应的对象，保证调用的index都是由write函数产生
    void update(T &t, const int index) {
        /* your code here */
        file.open(file_name,std::ios::out|std::ios::in);
        file.seekp(info_len*sizeof(int)+index*sizeofT);
        file.write(reinterpret_cast<char *>(&t), sizeofT);
        file.close();
    }
    void update_many(T *t, const int l,const int r) {
        /* your code here */
        if (r<=l){return;}
        file.open(file_name,std::ios::out|std::ios::in);
        file.seekp(info_len*sizeof(int)+l*sizeofT);
        file.write(reinterpret_cast<char *>(t), sizeofT*(r-l));
        file.close();
    }

    //读出位置索引index对应的T对象的值并赋值给t，保证调用的index都是由write函数产生
    void read(T &t, const int index) {
        /* your code here */
        file.open(file_name, std::ios::in);
        file.seekp(info_len*sizeof(int)+index*sizeofT);
        file.read(reinterpret_cast<char *>(&t), sizeofT);
        file.close();
    }
    void read_many (T *t, const int l,const int r) {
        if (r<=l){return;}
        /* your code here */
        file.open(file_name, std::ios::in);
        file.seekp(info_len*sizeof(int)+l*sizeofT);
        file.read(reinterpret_cast<char *>(t), sizeofT*(r-l));
        file.close();
    }

    //删除位置索引index对应的对象(不涉及空间回收时，可忽略此函数)，保证调用的index都是由write函数产生
    void Delete(int index) {
        /* your code here */
        return;
    }
};
const int base=257;
const int mod=1e9+7;
const int offset=4005;
template<class T>
class Block_Chain
{
    private:
        struct node
        {
            int hash_val;
            T value;
            node(int h=0,T val=T())
            {
                hash_val=h;value=val;
            }
            bool const operator < (node rhs) const{
                return hash_val!=rhs.hash_val?hash_val<rhs.hash_val:value<rhs.value;
            }
            bool const operator == (node rhs) const{
                return hash_val==rhs.hash_val&&value==rhs.value;
            }
            bool const operator != (node rhs) const{
                return hash_val!=rhs.hash_val||value!=rhs.value;
            }
        };
        MemoryRiver<node,offset*2>f;
        int block_len;
        int temp_len[offset],temp_addr[offset];
        node temp[offset];
    public:
        int hash(string s)
        {
            int len=s.length();long long v=0;
            for (int i=0;i<len;i++)
            {
                v=(v*base+s[i])%mod;
            }
            return v;
        }
        Block_Chain(const char * filename="ktest.out",int S=300)
        {
            block_len=S;
            if (access(filename,F_OK)!=0)
            {
                f.initialise(filename);
                f.write_many(temp,block_len);
            }
            else
            {
                f.name(filename);
            }
        }
        void make_new_block(int i)
        {
            //split block i(i>=1) into  blocks i and i+1
            //init_buffer
            int block_num=0;f.get_info(block_num,0);
            f.get_many_info(temp_len,i+1,block_num);
            f.get_many_info(temp_addr,i+1+offset,block_num+offset);
            
            //update blocknum and blocklen
            f.write_info(++block_num,0);
            f.write_info(block_len/2,i);
            f.write_info(block_len/2,i+1);
            f.write_many_info(temp_len,i+2,block_num);

            //update newblock_info
            int i_addr=0;f.get_info(i_addr,i+offset);
            f.read_many(temp,i_addr+block_len/2,i_addr+block_len);
            int index=f.write_many(temp,block_len);

            //update blockaddr
            f.write_info(index,i+1+offset);
            f.write_many_info(temp_addr,i+2+offset,block_num+offset);
        }
        void merge(int i)
        {
            //merge block i and i+1(i<block_num)
            //initialize buffer
            int block_num=0;f.get_info(block_num,0);
            int len1=0,len2=0;f.get_info(len1,i);f.get_info(len2,i+1);
            f.get_many_info(temp_len,i+2,block_num);
            f.get_many_info(temp_addr,i+2+offset,block_num+offset);
            
            //update newblock_info
            int i_addr=0,addr=0;f.get_info(i_addr,i+offset);f.get_info(addr,i+1+offset);
            f.read_many(temp,addr,addr+len2);
            f.update_many(temp,i_addr+len1,i_addr+len1+len2);

            //update blocknum and blocklen and blockaddr
            f.write_info(--block_num,0);
            f.write_info(len1+len2,i);
            f.write_many_info(temp_len,i+1,block_num);
            f.write_many_info(temp_addr,i+1+offset,block_num+offset);
        }
        void insert(string idx,T value)
        {
            node nd=node(hash(idx),value),cur=nd;
            int block_num=0;f.get_info(block_num,0);
            if (!block_num)
            {
                f.write_info(1,0);f.write_info(1,1);//initialize the first block
                f.write_info(0,1+offset);
                f.update(nd,0);
                return;
            }
            int L=1,R=block_num,addr=0,len=0;
            while (L<R)
            {
                int mid=((L+R+1)>>1);
                f.get_info(addr,mid+offset);
                f.read(cur,addr);
                if (cur<nd||cur==nd)
                {
                    L=mid;
                }
                else
                {
                    R=mid-1;
                }
            }
            int id=L;
            f.get_info(addr,id+offset);f.get_info(len,id);L=0;R=len-1;
            f.read(cur,addr+R);
            if (cur<nd)
            {
                f.update(nd,addr+len);
                f.write_info(++len,id);
            }
            else
            {
                while (L!=R)
                {
                    int mid=((L+R)>>1);
                    f.read(cur,addr+mid);
                    if (cur<nd)
                    {
                        L=mid+1;
                    }
                    else
                    {
                        R=mid;
                    }
                }
                f.read(cur,addr+L);
                if (cur==nd){return;}
                f.read_many(temp,addr+L,addr+len);
                f.update(nd,addr+L);
                f.write_info(++len,id);
                f.update_many(temp,addr+L+1,addr+len);
            }
            if (len==block_len)
            {
                make_new_block(id);
            }
        }
        void insert_to_the_end(string idx,T value)
        {
            node nd=node(hash(idx),value);
            int block_num=0;f.get_info(block_num,0);
            if (!block_num)
            {
                f.write_info(1,0);f.write_info(1,1);//initialize the first block
                f.write_info(0,1+offset);
                f.update(nd,0);
                return;
            }
            int id=block_num,addr=0,len=0;
            f.get_info(addr,id+offset);f.get_info(len,id);
            f.update(nd,addr+len);
            f.write_info(++len,id);
            if (len==block_len)
            {
                make_new_block(id);
            }
        }
        void modify(string idx,T value)
        {
            node nd=node(hash(idx),value),cur=nd;
            int block_num=0;f.get_info(block_num,0);
            int L=1,R=block_num,addr=0,len=0;
            while (L<R)
            {
                int mid=((L+R+1)>>1);
                f.get_info(addr,mid+offset);
                f.read(cur,addr);
                if (cur<nd||cur==nd)
                {
                    L=mid;
                }
                else
                {
                    R=mid-1;
                }
            }
            int id=L;
            f.get_info(addr,id+offset);f.get_info(len,id);L=0;R=len-1;
            f.read(cur,addr+R);
            if (cur<nd){return;}
            while (L!=R)
            {
                int mid=((L+R)>>1);
                f.read(cur,addr+mid);
                if (cur<nd)
                {
                    L=mid+1;
                }
                else
                {
                    R=mid;
                }
            }
            f.read(cur,addr+L);
            if (cur!=nd){return;}
            f.update(nd,addr+L);
        }
        void del(string idx,T value)
        {
            node nd=node(hash(idx),value),cur=nd;
            int block_num=0;f.get_info(block_num,0);
            if (!block_num){return;}
            int L=1,R=block_num,addr=0,len=0;
            f.read(cur,0);
            if (nd<cur){return;}
            while (L<R)
            {
                int mid=((L+R+1)>>1);
                f.get_info(addr,mid+offset);
                f.read(cur,addr);
                if (cur<nd)
                {
                    L=mid;
                }
                else if (cur==nd)
                {
                    L=mid;break;
                }
                else
                {
                    R=mid-1;
                }
            }
            int id=L;
            f.get_info(addr,id+offset);f.get_info(len,id);L=0;R=len-1;
            f.read(cur,addr+R);
            if (cur<nd){return;}
            while (L!=R)
            {
                int mid=((L+R)>>1);
                f.read(cur,addr+mid);
                if (cur<nd)
                {
                     L=mid+1;
                }
                else
                {
                    R=mid;
                }
            }
            f.read(cur,addr+L);
            if (cur!=nd){return;}
            f.read_many(temp,addr+L+1,addr+len);
            f.write_info(--len,id);
            f.update_many(temp,addr+L,addr+len);
            if (block_num==1&&len==0)
            {
                block_num=0;f.write_info(0,0);return;
            }
            if (id<block_num)
            {
                int nexlen=0;f.get_info(nexlen,id+1);
                if (len+nexlen<block_len)
                {
                    merge(id);
                }
            }
            f.get_info(len,id);
            if (id>1)
            {
                int prelen=0;f.get_info(prelen,id-1);
                if (prelen+len<block_len)
                {
                    merge(id-1);
                }
            }
        }
        void find(string idx)
        {
            int nd=hash(idx),block_num=0,addr=0,len=0,flg=0;node cur=node();
            f.get_info(block_num,0);
            for (int i=1;i<=block_num;i++)
            {
                f.get_info(len,i);f.get_info(addr,i+offset);f.read(cur,addr);
                if (cur.hash_val>nd){break;}
                f.read(cur,addr+len-1);
                if (cur.hash_val<nd){continue;}
                f.read_many(temp,addr,addr+len);
                for (int j=0;j<len;j++)
                {
                    if (temp[j].hash_val==nd)
                    {
                        temp[j].value.print();flg=1;
                    }
                    else if (temp[j].hash_val>nd){break;}
                }
            }
            if (!flg){std::cout<<std::endl;}
        }
        T findone(string idx)
        {
            int nd=hash(idx);node cur=node();
            int block_num=0,flg=0,addr=0,len=0;f.get_info(block_num,0);
            for (int i=1;i<=block_num;i++)
            {
                f.get_info(len,i);f.get_info(addr,i+offset);f.read(cur,addr);
                if (cur.hash_val>nd){break;}
                f.read(cur,addr+len-1);
                if (cur.hash_val<nd){continue;}
                f.read_many(temp,addr,addr+len);
                for (int j=0;j<len;j++)
                {
                    if (temp[j].hash_val==nd)
                    {
                        return temp[j].value;
                    }
                    else if (temp[j].hash_val>nd){break;}
                }
            }
            return T();
        }
        void all_out()
        {
            int block_num=0,addr=0,len=0;f.get_info(block_num,0);
            if (!block_num){std::cout<<std::endl;return;}
            for (int i=1;i<=block_num;i++)
            {
                f.get_info(len,i);f.get_info(addr,i+offset);
                f.read_many(temp,addr,addr+len);
                for (int j=0;j<len;j++)
                {
                    temp[j].value.print();
                }
            }
        }
};
}
