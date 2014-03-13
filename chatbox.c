#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include<signal.h>
void act_as_a_server();
void act_as_a_client();
pid_t pid;
int t_close=0;
int parent_pid;
int fd[2];
void cfilehash_verify(char send_data[], int *connected)
{
    char recv_data[100000];
    int t=0,i;
    if(send(*connected,send_data,1024,0)<0)
        error("ERROR in writing to socket");
    int f=0,flag=1;
    bzero(recv_data, 1024);
    printf("\nrecvd information : %s\n",recv_data);
    printf("File-name\tLatest-Timestamp\tMd5hash\n");
    while((f= recv(*connected, recv_data,1024,0)) > 0)
    {
        if(strcmp(recv_data,"END")==0)
        {flag=0;
            break;
        }
        else
            printf("%s\n",recv_data);
    }
    if(flag==1)
        exit(0);
}

void cfilehash_checkall(char write_buffer[], int *socketfd)
{
    int n;
    char read_buffer[1024];
    n = write(*socketfd,write_buffer,1024);
    if (n < 0)
        error("ERROR writing to socket");
    int t=0,i;

    int f=0,flag=1;
    bzero(read_buffer, 1024);
    printf("\nrecvd data : %s\n",read_buffer);
    printf("File-name   Latest-Timestamp    Md5hash\n");
    while((f= recv(*socketfd, read_buffer,1024,0)) > 0)
    {
        if(strcmp(read_buffer,"END")==0)
        {flag=0;
            break;
        }
        else
            printf("%s\n",read_buffer);
    }
    if(flag==1)
        exit(0);
}

void filehash_checkall(char recv_data[] , int *connected)
{
    char send_data[1000000];
    char line [100000];
    char list_of_commands[100000];
    char f_name[1000000];
    bzero(send_data,10000);
    int i,iterator=0;
    system("ls -l |grep -v ^d | awk '{print $9}' | tail -n +2 > list_files");
    int flag=0;
    FILE *fp = fopen ("list_files", "r" );
    if ( fp != NULL )
    {
        while ( fgets ( line, sizeof(line), fp ) != NULL )
        {
            char array[100000],array1[100000],array3[10000],array4[100000];
            int k=0,i=0,iterator=0;
            while(line[i]!='\n')
            {
                f_name[k]=line[i];
                k++;
                i++;
            }
            iterator++;
            f_name[k]='\0';
            system("touch timestamp_output");
            system("touch md5sum_output");
            char command2[500],command[100];
            strcpy(command,"stat ");
            strcat(command,f_name);
            strcat(command," | grep Modify | awk '{print $3}' > timestamp_output");
            strcpy(command2,"md5sum ");
            strcat(command2,f_name);
            strcat(command2," > md5sum_output");
            system(command2);
            system(command);
            printf("command--- %s\n",command);
            printf("command2--- %s\n",command2);
            FILE *f=fopen("md5sum_output","r");
            FILE *f1=fopen("timestamp_output","r");
            fgets(array,sizeof(array),f);
            fgets(array1,sizeof(array1),f1);
            system("rm -rf md5sum_output");
            system("rm -rf timestamp_output");
            printf("timestmp-- %s\n", array1);
            printf("md5-- %s\n", array);
            int var=0,var1=0;
            while(array[var]!=' ')//getting the md5sum from the file which has filename also
            {
                array3[var1]=array[var];
                var++;
                var1++;
            }
            printf("md53-- %s\n", array3);
            array3[var1]='\0';
            strcat(f_name,"\t");
            strcat(f_name,array3);
            strcat(f_name,"\t");
            array1[strlen(array1)-1]='\0';
            strcat(f_name,array1);
            strcpy(send_data,f_name);
            if(send(*connected, send_data, 1024,0) < 0)
            {
                error("\nERROR: Writing to socket");
                exit(1);
            }
            printf("yahhoo\n");
            iterator++;
        }
        flag=1;
        iterator++;
    }
    else
        error("Error in filen opening");
    if(flag==1)
    {
        bzero(send_data, 1024);
        strcpy(send_data,"END");
        printf("send-- %s\n",send_data);
        if(send(*connected, send_data, 1024,0) < 0)
        {
            error("\nERROR: Writing to socket");
            exit(1);
        }
    }
    system("rm -rf list_files");
}
void filehash_verify(char recv_data[],int *connected)
{
    char send_data[1024];
    bzero(send_data,1024);
    char l2[400];
    char line[100000];
    char list_of_commands[100000];
    char f_name[1000000];
    bzero(send_data,10000);
    int iterator=0;
    int k=0;
    int i=16;
    while(i!=strlen(recv_data))
    {
        f_name[k]=recv_data[i];
        k++;
        i++;
    }
    char array[100000],array1[100000],array3[10000],array4[100000];
    //l1[k]='\0';
    f_name[k]='\0';
    system("touch timestamp_output");
    system("touch md5sum_output");
    char command2[500],command[100];
    strcpy(command,"stat ");
    strcat(command,f_name);
    strcat(command," | grep Modify | awk '{print $3}' > timestamp_output");
    strcpy(command2,"md5sum ");
    strcat(command2,f_name);
    strcat(command2," > md5sum_output");
    system(command2);
    system(command);
    printf("command--- %s\n",command);
    printf("command2--- %s\n",command2);
    FILE *f=fopen("md5sum_output","r");
    FILE *f1=fopen("timestamp_output","r");
    fgets(array,sizeof(array),f);
    fgets(array1,sizeof(array1),f1);
    system("rm -rf md5sum_output");
    system("rm -rf timestamp_output");
    printf("timestmp-- %s\n", array1);
    printf("md5-- %s\n", array);
    int var=0,var1=0;
    while(array[var]!=' ')//getting the md5sum from the file which has filename also
    {
        array3[var1]=array[var];
        var++;
        var1++;
    }
    printf("md53-- %s\n", array3);
    array3[var1]='\0';
    strcat(f_name,"\t");
    strcat(f_name,array3);
    strcat(f_name,"\t");
    array1[strlen(array1)-1]='\0';
    strcat(f_name,array1);
    strcpy(send_data,f_name);
    if(send(*connected, send_data, 1024,0) < 0)
    {
        error("\nERROR: Writing to socket");
        exit(1);
    }
    bzero(send_data,1024);
    strcpy(send_data,"END");
    if(write(*connected, send_data, 1024) < 0)
    {
        error("\nERROR: Writing to socket");
        exit(1);
    }
}

