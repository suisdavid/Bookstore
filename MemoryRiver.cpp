#include "MemoryRiver.hpp"
#include <iostream>
using namespace std;
int tmp=1;
long long x=(1ll<<60),y=-5,z=5,h=10,p=-1;
long long a[3],b[3],c[3];
int main()
{
    MemoryRiver<long long>f;
    f.initialise("test.out");
    f.get_info(tmp,1);
    cout<<tmp<<endl;
    int index=f.write(x);
    int index2=f.write(z);
    f.update(h,1);
    f.read_many(a,0,2);
    cout<<a[0]<<" "<<a[1]<<endl;
    f.write_many(a,2);
    b[0]=114;b[1]=514;
    //f.update_many(b,2,4);
    f.read_many(c,2,4);
    cout<<c[0]<<" "<<c[1]<<endl;
}