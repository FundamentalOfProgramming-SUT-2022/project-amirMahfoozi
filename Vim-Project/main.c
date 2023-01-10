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
void copystr(char address[],int n)
{
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
void removestr(char address[],int n)
{
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
                removestr(address,cnt);
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
                removestr(address,cnt);
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
                copystr(address,cnt);
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
                copystr(address,cnt);
                return;
            }
        }
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
