#include <iostream>
#include <string>
using namespace std;
// char mbuf[N], pbuf[N];
typedef unsigned long long ull;
ull str2ull(const char *ch)
{
    ull l = 0;
    for(; *ch; ch++){
        l = (l<<1) + (*ch - '0');
    }
    return l;
}
string mbuf, pbuf; 
int n, k, bit;
int main(int argc, char *argv[])
{
    
    cin >> mbuf >> pbuf;
    n = mbuf.length();
    k = pbuf.length();
    if(argc > 1) {
        bit = str2ull(argv[1]) - 1;
        mbuf[bit] = (char)(mbuf[bit] ^ 1);
    }
    cout << mbuf << endl;
    cout << pbuf << endl;
}