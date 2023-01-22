// Amir mohammad Mahfoozi
// id : 401106469

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#define maxl 1000
#define max_com 1000
#define max_address 1000

void copystr(char address[],int n,int l,int p,int size,char flag);// address of the text file - size of address - line number - char pos - number of char to be copied - forward or backward
void removestr(char address[],int n,int l,int p,int size,char flag); // address of the text file - size of address - line number - char pos - number of char to be removed - forward or backward
void cutstr(char address[],int n,int l,int p,int size,char flag); // address of the text file - size of address - line number - char pos - number of char to be cutted - forward or backward
void insertstr(char address[],int n,int l,int p,char str[]); // address of the text file - size of address - line number - char pos - string to be added
void pastestr(char address[],int n,int l,int p);// address of the text file - size of address - line number - char pos
void cat(char address[],int n); // address of the text file - size of it
void createfile(char address[],int n); // address of the text file - size of it
void get_input();

void copystr(char address[],int n,int l,int p,int size,char flag)
{
    char filename[max_address];
    for(int i = 1;i<n;i++)
    {
        filename[i-1] = address[i];
    }
    filename[n-1] = '\0';
    // filename = address - '/'
    FILE* myfile;
    if(fopen(filename,"r")) // check if file is created
    {
        char wholetext[maxl*200];
        int countchar = 0;
        long long posi = 0; // position of the first char to be removed
        myfile = fopen(filename,"r");
        int number_of_line = 1;
        char line[200];
        while(1)
        {
            line[0] = '\0';
            if(fgets(line,200,myfile) == NULL) break;
            int len = strlen(line);
            if(number_of_line < l)
                posi+=len;
            number_of_line++;
            for(int i = 0;i<len;i++)
            {
                wholetext[countchar] = line[i];
                countchar++;
            }
        }
        posi+=p;
        FILE* clipboard = fopen("clipboard.txt","w");
        if(flag == 'b')
        {
             for(int i = 0;i<countchar;i++)
            {
                if(i<posi && i>=posi-size-1)
                {
                     fprintf(clipboard,"%c",wholetext[i]);
                }
            }
        }
        else if(flag == 'f')
        {
            for(int i = 0;i<countchar;i++)
            {
                if(i>=posi && i<=posi+size)
                {
                    fprintf(clipboard,"%c",wholetext[i]);
                }
            }
        }
        fclose(myfile);
        fclose(clipboard);
    }
    else
    {
        printf("This file doesn't exist !\n");
        return;
    }
}