void Indexget_regex(char read_buffer[],int *newsocketfd)
{   
    char write_buffer[1024];
    bzero(write_buffer,1024);
    char l[8]="LongList",r[5]="RegEx",s[9]="ShortList";
    int t=0,i;
    char st[500],str[400];
    int k=0;
    i=15;
    while(i!=strlen(read_buffer))
    {
        st[k]=read_buffer[i];
        k++;
        i++;
    }
    printf("111\n");
    st[k]='\0';
    strcpy(str,"find . -name \"");
    strcat(str,st);
    strcat(str,"\" -exec ls -l {} \\; | awk '{print $9\"\\t\"$5}' > Result");
    system("touch Result");
    system(str);
    bzero(write_buffer, 1024);
    int f=0,flag=0;
    FILE *fs = fopen("Result", "r");
    if(fs == NULL)
    {
        error("ERROR: File not found");
        exit(1);
    }
    while((f= fread(write_buffer, sizeof(char),1024, fs)) > 0)
    {
        if(write(*newsocketfd, write_buffer, 1024) < 0)
        {
            error("\nERROR: Writing to socket");
            exit(1);
            break;
        }
        bzero(write_buffer, 1024);
        //     flag=1;
    }
    // if(flag==1)
    // {
    bzero(write_buffer, 1024);
    strcpy(write_buffer,"END");
    if(write(*newsocketfd, write_buffer, 1024) < 0)
    {
        error("\nERROR: Writing to socket");
        exit(1);
    }
    system("rm -rf Result");
    // }
}
void cindexget_regex(char write_buffer[],int *socketfd)
{ 
    int n;
    n = send(*socketfd,write_buffer,1024,0);
    int f=0,flag=1;
    char l[8]="LongList",read_buffer[1024],r[5]="RegEx",s[9]="ShortList";
    bzero(read_buffer, 1024);
    printf("\nRecieved data : %s\n",read_buffer);
    printf("File-name Size\n");
    while((recv(*socketfd, read_buffer,1024,0)) > 0)
    {
        printf("inside_regex_server\n");
        if(strcmp(read_buffer,"END")==0)
        {flag=0;
            break;
        }
        else
            printf("%s\n",read_buffer);
    }
    if(flag==1)
        printf("exit regex\n");

}
void Indexget_longlist(char read_buffer[] ,int *newsocketfd)
{   
    char write_buffer[1024];
    bzero(write_buffer,1024);
    char l[8]="LongList",r[5]="RegEx",s[9]="ShortList";
    int t=0,i;
 system("touch Result");
        system("ls -l |grep -v ^d | tr -s ' ' | awk '{print $9\"\\t\"$5\"\\t\"$8}'| tail -n +2 > Result");
        bzero(write_buffer, 1024);
        int f=0,flag=0;
        FILE *fs = fopen("Result", "r");
        if(fs == NULL)
        {
            error("ERROR: File not found");
            exit(1);
        }
        while((f= fread(write_buffer, sizeof(char),1024, fs)) > 0)
        {
            if(write(*newsocketfd, write_buffer, 1024) < 0)
            {
                error("\nERROR: Writing to socket");
                exit(1);
                break;
            }
            bzero(write_buffer, 1024);
            flag=1;
        }
        if(flag==1)
        {
            bzero(write_buffer, 1024);
            strcpy(write_buffer,"END");
            if(write(*newsocketfd, write_buffer, 1024) < 0)
            {
                error("\nERROR: Writing to socket");
                exit(1);
            }
        }
        system("rm -rf Result");


}
void cindexget_longlist(char write_buffer[],int *socketfd)
{
    int n;
    n = send(*socketfd,write_buffer,1024,0);
    if (n < 0)
        error("ERROR writing to socket");
    char l[8]="LongList",read_buffer[1024],r[5]="RegEx",s[9]="ShortList";
    int f=0,flag=1;
        bzero(read_buffer, 1024);
        printf("\nrecvd data : %s\n",read_buffer);
        printf("File-name Size Timestamp\n");
        while((recv(*socketfd, read_buffer,1024,0)) > 0)
        {
            // printf("11111\n");
            if(strcmp(read_buffer,"END")==0)
            {flag=0;
                break;
            }
            else
                printf("%s\n",read_buffer);
        }
        if(flag==1)
            printf("exut\n");
       //     exit(0);

}
void cFiledownload(char write_buffer[],int *socketfd)
{
    int n=0;
    char read_buffer[1024],hashm[200],data[1024];
    n = write(*socketfd,write_buffer, 1024);
    if (n < 0)
        error("ERROR writing to socket");
    int i=0,k=0;
    char xname[200],timestamp[200],size[100],name[200];
    printf("[Client] File recieving from server\n");
    while(write_buffer[13+i]!='\0')
    {
        name[i]=write_buffer[13+i];
        i++;
    }
    name[i]='\0';
    bzero(read_buffer,1024);
    if(n= read(*socketfd, read_buffer,1024) > 0)
    {
        if(strncmp(read_buffer,"File Not there",14)==0)
        {
            return;
        }

    }
    else
        error("Error in reading from socket");

    bzero(read_buffer,1024);
    if(n= read(*socketfd, read_buffer,1024) > 0)
    {

        if(strncmp(read_buffer,"timestamp",9)==0)
        {
            i=9;
            k=0;
            while(read_buffer[i]!='\0')
            {
                timestamp[k]=read_buffer[i];
                k++;
                i++;
            }
            timestamp[k]='\0';
        }
    }
    else
        error("Error in reading from socket");
    bzero(read_buffer,1024);
    if(n= read(*socketfd, read_buffer,1024) > 0)
    {
        if(strncmp(read_buffer,"size",4)==0)
        {
            i=4;
            k=0;
            while(read_buffer[i]!='\0')
            {
                size[k]=read_buffer[i];
                k++;
                i++;
            }
            size[k]='\0';

        }
    }
    else
        error("Error in reading from socket");
    bzero(read_buffer,1024);
    if(n= read(*socketfd, read_buffer,1024) > 0)
    {
        if(strncmp(read_buffer,"name",4)==0)
        {
            i=4;
            k=0;
            while(read_buffer[i]!='\0')
            {
                xname[k]=read_buffer[i];
                k++;
                i++;
            }
            xname[k]='\0';

        }
    }
    else
        error("Error in reading from socket");
    bzero(read_buffer,1024);
    if(n= read(*socketfd, read_buffer,1024) > 0)
    {
        if(strncmp(read_buffer,"hash",4)==0)
        {
            i=4;
            k=0;
            while(read_buffer[i]!='\0')
            {
                hashm[k]=read_buffer[i];
                k++;
                i++;
            }
            hashm[k]='\0';

        }
    }
    else
        error("Error in reading from socket");
    printf("Timestamp   %s\n",timestamp);
    printf("Size        %s\n",size);
    printf("Name        %s\n",name);
    printf("Hash        %s\n",hashm);
    bzero(read_buffer,1024);
    FILE *fr = fopen(name, "wb");
    if(fr == NULL)
        printf("File %s Cannot be opened file on peer.\n", name);
    else
    {
        int f = 0,f1=0;
        unsigned int size = 0;
        while(f = recv(*socketfd,&size,sizeof(size),0)>0)
        {
            int p=ntohl(size);
            f1 = recv(*socketfd, read_buffer,1024, 0);
            if(strcmp(read_buffer,"END")==0)
                break;
            if(f1 > 0) {

                int w= fwrite(read_buffer, sizeof(char), p, fr);
                if(w < p)
                {
                    error("[Client] File write failed on client.\n");
                }
                if (p == 0 || p!= 1024)
                {
                    break;
                }
            }
            if(f1 < 0)
            {
                error("Error receiving file from client to server.\n");
            }
            if(f < 0)
            {
                error("Error receiving file from client to server.\n");
            }
            bzero(read_buffer, 1024);
        }
        printf("[Client]File received from server!\n");
        fclose(fr);
    }
}
void cFileupload(char write_buffer[],int *socketfd)
{
    int n=0;
    char read_buffer[1024];
    n = write(*socketfd,write_buffer, 1024);
    if (n < 0)
        error("ERROR writing to socket");
    FILE *fpointer;
    char *upload_c;
    fpointer=fopen("upload_choice","r");
    fscanf(fpointer,"%s",upload_c);
    if(strcmp(upload_c,"Not allow")==0)
    {
        printf("[Client] Server has denied to upload\n");
        return;
    }
    else
    {
        int i=0,k=0;
        char size[100],name[200],u[300],com[400];
        while(write_buffer[11+i]!='\0')
        {
            name[i]=write_buffer[11+i];
            i++;
        }
        name[i]='\0';
        bzero(write_buffer,1024);
        strcpy(write_buffer,"f_name");
        strcat(write_buffer,name);
        if(n= send(*socketfd, write_buffer,1024,0) < 0)
        {
            error("Error in writing in socket");
        }
        bzero(write_buffer,1024);
        strcpy(write_buffer,"file_size");
        strcpy(com,"stat ");
        strcat(com,name);
        strcat(com," | grep Size | awk '{print $2}' > r.txt");
        system(com);
        FILE *f;
        f=fopen("r.txt","r");
        fgets(u,sizeof(u),f);
        fclose(f);
        strcat(write_buffer,u);
        system("rm -rf r.txt");
        if(n= send(*socketfd, write_buffer,1024,0) < 0)
        {
            error("Error in writing in socket");
        }
        bzero(read_buffer,1024);
        /* if(n=recv(*socketfd,read_buffer,1024,0)<0)
           {
           error("Error in reading from socket");
           }*/
 //       FILE *fpointer;
   //     char *upload_c;
     //   fpointer=fopen("upload_choice","r");
       // fscanf(fpointer,"%s",upload_c);
        printf("%s\n",name);
        printf("[Client ]File uploading to server\n");
        FILE *fs = fopen(name, "rb");
        if(fs == NULL)
        {
            printf("ERROR: File %s not found on server.\n", name);
            exit(1);
        }
        bzero(write_buffer, 1024);
        int f1;
        while((f1 = fread(write_buffer, sizeof(char), 1024, fs))>0)
        {
            unsigned int sdlen=htonl(f1);
            printf("htonl --- %d\n", sdlen);
            if(send(*socketfd,&sdlen,sizeof(sdlen),0) < 0){
                error("[Server] Sending Header Failed : ");
                break;
            }
            if(send(*socketfd, write_buffer, f1,0) < 0)
            {
                printf("ERROR: Failed to send file %s.\n", name);
                exit(1);
            }
            bzero(write_buffer, 1024);
        }
        bzero(write_buffer, 1024);
        strcpy(write_buffer,"END");
        if(send(*socketfd, write_buffer, 1024,0) < 0)
        {
            printf("ERROR: Failed to write in socket \n");
            exit(1);
        }
        fclose(fs);
        printf("[Client] File uploaded to the server\n");
    }
}
void Fileupload(char read_buffer[],int *newsocketfd)
{
    int n=0;
    int i=0,k=0;
    char size[300],name[300],write_buffer[1024];
    printf("[Server] File recieving from client\n");
    if(n= recv(*newsocketfd, read_buffer,1024,0) > 0)
    {
        if(strncmp(read_buffer,"name",4)==0)
        {
            i=4;
            k=0;
            while(read_buffer[i]!='\0')
            {
                name[k]=read_buffer[i];
                i++;k++;
            }
            name[k]='\0';
        }

    }
    else
        error("Error in reading from socket");
    printf("name-- %s\n", name);
    bzero(read_buffer,1024);
    if(n= recv(*newsocketfd, read_buffer,1024,0) > 0)
    {
        if(strncmp(read_buffer,"size",4)==0)
        {
            i=4;
            k=0;
            while(read_buffer[i]!='\0')
            {
                size[k]=read_buffer[i];
                k++;
                i++;
            }
            size[k]='\0';
        }
    }
    else
        error("Error in reading from socket");
    printf("[Server] File name  %s\n",name);
    printf("[Server] File size  %s\n",size);
   // printf("[Server] Allow/Deny the client to upload: ");
    fflush(stdout);
 //   char p[100];
   // p[0] = '\0';
//    kill (pid,SIGSTOP);
  /*  while(p[0] == '\0')
    {
        scanf("%[^\n]%*c",p);
    }*/
  //  kill (pid,SIGCONT);
 /*   FILE *fpointer;
    char *upload_c;
    fpointer=fopen("upload_choice","r");
    fscanf(fpointer,"%s",upload_c);*/
/*    if(strcmp(upload_c,"Not allow")==0)
    {
        bzero(write_buffer,1024);
        strcpy(write_buffer,"FileUpload Denied by the server\0");
        if(n=send(*newsocketfd,write_buffer,1024,0)<0)
        {
            error("ERROR writing to socket");
        }
        return;
       // printf("[Client] Server has denied to upload\n");
       // return;
    }
    else
    {*/
        bzero(write_buffer,1024);
        strcpy(write_buffer,"FileUpload is allowed by the server\0");
        if(n=write(*newsocketfd,write_buffer,1024)<0)
        {
            error("ERROR writing to socket");
        }
        printf("[Server] Getting file from client\n");
        FILE *fr = fopen(name, "wb");
        if(fr == NULL)
            printf("File %s Cannot be opened file on peer.\n", name);
        else
        {
            bzero(read_buffer, 1024);
            int f = 0,f1=0;
            unsigned int size = 0;
            while(f = recv(*newsocketfd,&size,sizeof(size),0)>0)
            {
                int p=ntohl(size);
                printf("ntohl-- %d\n",p);
                f1 = recv(*newsocketfd, read_buffer,1024, 0);
                if(strcmp(read_buffer,"END")==0)
                    break;
                if(f1 > 0) {

                    int w= fwrite(read_buffer, sizeof(char), p, fr);
                    if(w < p)
                    {
                        error("[Server] File write failed on client.\n");
                    }
                    if (p == 0 || p!= 1024)
                    {
                        break;
                    }
                }
                if(f1 < 0)
                {
                    error("Error receiving file from client to server.\n");
                }
                if(f < 0)
                {
                    error("Error receiving file from client to server.\n");
                }
                bzero(read_buffer, 1024);
            }
            fclose(fr);
            printf("[Server] File got from client\n");
       }
   // }
}

