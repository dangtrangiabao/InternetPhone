# InternetPhone

- To run server machine: 
  - Create program file `gcc -pthread -o serv_send2 serv_send2.c`
  - Run the program `./serv_send2 [Port Number]`
- After initializing server, run client on another machine or a new terminal window: 
  - Create protgram file `gcc -o client_recv client_recv.c`
  - Run the program `./client_recv [Server IP address] [Server Port Number]`
