#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#define maxlength 1000
char createfile[maxlength] = "createfile";
void clear_string(char arr[])
{
    for(int i = 0;i<maxlength;i++)
    {
        arr[i] = NULL;
    }
}
void create_file(char address[],int counter)
{
    int path;
    for(int i = 0;i<counter;i++)
    {
        if(address[i] =='/')
        {
            path = i;
        }
    }
    char pathchar[maxlength];
    clear_string(pathchar);
    for(int i = 0;i<path;i++)
    {
        pathchar[i] = address[i];
    }
    mkdir(pathchar,0777);
    FILE *OutFile;
    if(OutFile = fopen(address,"r"))
    {
        fclose(OutFile);
        printf("The file already exist\n");
    }
    else{
        OutFile = fopen(address,"w");
        fclose(OutFile);
    }
}
void detect_type(char type[],int length_of_type,char command_line[],int length)
{
    //check if it was createfile
    int flag_type = 1;
    for(int i = 0;i<length_of_type;i++)
    {
        if(type[i] != createfile[i])
        {
            flag_type = 0;
            break;
        }
    }
    if(flag_type)
    {
        char address[maxlength];
        clear_string(address);
        int counter =  0;
        for(int i = 19;i<length;i++)
        {
            address[i-19] = command_line[i];
            counter++;
        }
        create_file(address,counter);
    }
}
void process_line(char command_line[],int length)
{
    char type[maxlength];
    clear_string(type);
    for(int i = 0;i<length;i++)
    {
        if(command_line[i] != ' ')
        {
            type[i] = command_line[i];
        }
        else{
            detect_type(type,i,command_line,length);
            break;
        }
    }
    printf("\n");
}
void get_input()
{
    char command_line[maxlength];
    clear_string(command_line);
    char c;
    int temp = 0;
    while(1)
    {
        c = getchar();
        if(c == '\n')
        {
            process_line(command_line,temp);
            return;
        }
        else{
            command_line[temp] = c;
            temp++;
        }
    }
}

int main()
{
    get_input();
    return 0;
}
