/***************************************************************************/
/*                                                                         */    
/* Client program which gets as parameter the server name or               */    
/*     address and tries to send the data into non-blocking server.        */    
/*                                                                         */    
/* The message is sent after 5 seconds of wait                             */    
/*                                                                         */    
/*                                                                         */    
/* based on Beej's program - look in the simple TCp client for further doc.*/    
/*                                                                         */    
/*                                                                         */    
/***************************************************************************/
    #include <stdio.h> 
    #include <stdlib.h> 
    #include <errno.h> 
    #include <string.h> 
    #include <netdb.h> 
    #include <sys/types.h> 
    #include <netinet/in.h> 
    #include <sys/socket.h> 
    #include <unistd.h>

    #define PORT 80    /* the port client will be connecting to */

    #define MAXDATASIZE 1000 /* max number of bytes we can get at once */

    int main(int argc, char *argv[])
    {
        int sockfd, numbytes;  
        char buf[MAXDATASIZE];
        struct hostent *he;
        struct sockaddr_in their_addr; /* connector's address information */

        if (argc != 2) {
            fprintf(stderr,"usage: client hostname\n");
            exit(1);
        }

        if ((he=gethostbyname(argv[1])) == NULL) {  /* get the host info */
            herror("gethostbyname");
            exit(1);
        }

        if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
            perror("socket");
            exit(1);
        }

        their_addr.sin_family = AF_INET;      /* host byte order */
        their_addr.sin_port = htons(PORT);    /* short, network byte order */
        their_addr.sin_addr = *((struct in_addr *)he->h_addr);
        bzero(&(their_addr.sin_zero), 8);     /* zero the rest of the struct */

        if (connect(sockfd, (struct sockaddr *)&their_addr, \
                                              sizeof(struct sockaddr)) == -1) {
            perror("connect");
            exit(1);
        }
	char messageBuf[100];
	char * message = "GET /channels/227943/field/1/last.json?api_key=372709TAH7GDVVS0 HTTP/1.1\r\nHost: api.thingspeak.com\r\nConnection: keep-alive\r\n\r\n";
	strcpy(messageBuf,message);
//	while (1) {
		if (send(sockfd, messageBuf, strlen(messageBuf), 0) == -1){
                      perror("send");
		      exit (1);
		}
		printf("After the send function \n");

        	if ((numbytes=recv(sockfd, buf, MAXDATASIZE, 0)) == -1) {
			printf("nic nie odebrano\r\n");
            		perror("recv");
            		exit(1);
		}	
		printf("odebrano %d bajtow\r\n",numbytes);

	        buf[numbytes] = '\0';

        	printf("Received in pid=%d, text=: %s \n",getpid(), buf);
		sleep(1);

//	}

        close(sockfd);

        return 0;
    }



