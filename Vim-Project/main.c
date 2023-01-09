// Amir mohammad Mahfoozi
// id : 401106469

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#define maxl 1000
#define max_com 1000
#define max_address 1000
void createfile(char address[],int n)
{
    char temp[max_address];
    int index;
    for(int i = 1;i<n;i++)
    {
        if(address[i] == '/')
        {
            index = i;
            temp[i-1] = '\0';
            mkdir(temp,0777);
        }
        temp[i-1] = address[i];
    }
    char filename[max_address];
    for(int i = 1;i<n;i++)
    {
        filename[i-1] = address[i];
    }
    filename[n-1] = '\0';

    FILE* myfile;
    if(fopen(filename,"r"))
    {
        fclose(myfile);
        printf("This file already exist !\n");
    }
    else
    {
        myfile = fopen(filename,"w");
        fclose(myfile);
    }
}
void get_input()
{
    // this function get the command and check if it is available
    // if command was available check the sub-command
    // if both were correct get the data and pass it to other functions
    char command[max_com],sub_command[max_com];
    scanf("%s",command);
    scanf("%s",sub_command);
    int found = 0;
    if(!strcmp(command,"createfile"))
    {
        if(!strcmp(sub_command,"--file"))
        {
            found = 1;
            char temp;
            char address[max_address];
            temp = getchar();//space
            temp = getchar();// check if it is " or not
            if(temp == '"')
            {
                temp = getchar();
                int cnt = 0;
                while(temp!='"')
                {
                    address[cnt] = temp;
                    temp = getchar();
                    cnt++;
                }
                address[cnt] = '\0';
                createfile(address,cnt);
                return;
            }
            else
            {
                int cnt = 0;
                while(temp!='\n')
                {
                    address[cnt] = temp;
                    temp = getchar();
                    cnt++;
                }
                address[cnt] = '\0';
                createfile(address,cnt);
                return;
            }
        }
    }
    else if(!strcmp(command,"cat"))
    {

    }
    else if(!strcmp(command,"remove"))
    {

    }
    else if(!strcmp(command,"copy"))
    {

    }
    else if(!strcmp(command,"cut"))
    {

    }
    else if(!strcmp(command,"paste"))
    {

    }
    else if(!strcmp(command,"find"))
    {

    }
    else if(!strcmp(command,"replace"))
    {

    }
    else if(!strcmp(command,"grep"))
    {

    }
    else if(!strcmp(command,"undo"))
    {

    }
    else if(!strcmp(command,"exit"))
    {
        return 0;
    }
    if(!found)
    {
        printf("invalid command\n");
        return;
    }
}

int main()
{
    while(1)
    {
        get_input();
    }
    return 0;
}
