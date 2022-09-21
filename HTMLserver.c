/*///////////////////////////////////////////////////////////
	Network Programming assignment made by Achyut Tiwari
	Roll No. : 102003640
	Group : 3CO25
*////////////////////////////////////////////////////////////

#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define SERVER_PORT 80 

int main()
{
	int listen_sd, comm_sd, portno;
	long valread; 
	char buff[70000];

    /* Reading from config.txt */
    FILE *config_data;
	config_data = fopen("config.txt", "r");
	char *tok,html_files[30000]="",string_config_data[70000];
	bzero(string_config_data,70000);
	fgets(string_config_data,70000,config_data);
	tok = strtok(string_config_data," ");
	tok = strtok(NULL," ");
	for(int i=0; tok != NULL ; i++) 
    {
    	if (i == 0)
    	{
    		sscanf(tok,"%d",&portno);
    	}
        tok = strtok(NULL," ");
        if (tok!=NULL && i > 0)
            strcat(html_files,tok);
        if (i>0)
            strcat(html_files," ");
    }
    printf("Port Number: %d\n",portno);
    printf("Files: %s\n",html_files);
    char temp_html_files[70000];
    strcpy(temp_html_files,html_files);

    /* Creating our HTML file by attaching suitable header */
	FILE *html_data;
	html_data = fopen("index.html","r");
	char string_html[70000];
	char html_header[70000] = "HTTP1.1 200 OK\r\n\r\n";
	bzero(string_html,70000);
	fgets(string_html,70000,html_data);
	strcat(html_header,string_html);

    /* Socket Programming */
	listen_sd = socket(AF_INET, SOCK_STREAM,0);
	struct sockaddr_in servaddr;
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(portno);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	bind(listen_sd,(struct sockaddr*)&servaddr,sizeof(servaddr));
	listen(listen_sd,70000); 

    /* Server side will run forever to recieve client connections */
	for(;;)
	{
		int file_avail = 0;
		printf("Waiting for connection...\n");
		fflush(stdout);

		comm_sd = accept(listen_sd,(struct sockaddr*)NULL,NULL);

		/* Accept Client Request and store request in tok*/
		char buffer[70000] = {0};
        valread = read(comm_sd ,buffer,70000);
        printf("%s\n",buffer );   
        tok = strtok(buffer," ");
        tok = strtok(NULL," ");
        memmove(tok, tok+1, strlen(tok));

        /* To identify whether requested file is available or not */
        char *token;
        token = strtok(html_files," ");
        while (token != NULL)
        {
        	printf("\nHeaders:%s %s\n",token,tok);
        	if(strcmp(token,tok) == 0)
        		file_avail = 1;
        	token = strtok(NULL," ");
        }
        strcpy(html_files,temp_html_files);
      
        if (file_avail == 1)
        {
        	/* Creating our HTML file by attaching suitable header */
        	FILE *html_data;
			html_data = fopen(tok,"r");
			strcpy(html_header,"HTTP1.1 200 OK\r\n\r\n");
			bzero(string_html,70000);
			fgets(string_html,70000,html_data);
			strcat(html_header,string_html);	
        }
printf("%s\n",html_header);
        bzero( buff, 70000);
		snprintf((char*)buff,sizeof(buff),"%s",html_header);
		//write(comm_sd,(char *)buff,strlen((char *)buff));
		//send(comm_sd,html_header,sizeof(html_header),0);
write(comm_sd,(char *)buff,strlen((char *)buff));
		close(comm_sd);
	}
    return 0;
}