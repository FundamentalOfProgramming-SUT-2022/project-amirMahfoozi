// Amir mohammad Mahfoozi
// id : 401106469

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <stdbool.h>
#include <dirent.h>
#define maxl 1000
#define max_com 1000
#define max_address 1000

int exited = 0;
char arman_str[10000]={0};
int arman_activate = 0;
int to_be_replaced[maxl] = {-2};
int index_to_be_replaced = 0;

void copystr(char address[],int l,int p,int size,char flag);// address of the text file  - line number - char pos - number of char to be copied - forward or backward
void removestr(char address[],int l,int p,int size,char flag); // address of the text file  - line number - char pos - number of char to be removed - forward or backward
void cutstr(char address[],int l,int p,int size,char flag); // address of the text file  - line number - char pos - number of char to be cutted - forward or backward
void insertstr(char address[],int l,int p,char str[]); // address of the text file  - line number - char pos - string to be added
void pastestr(char address[],int l,int p);// address of the text file  - line number - char pos
void cat(char address[]); // address of the text file
void createfile(char address[]); // address of the text file
void get_input();
char* detect_dbl_rcc(char address[],int *l_ptr,int *p_ptr,int *size_ptr,char *flag_ptr);// to improve readablity for remove-cut-copy checks for the "
char* detect_dbl(char address[]); // to improve readability for createfile-cat-undo checks for the "

int searchline(char *s, char *srch)
{

    int n = strlen(srch);
    char *str = malloc((n) * sizeof(char));
    int cnt = 0;
    for(int i = 0;i<n;i++)
    {
        strncpy(str, &s[i],n);
        if (strcmp(srch, str) == 0)
        {
            cnt++;
        }
    }
    return cnt;
}

char* detect_dbl_rcc(char address[],int *l_ptr,int *p_ptr,int *size_ptr,char *flag_ptr)
{
    char temp;
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
        *l_ptr = l;
        *p_ptr = p;
        *size_ptr = size;
        *flag_ptr = flag;
        return address;
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
        *l_ptr = l;
        *p_ptr = p;
        *size_ptr = size;
        *flag_ptr = flag;
        return address;
    }
}

char* detect_dbl(char address[])
{
    char temp;
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
        return address;
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
        return address;
    }
}

