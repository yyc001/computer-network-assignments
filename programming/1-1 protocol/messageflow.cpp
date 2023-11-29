#include<stdio.h>
#include<math.h>
#include<string.h>

int min(int x, int y){
    return x<y ? x: y;
}

int PL_send(char *message, int len){
    // pretend there is a network interface card
    fwrite(message, len , 1, stdout);
    fwrite("\n", 1 , 1, stdout);
    return 0;
}

char DLL_header[] = "[DLL header]";
#define DLL_BUF 128
#define DLL_MF 116
#define DLL_HL 12
int DLL_send(char *message, int len){
    char buf[DLL_BUF];
    int send_len, ret;
    while (len) {
        send_len = min(DLL_MF, len);
        memcpy(buf, DLL_header, DLL_HL);
        memcpy(buf+DLL_HL, message, send_len);
        if(ret = PL_send(buf, DLL_HL+send_len)) {
            return ret;
        }
        len -= send_len;
        message += send_len;
    }
    return 0;
}

char NL_header[] = "[NL header]";
#define NL_BUF 128
#define NL_MF 117
#define NL_HL 11
int NL_send(char *message, int len){
    char buf[NL_BUF];
    int send_len, ret;
    while (len) {
        send_len = min(NL_MF, len);
        memcpy(buf, NL_header, NL_HL);
        memcpy(buf+NL_HL, message, send_len);
        if(ret = DLL_send(buf, NL_HL+send_len)) {
            return ret;
        }
        len -= send_len;
        message += send_len;
    }
    return 0;
}


char TL_header[] = "[TL header]";
#define TL_BUF 128
#define TL_MF 117
#define TL_HL 11
int TL_send(char *message, int len){
    char buf[TL_BUF];
    int send_len, ret;
    while (len) {
        send_len = min(TL_MF, len);
        memcpy(buf, TL_header, TL_HL);
        memcpy(buf+TL_HL, message, send_len);
        if(ret = NL_send(buf, TL_HL+send_len)) {
            return ret;
        }
        len -= send_len;
        message += send_len;
    }
    return 0;
}


char SL_header[] = "[SL header]";
#define SL_BUF 128
#define SL_MF 117
#define SL_HL 11
int SL_send(char *message, int len){
    char buf[SL_BUF];
    int send_len, ret;
    while (len) {
        send_len = min(SL_MF, len);
        memcpy(buf, SL_header, SL_HL);
        memcpy(buf+SL_HL, message, send_len);
        if(ret = TL_send(buf, SL_HL+send_len)) {
            return ret;
        }
        len -= send_len;
        message += send_len;
    }
    return 0;
}



char AL_header[] = "[AL header]";
#define AL_BUF 128
#define AL_MF 117
#define AL_HL 11
int AL_send(char *message, int len){
    char buf[AL_BUF];
    int send_len, ret;
    while (len) {
        send_len = min(AL_MF, len);
        memcpy(buf, AL_header, AL_HL);
        memcpy(buf+AL_HL, message, send_len);
        if(ret = SL_send(buf, AL_HL+send_len)) {
            return ret;
        }
        len -= send_len;
        message += send_len;
    }
    return 0;
}

int main(){
    char och[88] = "\0";
    fgets(och, 88, stdin);
    AL_send(och, strlen(och));
}