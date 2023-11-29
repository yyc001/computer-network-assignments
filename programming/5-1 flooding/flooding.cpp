#include <iostream>
#include <vector>
#include <list>
using namespace std;
const int N = 10;
const int M = 100;
const int TTL_MAX = 10;
struct Packet{
    int ttl, flag, from, to, ds;
};
struct Router{
    int algo;
    int remember;
    vector<int> lines;
    vector< vector<int> > delay_table;
    list<Packet> packets;
}router[N];
struct Line{
    int from, to;
    // int maxneed_bandwidth;
    // vector<Packet> transmitting[2];
}line[M];

int n;
int mask, lcnt;

void connect(int v1, int v2)
{
    line[lcnt].from = v1;
    line[lcnt].to = v2;
    router[v1].lines.push_back(lcnt++);

    line[lcnt].from = v2;
    line[lcnt].to = v1;
    router[v2].lines.push_back(lcnt++);
}
// int flip()
// {
//     int cnt = 0;
//     for(int i = 0; i < lcnt; i++){
//         line[i].transmitting[mask].clear();
//         line[i].maxneed_bandwidth = max(line[i].maxneed_bandwidth, (int)(line[i].transmitting[mask^1].size()) );
//         cnt += line[i].transmitting[mask^1].size();
//         // cout << "Line " << i << " from " << line[i].from << " to " << line[i].to << " maxneed=" << line[i].maxneed_bandwidth << " , nxtneed=" << line[i].transmitting[mask^1].size() << endl;
//     }
//     // cout << endl;
//     mask = mask ^ 1;
//     return cnt;
// }
int push(int r)
{
    if(router[r].packets.empty()) {
        return -1;
    }
    Packet p = router[r].packets.front();
    router[r].packets.pop_front();
    if (p.to == r) {
        // cout << "Router "<< r << " recieved a packet as destination." << endl;
        return 1;
    }
    if(p.ttl == 0 ) {
        return 0;
    }
    // if(router[r].remember == p.flag) {
    //     return 0;
    // }
    // router[r].remember = p.flag;
    if(router[r].algo == 0) {
        for(int vo: router[r].lines) {
            router[line[vo].to].packets.push_back((Packet){p.ttl-1, p.flag, p.from, p.to, r});
        }
    } else if(router[r].algo == -1) {
        for(int vo: router[r].lines) {
            // cout << r << " " << line[vo].from << " "<< line[vo].to << " " << p.ds << endl;
            if(line[vo].to == p.ds) continue;
            router[line[vo].to].packets.push_back((Packet){p.ttl-1, p.flag, p.from, p.to, r});
        }
    } else if(router[r].algo > 0) {
        for(int d=0; d < router[r].algo; d++) {
            int vo = router[r].delay_table[p.to][d];
            router[line[vo].to].packets.push_back((Packet){p.ttl-1, p.flag, p.from, p.to, r});
        }
    }
    return 0;
}
void genpacket(int r, int t){
    router[r].packets.push_back((Packet){TTL_MAX, rand(), r, t, -1});
    // if(router[r].algo == 0) {
    //     for(int vo: router[r].lines) {
    //         line[vo].transmitting[mask^1].push_back((Packet){TTL_MAX, rand(), r, t});
    //     }
    // } else if(router[r].algo == -1) {
    //     for(int vo: router[r].lines) {
    //         line[vo].transmitting[mask^1].push_back((Packet){TTL_MAX, rand(), r, t});
    //     }
    // } else {

    // }
}
void flooding(){
    int maxneed_tot = 0, ctot = 0, flg=0;
    while(1){
        flg = 1;
        ctot++;
        for (int i=0; i<n; i++){
            if(push(i) != -1) {
                flg = 0;
            }
        }
        if(flg) break;
    }
    cout << "With algo=" << router[0].algo << ", delay=" << ctot <<endl;
}
int main()
{
    srand(time(NULL));
    // cin >> n;
    n = 10;
    connect(0,4);
    connect(0,5);
    connect(0,8);
    connect(1,4);
    connect(1,5);
    connect(2,4);
    connect(2,3);
    connect(3,9);
    connect(4,5);
    connect(5,6);
    connect(5,7);
    connect(6,9);

    genpacket(8, 2);
    for (int i=0; i<n; i++){
        router[i].algo = 0;
    }
    flooding();

    genpacket(8, 2);
    for (int i=0; i<n; i++){
        router[i].algo = -1;
    }
    flooding();

    genpacket(8, 2);
    for (int i=0; i<n; i++){
        router[i].algo = 1;
        router[i].delay_table.resize(n);
        // for(int j=0; j<n; j++){
        //     router[i].delay_table[j];
        // }
    }
    router[0].delay_table[2].push_back(0);
    router[1].delay_table[2].push_back(6);
    router[2].delay_table[2].push_back(12);
    router[3].delay_table[2].push_back(13);
    router[4].delay_table[2].push_back(11);
    router[5].delay_table[2].push_back(17);
    router[6].delay_table[2].push_back(22);
    router[7].delay_table[2].push_back(21);
    router[8].delay_table[2].push_back(5);
    router[9].delay_table[2].push_back(15);

    // for(int k=0; k<n;k++) {
    //     for(int i=0; i<n; i++) {
    //         for(int j=0; j<n;j++) {
                
    //         }
    //     }
    // }
    flooding();
}