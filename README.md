# InternetPhone
- Require sox to play the audio `sudo apt install sox`
- To run server machine: 
  - Create program file `gcc -pthread -o serv_send2 serv_send2.c`
  - Run the program `rec -t raw -b 16 -c 1 -e s -r 44100 - | ./serv_send2 [Port Number]`
- After initializing server, run client on another machine or a new terminal window: 
  - Create protgram file `gcc -o client_recv client_recv.c`
  - Run the program `./client_recv [Server IP address] [Server Port Number] | play -t raw -b 16 -c 1 -e s -r 44100 -`
