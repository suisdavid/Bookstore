#ifndef BPT_MEMORYRIVER_HPP
#define BPT_MEMORYRIVER_HPP

#include <fstream>
#include <iostream>
#include <type_traits>
#include <filesystem>
#include <system_error>

using std::string;
using std::fstream;
using std::ifstream;
using std::ofstream;

template<class T, int info_len = 2>
class MemoryRiver {
private:
    static_assert(std::is_trivially_copyable<T>::value,
                  "MemoryRiver requires trivially copyable (POD-like) types T");

    fstream file;
    string file_name;
    int sizeofT = static_cast<int>(sizeof(T));
    int T_cnt = 0; // number of T objects currently in file (derived from file size)

    // Ensure the file exists and has at least `info_len` ints reserved for header
    void ensure_file_exists() {
        if (file_name.empty()) return;
        std::error_code ec;
        if (!std::filesystem::exists(file_name, ec)) {
            std::ofstream ofs(file_name, std::ios::binary);
            int tmp = 0;
            for (int i = 0; i < info_len; ++i)
                ofs.write(reinterpret_cast<char *>(&tmp), sizeof(int));
            ofs.close();
        }
    }

    // Recompute T_cnt from the current file size. Call after ensure_file_exists().
    void refresh_T_cnt() {
        if (file_name.empty()) { T_cnt = 0; return; }
        std::error_code ec;
        auto sz = std::filesystem::file_size(file_name, ec);
        if (ec) { T_cnt = 0; return; }
        long long header = static_cast<long long>(info_len) * static_cast<long long>(sizeof(int));
        if (sz <= static_cast<uintmax_t>(header)) { T_cnt = 0; return; }
        T_cnt = static_cast<int>((static_cast<long long>(sz) - header) / sizeofT);
    }

    // Open file for reading (binary). Creates file if missing.
    std::ifstream open_ro() {
        ensure_file_exists();
        std::ifstream in(file_name, std::ios::binary);
        return in;
    }

    // Open file for read/write (binary). Creates file if missing.
    std::fstream open_rw() {
        ensure_file_exists();
        std::fstream f(file_name, std::ios::in | std::ios::out | std::ios::binary);
        if (!f.is_open()) {
            // try to create then reopen
            std::ofstream ofs(file_name, std::ios::binary | std::ios::app);
            ofs.close();
            f.open(file_name, std::ios::in | std::ios::out | std::ios::binary);
        }
        return f;
    }

public:
    MemoryRiver() = default;

    MemoryRiver(const string& file_name) : file_name(file_name) {
        ensure_file_exists();
        refresh_T_cnt();
    }

    void name(string FN)
    {
        file_name = FN;
        ensure_file_exists();
        refresh_T_cnt();
    }

    void initialise(string FN = "") {
        if (FN != "") file_name = FN;
        ensure_file_exists();
        // reset header
        std::ofstream ofs(file_name, std::ios::binary | std::ios::trunc);
        int tmp = 0;
        for (int i = 0; i < info_len; ++i)
            ofs.write(reinterpret_cast<char *>(&tmp), sizeof(int));
        ofs.close();
        T_cnt = 0;
    }

    //读出第n个int的值赋给tmp，0_base
    void get_info(int &tmp, int n) {
        if (n >= info_len) return;
        std::ifstream in = open_ro();
        if (!in.is_open()) return;
        in.seekg(static_cast<std::streamoff>(n) * sizeof(int), std::ios::beg);
        in.read(reinterpret_cast<char *>(&tmp), sizeof(int));
    }

    void get_many_info(int *tmp,int l,int r)
    {
        if (r >= info_len || r < l) return;
        std::ifstream in = open_ro();
        if (!in.is_open()) return;
        in.seekg(static_cast<std::streamoff>(l) * sizeof(int), std::ios::beg);
        in.read(reinterpret_cast<char *>(tmp), sizeof(int) * (r - l + 1));
    }

    //将tmp写入第n个int的位置，0_base
    void write_info(int tmp, int n) {
        if (n >= info_len) return;
        std::fstream f = open_rw();
        if (!f.is_open()) return;
        f.seekp(static_cast<std::streamoff>(n) * sizeof(int), std::ios::beg);
        f.write(reinterpret_cast<char *>(&tmp), sizeof(int));
    }

    void write_many_info(int *tmp, int l,int r) {
        if (r >= info_len || r < l) return;
        std::fstream f = open_rw();
        if (!f.is_open()) return;
        f.seekp(static_cast<std::streamoff>(l) * sizeof(int), std::ios::beg);
        f.write(reinterpret_cast<char *>(tmp), sizeof(int) * (r - l + 1));
    }

    //在文件合适位置写入类对象t，并返回写入的位置索引index
    //位置索引意味着当输入正确的位置索引index，在以下三个函数中都能顺利的找到目标对象进行操作
    //位置索引index可以取为对象写入的起始位置
    int write(T &t) {
        std::fstream f = open_rw();
        if (!f.is_open()) return -1;
        refresh_T_cnt();
        f.seekp(static_cast<std::streamoff>(info_len * sizeof(int) + T_cnt * sizeofT), std::ios::beg);
        f.write(reinterpret_cast<char *>(&t), sizeofT);
        int ret = T_cnt;
        ++T_cnt;
        return ret;
    }

    int write_many(T *t,int len) {
        if (len <= 0) return -1;
        std::fstream f = open_rw();
        if (!f.is_open()) return -1;
        refresh_T_cnt();
        f.seekp(static_cast<std::streamoff>(info_len * sizeof(int) + T_cnt * sizeofT), std::ios::beg);
        f.write(reinterpret_cast<char *>(t), static_cast<std::streamsize>(sizeofT) * len);
        int ret = T_cnt;
        T_cnt += len;
        return ret;
    }

    //用t的值更新位置索引index对应的对象，保证调用的index都是由write函数产生
    void update(T &t, const int index) {
        if (index < 0) return;
        std::fstream f = open_rw();
        if (!f.is_open()) return;
        f.seekp(static_cast<std::streamoff>(info_len * sizeof(int) + index * sizeofT), std::ios::beg);
        f.write(reinterpret_cast<char *>(&t), sizeofT);
    }

    void update_many(T *t, const int l,const int r) {
        if (r <= l) { return; }
        std::fstream f = open_rw();
        if (!f.is_open()) return;
        f.seekp(static_cast<std::streamoff>(info_len * sizeof(int) + l * sizeofT), std::ios::beg);
        f.write(reinterpret_cast<char *>(t), static_cast<std::streamsize>(sizeofT) * (r - l));
    }

    //读出位置索引index对应的T对象的值并赋值给t，保证调用的index都是由write函数产生
    void read(T &t, const int index) {
        if (index < 0) return;
        std::ifstream in = open_ro();
        if (!in.is_open()) return;
        in.seekg(static_cast<std::streamoff>(info_len * sizeof(int) + index * sizeofT), std::ios::beg);
        in.read(reinterpret_cast<char *>(&t), sizeofT);
    }

    void read_many (T *t, const int l,const int r) {
        if (r <= l) { return; }
        std::ifstream in = open_ro();
        if (!in.is_open()) return;
        in.seekg(static_cast<std::streamoff>(info_len * sizeof(int) + l * sizeofT), std::ios::beg);
        in.read(reinterpret_cast<char *>(t), static_cast<std::streamsize>(sizeofT) * (r - l));
    }

    //删除位置索引index对应的对象(不涉及空间回收时，可忽略此函数)，保证调用的index都是由write函数产生
    void Delete(int index) {
        (void)index;
        return;
    }
};


#endif //BPT_MEMORYRIVER_HPP