void Filedownload(char read_buffer[],int *newsocketfd)
{

    char write_buffer[1024];
    bzero(write_buffer,1024);
    int t=0,i=0,n=0;
    char name[200],com[500];
    while(read_buffer[13+i]!='\0')
    {
        name[i]=read_buffer[13+i];
        i++;
    }
    name[i]='\0';
    bzero(write_buffer,1024);
    FILE *f=fopen(name,"r");
    if(f==NULL)
    {
        strcpy(write_buffer,"File Not there");
        if(n=write(*newsocketfd,write_buffer,1024)<0)
        {
            error("Error in writing in socket");
        }
        return;
    }
    else
    {
        if(n=write(*newsocketfd,write_buffer,1024)<0)
        {
            error("Error in writing in socket");
        }
        fclose(f);
    }
    bzero(write_buffer,1024);
    strcpy(write_buffer,"timestamp");
    system("touch r.txt");
    strcpy(com,"stat ");
    strcat(com,name);
    strcat(com," | grep Modify | awk '{print $3}' > r.txt");
    system(com);
    f=fopen("r.txt","r");
    char u[300],a[300];
    fgets(u,sizeof(u),f);
    strcat(write_buffer,u);
    fclose(f);
    system("rm -rf r.txt");
    if(n=write(*newsocketfd,write_buffer,1024)<0)
    {
        error("Error in writing in socket");
    }
    bzero(write_buffer,1024);
    strcpy(write_buffer,"size");
    strcpy(com,"stat ");
    strcat(com,name);
    strcat(com," | grep Size | awk '{print $2}' > r.txt");
    system(com);
    f=fopen("r.txt","r");
    fgets(u,sizeof(u),f);
    fclose(f);
    strcat(write_buffer,u);
    system("rm -rf r.txt");
    if(n=write(*newsocketfd,write_buffer,1024)<0)
    {
        error("Error in writing in socket");
    }
    bzero(write_buffer,1024);
    strcpy(write_buffer,"name");
    strcat(write_buffer,name);
    if(n=write(*newsocketfd,write_buffer,1024)<0)
    {
        error("Error in writing in socket");
    }
    char str[500];
    strcpy(str,"md5sum ");
    strcat(str,name);
    strcat(str," > r.txt");
    system(str);
    f=fopen("r.txt","r");
    fgets(u,sizeof(u),f);
    int k=0;
    i=0,k=0;
    while(u[i]!=' ')
    {
        a[k]=u[i];
        i++;
        k++;
    }
    a[k]='\0';
    strcpy(write_buffer,"hash");
    strcat(write_buffer,a);
    if(n=write(*newsocketfd,write_buffer,1024)<0)
    {
        error("Error in writing in socket");
    }
    system("rm -rf r.txt");
    FILE *fs = fopen(name, "rb");
    if(fs == NULL)
    {
        printf("ERROR: File %s not found on server.\n", name);
        exit(1);
    }

    bzero(write_buffer, 1024);
    int f1;
    while((f1 = fread(write_buffer, sizeof(char), 1024, fs))>0)
    {
        unsigned int sdlen=htonl(f1);
        if(send(*newsocketfd,&sdlen,sizeof(sdlen),0) < 0){
            error("[Server] Sending Header Failed : ");
            break;
        }
        if(send(*newsocketfd, write_buffer, f1,0) < 0)
        {
            printf("ERROR: Failed to send file %s.\n", name);
            exit(1);
        }
        bzero(write_buffer, 1024);
    }
    bzero(write_buffer, 1024);
    strcpy(write_buffer,"END");
    if(send(*newsocketfd, write_buffer, 1024,0) < 0)
    {
        printf("ERROR: Failed to write in socket \n");
        exit(1);
    }
    fclose(fs);
    printf("[Server]File sent to client!\n");
}



