#include"util.h"
  
void * recvMessage(void * socket) {
 int sockfd, returnRecvfrom;
 char buffer[MAX]; 
 sockfd = (int) socket;

while(1) {
  //fill with zero
   memset(buffer, 0, MAX);  
  returnRecvfrom = recvfrom(sockfd, buffer, MAX, 0, NULL, NULL);
  if(returnRecvfrom< 0){
    perror("Error recvfrom");
   } else{
  printf("server: ");

  //fput means string  put
  fputs(buffer, stdout);  
 //strncmp for use string compare
 if((strncmp(buffer,"bye",3))==0)
{
printf("Client Exit...\n");
break;  
 }
}
}
}

int main(int argc, char *argv[]) { 

 
 int sockfd, returnStatus ,simplePort ,returnThread,returnSendto;  
 char buffer[MAX];
struct sockaddr_in addr ;
 pthread_t rThread;

 if (argc != 3){

  fprintf(stderr, "Usage : <port>  <ip address> \n" ,argv[0]);
  exit(1);
 }


 
  //creating socket for client
 sockfd = socket(AF_INET, SOCK_STREAM, 0);  

 if (sockfd < 0) {
    fprintf(stderr, "Could not create a socket!");
    exit(1);
 }else{
     printf("Socket created !\n");

 }



//fill with zero
 memset(&addr, 0, sizeof(addr)); 

 // atoi converts from string to integer  
 simplePort=atoi(argv[1]); 

//aderss family IPv4 Internet protocols
 addr.sin_family = AF_INET; 
 
//inet_addr,function that makes it easy to convert strings into IP addresses that are compatible with the sockaddr structure
 inet_addr(argv[2], &addr.sin_addr.s_addr);
 //hton will convert integers from local host’s native byte order to network byte order 
 addr.sin_port = htons(simplePort);     

 returnStatus = connect(sockfd, (struct sockaddr *) &addr, sizeof(addr)); 

//If successful, connect() returns 0 ,otherwise, –1 is returned
if (returnStatus == 0) {
  fprintf(stderr, "Connect successful!\n");
}else {

  fprintf(stderr, "Could not connect to address!\n");
  close(sockfd);
  exit(1);
}
//fill with zero
 memset(buffer, 0, MAX);
 printf("Enter your messages!\n");

 //creating a new thread for receive messages from  server
 returnThread = pthread_create(&rThread, NULL, recvMessage, (void *) sockfd);
 if (returnThread) {
  printf("ERROR: %d\n", returnThread);
  exit(1);
 }


//fgets() ,Reads the character of the string type
 while (fgets(buffer, MAX, stdin) != NULL) {
  returnSendto = sendto(sockfd, buffer, MAX, 0, (struct sockaddr *) &addr, sizeof(addr));  
  if (returnSendto < 0) {  
   printf("Error sending data!\n\t-%s", buffer);  
  }
 }


//close() function to close socket

 close(sockfd);
 pthread_exit(NULL);
 
 return 0;    
}