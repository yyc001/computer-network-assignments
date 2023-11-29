#include <bits/stdc++.h>
using namespace std;

struct Rule{
    uint ip, mask, line;
    Rule(const char *strip, int dot, int line);
};

vector<Rule> cidr; 

int forward(uint ip, int default_line){
    for(Rule r: cidr){
        if(r.ip == (ip&r.mask)) {
            return r.line;
        }
    }
    return default_line;
}
uint ip2int(const char *ch)
{
    uint ret = 0;
    char chbuf[16];
    strcpy(chbuf, ch);
    char* p = strtok(chbuf, ".");

    for(int i=0; i<4; i++) {
        ret += atoi(p)<<((3-i)*8);
        p = strtok(NULL,".");
    }

    return ret;
}

Rule::Rule(const char *strip, int dot, int line){
    this->ip = ip2int(strip);
    this->mask = ((uint)-1) << (32-dot);
    this->line = line;
}
int main(){
    cidr.push_back(Rule("132.0.0.0", 11, 1));
    cidr.push_back(Rule("132.0.0.0", 8, 2));
    cidr.push_back(Rule("132.19.232.0", 22, 3));
    cidr.push_back(Rule("0.0.0.0", 0, 4));
    char buf[20];
    cin >> buf;
    cout << forward(ip2int(buf), 0) << endl;
}