/*void Filehash(char read_buffer[] , int *newsocketfd)
{
    char write_buffer[1024];
    bzero(write_buffer,1024);
    char v[6]="Verify",c[8]="CheckAll";
    int t=0,i;
    for(i=0;i<6;i++)
        if(v[i]!=read_buffer[9+i])
            t=1;
    if(t==1)
    {
        for(i=0;i<8;i++)
            if(c[i]!=read_buffer[9+i])
                t=2;
    }
    if(t==0)
    {
        char l1[500],l2[400];
        int k=0;
        i=16;
        while(i!=strlen(read_buffer))
        {
            l1[k]=read_buffer[i];
            k++;
            i++;
        }
        l1[k]='\0';
        hashing(l1,newsocketfd);
        bzero(write_buffer,1024);
        strcpy(write_buffer,"END");
        if(write(*newsocketfd, write_buffer, 1024) < 0)
        {
            error("\nERROR: Writing to socket");
            exit(1);
        }
    }
    if(t==1)
    {
        system("ls -l |grep -v ^d | awk '{print $9}' | tail -n +2 > fd");
        int flag=0;
        FILE *file = fopen ( "fd", "r" );
        if(file == NULL)
            printf("There is a error on file opening\n");
        else( file != NULL )
        {
            char line [500];
            while ( fgets ( line, sizeof(line), file ) != NULL )
            {
                char te[400];
                int k=0;
                i=0;
                while(line[i]!='\n')
                {
                    te[k]=line[i];
                    k++;
                    i++;
                }
                te[k]='\0';
                hashing(te,newsocketfd);
            }
            flag=1;
        }
        if(flag==1)
        {
            bzero(write_buffer, 1024);
            strcpy(write_buffer,"END");
            if(write(*newsocketfd, write_buffer, 1024) < 0)
            {
                error("\nERROR: Writing to socket");
                exit(1);
            }
        }
        system("rm -rf fd");

    }

}*/

