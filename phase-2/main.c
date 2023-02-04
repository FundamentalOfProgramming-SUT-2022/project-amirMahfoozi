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
		if( text_box[*line][*pos+1] == '\n')
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
                if(*pos-1 < 0 || text_box[*line][*pos-1] == '\0' || text_box[*line][*pos-1] == '\n')
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
                        wmove(win,y-1,x);
                        wrefresh(win);
                        *line--;
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
			wmove(win,y+1,x);
			wrefresh(win);
			*line++;
		}
	}

	return 0;
}

void open(WINDOW *win,WINDOW *win_mode,char address[])
{
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
        while(1)
        {
		if(cnt_line == ymax-1) break;
            line[0] = '\0';
            if(fgets(line,200,myfile) == NULL) break;
            mvwprintw(win,cnt_line,2,"%s",line);
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
		wclear(mode_win);
		box(mode_win,0,0);
		wattron(mode_win,COLOR_PAIR(2));
        wattron(mode_win,A_BOLD);
		mvwprintw(mode_win,1,1,"SAVED");
		wrefresh(mode_win);
        wattroff(mode_win,A_BOLD);
        wattroff(mode_win,COLOR_PAIR(2));
		wrefresh(mode_win);
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
		clear_name();
		strcpy(name,"new text file");

	
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
	}	
	endwin();
	return 0;
}