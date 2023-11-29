#include<iostream>
#include<cstring>
using namespace std;

#define MAX_ROUTER 10
#define MAX_BUF 3
#define MAX_RETRY 2
// #define TIMEOUT 9
int TIMEOUT;
int n, global_clk;
int global_rty, global_fail, global_suc;

#define CODE_MSG 0
#define CODE_ARP 1
struct Packet{
    int flag, src, dst, code;
};
ostream& operator<<(ostream& out, const Packet& p)
{
    out << "Packet flag="<< p.flag << ", src="<< p.src << ", dst=" << p.dst << ", code="<<p.code;
    return out;
}

#define BUF_EMPTY 0
#define BUF_RECIEVE 1
#define BUF_SENDING 2
struct PacketBuf{
    Packet packet;
    int status, retry;
    uint clk;
};
struct Router{
    int idx;
    int route_table[MAX_ROUTER];
    PacketBuf buf[MAX_BUF];
    void push_packet(const Packet &p){
        for(int i=0; i<MAX_BUF; i++) {
            if(buf[i].status == BUF_EMPTY) {
                buf[i].packet = p;
                buf[i].status = BUF_RECIEVE;
                buf[i].clk = global_clk;
                return;
            }
        }
    }
    int retrying();
    int flowing();
    void jump();

} router[MAX_ROUTER];

int Router::retrying()
{
    for(int i=0; i<MAX_BUF; i++) {
        if(buf[i].status == BUF_SENDING && buf[i].clk + TIMEOUT <= global_clk && buf[i].retry >= MAX_RETRY) {
            buf[i].status = BUF_EMPTY;
            // cout << "Router "<< idx << " failed to transmit " << buf[i].packet.flag << "." << endl;
            global_fail++;
        }
    }
    for(int i=0; i<MAX_BUF; i++) {
        if(buf[i].status == BUF_SENDING && buf[i].clk + TIMEOUT <= global_clk) {
            // cout << buf[i].packet << endl;
            buf[i].clk = global_clk;
            buf[i].retry++;
            int reqto = route_table[buf[i].packet.dst];
            router[reqto].push_packet(buf[i].packet);
            // cout << "reqto=" << reqto<< endl;
            // cout << "Router "<< idx << " retry " << buf[i].packet.flag << " " << buf[i].retry << " times." << endl;
            global_rty++;
            return 1;
        }
    }
    return 0;
}
int Router::flowing()
{
    uint minclk = 0x3f3f3f3f;
    int b = -1;
    for(int i=0; i<MAX_BUF; i++) {
        if(buf[i].clk < minclk && buf[i].status == BUF_RECIEVE) {
            b = i;
            minclk = buf[i].clk;
        }
    }
    if(b == -1) return 0;

    if(buf[b].packet.dst == idx) {
        if(buf[b].packet.code == CODE_MSG) {
            int reqto = route_table[buf[b].packet.src];
            buf[b].status = BUF_EMPTY;
            router[reqto].push_packet((Packet){buf[b].packet.flag, idx, buf[b].packet.src, CODE_ARP});
            return 1;
        } else {
            // cout << "Router "<< idx << " arp " << buf[b].packet.flag << " acknowlegement." << endl;
            buf[b].status = BUF_EMPTY;
            for(int i=0; i<MAX_BUF; i++){
                if(buf[i].status == BUF_SENDING && buf[i].packet.flag == buf[b].packet.flag) {
                    // cout << "Router "<< idx << " succesfully sent" << buf[i].packet.flag << " and heard an acknowlegement." << endl;
                    global_suc++;
                    buf[i].status = BUF_EMPTY;
                    return 0;
                }
            }
        }
    } else if(buf[b].packet.src == idx) { // 理论上只有初始化的packet能到达这里
        int reqto = route_table[buf[b].packet.dst];
        buf[b].status = BUF_SENDING;
        buf[b].retry = 0;
        buf[b].clk = global_clk;
        router[reqto].push_packet(buf[b].packet);
        // cerr << "???????" << endl;
        return 1;
    } else {
        int reqto = route_table[buf[b].packet.dst];
        buf[b].status = BUF_EMPTY;
        router[reqto].push_packet(buf[b].packet);
        return 1;
    }
    return 0;
}

void Router::jump(){

    if(retrying()) return;

    flowing();
}
int d[MAX_ROUTER][MAX_ROUTER];
void extend_route_table(){
    for(int k=0; k<n; k++) {
        for(int i=0; i<n; i++) {
            for(int j=0; j<n; j++) {
                if(d[i][j] > d[i][k] + d[k][j]) {
                    d[i][j] = d[i][k] + d[k][j];
                    router[i].route_table[j] = router[i].route_table[k];
                }
            }
        }
    }
    for(int i=0; i<n; i++) {
        for(int j=0; j<n; j++) {
            if(d[i][j] == 0x3f3f3f3f) {
                cerr << "Network is not connected." << endl;
            }
        }
    }
    for(int i=0; i<n; i++) {
        for(int j=0; j<n; j++) {
            cout << router[i].route_table[j] << " ";
        }
        cout << endl;
    }
    cout << "--" << endl;
}
void connect(int a, int b){
    d[a][b] = d[b][a] = 1;
    router[a].route_table[b] = b;
    router[b].route_table[a] = a;
}
int main(){
    memset(d, 0x3f, sizeof(d));
    n = 10;
    for(int i=0; i<n; i++){
        d[i][i] = 0;
        router[i].route_table[i] = i;
        router[i].idx = i;
    }
    
    connect(0, 1);
    connect(1, 2);
    connect(2, 3);
    connect(3, 4);
    connect(4, 5);
    connect(5, 6);
    connect(6, 7);
    connect(7, 8);
    connect(8, 9);
    connect(9, 0);
    connect(0, 5);
    connect(1, 6);
    connect(2, 7);
    extend_route_table();

    cout << "ok" << endl;

    for(TIMEOUT = 1; TIMEOUT < 10; TIMEOUT++) {
        router[0].push_packet((Packet){1, 0, 8, CODE_MSG});
        router[1].push_packet((Packet){2, 1, 0, CODE_MSG});
        router[2].push_packet((Packet){3, 2, 5, CODE_MSG});
        global_rty = global_fail = global_suc = global_clk = 0;
        int flg = 1;
        while(flg){
            flg = 0;
            for(int i=0; i<n; i++) router[i].jump();
            for(int i=0; i<n; i++) {
                for(int j=0; j<MAX_BUF; j++) if(router[i].buf[j].status) {
                    flg = 1;
                    // cout << router[i].buf[j].status << " " <<  router[i].buf[j].retry << " " << router[i].buf[j].clk << endl;
                    // cout << router[i].buf[j].packet << endl; 
                }
            }
            global_clk++;
        }
        cout << "With timeout=" << TIMEOUT << ": suc=" << global_suc << ",  rty=" << global_rty << ", fail=" << global_fail << endl;
    }
}