void cFilehash(char write_buffer[], int *socketfd)
{
    int n;
    char read_buffer[1024];
    n = write(*socketfd,write_buffer,1024);
    if (n < 0)
        error("ERROR writing to socket");
    int t=0,i;
    char v[6]="Verify",c[8]="CheckAll";
    for(i=0;i<6;i++)
        if(v[i]!=write_buffer[9+i])
            t=1;
    if(t==1)
    {
        for(i=0;i<8;i++)
            if(c[i]!=write_buffer[9+i])
                t=2;
    }
    if(t==0)
    {

        int f=0,flag=1;
        bzero(read_buffer, 1024);
        printf("\nrecvd data : %s\n",read_buffer);
        printf("File-name   Latest-Timestamp    Md5hash\n");
        while((f= read(*socketfd, read_buffer,1024)) > 0)
        {
            if(strcmp(read_buffer,"END")==0)
            {flag=0;
                break;
            }
            else
                printf("%s\n",read_buffer);
        }
        if(flag==1)
            exit(0);
    }
    if(t==1)
    {
        int f=0,flag=1;
        bzero(read_buffer, 1024);
        printf("\nrecvd data : %s\n",read_buffer);
        printf("File-name   Latest-Timestamp    Md5hash\n");
        while((f= read(*socketfd, read_buffer,1024)) > 0)
        {
            if(strcmp(read_buffer,"END")==0)
            {flag=0;
                break;
            }
            else
                printf("%s\n",read_buffer);
        }
        if(flag==1)
            exit(0);
    }
}


