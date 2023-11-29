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
string ull2str(ull val, int len)
{
    string st;
    for(int i = len-1; i>=0; i--){
        st += (char)('0' + ((val>>i)&1));
    }
    return st;
}
string mbuf, pbuf; 
int n, k;
ull mod, rem;
int main()
{
    cin >> mbuf >> pbuf;
    // cout << mbuf;
    n = mbuf.length();
    k = pbuf.length();

    rem = str2ull(mbuf.substr(0, k-1).c_str());
    mod = str2ull(pbuf.c_str());

    for(int i=k-1; i<n; i++){
        rem = (rem<<1) + (ull)(mbuf[i] - '0');
        if(rem & (1<<k-1)) {
            rem = rem ^ mod;
        }
        // cerr << ull2str(rem, k) << endl;
    }
    // cout << ull2str(rem, k-1) << endl;
    if (rem == 0) {
        cout << "Correct!" <<  endl;
    } else {
        cout << "Wrong!" << endl;
    }
    // cout << pbuf << endl;
}