void copystr(char address[],int l,int p,int size,char flag)
{
    int n = strlen(address);
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
                if(i<posi && i>posi-size-1)
                {
                     fprintf(clipboard,"%c",wholetext[i]);
                }
            }
        }
        else if(flag == 'f')
        {
            for(int i = 0;i<countchar;i++)
            {
                if(i>=posi && i<posi+size)
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

void removestr(char address[],int l,int p,int size,char flag)
{
    copy_for_undo(address);
    int n = strlen(address);
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
                if(i<posi && i>posi-size-1)
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
                if(i>=posi && i<posi+size)
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
void remove_word(char address[],int index)
{
    printf("2");
    copy_for_undo(address);
    int n = strlen(address);
    char filename[max_address];
    for(int i = 1;i<n;i++)
    {
        filename[i-1] = address[i];
    }
    filename[n-1] = '\0';
    // filename = address - '/'
    FILE* myfile;
    FILE* temporary;
    if(fopen(filename,"r")) // check if file is created
    {
        myfile = fopen(filename,"r");
        temporary = fopen("temporary.txt","w");
        char ch;
        int index_char = 0;
        while(1)
        {
            if(index_char == index)
            {
                while(ch != ' ' && ch != '\n' && ch != EOF)
                {
                    ch = fgetc(myfile);
                }
            }
            ch = fgetc(myfile);
            if(ch != '\n')
            {
                index_char++;
            }
            fputc(ch,temporary);
        }
        fclose(myfile);
        fclose(temporary);
        myfile = fopen(filename,"w");
        temporary = fopen("temporary.txt","r");
        while(1)
        {
            ch = fgetc(temporary);
            fputc(ch,myfile);
        }
    }
    else
    {
        printf("This file doesn't exist !\n");
        return;
    }
}
void cutstr(char address[],int l,int p,int size,char flag)
{
    copy_for_undo(address);
    copystr(address,l,p,size,flag);
    removestr(address,l,p,size,flag);
}
void insertstr(char address[],int l,int p,char str[])
{
    copy_for_undo(address);
        int n = strlen(address);
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
void pastestr(char address[],int l,int p)
{
    copy_for_undo(address);
    int n = strlen(address);
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
    insertstr(address,l,p,text);
}
void cat(char address[])
{
    int n = strlen(address);
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

void createfile(char address[])
{
    // seperate the dir by '/'
    int n = strlen(address);
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
int grep(char mode,char str[],char address[])
{
    int n = strlen(address);
    char filename[max_address];
    for(int i = 1;i<n;i++)
    {
        filename[i-1] = address[i];
    }
    filename[n-1] = '\0';
    // filename = address - '/'
    FILE* myfile;
    //printf("%s\n",str);
    //printf("%s\n",address);
    if(fopen(filename,"r")) // check if file is created
    {
        int fnd = 0;
        int cntr = 0;
        myfile = fopen(filename,"r");
        char line[200];
        while(1)
        {
            line[0] = '\0';
            if(fgets(line,200,myfile) == NULL) break;
            fnd = searchline(line,str);
            if(fnd){
                if(mode == 'n')
                {
                    printf("%s : %s\n",filename,line);
                }
                else if(mode == 'l')
                {
                    printf("%s\n",filename);
                    break;
                }
                cntr++;
            }
        }
        if(mode == 'c')
        {
            return cntr;
        }
        fclose(myfile);
    }
    else{
        printf("This file doesn't exist !\n");
        return;
    }
}
void tree(char path[],int root,const int mainroot)
{
        DIR *directory;
        struct dirent *entry;
        directory = opendir(path);
        if(directory == NULL)
        {
            printf("error opening file");
            return 1;
        }
        while((entry = readdir(directory)) != NULL)
        {
            if(!strcmp((entry->d_name),"."))
                continue;
            if(!strcmp((entry->d_name),".."))
                continue;

            printf("├");
            strcat(arman_str,"├");
            for(int i = 0;i<mainroot-root+1;i++)
            {
                printf("──");
            }
            if(entry->d_type == DT_DIR)
            {
                printf(" %s:\n",entry->d_name);
                strcat(arman_str,entry->d_name);
                strcat(arman_str,":\n");
            }

            else
            {
                printf(" %s\n",entry->d_name);
                strcat(arman_str,entry->d_name);
                strcat(arman_str,"\n");
            }


            if(entry->d_type == DT_DIR && root > 0)
            {
                char newpath[maxl];
                int n = strlen(path);
                for(int i = 0;i<n;i++)
                {
                    newpath[i] = path[i];
                }
                newpath[n] = '/';
                for(int i = 0;i<strlen(entry->d_name);i++)
                {
                    newpath[i+n+1] = (entry->d_name)[i];
                }
                newpath[n+strlen(entry->d_name)+1] = '\0';
                tree(newpath,root-1,mainroot);
            }
        }
        closedir(directory);
}
void auto_indent(char address[])
{
    copy_for_undo(address);
    int n = strlen(address);
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
        FILE* temporary = fopen("temporary.txt","w");
        char line[200];
        int depth = 0;
        char before;
        int haveword = 0;
        while(1)
        {
            haveword = 0;
            line[0] = '\0';
            if(fgets(line,200,myfile) == NULL) break;
            int n = strlen(line);
            for(int i = 0;i<n;i++)
            {
                if(line[i] == '{')
                    {
                        if(line[i-1] != ' ')
                        {

                        }
                        else{
                            int tempi = i-1;
                            while(line[tempi] == ' ')
                                {
                                    line[tempi] = 126;
                                    tempi--;
                                }

                        }
                    }
            }
            for(int i = 0;i<depth*4;i++)
            {
                fputc(' ',temporary);
                before = ' ';
            }
            int last = 0;
            for(int i = 0;i<n;i++)
            {
                if(line[i] != ' ')
                {
                    last = i;
                    break;
                }
            }
            for(int i = last;i<n;i++)
            {
                if(line[i] == '{')
                {
                    depth++;
                    if(haveword && before != ' ' && before != '\n')
                        fputc(' ',temporary);
                    fputc(line[i],temporary);
                    if(line[i+1] != '\n')
                    {
                        fputc('\n',temporary);
                        before = '\n';
                        haveword = 0;
                    }
                    if(line[i+1] == '\n')
                    {
                        fputc('\n',temporary);
                        before = '\n';
                        haveword = 0;
                        i++;
                    }
                    int temp = i+1;
                    while(line[temp] != '}' && temp < n)
                    {
                        if(line[temp] != '}' && line[temp] != ' ' && line[temp] != '\n')
                        {
                            haveword = 1;
                            break;
                        }
                        temp++;
                    }
                    if(haveword)
                    {
                        for(int j = 0;j<depth*4;j++)
                        {
                            fputc(' ',temporary);
                            before = ' ';
                        }
                    }

                    while(line[i+1] == ' ')
                    {
                        i++;
                    }
                }
                else if(line[i] == '}')
                {
                    depth--;
                    if(before != '\n' && haveword)
                    {
                        fputc('\n',temporary);
                        before = '\n';
                        haveword = 0;
                    }
                    for(int j = 0;j<depth*4;j++)
                    {
                        fputc(' ',temporary);
                        before = ' ';
                    }
                    fputc(line[i],temporary);
                    if(line[i+1] != '\n' && line[i+1] != '\0')
                    {
                        fputc('\n',temporary);
                        before = '\n';
                        haveword = 0;
                    }
                    while(line[i+1] == ' ')
                    {
                        i++;
                    }
                }
                else if(line[i] != 126)
                {
                    if(line[i] != ' ')
                        haveword = 1;
                    fputc(line[i],temporary);
                    before = line[i];
                }
            }
        }

        fclose(myfile);
        fclose(temporary);
        myfile = fopen(filename,"w");
        temporary = fopen("temporary.txt","r");
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

void compare(char address[],char address2[])
{
    int n = strlen(address);
    int n2 = strlen(address2);
    char filename[max_address];
    char filename2[max_address];
    for(int i = 1;i<n;i++)
    {
        filename[i-1] = address[i];
    }
    for(int i = 1;i<n2;i++)
    {
        filename2[i-1] = address2[i];
    }
    filename[n-1] = '\0';
    filename2[n2-1] = '\0';
    int cntlines = 0,cntlines2 = 0;
    // filename = address - '/'
    FILE* myfile;
    FILE* myfile2;
    char line11[200];
    myfile = fopen(filename,"r");
    myfile2 = fopen(filename2,"r");
    while(1)
    {
        line11[0] = '\0';
        if(fgets(line11,200,myfile) == NULL) break;
        cntlines++;
    }
    fclose(myfile);
    char line22[200];
    while(1)
    {
        line22[0] = '\0';
        if(fgets(line22,200,myfile2) == NULL) break;
        cntlines2++;
    }
    fclose(myfile2);

    if(fopen(filename,"r") && fopen(filename2,"r")) // check if file is created
    {
        int cnt = 0,cnt2 = 0;
        int f1_ended = 0;
        int f2_ended = 0;
        myfile = fopen(filename,"r");
        myfile2 = fopen(filename2,"r");
        char line[200];
        char line2[200];
        while(1)
        {
            line[0] = '\0';
            line2[0] = '\0';
            if(fgets(line,200,myfile) == NULL)
            {
                f1_ended = 1;
                break;
            }
            if(fgets(line2,200,myfile2) == NULL)
            {
                f2_ended = 1;
                break;
            }
            if(line[strlen(line)-1] != '\n')
            {
                strcat(line, "\n");
            }
            if(line2[strlen(line2)-1] != '\n')
            {
                strcat(line2, "\n");
            }
            if(line[strlen(line)-2] == 13)
            {
                line[strlen(line)-2] = '\n';
                line[strlen(line)-1] = '\0';
            }
            if(line2[strlen(line2)-2] == 13)
            {
                line2[strlen(line2)-2] = '\n';
                line2[strlen(line2)-1] = '\0';
            }
            cnt++;
            cnt2++;
            if(strcmp(line,line2))
            {
                     printf("=========== %d ===========\n%s%s", cnt, line, line2);
            }
        }
        if(f1_ended)
        {
            if(fgets(line2,200,myfile2) == NULL)
            {
                f2_ended = 1;
            }
        }
        if(!f1_ended && f2_ended)
        {
            printf("<<<<<<<<<< %d - %d <<<<<<<<<<\n",cnt+1,cntlines);
            printf("%s",line);
            while(1)
            {
                line[0] = '\0';
                if(fgets(line,200,myfile) == NULL) break;
                printf("%s",line);
            }
        }
        else if (f1_ended && !f2_ended)
        {
            printf(">>>>>>>>>> %d - %d >>>>>>>>>>\n",cnt2+1,cntlines2);
            printf("%s",line2);
            while(1)
            {
                line2[0] = '\0';
                if(fgets(line2,200,myfile2) == NULL) break;
                printf("%s",line2);
            }
        }
        fclose(myfile);
        fclose(myfile2);
    }
    else{
        printf("This file doesn't exist !\n");
        return;
    }
}

void copy_for_undo(char address[])
{
    int n = strlen(address);
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
        filename[0] = 'u';
        filename[1] = 'n';
        filename[2] = 'd';
        filename[3] = 'o';
        FILE* undo = fopen(filename,"w");
        char line[200];
        while(1)
        {
            line[0] = '\0';
            if(fgets(line,200,myfile) == NULL) break;
            fprintf(undo,"%s",line);
        }

        fclose(myfile);
        fclose(undo);
    }
    else{
        printf("This file doesn't exist !\n");
        return;
    }
}

void undo(char address[])
{
    int n = strlen(address);
    char filename[max_address];
    for(int i = 1;i<n;i++)
    {
        filename[i-1] = address[i];
    }
    filename[n-1] = '\0';
    // filename = address - '/'
    FILE* myfile;
    if(fopen(filename,"w")) // check if file is created
    {
        myfile = fopen(filename,"w");
        filename[0] = 'u';
        filename[1] = 'n';
        filename[2] = 'd';
        filename[3] = 'o';
        FILE* undo = fopen(filename,"r");
        char line[200];
        while(1)
        {
            line[0] = '\0';
            if(fgets(line,200,undo) == NULL) break;
            fprintf(myfile,"%s",line);
        }

        fclose(myfile);
        fclose(undo);
    }
    else{
        printf("This file doesn't exist !\n");
        return;
    }
}
void arman_detector()
{
    char armantemp;
        armantemp = getchar();
        if(armantemp == ' ')
        {
            armantemp = getchar();
            if(armantemp == '=')
            {
                armantemp = getchar();
                if(armantemp == 'D')
                {
                    arman_activate = 1;
                }
            }
        }
}

int is_match(char str[], char pattern[]) // can handle *a and a* and a*b
{
    int n = strlen(str);
    int m = strlen(pattern);
    //both empty is true (base case)
    if (m == 0)
        return (n == 0);
    // we use dp method to find
    int dp[n + 1][m + 1];
    memset(dp, 0, sizeof(dp));
    dp[0][0] = 1;// empty pattern and empty string

    // Only '*' can match with empty string
    for (int j = 1; j <= m; j++)
    {
        if (pattern[j - 1] == '*')
            dp[0][j] = dp[0][j - 1];
    }
    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= m; j++)
        {
            // Two cases if we see a '*'
            // ‘*’ indicates an empty sequence
            // '*' character matches with ith character in input
            if (pattern[j - 1] == '*' && str[j-1] != ' ')
                dp[i][j] = dp[i][j - 1] || dp[i - 1][j];
            //characters actually match
            else if (str[i - 1] == pattern[j - 1])
                dp[i][j] = dp[i - 1][j - 1];

            // If characters don't match
            else
                dp[i][j] = 0;
        }
    }

    return dp[n][m];
}

int find(char pattern[],char address[],int at_cnt,int at_at,int at_byword,int at_all)
{

    int n = strlen(address);
    char filename[max_address];
    for(int i = 1;i<n;i++)
    {
        filename[i-1] = address[i];
    }
    filename[n-1] = '\0';
    // filename = address - '/'
    FILE* myfile;
    int untilnow = 0;
    if(fopen(filename,"r")) // check if file is created
    {

        myfile = fopen(filename,"r");
        char line[200];
        int space[100],found[100],found_word[100];
        int flag=0,index_find = 0,index_space = 0;
        for (int i = 0; i < 100; i++)
        {
            space[i] = -1;
            found[i] = -1;
            found_word[i] = -1;
        }
        while(1)
        {
            int to_khat_peida_shod = 0;
        if(fgets(line,200,myfile) == NULL)
            break;
        int len = strlen(line) - strlen(pattern);
        int wild = 0;
        if (pattern[0] == '*')
        {
            wild = 1;
        }
        for (int i = 1; i < len - 1; i++)
        {
            if (pattern[i] == '*' && pattern[i - 1] != '\\')
            {
                wild = 1;
            }
            if (pattern[i] == '*' && pattern[i - 1] == '\\') // \*
            {
                for (int j = i - 1; j < len; j++)
                {
                    pattern[j] = pattern[j + 1];
                }
                pattern[len - 1] = '\0';
                pattern[len - 2] = '\0';
                //printf("%s",pattern);
            }
        }
        for (int i = 0; i <= len ; i++)
        {
            char temp[200] = {};
            strncpy(temp, line+i, strlen(pattern));
            //printf("%s/",temp);
            if (line[i] == ' ')
            {
                space[index_space] = i;
                index_space++;
                flag = 0;
            }
            if(wild)
            {
                if (is_match(temp,pattern))
                {
                    flag++;
                    if (flag == 1)
                    {
                        to_khat_peida_shod = 1;
                        found[index_find] = i+untilnow;
                        index_find++;
                    }
                }
            }
            else
            {
                if (!strcmp(pattern, temp))
                {
                    flag++;
                    to_khat_peida_shod = 1;
                    found[index_find] = i+untilnow;
                    index_find++;
                }
            }
        }
        if(to_khat_peida_shod)
        {
            for (int i = 0; found[i] != -1; i++)
            {
                int word = 1;
                for (int j = 0; space[j] != -1; j++)
                {
                    if (space[j] < found[i])
                    {
                        word = j + 2;
                    }
                }
                found_word[i] = word;
            }
        }
        untilnow+=strlen(line)-2;
        }
        fclose(myfile);
        if(!at_all && !at_at && !at_byword && !at_cnt) // no option
        {
            if(found[0] == -1)
            {
                printf("not found\n");
                return -1;
            }
            printf("%d\n", found[0]);
            int ans = found[0];
            return 0;

        }
        else if(at_all && !at_at && !at_byword && !at_cnt) // all
        {
            int where = index_to_be_replaced;
             if(found[0] == -1)
            {
                printf("not found\n");
                return -1;
            }
            for (int i = 0; found[i] != -1; i++)
            {
                printf("%d ", found[i]);
                to_be_replaced[index_to_be_replaced] = found[i];
                index_to_be_replaced++;
            }
            printf("\n");
            return where;
        }
        else if(!at_all && at_at && !at_byword && !at_cnt) // at
        {
            if(at_at-1 < 0)
            {
                printf("not found\n");
                return -1;
            }

            else
            {
                printf("%d\n", found[at_at-1]);
                return found[at_at-1];
            }

        }
         else if(!at_all && !at_at && at_byword && !at_cnt) // byword
        {
             if(found_word[0] == -1)
            {
                printf("not found\n");
                return -1;
            }
            printf("%d\n",found_word[0]);
            return found_word[0];
        }
        else if(!at_all && !at_at && !at_byword && at_cnt) //count
        {
             printf("%d\n", index_find);
             return index_find;
        }
         else if(at_all && !at_at && at_byword && !at_cnt) // all and byword
        {
             if(found_word[0] == -1)
            {
                printf("not found\n");
                return -1;
            }
            for (int i = 0; found_word[i] != -1; i++)
            {
                printf("%d ", found_word[i]);
            }
            printf("\n");
        }
        else if(!at_all && at_at && at_byword && !at_cnt) //at and byword
        {
             if(at_at-1 < 0)
                 printf("not found\n");
            else
                printf("%d\n", found_word[at_at-1]);
        }
        else{
            printf("non valid combination");
        }
    }
    else{
        printf("This file doesn't exist !\n");
        return -1;
    }

}

void replacestr(char str[],char str2[],char address[],int at_at,int at_all)
{
    copy_for_undo(address);

    if(at_all && at_at)
    {
        printf("not valid combination\n");
        return;
    }

    if(!at_all && !at_at)
    {
        int index_found;
        index_found = find(str,address,0,0,0,0);
        int sizee = strlen(str);
        removestr(address,1,index_found,sizee,'f');
        insertstr(address,1,index_found,str2);
    }
    if(at_at && !at_all)
    {
        int index_found;
        index_found = find(str,address,0,at_at,0,0);
        int sizee = strlen(str);
        removestr(address,1,index_found,sizee,'f');
        insertstr(address,1,index_found,str2);
    }
    if(at_all && !at_at)
    {
        int cnt = find(str,address,1,0,0,0);
        for(int i = 1;i<=cnt;i++)
        {
            int index_found;
            index_found = find(str,address,0,1,0,0);
            int sizee = strlen(str);
            removestr(address,1,index_found,sizee,'f');
            insertstr(address,1,index_found,str2);
        }
    }
}
void get_input()
{
    // this function get the command and check if it is available
    // if command was available check the sub-command
    // if both were correct get the data and pass it to other functions

    char command[max_com],sub_command[max_com];
    scanf("%s",command);
    if(!strcmp(command,"exit"))
    {
        exited = 1;
        return;
    }
    scanf("%s",sub_command);
    int found = 0;

    if(!strcmp(command,"createfile"))
    {
        if(!strcmp(sub_command,"--file"))
        {
            found = 1;
            char address[max_address];
            createfile(detect_dbl(address));
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
                insertstr(address,l,p,str);
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
                insertstr(address,l,p,str);
                return;
            }
        }
    }
    else if(!strcmp(command,"cat"))
    {
        if(!strcmp(sub_command,"--file"))
        {
            found = 1;
            char address[max_address];
            cat(detect_dbl(address));
        }
    }
    else if(!strcmp(command,"removestr"))
    {
        if(!strcmp(sub_command,"--file"))
        {
            found = 1;
            char address[maxl];
            int l,p,size;
            char flag;
            char *newaddress = detect_dbl_rcc(address,&l,&p,&size,&flag);
            removestr(newaddress,l,p,size,flag);
        }
    }
    else if(!strcmp(command,"copystr"))
    {
        if(!strcmp(sub_command,"--file"))
        {
            found = 1;
            char address[maxl];
            int l,p,size;
            char flag;
            char *newaddress = detect_dbl_rcc(address,&l,&p,&size,&flag);
            copystr(newaddress,l,p,size,flag);
        }
    }
    else if(!strcmp(command,"cutstr"))
    {
        if(!strcmp(sub_command,"--file"))
        {
            found = 1;
            char address[maxl];
            int l,p,size;
            char flag;
            char *newaddress = detect_dbl_rcc(address,&l,&p,&size,&flag);
            cutstr(newaddress,l,p,size,flag);
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
                pastestr(address,l,p);
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
                pastestr(address,l,p);
                return;
            }
        }
    }
    else if(!strcmp(command,"find"))
    {
        if(!strcmp(sub_command,"--str"))
        {
            char str[maxl],file[maxl];
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
                found = 1;
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
                char s[maxl];
                scanf("%[^\n]%*c",s);
                char *token;
                const char c[2] = " ";
                token = strtok(s,c);
                int at_count = 0,at_at = 0,at_byword = 0,at_all = 0;
                while( token != NULL ) {
                  if(!strcmp(token,"-count"))
                  {
                      at_count = 1;
                  }
                  else if(!strcmp(token,"-at"))
                  {
                      at_at = 1;
                  }
                  else if(!strcmp(token,"-byword"))
                  {
                      at_byword = 1;
                  }
                  else if(!strcmp(token,"-all"))
                  {
                      at_all = 1;
                  }
                  else
                  {
                      at_at = atoi(token);
                  }
                  token = strtok(NULL, c);
               }
               // printf("%s,%s,%d,%d,%d,%d",str,file,at_count,at_at,at_byword,at_all);
               find(str,file,at_count,at_at,at_byword,at_all);
            }

        }
    }
    else if(!strcmp(command,"tree"))
    {
        found = 1;
        int d;
        if(sub_command[0] == '-')
            d = -(sub_command[1] -'0');
        else
            d = sub_command[0]-'0';
        arman_detector();
        if(d < -1)
        {
            printf("invalid depth\n");
        }
        else if (d == -1)
            tree("root",100,100);
        else
            tree("root",d,d);
    }
    else if(!strcmp(command,"undo"))
    {
        if(!strcmp(sub_command,"--file"))
        {
            found = 1;
            char address[max_address];
            undo(detect_dbl(address));
        }
    }
    else if(!strcmp(command,"auto-indent"))
    {
        if(!strcmp(sub_command,"--file"))
        {
            found = 1;
            char address[max_address];
            auto_indent(detect_dbl(address));
        }
    }
    else if(!strcmp(command,"grep"))
    {
        int sum = 0; // for option c
        char mode = 'n';
        if(!strcmp(sub_command,"-c"))
        {
            mode = 'c';
            scanf("%s",sub_command);
        }
        else if(!strcmp(sub_command,"-l"))
        {
            mode = 'l';
            scanf("%s",sub_command);
        }
        if(!strcmp(sub_command,"--str"))
        {
            char str[maxl];
            int index = 0;
            found = 1;
            char temp;
            temp = getchar();//space
            temp = getchar();//check if it is " or not
            if(temp == '"')
            {
                temp = getchar();
                while(temp!='"')
                {
                    str[index] = temp;
                    index++;
                    temp = getchar();
                }
                str[index] = '\0';
                //printf("%s",str);
            }
            else
            {
                str[0] = temp;
                index++;
                temp = getchar();
                while(temp!=' ')
                {
                    str[index] = temp;
                    index++;
                    temp = getchar();
                }
                str[index] = '\0';
               // printf("%s",str);
            }
           scanf("%s",sub_command);
           char address[maxl];
           index = 0;
           if(!strcmp(sub_command,"--files"))
           {
               char temp;
               temp = getchar();//space
               while(1)
               {
                temp = getchar();
               if(temp == '"')
               {
                   temp = getchar();
                   while(temp != '"')
                   {
                       address[index] = temp;
                       index++;
                       temp = getchar();
                   }
                   address[index] = '\0';
                   temp = getchar();
               }
               else
               {
                   while(temp != ' ' && temp !=  '\n')
                   {
                       address[index] = temp;
                       index++;
                       temp = getchar();
                   }
                   address[index] = '\0';
               }
               sum+=grep(mode,str,address);
               index = 0;
                    if(temp ==  '\n')
                    {
                        break;
                    }
               }
           }
           if(mode == 'c')
           {
               printf("%d\n",sum);
           }
        }
    }
    else if(!strcmp(command,"compare"))
    {
        if(!strcmp(sub_command,"--files"))
        {
            found = 1;
            int index = 0;
            char address1[max_address];
            char address2[max_address];
            char temp;
            temp = getchar();//space
            temp = getchar();
           if(temp == '"')
           {
               temp = getchar();
               while(temp != '"')
               {
                   address1[index] = temp;
                   index++;
                   temp = getchar();
               }
               address1[index] = '\0';
               temp = getchar();
           }
           else
           {
               while(temp != ' ' && temp !=  '\n')
               {
                   address1[index] = temp;
                   index++;
                   temp = getchar();
               }
               address1[index] = '\0';
           }
           index = 0;
           temp = getchar();
           if(temp == '"')
           {
               temp = getchar();
               while(temp != '"')
               {
                   address2[index] = temp;
                   index++;
                   temp = getchar();
               }
               address2[index] = '\0';
               temp = getchar();
           }
           else
           {
               while(temp != ' ' && temp !=  '\n')
               {
                   address2[index] = temp;
                   index++;
                   temp = getchar();
               }
               address2[index] = '\0';
           }
           //printf("%s   %s",address1,address2);
           compare(address1,address2);
        }
    }
    else if(!strcmp(command,"replace"))
    {
        if(!strcmp(sub_command,"--str1"))
        {
            char str[maxl],file[maxl];
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
            char str2[maxl];
            index_str = 0;
            temp = getchar(); // space
            temp = getchar(); //first char to check if it is " or not
            if(temp == '"')
            {
                temp = getchar();
                while(1)
                {
                    str2[index_str] = temp;
                    index_str++;
                    temp = getchar();
                    if(temp == '"' && str2[index_str-1] != '\\')
                    {
                        break;
                    }
                    if(temp == '"' && str2[index_str-1] == '\\')
                    {
                        index_str--;
                    }
                }
                str2[index_str] = '\0';
                //printf("%s",str);
                temp = getchar(); //space
            }
            else
            {
                str2[index_str] = temp;
                index_str++;
                temp = getchar();
                while(1)
                {
                    str2[index_str] = temp;
                    index_str++;
                    temp = getchar();
                    if(temp == ' ')
                    {
                        break;
                    }
                    if(temp == '"' && str2[index_str-1] == '\\')
                    {
                        index_str--;
                    }
                }
                str2[index_str] = '\0';
               // printf("%s",str);
            }
            scanf("%s",sub_command);
            if(!strcmp(sub_command,"--file"))
            {
                found = 1;
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
                char s[maxl];
                scanf("%[^\n]%*c",s);
                char *token;
                const char c[2] = " ";
                token = strtok(s,c);
                int at_at = 0,at_all = 0;
                while( token != NULL ) {
                  if(!strcmp(token,"-at"))
                  {
                      at_at = 1;
                  }
                  else if(!strcmp(token,"-all"))
                  {
                      at_all = 1;
                  }
                  else
                  {
                      at_at = atoi(token);
                  }
                  token = strtok(NULL, c);
               }
                replacestr(str,str2,file,at_at,at_all);
            }
        }
    }
    if(!found)
    {
        printf("invalid command\n");
        return;
    }
}

int main()
{
    while(!exited)
    {
        get_input();
    }
    return 0;
}