int main()
{
    int server_port,client_port;
    char client_ip[10];
    FILE *fpointer;
    fpointer = fopen("upload_choice","w");
    fprintf(fpointer,"%s","allow");
    fclose(fpointer);
    printf("Enter the port on which you want have server\n");
    scanf("%d", &server_port);
    printf("Enter the ip of the server to which you want to connect\n");
    scanf("%s",client_ip);
    printf("%s", client_ip);
    printf("Enter the port of the same server to which you want to connect\n");
    scanf("%d",&client_port);
    printf("Enter 0 for tcp mode and 1 for udp mode\n");
    parent_pid = getpid();
    pid = fork();
    if(pid==0)
    {
        printf("getpid--- %d\n",getpid());
        act_as_a_server(server_port);
    }
    else if(pid > 0)
    {
          act_as_a_client(client_ip,client_port);
    }
    return 0;
}

void act_as_a_server(int server_port)
{
    printf("parent--%d present---%d\n",parent_pid,getpid());
    int listenfd = 0, connected;
    int sin_size,bytes_received;
    struct sockaddr_in serv_addr,client_addr; 
    char recv_data[1025];
    //creating socket
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd < 0)
        error("ERROR opening socket");
    else
        printf("[Server] Socket intialized \n");

    memset(&serv_addr, '0', sizeof(serv_addr));
   // memset(sendBuff, '0', sizeof(sendBuff)); 
    //setting server port,family
    //printf("tyahh\n");
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(server_port);
    //binding socket to the server
    if(bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr))<0)
    {
        perror("Error in binding");
        int b = getppid();
        kill(b,SIGKILL);
        exit(1);
    }
    else
        printf("socket binded to the server\n");

    if(listen(listenfd, 10)<0)
    {
        perror("Error in listening");
        exit(1);
    }
    printf("[Server]TCPServer waiting for an client on port %d", server_port);
    fflush(stdout);
    sin_size = sizeof(struct sockaddr_in);
    connected = accept(listenfd, (struct sockaddr *)&client_addr,&sin_size);
    printf("\n I got a connection from (%s , %d)",inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));
    while (1)
    {
        bytes_received = recv(connected,recv_data,1024,0);
        recv_data[bytes_received] = '\0';
        if (strcmp(recv_data , "q") == 0 || strcmp(recv_data , "Q") == 0)
        {
            printf("%s\n",recv_data);
            t_close=1;
            close(connected);
            int a = getppid();
            kill(a,SIGKILL);
            break;
        }
        else if((strncmp(recv_data,"IndexGet ShortList",18)==0) | (strncmp(recv_data,"IndexGet LongList",17)==0) | (strncmp(recv_data,"IndexGet RegEx",14)==0))
        {
            //   if((strncmp(recv_data,"IndexGet ShortList",18)==0))
            //              //     Indexget_shortlist(recv_data,&connected);
            if((strncmp(recv_data,"IndexGet LongList",17)==0))
            {
                Indexget_longlist(recv_data,&connected);
            }
            else if(strncmp(recv_data,"IndexGet RegEx",14)==0)
                Indexget_regex(recv_data,&connected);
        }
        else if((strncmp(recv_data,"FileHash verify",15)==0) |(strncmp(recv_data,"FileHash CheckAll",17)==0))
        {
            if((strncmp(recv_data,"FileHash verify",15)==0))
            {
                filehash_verify(recv_data,&connected);
            }
            else if(strncmp(recv_data,"FileHash CheckAll",17)==0)
                filehash_checkall(recv_data,&connected);
        }
        else if(strncmp(recv_data,"FileDownload",12)==0)
        {
            Filedownload(recv_data,&connected);
        }
        else if(strncmp(recv_data,"FileUpload",10)==0)
        {
            Fileupload(recv_data,&connected);
        }
        else
        {
            printf("\n recvD DATA = %s\n" , recv_data);
            fflush(stdout);
        }
    }
    close(listenfd);
    printf("t_close %d\n",t_close);
    return;
}


