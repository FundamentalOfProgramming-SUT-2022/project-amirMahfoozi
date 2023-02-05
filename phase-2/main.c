#include <stdio.h>
#include <string.h>
#include <ncurses.h>
#include <stdlib.h>
#include <math.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <stdbool.h>
#include <dirent.h>
#define maxl 1000
#define max_com 1000
#define max_address 1000

int MODE; //1 == insert // 2 ==normal // 3 == visual
char text_box[1000][100] = {};
char name[50] = "new text file";
int to_be_replaced[maxl] = {-2};
int index_to_be_replaced = 0;
void copy_for_undo(char address[]);

void clear_text_box()
{
	for(int i = 0;i<1000;i++)
	{
		for(int j = 0;j<100;j++)
		{
			text_box[i][j] = '\0';
		}
	}
}
void clear_name()
{
        for(int i = 0;i<50;i++)
        {
                name[i] = 0;
        }
}
void number_win(WINDOW *win)
{
	int ymax,xmax;
	getmaxyx(win,ymax,xmax);
	for(int i = 1;i<ymax-1;i++)
	{
		mvwprintw(win,i,0,"%d",i);
	}
}
int navigation(WINDOW* win,int t,int *line,int *pos)
{
	keypad(win,true);
	if(t == KEY_RIGHT)
	{
		noecho();
		if( text_box[*line-1][*pos+1-2] == '\n')
		{
			;
		}
		else{
			int y,x;
			getyx(win,y,x);
			wmove(win,y,x+1);
			wrefresh(win);
			*pos++;
		}
		echo();
		return 1;
	}
	if(t == KEY_LEFT)
        {
		noecho();
                if(*pos-1 < 2 || text_box[*line-1][*pos-1-2] == '\0' || text_box[*line-1][*pos-1-2] == '\n')
                {
                        ;
                }
                else{
                        int y,x;
                        getyx(win,y,x);
                        wmove(win,y,x-1);
			wrefresh(win);
			*pos--;
                }
		echo();
                return 1;
        }
	if(t == KEY_UP)
        {
                noecho();
                int y,x;
                getyx(win,y,x);
                if(y == 1)
                {
                        ;
                }
                else{
			while(text_box[y-1-1][x-2] == '\0')
			{
				x--;	
			}
                        wmove(win,y-1,x);
                        wrefresh(win);
                        *line--;
			*pos = x;
                }
                echo();
                return 1;
        }
	if(t == KEY_DOWN)
	{
		noecho();
		int y,x,ymax,xmax;
		getyx(win,y,x);
		getmaxyx(win,ymax,xmax);
		if(y == ymax-4)
		{
			;
		}
		else
		{
			while(text_box[y+1-1][x-2] == '\0')
			{
				x--;
			}
			wmove(win,y+1,x);
			wrefresh(win);
			*line++;
			*pos = x;
		}
	}

	return 0;
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

void open(WINDOW *win,WINDOW *win_mode,char address[])
{
	clear_text_box();
    wclear(win_mode);
    box(win_mode,0,0);
    number_win(win);
    wattron(win_mode,COLOR_PAIR(1));
        wattron(win_mode,A_BOLD);

    mvwprintw(win_mode,1,1,"OPENED %s",name);
    wrefresh(win_mode);
    wattroff(win_mode,COLOR_PAIR(1));
        wattroff(win_mode,A_BOLD);

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
	int cnt_line = 1;
	int ymax,xmax;
	getmaxyx(win,ymax,xmax);
	int cnt = 0;
        while(1)
        {
		if(cnt_line == ymax-1) break;
            line[0] = '\0';
            if(fgets(line,200,myfile) == NULL) break;
            mvwprintw(win,cnt_line,2,"%s",line);
		strcpy(text_box[cnt],line);
	    cnt_line++;
        }
        fclose(myfile);
    }
    else{
        wprintw(win,"This file doesn't exist !\n");
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

void copy_from_new_to_file(char address[])
{
copy_for_undo(address);
    int l = 0,p = 0;
    int n = strlen(address);
    char text[maxl*200];
    FILE* clipboard = fopen("main_page.txt","r");
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
void normal_mode()
{
	echo();
	start_color();
        init_pair(1,COLOR_BLUE,COLOR_WHITE);
	int y_max = 20,x_max = 100;
        getmaxyx(stdscr,y_max,x_max);
        //create a window for input
        WINDOW *input = newwin(3,x_max,y_max-3,0);
        box(input,0,0);
        refresh();
        wrefresh(input);
        //create a window for mode
        WINDOW *mode_win = newwin(3,x_max,y_max-6,0);
        box(mode_win,0,0);
        refresh();
        wrefresh(mode_win);
        //create a window for text
        WINDOW *text = newwin(y_max-6,x_max,0,0);
        box(text,0,0);
	number_win(text);
        refresh();
        wrefresh(text);

	MODE = 2;
        wattron(mode_win,COLOR_PAIR(1));
        wattron(mode_win,A_BOLD);
        mvwprintw(mode_win,1,1,"NORMAL");
        wrefresh(mode_win);
  	wattroff(mode_win,A_BOLD);
        wattroff(mode_win,COLOR_PAIR(1));
	char command[maxl];
	char filename[maxl];
	mvwscanw(input,1,1,"%s",command);
	if(!strcmp(command,"saveas")||!strcmp(command,"save")) 
	{
		wclear(input);
		wclear(mode_win);
		box(mode_win,0,0);
		init_pair(2,COLOR_MAGENTA,COLOR_BLACK);
		wattron(mode_win,COLOR_PAIR(2));
        wattron(mode_win,A_BOLD);
        mvwprintw(mode_win,1,1,"ENTER THE NAME :");
        wrefresh(mode_win);
        wattroff(mode_win,A_BOLD);
        wattroff(mode_win,COLOR_PAIR(2));

		box(input,0,0);
		wrefresh(input);
		if(!strcmp(command,"save") && strcmp(name,"new text file"))
		{
			createfile(name);
			copy_from_new_to_file(name);
		}
		else{
			mvwscanw(input,1,1,"%[^\n]%*c",filename);
                createfile(filename);
                copy_from_new_to_file(filename);
		}
		wclear(input);
		box(input,0,0);
		wclear(mode_win);
		box(mode_win,0,0);
		wattron(mode_win,COLOR_PAIR(2));
        wattron(mode_win,A_BOLD);
		mvwprintw(mode_win,1,1,"SAVED");
		wrefresh(mode_win);
        wattroff(mode_win,A_BOLD);
        wattroff(mode_win,COLOR_PAIR(2));
		wrefresh(input);
	}
	else if(!strcmp(command,"open"))
	{
		wclear(input);
                wclear(mode_win);
                box(mode_win,0,0);
               // init_pair(2,COLOR_MAGENTA,COLOR_BLACK);
                wattron(mode_win,COLOR_PAIR(2));
        wattron(mode_win,A_BOLD);
       // mvwprintw(mode_win,1,1,"ENTER THE NAME TO OPEN:");
        wrefresh(mode_win);
        wattroff(mode_win,A_BOLD);
        wattroff(mode_win,COLOR_PAIR(2));

                box(input,0,0);
                wrefresh(input);
                if( strcmp(name,"new text file"))
                {
                        createfile(name);
                        copy_from_new_to_file(name);
                }
                else{
wclear(input);
                wclear(mode_win);
                box(mode_win,0,0);
                init_pair(2,COLOR_MAGENTA,COLOR_BLACK);
                wattron(mode_win,COLOR_PAIR(2));
        wattron(mode_win,A_BOLD);
        mvwprintw(mode_win,1,1,"ENTER THE NAME TO OPEN:");
        wrefresh(mode_win);
        wattroff(mode_win,A_BOLD);
        wattroff(mode_win,COLOR_PAIR(2));

                        mvwscanw(input,1,1,"%[^\n]%*c",filename);
                createfile(filename);
                copy_from_new_to_file(filename);
                }
                wclear(mode_win);
                box(mode_win,0,0);
                wattron(mode_win,COLOR_PAIR(2));
        wattron(mode_win,A_BOLD);
                mvwprintw(mode_win,1,1,"SAVED");
		 wrefresh(mode_win);
        wattroff(mode_win,A_BOLD);
        wattroff(mode_win,COLOR_PAIR(2));
                wrefresh(mode_win);


		wclear(input);
                wclear(mode_win);
                box(mode_win,0,0);
                init_pair(2,COLOR_MAGENTA,COLOR_BLACK);
                wattron(mode_win,COLOR_PAIR(2));
        wattron(mode_win,A_BOLD);
        mvwprintw(mode_win,1,1,"ENTER THE NAME TO OPEN :");
        wrefresh(mode_win);
        wattroff(mode_win,A_BOLD);
        wattroff(mode_win,COLOR_PAIR(2));

                box(input,0,0);
                wrefresh(input);
                mvwscanw(input,1,1,"%[^\n]%*c",filename);
		clear_name();
		strcpy(name,filename);
		open(text,mode_win,filename);
		wrefresh(text);

	
	wclear(input);
	box(input,0,0);
	wrefresh(input);
	}
	else if(!strcmp(command,"undo"))
	{
		wclear(input);
                wclear(mode_win);
                box(mode_win,0,0);
                init_pair(2,COLOR_MAGENTA,COLOR_BLACK);
                wattron(mode_win,COLOR_PAIR(2));
        wattron(mode_win,A_BOLD);
        mvwprintw(mode_win,1,1,"ENTER THE NAME :");
        wrefresh(mode_win);
        wattroff(mode_win,A_BOLD);
        wattroff(mode_win,COLOR_PAIR(2));

                box(input,0,0);
                wrefresh(input);
                mvwscanw(input,1,1,"%[^\n]%*c",filename);
                undo(filename);
		wclear(mode_win);
                box(mode_win,0,0);
                wattron(mode_win,COLOR_PAIR(2));
        wattron(mode_win,A_BOLD);
                mvwprintw(mode_win,1,1,"DONE");
                wrefresh(mode_win);
        wattroff(mode_win,A_BOLD);
        wattroff(mode_win,COLOR_PAIR(2));
                wrefresh(mode_win);

	}
}
void insert_mode()
{
	clear_text_box();
	echo();
	start_color();
	init_pair(1,COLOR_BLUE,COLOR_WHITE);
        init_pair(2,COLOR_MAGENTA,COLOR_BLACK);
	int y_max = 20,x_max = 100;
        getmaxyx(stdscr,y_max,x_max);
        //create a window for input
        WINDOW *input = newwin(3,x_max,y_max-3,0);
        box(input,0,0);
        refresh();
        wrefresh(input);
        //create a window for mode
        WINDOW *mode_win = newwin(3,x_max,y_max-6,0);
        box(mode_win,0,0);
        refresh();
        wrefresh(mode_win);
        //create a window for text
        WINDOW *text = newwin(y_max-6,x_max,0,0);
        box(text,0,0);
	number_win(text);
        refresh();
        wrefresh(text);
				
	 MODE = 1;
        wattron(mode_win,COLOR_PAIR(1));
        wattron(mode_win,A_BOLD);
        mvwprintw(mode_win,1,1,"INSERT   ");
	wprintw(mode_win,"%s +",name);
        wrefresh(mode_win);
        wattroff(mode_win,A_BOLD);
        wattroff(mode_win,COLOR_PAIR(1));
	int ch;
	int line = 1;
	int pos = 2;
	keypad(text,true);
	wmove(text,1,2);
	ch = wgetch(text);
	while(ch != 27)
	{
		if(navigation(text,ch,&line,&pos))
		{
			;
		}
		else
		{
		echo();
		text_box[line-1][pos-2] = ch;
		pos++;
		if(ch == '\n')
		{
			text_box[line-1][pos-2] = '\0';
			line++;
			pos = 2;
			wmove(text,line,pos);
		}
		}
		ch = wgetch(text);
		
	}
	text_box[line-1][pos-2] = 0;
	FILE *newfile = fopen("main_page.txt","w");
	for(int i = 0;i<line;i++)
	{
	    fputs(text_box[i],newfile);
	}
	fclose(newfile);
        wclear(input);
        box(input,0,0);
        wrefresh(input);
}
void visual_mode()
{
	echo();
        start_color();
        init_pair(1,COLOR_BLUE,COLOR_WHITE);
        init_pair(2,COLOR_MAGENTA,COLOR_BLACK);
        int y_max = 20,x_max = 100;
        getmaxyx(stdscr,y_max,x_max);
        //create a window for input
        WINDOW *input = newwin(3,x_max,y_max-3,0);
        box(input,0,0);
        refresh();
        wrefresh(input);
        //create a window for mode
        WINDOW *mode_win = newwin(3,x_max,y_max-6,0);
        box(mode_win,0,0);
        refresh();
        wrefresh(mode_win);
        //create a window for text
        WINDOW *text = newwin(y_max-6,x_max,0,0);
        box(text,0,0);
        number_win(text);
        refresh();
        wrefresh(text);

	 MODE = 3;
        wattron(mode_win,COLOR_PAIR(1));
        wattron(mode_win,A_BOLD);
        mvwprintw(mode_win,1,1,"VISUAL   ");
        wprintw(mode_win,"%s +",name);
        wrefresh(mode_win);
        wattroff(mode_win,A_BOLD);
        wattroff(mode_win,COLOR_PAIR(1));
	
	int n = strlen(name);
    char filename[max_address];
    for(int i = 1;i<n;i++)
    {
        filename[i-1] = name[i];
    }
    filename[n-1] = '\0';
    // filename = address - '/'
    FILE* myfile;
    if(fopen(filename,"r")) // check if file is created
    {
        myfile = fopen(filename,"r");
        char line[200];
        int cnt_line = 1;
        int ymax,xmax;
        getmaxyx(text,ymax,xmax);
        int cnt = 0;
        while(1)
        {
                if(cnt_line == ymax-1) break;
            line[0] = '\0';
            if(fgets(line,200,myfile) == NULL) break;
            mvwprintw(text,cnt_line,2,"%s",line);
                strcpy(text_box[cnt],line);
            cnt_line++;
        }
        fclose(myfile);
	}
    	wrefresh(text);
	int firsty,firstx;
	int ch;
	ch = wgetch(text);
	int line = 1, pos = 2;
	while(ch != 27)
        {
		if(ch == (int)'s')
		{
			getyx(text,firsty,firstx);
		}
                if(navigation(text,ch,&line,&pos))
                {
                        ;
                }
                ch = wgetch(text);
        }
	int lasty,lastx;
	getyx(text,lasty,lastx);
	firsty--;
	lasty--;
	firstx-=2;
	lastx-=2;
	FILE* clipboard = fopen("clipboard.txt","w");
	for(int i = firsty;i<=lasty;i++)
	{
		for(int j = firstx;j<=lastx;j++)
		{
			fprintf(clipboard,"%c",text_box[i][j]);
		}
	}
	fclose(clipboard);
}
int main()
{
	initscr();
	cbreak();
	int y_max = 20,x_max = 100;
        getmaxyx(stdscr,y_max,x_max);
        //create a window for input
        WINDOW *input = newwin(3,x_max,y_max-3,0);
        box(input,0,0);
        refresh();
        wrefresh(input);
        //create a window for mode
        WINDOW *mode_win = newwin(3,x_max,y_max-6,0);
        box(mode_win,0,0);
        refresh();
        wrefresh(mode_win);
        //create a window for text
        WINDOW *text = newwin(y_max-6,x_max,0,0);
        box(text,0,0);
	number_win(text);
        refresh();
        wrefresh(text);

        while(1)
	{
		char temp;
		noecho();
		start_color();
        	init_pair(1,COLOR_BLUE,COLOR_WHITE);
		temp = getch();
		if(temp == 'e')
		{
			break;
		}
		if(temp == 'i')
		{
		clear_text_box();
		 insert_mode();
		}
		else if(temp == 'v')
		{
		 visual_mode();
		}
		else if(temp == ':')
		{
		  normal_mode();
		}
		else if(temp == 'u')
		{
			char filename[1000];
			wclear(input);
                wclear(mode_win);
                box(mode_win,0,0);
                init_pair(2,COLOR_MAGENTA,COLOR_BLACK);
                wattron(mode_win,COLOR_PAIR(2));
        wattron(mode_win,A_BOLD);
        mvwprintw(mode_win,1,1,"ENTER THE NAME :");
        wrefresh(mode_win);
        wattroff(mode_win,A_BOLD);
        wattroff(mode_win,COLOR_PAIR(2));

                box(input,0,0);
                wrefresh(input);
                mvwscanw(input,1,1,"%[^\n]%*c",filename);
                undo(filename);
                wclear(mode_win);
                box(mode_win,0,0);
                wattron(mode_win,COLOR_PAIR(2));
        wattron(mode_win,A_BOLD);
                mvwprintw(mode_win,1,1,"DONE");
                wrefresh(mode_win);
        wattroff(mode_win,A_BOLD);
        wattroff(mode_win,COLOR_PAIR(2));
                wrefresh(mode_win);

			undo(filename);
		}
		else if(temp == '=')
		{
			auto_indent(name);
		}
	}	
	endwin();
	return 0;
}
