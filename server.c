
#include"util.h"

void * recvMessage(void * socket)
{
  int sockfd, returnRecvfrom;
  char buffer[MAX]; 
  sockfd = (int) socket;

   while(1)
   {
   //fill with zero
    memset(buffer, 0, MAX);  
    returnRecvfrom = recvfrom(sockfd, buffer, MAX, 0, NULL, NULL); 
       if(returnRecvfrom< 0)
       {
          perror("Error recvfrom");
       } 
       else
       {
          printf("Client: ");
          fputs(buffer, stdout);

            if((strncmp(buffer,"bye",3))==0)
            {
              printf("Server Exit...\n");
              break;
            }
         }  
     }
}

int  main(int argc , char *argv[]) 
{

   int sockfd, clientLen, returnStatus, newsockfd ,returnThread ,returnSendto, simplePort;
   char buffer[MAX];
   struct sockaddr_in addr, client_addr;
   pthread_t Thread;

     if (argc != 2)
     {
        fprintf(stderr, "Usage : <port> \n" ,argv[0]);
        exit(1);
     }
   //creating socket for server
   sockfd = socket(AF_INET, SOCK_STREAM, 0);

   if (sockfd < 0) 
   {
     fprintf(stderr, "Could not create a socket!");
     exit(1);
   }
   else
   {
       printf("Socket created !\n");
   }
 
   // atoi converts from string to integer  
   simplePort=atoi(argv[1]);
 
   //fill with zero
   memset(&addr, 0, sizeof(addr));
 
   //aderss family IPv4 Internet protocols
   addr.sin_family = AF_INET;

   //hton will convert integers from local host’s native byte order to network byte order 
   //INADDR_ANY so that our socket will bind to any of the local addresses 
   addr.sin_addr.s_addr =htonl(INADDR_ANY);
   addr.sin_port = htons(simplePort);

   //function is used to bind the socket to a given address once the socket is created ,return = is successfull
   returnStatus = bind(sockfd, (struct sockaddr *) &addr, sizeof(addr));

   if (returnStatus == 0) 
   {
      fprintf(stderr, "Bind completed!\n");
   } 
   else 
   {
      fprintf(stderr, "Could not bind to address!\n");
      close(sockfd);
      exit(1);
   }

   //funnction tells our socket that we’re ready to accept connections
   listen(sockfd, 5);
 
   clientLen = sizeof(client_addr);

   //accept() returns a new socket descriptor for the new connection
   newsockfd = accept(sockfd, (struct sockaddr *) &client_addr, &clientLen);

   if ( newsockfd == -1) 
   {
     fprintf(stderr, "Cannot accept connections!\n");
     close( newsockfd);
     exit(1);
   }

   memset(buffer, 0, MAX);
   printf("Enter your messages !\n");

   //creating a new thread for receive messages from client
   returnThread = pthread_create(&Thread, NULL, recvMessage, (void *) newsockfd);

   if (returnThread) 
   {
     printf("ERROR: %d\n", returnThread);
     exit(1);
   }

   //fgets() ,Reads the character of the string type
   while (fgets(buffer, MAX, stdin) != NULL) {

     returnSendto = sendto(newsockfd, buffer, MAX, 0, (struct sockaddr *) &client_addr, clientLen);  
        if (returnSendto < 0) 
        {  
           printf("Error sending data!\n");  
           exit(1);
        }

    }   

   //calls close()to terminate the connection
   close(newsockfd);
   close(sockfd);
   pthread_exit(NULL);
 
   return 0;
}