void act_as_a_client(char *client_ip,int client_port)
{
    int socketfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char send_data[1024];

    socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketfd < 0)
    {
        error("ERROR opening socket");
        exit(1);
    }
    else
        printf("[Client] Socket created\n");

    server = gethostbyname(client_ip);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(client_port);
    serv_addr.sin_addr = *((struct in_addr *)server->h_addr);
    bzero(&(serv_addr.sin_zero),8);
    while(connect(socketfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) == -1)
    {
        sleep(1);
    }
    while(1)
    {
        bzero(send_data, 1024);
     //   printf("Enter the message:\n");
        gets(send_data);
        printf("t_close-- %d\n", t_close);
        if((strcmp(send_data, "Q")==0) ||(strcmp(send_data, "q")==0) ||(t_close==1))
        {
                printf("quit\n");
                if(t_close==1)
                    printf("bye\n");
                send(socketfd,send_data,strlen(send_data), 0); 
                printf("[Client] Connection closed\n");
                kill(pid,SIGTERM);                                            
                fflush(stdout);
                kill(parent_pid,SIGTERM);
               // exit(0);
                break;
        }
        else if((strncmp(send_data,"IndexGet ShortList",18)==0) | (strncmp(send_data,"IndexGet LongList",17)==0) | (strncmp(send_data,"IndexGet RegEx",14)==0))
        {
            //   if((strncmp(recv_data,"IndexGet ShortList",18)==0))
            //              //     Indexget_shortlist(recv_data,&connected);
            if((strncmp(send_data,"IndexGet LongList",17)==0))
            {
                cindexget_longlist(send_data,&socketfd);
            }
            else if(strncmp(send_data,"IndexGet RegEx",14)==0)
                cindexget_regex(send_data,&socketfd);
        }
        else if((strncmp(send_data,"FileHash verify",15)==0) |(strncmp(send_data,"FileHash CheckAll",17)==0))
        {
            if((strncmp(send_data,"FileHash verify",15)==0))
            {
                cfilehash_verify(send_data,&socketfd);
            }
            else if(strncmp(send_data,"FileHash CheckAll",17)==0)
                cfilehash_checkall(send_data,&socketfd);
        }
        else if(strncmp(send_data,"FileDownload",12)==0)
        {
            cFiledownload(send_data,&socketfd);
        }
        else if(strncmp(send_data,"FileUpload",10)==0)
        {
            cFileupload(send_data,&socketfd);
        }
        else
        {
                send(socketfd,send_data,strlen(send_data), 0); 
        }
    }
    close(socketfd);
    _exit(0);
    return;
}