void removestr(char address[],int n,int l,int p,int size,char flag)
{
    char filename[max_address];
    for(int i = 1;i<n;i++)
    {
        filename[i-1] = address[i];
    }
    filename[n-1] = '\0';
    // filename = address - '/'
    FILE* myfile;
    if(fopen(filename,"r")) // check if file is created
    {
        char wholetext[maxl*200];
        int countchar = 0;
        long long posi = 0; // position of the first char to be removed
        myfile = fopen(filename,"r");
        int number_of_line = 1;
        char line[200];
        while(1)
        {
            line[0] = '\0';
            if(fgets(line,200,myfile) == NULL) break;
            int len = strlen(line);
            if(number_of_line < l)
                posi+=len;
            number_of_line++;
            for(int i = 0;i<len;i++)
            {
                wholetext[countchar] = line[i];
                countchar++;
            }
        }
        posi+=p;
        FILE* temporary = fopen("temporary.txt","w");
        if(flag == 'b')
        {
             for(int i = 0;i<countchar;i++)
            {
                if(i<posi && i>=posi-size-1)
                {
                    ;
                }
                else
                    fprintf(temporary,"%c",wholetext[i]);
            }
        }
        else if(flag == 'f')
        {
            for(int i = 0;i<countchar;i++)
            {
                if(i>=posi && i<=posi+size)
                {
                    ;
                }
                else
                    fprintf(temporary,"%c",wholetext[i]);
            }
        }
        fclose(myfile);
        fclose(temporary);
        temporary = fopen("temporary.txt","r");
            myfile = fopen(filename,"w");
            while(1)
            {
                line[0] = '\0';
                if(fgets(line,200,temporary) == NULL) break;
                fprintf(myfile,"%s",line);
            }
            fclose(myfile);
            fclose(temporary);
    }
    else
    {
        printf("This file doesn't exist !\n");
        return;
    }
}
void cutstr(char address[],int n,int l,int p,int size,char flag)
{
    copystr(address,n,l,p,size,flag);
    removestr(address,n,l,p,size,flag);
}
void insertstr(char address[],int n,int l,int p,char str[])
{
        char filename[max_address];
        for(int i = 1;i<n;i++)
        {
            filename[i-1] = address[i];
        }
        // finish getting input
        filename[n-1] = '\0';
        // filename = address - '/'
        FILE* myfile;
        if(fopen(filename,"r")) // check if file is created
        {
            myfile = fopen(filename,"r");
            FILE* temporary = fopen("temporary.txt","w");
            char line[200];
            int num = 1; // number of lines passed : current line starting from 1
            while(num < l)
            {
                line[0] = '\0';
                if(fgets(line,200,myfile) == NULL) break;
                fprintf(temporary,"%s",line);
                num++;
            }
            int positionchar = 0;
            line[0] = '\0';
            fgets(line,200,myfile);
            int len = strlen(line);
            while(positionchar < p)
            {
                fputc(line[positionchar],temporary);
                positionchar++;
            }
            int tool = strlen(str);
            for(int j = 0;j<tool;j++)//handeling \\n and \n
            {
                if(str[j] == '\\' && str[abs(j-1)] != '\\' && str[(j+1)] == 'n')
                {
                    fprintf(temporary,"%c",'\n');
                    j++;
                }
                else if(str[j] == '\\' && str[abs(j-1)] == '\\' && str[(j+1)] == 'n')
                {
                    ;
                }
                else{
                    fprintf(temporary,"%c",str[j]);
                }

            }

            while(positionchar < len)
            {
                fputc(line[positionchar],temporary);
                positionchar++;
            }
            while(1)
            {
                line[0] = '\0';
                if(fgets(line,200,myfile) == NULL) break;
                fprintf(temporary,"%s",line);
            }
            fclose(myfile);
            fclose(temporary);
            temporary = fopen("temporary.txt","r");
            myfile = fopen(filename,"w");
            while(1)
            {
                line[0] = '\0';
                if(fgets(line,200,temporary) == NULL) break;
                fprintf(myfile,"%s",line);
            }
            fclose(myfile);
            fclose(temporary);
        }
        else{
            printf("This file doesn't exist !\n");
            return;
        }
}
void pastestr(char address[],int n,int l,int p)
{
    char text[maxl*200];
    FILE* clipboard = fopen("clipboard.txt","r");
    char line[200];
    int number_of_chars = 0;
    while(1)
    {
        line[0] = '\0';
        if(fgets(line,200,clipboard) == NULL) break;
        int len = strlen(line);
        for(int i = 0;i<len;i++)
        {
            text[number_of_chars] = line[i];
            number_of_chars++;
        }
    }
    fclose(clipboard);
    insertstr(address,n,l,p,text);
}
void cat(char address[],int n)
{
    char filename[max_address];
    for(int i = 1;i<n;i++)
    {
        filename[i-1] = address[i];
    }
    filename[n-1] = '\0';
    // filename = address - '/'
    FILE* myfile;
    if(fopen(filename,"r")) // check if file is created
    {
        myfile = fopen(filename,"r");
        char line[200];
        while(1)
        {
            line[0] = '\0';
            if(fgets(line,200,myfile) == NULL) break;
            printf("%s",line);
        }
        fclose(myfile);
    }
    else{
        printf("This file doesn't exist !\n");
        return;
    }
}

