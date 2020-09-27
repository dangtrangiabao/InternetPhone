#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h> 
#define N 1024

unsigned char data[N];
int state = 0;
//pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void die(char *s){
  perror(s);
  exit(1);
}

void *read_stdin(void *arg){
  while(1){
    //pthread_mutex_lock(&lock);
    if ((read(0,data,N)) == -1) die("read");
    //pthread_mutex_unlock(&lock);
    if (state) break;
  }
  return NULL;
}

int main(int argc, char **argv){
  if (argc!=2) {
    printf("Usage: ./serv_send2 [Port]\n");
    return 2;
  }

  char* p;
  errno = 0; 
  unsigned int port=strtol(argv[1], &p, 10);
  if (*p != '\0' || errno != 0){
    printf("Port should be a number\n");
    return 3;
  }
  
  pthread_t thread_id;
  if (pthread_create(&thread_id, NULL, read_stdin, NULL)) die("pthread_create");

  int ss = socket(PF_INET, SOCK_STREAM, 0);
  if (ss == -1) die("socket");  

  struct sockaddr_in addr; 
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = INADDR_ANY; 
  
  if (bind(ss, (struct sockaddr *)&addr, sizeof(addr)) == -1) die("bind"); 
  else printf("Binded Successfully\n"); 

  if (listen(ss,10) == -1) die("listen");
  else printf("Listening...\n");  

  struct sockaddr_in client_addr;  
  socklen_t len = sizeof(struct sockaddr_in);
  int s = accept(ss, (struct sockaddr *)&client_addr, &len);
  if (s == -1) die("accept");
  close(ss);

  //pthread_mutex_lock(&lock);
  state = 1;
  //pthread_mutex_unlock(&lock);

  if (pthread_join(thread_id, NULL)) die("thread_join");
  //pthread_mutex_destroy(&lock);
  
  while(1){
    memset(data,0,N);
    int n = (read(0,data,N));
    if (n == -1) die("Read");
    if (n == 0) break;
    send(s, data, N, 0);
  }

  close(s);
}