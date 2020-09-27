#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#define N 1

void die(char *s){
  perror(s);
  exit(1);
}

int main(int argc, char **argv){
  if(argc!=3) {
    printf("Usage: ./client_recv [IP address] [Port]\n");
    return 1;// In main(), returning non-zero means failure
  }

  char* p;
  errno = 0; 
  unsigned int port=strtol(argv[2], &p, 10);
  if (*p != '\0' || errno != 0){
    printf("Port should be a number\n");
    return 2;
  }

  int s = socket(PF_INET, SOCK_STREAM, 0);
  if (s == -1) die("socket");

  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  if (inet_aton(argv[1],&addr.sin_addr) == 0) die("inet_aton");
  addr.sin_port = htons(port);
  
  if (connect(s, (struct sockaddr *)&addr, sizeof(addr)) == -1) die("connect");

  long i=0;
  unsigned char data[N];
  while(1){
    int n = (read(s,data,N));
    if (n == -1) die("Read");
    if (n == 0) break;
    //printf("%c",*data);
    write(1, data, N);
    i++;
  }
  shutdown(s, SHUT_WR);
  //close(s);
}