void createfile(char address[],int n)
{
    // seperate the dir by '/'
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
    // filename = address - '/'
    FILE* myfile;
    if(fopen(filename,"r")) // check if file is created
    {
        printf("This file already exist !\n");
    }
    else
    {
        myfile = fopen(filename,"w");
        fclose(myfile);
    }
}
int findstr(char str[],char address[],char attribute,char second_attribute)
{

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
    else if(!strcmp(command,"insertstr"))
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
                int l;// line of position
                int p;// char of position
                char str[max_com];
                scanf("%s",str);
                if(!strcmp(str,"--str"))
                {
                    char strandpos[maxl];
                    char temp;
                    temp = getchar();//space
                    temp = getchar();
                    int cnt = 0;
                    while(temp != '\n')
                    {
                       strandpos[cnt] = temp;
                       temp = getchar();
                       cnt++;
                    }
                    strandpos[cnt] = '\0';
                    for(int i = 0;i<cnt-1;i++)
                    {
                        if(strandpos[i] == '-' && strandpos[i+1] == '-')
                        {
                            l = strandpos[i+6]-'0';
                            p = strandpos[i+8]-'0';
                            cnt = i-1;
                            break;
                        }
                    }
                    if(strandpos[0] == '"')
                    {
                        for(int i = 1;i<cnt-1;i++)
                        {
                            str[i-1] = strandpos[i];
                        }
                        str[cnt-2] = '\0';
                    }
                    else
                    {
                        for(int i = 0;i<cnt;i++)
                        {
                            str[i] = strandpos[i];
                        }
                        str[cnt] = '\0';
                    }
                }
                insertstr(address,cnt,l,p,str);
                return;
            }
            else
            {
                int cnt = 0;
                while(temp!=' ')
                {
                    address[cnt] = temp;
                    temp = getchar();
                    cnt++;
                }
                address[cnt] = '\0';
                int l;// line of position
                int p;// char of position
                char str[max_com];
                scanf("%s",str);
                if(!strcmp(str,"--str"))
                {
                    char strandpos[maxl];
                    char temp;
                    temp = getchar();//space
                    temp = getchar();
                    int cnt = 0;
                    while(temp != '\n')
                    {
                       strandpos[cnt] = temp;
                       temp = getchar();
                       cnt++;
                    }
                    strandpos[cnt] = '\0';
                    for(int i = 0;i<cnt-1;i++)
                    {
                        if(strandpos[i] == '-' && strandpos[i+1] == '-')
                        {
                            l = strandpos[i+6]-'0';
                            p = strandpos[i+8]-'0';
                            cnt = i-1;
                            break;
                        }
                    }
                    if(strandpos[0] == '"')
                    {
                        for(int i = 1;i<cnt-1;i++)
                        {
                            str[i-1] = strandpos[i];
                        }
                        str[cnt-2] = '\0';
                    }
                    else
                    {
                        for(int i = 0;i<cnt;i++)
                        {
                            str[i] = strandpos[i];
                        }
                        str[cnt] = '\0';
                    }
                }
                insertstr(address,cnt,l,p,str);
                return;
            }
        }
    }
    else if(!strcmp(command,"cat"))
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
                cat(address,cnt);
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
                cat(address,cnt);
                return;
            }
        }
    }
    else if(!strcmp(command,"removestr"))
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
                int l;// line of position
                int p;// char of position
                int size = 0; // number of chars to be removed
                char flag; //backward or forward
                char pos[max_com];
                scanf("%s",pos);
                if(!strcmp(pos,"--pos"))
                {
                    scanf("%s",pos);
                    l = pos[0] - '0';
                    p = pos[2] - '0';
                }
                else
                {
                    printf("invalid command\n");
                    return;
                }
                scanf("%s",pos);
                if(!strcmp(pos,"-size"))
                {
                    scanf("%s",pos);
                    int cntpow = 0;
                    for(int i = strlen(pos)-1;i>=0;i--)
                    {
                        size += (pos[i]-'0')*pow(10,cntpow);
                        cntpow++;
                    }
                    scanf("%s",pos);
                    flag = pos[1];
                }
                else{
                    printf("invalid command\n");
                    return;
                }
                removestr(address,cnt,l,p,size,flag);
                return;
            }
            else
            {
                int cnt = 0;
                while(temp!=' ')
                {
                    address[cnt] = temp;
                    temp = getchar();
                    cnt++;
                }
                address[cnt] = '\0';
                int l;// line of position
                int p;// char of position
                int size = 0; // number of chars to be removed
                char flag; //backward or forward
                char pos[max_com];
                scanf("%s",pos);
                if(!strcmp(pos,"--pos"))
                {
                    scanf("%s",pos);
                    l = pos[0] - '0';
                    p = pos[2] - '0';
                }
                else
                {
                    printf("invalid command\n");
                    return;
                }
                scanf("%s",pos);
                if(!strcmp(pos,"-size"))
                {
                    scanf("%s",pos);
                    int cntpow = 0;
                    for(int i = strlen(pos)-1;i>=0;i--)
                    {
                        size += (pos[i]-'0')*pow(10,cntpow);
                        cntpow++;
                    }
                    scanf("%s",pos);
                    flag = pos[1];
                }
                else{
                    printf("invalid command\n");
                    return;
                }
                removestr(address,cnt,l,p,size,flag);
                return;
            }
        }
    }
    else if(!strcmp(command,"copystr"))
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
                int l;// line of position
                int p;// char of position
                int size = 0; // number of chars to be removed
                char flag; //backward or forward
                char pos[max_com];
                scanf("%s",pos);
                if(!strcmp(pos,"--pos"))
                {
                    scanf("%s",pos);
                    l = pos[0] - '0';
                    p = pos[2] - '0';
                }
                else
                {
                    printf("invalid command\n");
                    return;
                }
                scanf("%s",pos);
                if(!strcmp(pos,"-size"))
                {
                    scanf("%s",pos);
                    int cntpow = 0;
                    for(int i = strlen(pos)-1;i>=0;i--)
                    {
                        size += (pos[i]-'0')*pow(10,cntpow);
                        cntpow++;
                    }
                    scanf("%s",pos);
                    flag = pos[1];
                }
                else{
                    printf("invalid command\n");
                    return;
                }
                copystr(address,cnt,l,p,size,flag);
                return;
            }
            else
            {
                int cnt = 0;
                while(temp!=' ')
                {
                    address[cnt] = temp;
                    temp = getchar();
                    cnt++;
                }
                address[cnt] = '\0';
                int l;// line of position
                int p;// char of position
                int size = 0; // number of chars to be removed
                char flag; //backward or forward
                char pos[max_com];
                scanf("%s",pos);
                if(!strcmp(pos,"--pos"))
                {
                    scanf("%s",pos);
                    l = pos[0] - '0';
                    p = pos[2] - '0';
                }
                else
                {
                    printf("invalid command\n");
                    return;
                }
                scanf("%s",pos);
                if(!strcmp(pos,"-size"))
                {
                    scanf("%s",pos);
                    int cntpow = 0;
                    for(int i = strlen(pos)-1;i>=0;i--)
                    {
                        size += (pos[i]-'0')*pow(10,cntpow);
                        cntpow++;
                    }
                    scanf("%s",pos);
                    flag = pos[1];
                }
                else{
                    printf("invalid command\n");
                    return;
                }
                copystr(address,cnt,l,p,size,flag);
                return;
            }
        }
    }
    else if(!strcmp(command,"cutstr"))
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
                int l;// line of position
                int p;// char of position
                int size = 0; // number of chars to be removed
                char flag; //backward or forward
                char pos[max_com];
                scanf("%s",pos);
                if(!strcmp(pos,"--pos"))
                {
                    scanf("%s",pos);
                    l = pos[0] - '0';
                    p = pos[2] - '0';
                }
                else
                {
                    printf("invalid command\n");
                    return;
                }
                scanf("%s",pos);
                if(!strcmp(pos,"-size"))
                {
                    scanf("%s",pos);
                    int cntpow = 0;
                    for(int i = strlen(pos)-1;i>=0;i--)
                    {
                        size += (pos[i]-'0')*pow(10,cntpow);
                        cntpow++;
                    }
                    scanf("%s",pos);
                    flag = pos[1];
                }
                else{
                    printf("invalid command\n");
                    return;
                }
                cutstr(address,cnt,l,p,size,flag);
                return;
            }
            else
            {
                int cnt = 0;
                while(temp!=' ')
                {
                    address[cnt] = temp;
                    temp = getchar();
                    cnt++;
                }
                address[cnt] = '\0';
                int l;// line of position
                int p;// char of position
                int size = 0; // number of chars to be removed
                char flag; //backward or forward
                char pos[max_com];
                scanf("%s",pos);
                if(!strcmp(pos,"--pos"))
                {
                    scanf("%s",pos);
                    l = pos[0] - '0';
                    p = pos[2] - '0';
                }
                else
                {
                    printf("invalid command\n");
                    return;
                }
                scanf("%s",pos);
                if(!strcmp(pos,"-size"))
                {
                    scanf("%s",pos);
                    int cntpow = 0;
                    for(int i = strlen(pos)-1;i>=0;i--)
                    {
                        size += (pos[i]-'0')*pow(10,cntpow);
                        cntpow++;
                    }
                    scanf("%s",pos);
                    flag = pos[1];
                }
                else{
                    printf("invalid command\n");
                    return;
                }
                cutstr(address,cnt,l,p,size,flag);
                return;
            }
        }
    }
    else if(!strcmp(command,"pastestr"))
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
                int l;
                int p;
                char pos[max_com];
                scanf("%s",pos);
                if(!strcmp(pos,"--pos"))
                {
                    scanf("%s",pos);
                    l = pos[0] - '0';
                    p = pos[2] - '0';
                }
                else
                {
                    printf("invalid command\n");
                    return;
                }
                pastestr(address,cnt,l,p);
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
                int l;
                int p;
                char pos[max_com];
                scanf("%s",pos);
                if(!strcmp(pos,"--pos"))
                {
                    scanf("%s",pos);
                    l = pos[0] - '0';
                    p = pos[2] - '0';
                }
                else
                {
                    printf("invalid command\n");
                    return;
                }
                pastestr(address,cnt,l,p);
                return;
            }
        }
    }
    else if(!strcmp(command,"find"))
    {
        if(!strcmp(sub_command,"--str"))
        {
            char str[maxl],file[maxl];
            char attribute = '?',second_attribute = '?';
            int index_str = 0,index_file = 0;
            char temp;
            temp = getchar(); // space
            temp = getchar(); //first char to check if it is " or not
            if(temp == '"')
            {
                temp = getchar();
                while(1)
                {
                    str[index_str] = temp;
                    index_str++;
                    temp = getchar();
                    if(temp == '"' && str[index_str-1] != '\\')
                    {
                        break;
                    }
                    if(temp == '"' && str[index_str-1] == '\\')
                    {
                        index_str--;
                    }
                }
                str[index_str] = '\0';
                //printf("%s",str);
                temp = getchar(); //space
            }
            else
            {
                str[index_str] = temp;
                index_str++;
                temp = getchar();
                while(1)
                {
                    str[index_str] = temp;
                    index_str++;
                    temp = getchar();
                    if(temp == ' ')
                    {
                        break;
                    }
                    if(temp == '"' && str[index_str-1] == '\\')
                    {
                        index_str--;
                    }
                }
                str[index_str] = '\0';
               // printf("%s",str);
            }
            scanf("%s",sub_command);
            if(!strcmp(sub_command,"--file"))
            {
                temp = getchar(); // space
                temp = getchar();//first char to check if it is " or not for file address
                if(temp == '"')
                {
                    temp = getchar();
                    while(1)
                    {
                        file[index_file] = temp;
                        index_file++;
                        temp = getchar();
                        if(temp == '"' && file[index_file-1] != '\\')
                        {
                            break;
                        }
                        if(temp == '"' && file[index_file-1] == '\\')
                        {
                            index_file--;
                        }
                    }
                    file[index_file] = '\0';
                    //printf("%s",file);
                    temp = getchar(); //space or \n
                    if(temp == '\n')
                    {
                        found = 1;
                        attribute = 'n';//null attribute
                        second_attribute = 'n';
                        findstr(str,file,attribute,second_attribute);
                    }
                }
                else{
                    file[index_file] = temp;
                    index_file++;
                    temp = getchar();
                    while(1)
                    {
                        file[index_file] = temp;
                        index_file++;
                        temp = getchar();
                        if(temp == ' ')
                        {
                            break;
                        }
                        if(temp == '\n')
                        {
                            found = 1;
                            attribute = 'n'; // null attribute
                            second_attribute = 'n';
                            findstr(str,file,attribute,second_attribute);
                            break;
                        }
                        if(temp == '"' && file[index_file-1] == '\\')
                        {
                            index_file--;
                        }
                    }
                    file[index_file] = '\0';
                    //printf("%s",file);
                }
                if(attribute != 'n')
                {
                    scanf("%s",sub_command);
                    if(!strcmp(sub_command,"-count"))
                    {
                        found = 1;
                        attribute = 'c';
                        temp = getchar();
                        if(temp == '\n')
                        {
                            second_attribute = 'n';
                        }
                        else{
                            scanf("%s",sub_command);
                        }
                    }
                    else if(!strcmp(sub_command,"-at"))
                    {
                        found = 1;
                        attribute = 't';
                        temp = getchar();
                        if(temp == '\n')
                        {
                            second_attribute = 'n';
                        }
                        else{
                            scanf("%s",sub_command);
                        }
                    }
                    else if(!strcmp(sub_command,"-byword"))
                    {
                        found = 1;
                        attribute = 'b';
                        temp = getchar();
                        if(temp == '\n')
                        {
                            second_attribute = 'n';
                        }
                        else{
                            scanf("%s",sub_command);
                        }
                    }
                    else if(!strcmp(sub_command,"-all"))
                    {
                        found = 1;
                        attribute = 'a';
                        temp = getchar();
                        if(temp == '\n')
                        {
                            second_attribute = 'n';
                        }
                        else{
                            scanf("%s",sub_command);
                        }
                    }
                    if(second_attribute != 'n')
                    {
                        if(!strcmp(sub_command,"-count"))
                        {
                            second_attribute = 'c';
                        }
                        if(!strcmp(sub_command,"-at"))
                        {
                            second_attribute = 't';
                        }
                        if(!strcmp(sub_command,"-byword"))
                        {
                            second_attribute = 'b';
                        }
                        if(!strcmp(sub_command,"-all"))
                        {
                            second_attribute = 'a';
                        }
                    }
                    findstr(str,file,attribute,second_attribute);
                }
            }

        }
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
