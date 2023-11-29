#include <iostream>
#include <ctime>
using namespace std;

const int N = 500;
const int framesz = 24;
struct Station{
    int idx;
    int wait_time, fail_time;
    int avoid_mask;
    int active;
    int req(){
        return active && wait_time <= 0;
    }
    void block(){
        fail_time++;
        if(fail_time >= 16){
            active = 0;
            cerr << idx << " failed!" << endl;
            exit(1);
        }
        avoid_mask = (avoid_mask<<1|1) & 1023;
        wait_time = rand() & avoid_mask;
    }
};

Station station[N];
int main() {
    srand(time(NULL));
    int n, cnt, cnted = 0, clk = 1;
    for(int i=0; i< N; i++) {
        station[i].idx = i;
        station[i].active = 1;
    }
    for(clk = 1; cnted < N; clk++){
        // cout << "clk=" << clk << ",cnted=" << cnted << endl;
        cnt = 0;
        for(int i=0; i< N; i++) {
            station[i].wait_time--;
            if(station[i].req()){
                // cout << "i="<<i << endl;
                n = i;
                cnt++;
            }
        }
        if(cnt == 1){
            // cout << "ti="<<n << endl;
            station[n].active = 0;
            cout << n << " successfully start transmitting at " << (clk + framesz * cnted) << endl; 
            cnted++;
        }
        else if(cnt > 1) {
            for(int i=0; i< N; i++) {
                if(station[i].req()){
                    station[i].block();
                }
            }
        }
    }
    cout << "After " << clk << " slots." << endl;
    cout << "That means " << (clk + framesz * N)*51.2 << " us." << endl;
}