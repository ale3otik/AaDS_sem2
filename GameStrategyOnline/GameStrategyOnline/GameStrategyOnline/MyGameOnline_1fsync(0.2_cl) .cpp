#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>
#include <dirent.h>
/////////game size//////////////////////
#define  I_KUB 5
#define  B_KUB 8
#define  M_MAX_I 10
#define  M_MAX_B 16
#define SIZEGAME  int
#define SZ_1 1
#define SZ_2 2
#define SZ_3 3
#define SZ_4 4 
#define SET 1
#define GET 2
////////////////////////////
#define MAXNAME 30
#define longmenu 3
#define LONG_GAM_MENU 4
#define DEFENDER 500
#define MaxCoins 150
#define MaxArmy 2000
#define NEXT_TURN -2
#define NextTURNmenuX MAX_B*B_KUB + 2
#define NextTURNmenuY 15
#define LeftEMX MAX_B*B_KUB + 1
#define DEFCOF 0.30
#define KINGCOF 0.25
#define EXIT -3
///////////key/////////
#define KEY_SPEC 224
#define KEY_ESC 27
#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77

#define Ctrl_KEY_UP 141
#define Ctrl_KEY_DOWN 145
#define Ctrl_KEY_LEFT 115
#define Ctrl_KEY_RIGHT 116

#define ALT_SPEC 0
#define Alt_KEY_UP 152
#define Alt_KEY_DOWN 160
#define Alt_KEY_LEFT 155
#define Alt_KEY_RIGHT 157

#define PLUS1 61
#define PLUS2 43
#define MINUS1 45
#define MINUS2 95

#define ORKCOF 1.2
#define ORKCOF_5 1.7
#define ORKCOF_10 2.4
#define ORKCOF_15 3.6

#define PEPCOF 1
#define PEPCOF_5 1.5
#define PEPCOF_10 2
#define PEPCOF_15 3
//////////////////////////
#define LASTMAP 1488
/////////////////////////////////
#define _SIDE 1 ///// 1- people,2-orcs
char kl_name[] = "settings/kl_1";
char kl_stop_name[] = "settings/kl_stop_1";
char sr_go[] = "settings/sr_go_1";
char name[] = "settings/123";
int mapcheck = 1;
//////////////////////////////////
//char name[MAXNAME]="settings/";
SIZEGAME MAX_I = 5;
SIZEGAME MAX_B = 7;
FILE *fwr;
///////HANDLE////
HANDLE consoleOutput;
void HandleSettle()
{
     consoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
     COORD crdwin = {167,57};
     SMALL_RECT src = {0, 0, crdwin.X-1, crdwin.Y-1};
    SetConsoleWindowInfo (consoleOutput, true, &src);
    SetConsoleScreenBufferSize(consoleOutput, crdwin); 
    CONSOLE_CURSOR_INFO cci = {1,FALSE};
    SetConsoleCursorInfo(consoleOutput,&cci);
}
////functions////
void PrintMap(int,int);
int NextTurnMenu(void);
void WarCount(void);
int defender(int,int);
int recruit(int,int);
int disband(int,int);
int move(int,int);
int move_menu(int,int,int);
void GotoXY(int , int );
void EnemyMainFunc(void);
void EnemyMapsState(void);
int  EnemyFreeResourceCalculate(void);
int buy_army_attack(int ,int );
int EnemyDefFunc(int,int);
int EnemyRandomDefFunc(int);
int EnemyRandomAttack(int);
int EnemyRandomFreeAttack(int);
void attack_free(int,int);//enemy free attack
int set_get_size_game(int,int);
int countpoints(void);
int countmaps(void);
int game_menuIB(int, int);
void HandleSettle(void);
void left_edge_menu(void);
void peoplewinfunc(void);
void opkwinfunc(void);
int new_game(void);
int open_game(void);
void file_in(void);
void hide_game_settings(FILE*);
void show_game_settings(FILE*);
void write_move(int,int,int,int,int);
void write_coins(int);
void write_army(int,int,int);
void write_def(int,int);
void write_disband(int,int,int);
// объявляем новый тип структур
struct country
{
       unsigned int army;
       long coins;
       int maps;
       int points;
}people,orks;
struct neighbours
{
       int left;
       int right;
       int top;
       int under;
};
struct location
{
     int army;
     int lastarmy;
     int country;
     int def;
     int kurs;
     int movecheck;
     int bufpeople;
     int buforks;
     ///метки для ИИ/////////
     int need_def_help;
     //int need_attack_help;
     int free_army;
     neighbours bngb;  
     neighbours sngb; 
     int fratt;
     int batt;
}loc[M_MAX_I][M_MAX_B];// 2мерный массив структур типа location максимального размера
void left_edge_menu()
{
     SetConsoleTextAttribute(consoleOutput, 15);
     GotoXY(LeftEMX+2,4); printf("coins:");
     printf("          \b\b\b\b\b\b\b\b\b\b");
     SetConsoleTextAttribute(consoleOutput, 14); printf("%d",people.coins);
     
     SetConsoleTextAttribute(consoleOutput, 15);
     GotoXY(LeftEMX+2,6); printf("TurnPoints:");
     printf("   \b\b\b");
     SetConsoleTextAttribute(consoleOutput, 10); printf("%d",people.points);    
}
int ExitMenu()
{
       int X= B_KUB * int(MAX_B/2),Y = I_KUB * int(MAX_I/2);
       int z,v,c;
       int k = 0;
       const char* lines[2];
       lines[0] = "Yes";
       lines[1] = "No";
       GotoXY(X,Y);
       SetConsoleTextAttribute(consoleOutput, 143);
       for(z=0;z<(2*I_KUB);z++)
          {
           for(v=0;v< 3*B_KUB;v++)
           printf(" ");
           GotoXY(X,Y+z+1);
           }
            GotoXY(X+4,Y+2);
            printf("Exit?");
            while(1 )
            {
               for(int i=0;i<2;i++)
               {
                  GotoXY(X + (i*5)+4,Y+5);
                   SetConsoleTextAttribute(consoleOutput, 143); 
                  if(i == k)SetConsoleTextAttribute(consoleOutput, 80); 
                  printf("%s",lines[i]);
               }
               if((c=getch())== KEY_SPEC)//код управляющего символа первый символ
                {
                      c = getch();
                     if(c==KEY_LEFT || c== KEY_RIGHT || c == KEY_UP || c == KEY_DOWN)
                     {
                       if(c== KEY_UP|| c ==KEY_RIGHT)
                           if(k==0) k= 1;
                       else k--;
                       if(c== KEY_LEFT || c == KEY_DOWN) 
                             if(k== 1)k=0;
                             else k++;
                      }
                }
              if(c == '\r')
              {
                  if(k == 0) 
                  {
                     return EXIT;
                  }
               break;
              }
              if(c== KEY_ESC) break;
          }
          // clear ////
          for(z=0;z<2;z++)
              for(v=0;v<3;v++) PrintMap(v+int(MAX_B/2),z+int(MAX_I/2));
         return 0;
}
int Menu ()
{
    SetConsoleTextAttribute(consoleOutput, 15); 
    system("cls");
    int c, i,b;
    const char *line[longmenu];
    line[1] = "New game";
    line[0] = "Open game";
    line[2] = "Exit";
    
 SetConsoleTextAttribute(consoleOutput, 127);
   printf("%s\n",line[0]);
 SetConsoleTextAttribute(consoleOutput, 15); 
 for(b=1;b<longmenu;b++)
   printf("%s\n",line[b]);
 i=0; 
while((c=getch()) != '\r' )
{
 if(c== KEY_SPEC)//код управляющего символа первый символ
  {

        c = getch();
        if(c==KEY_UP || c== KEY_DOWN)
        { 
            if(c== KEY_UP)
                if(i==0)i=longmenu-1;
                else i--;
            if(c== KEY_DOWN) 
                if(i==longmenu-1)i=0;
                else i++;
            system ("cls");
            for(b=0;b<longmenu;b++)
            {
               if(b==i)
               {
                   SetConsoleTextAttribute(consoleOutput, 127);
                   printf("%s\n",line[b]);
                   SetConsoleTextAttribute(consoleOutput, 15);
               }
               else  printf("%s\n",line[b]);
            }
         }    
    }
    }
    system ("cls");
    return i;
}
//gotoXY
void GotoXY(int X, int Y)//функция перевода на координату
{
	COORD coord = { X, Y };
	SetConsoleCursorPosition(consoleOutput, coord);
}

// printmap
void PrintMap(int X, int Y)//печатает кубик. 3 аргумент структура с именем param, тип location
{
     int i = Y,b = X;
     int f,j;
     X = (X*B_KUB);
     Y = (Y*I_KUB);
     // базовая заливка

      SetConsoleTextAttribute(consoleOutput, 15);
        for(f=0;f<I_KUB-2;f++)     //печатает фон
        {
        GotoXY(X+1,Y+f+1);
        printf("      ");
       }
      // Войска
      if(loc[i][b].army >= 1000) GotoXY(X+2,Y+2);
      if(loc[i][b].army < 1000)  GotoXY(X+3,Y+2);
      if(loc[i][b].army < 100)  GotoXY(X+4,Y+2);
      
        SetConsoleTextAttribute(consoleOutput, 15);
        if(loc[i][b].country == 1) SetConsoleTextAttribute(consoleOutput, 14);
        if(loc[i][b].country == 2) SetConsoleTextAttribute(consoleOutput, 12);
        if(_SIDE == 2)
        {
            if(loc[i][b].country == 2) SetConsoleTextAttribute(consoleOutput, 14);
            if(loc[i][b].country == 1) SetConsoleTextAttribute(consoleOutput, 12);
        }
        if(loc[i][b].army >= 1000000) printf("%d..",loc[i][b].army/10000);
        else
           if(loc[i][b].army >= 100000) printf("%d...",loc[i][b].army/1000);
           else printf("%d",loc[i][b].army);
         
     //рамка
     SetConsoleTextAttribute(consoleOutput, 119);
     if(loc[i][b].country == 1)
        SetConsoleTextAttribute(consoleOutput, 17);
     if(loc[i][b].country == 2)
        SetConsoleTextAttribute(consoleOutput, 68);
     if(_SIDE == 2)
        {
               if(loc[i][b].country == 2)
                    SetConsoleTextAttribute(consoleOutput, 17);
               if(loc[i][b].country == 1)
                    SetConsoleTextAttribute(consoleOutput, 68);
        }
        // move army color
     if(_SIDE == 2)
     {
              if(loc[i][b].movecheck == 1 && loc[i][b].country == 0)
                 SetConsoleTextAttribute(consoleOutput, 102);
              if(loc[i][b].movecheck == 1 && loc[i][b].country == 2)
                 SetConsoleTextAttribute(consoleOutput, 51);
              if(loc[i][b].movecheck == 1 && loc[i][b].country == 1)
                 SetConsoleTextAttribute(consoleOutput, 85);
                  // kursor
              if(loc[i][b].kurs == 1 )
                  SetConsoleTextAttribute(consoleOutput, 231);
              if(loc[i][b].kurs == 1 && loc[i][b].country == 1 )
                  SetConsoleTextAttribute(consoleOutput, 204);
              if(loc[i][b].kurs == 1 && loc[i][b].country == 2 )
                  SetConsoleTextAttribute(consoleOutput, 153);
      }
      else
      {
          if(loc[i][b].movecheck == 1 && loc[i][b].country == 0)
         SetConsoleTextAttribute(consoleOutput, 102);
         if(loc[i][b].movecheck == 1 && loc[i][b].country == 1)
         SetConsoleTextAttribute(consoleOutput, 51);
        if(loc[i][b].movecheck == 1 && loc[i][b].country == 2)
     SetConsoleTextAttribute(consoleOutput, 85);
        // kursor
     if(loc[i][b].kurs == 1 )
        SetConsoleTextAttribute(consoleOutput, 231);
     if(loc[i][b].kurs == 1 && loc[i][b].country == 2 )
        SetConsoleTextAttribute(consoleOutput, 204);
     if(loc[i][b].kurs == 1 && loc[i][b].country == 1 )
        SetConsoleTextAttribute(consoleOutput, 153);
      }
        //
        GotoXY(X,Y);
        for(j=0;j<B_KUB;j++) 
		{
		   //печатает верхнюю линию        {
         printf(" ");
        }
     for(f=0;f<I_KUB-2;f++)     //печатает боковые
     {
        GotoXY(X,Y+f+1);
        printf(" ");
        GotoXY(X+B_KUB-1,Y+f+1);
         printf(" ");
     }
        GotoXY(X,Y+I_KUB-1);
         for(j=0;j<B_KUB;j++)   //печатает нижнюю грань
        {
         printf(" ");
        //печатать пробел
        }
        
        // Укрепление
        if(loc[i][b].def == 1)
        {  
           GotoXY(X+1,Y+3);
           SetConsoleTextAttribute(consoleOutput, 48);
           for(f=0;f<B_KUB-2;f++)
           printf("x");// можно просто печатать пробел9
        }
        
}
/// main gotomap
int gotomap()
{
    static int i=0;// ось Y
    static int b=0;// ось X
    static int c=0;
    int x,y;
    loc[i][b].kurs=1;
    PrintMap(b, i);
while(1)
{ 
    
    if((c=getch())== KEY_SPEC)
    {
    c=getch();
    if(c==KEY_DOWN)   // стрелочка вниз
    {
        if(i+1 < MAX_I )
        {
            
                loc[i][b].kurs=0;
                PrintMap(b,i);
                i++;
                loc[i][b].kurs=1;
                PrintMap(b, i);
            
        }
    }
    if(c==KEY_UP)   //стрелочка вверх
    {
        if(i-1 >= 0 )
        {
        loc[i][b].kurs=0;
        PrintMap(b,i);
        i--;
        loc[i][b].kurs=1;
        PrintMap(b,i);
        }
    }
    if(c==KEY_LEFT)   //стрелочка влево
    {
      if(b-1 >= 0)
      {
        loc[i][b].kurs=0;
        PrintMap(b,i);
        b--;
        loc[i][b].kurs=1;
        PrintMap(b, i);
      }
    }
    if(c==KEY_RIGHT)   //стрелочка вправо
    {
        if(b+1< MAX_B)
        {
        loc[i][b].kurs=0;
        PrintMap(b,i);
        b++;
        loc[i][b].kurs=1;
        PrintMap(b, i);
        }
    }
  }
   else
   {
       if(c== '\r') return i*100+b;
       if(c == KEY_ESC) return EXIT;
       if(c== ' ') 
       {
           loc[i][b].kurs = 0;
           PrintMap(b,i);
           if(NextTurnMenu()) return NEXT_TURN;
           loc[i][b].kurs = 1;
           PrintMap(b,i);
           
           
       }
   }
 }
}
////////////NextTurnMenu////////////////////////////////////////////////////
int NextTurnMenu()
{
    
    int c,k=0,i;
   const char *line_next_turn[2];
    line_next_turn[0] = "NextTurn";
    line_next_turn[1] = "Back to Game";
    while(1 )
   {
      for(i=0;i<2;i++)
      {
          GotoXY(NextTURNmenuX,NextTURNmenuY+i+2);
          SetConsoleTextAttribute(consoleOutput, 15); 
          if(i == k)SetConsoleTextAttribute(consoleOutput, 80); 
          printf("%s",line_next_turn[i]);
      }
      if((c=getch())== KEY_SPEC)//код управляющего символа первый символ
        {
           c = getch();
           if(c==KEY_LEFT || c== KEY_RIGHT || c == KEY_UP || c == KEY_DOWN)
           {
               if(c== KEY_UP|| c ==KEY_RIGHT)
                   if(k==0) k= 1;
                   else k--;
               if(c== KEY_LEFT || c == KEY_DOWN) 
                  if(k== 1)k=0;
                  else k++;
           }
        }
      if(c == '\r')
      {
          if(k == 0) 
          {
               ///////////////////clear/////
              SetConsoleTextAttribute(consoleOutput, 15); 
              GotoXY(NextTURNmenuX,NextTURNmenuY+2); printf("             ");
              GotoXY(NextTURNmenuX,NextTURNmenuY+1+2); printf("             ");
              ////////////////////////////
               return NEXT_TURN;
          }
          break;
      }
      if(c== 27) break;
  }
///////////////////clear/////
SetConsoleTextAttribute(consoleOutput, 15); 
 GotoXY(NextTURNmenuX,NextTURNmenuY+2); printf("             ");
  GotoXY(NextTURNmenuX,NextTURNmenuY+1+2); printf("             ");
////////////////////////////

   return 0; 
}
///geme menu//////////////////////////////////////////////////////////////
int game_menuIB(int i,int b)
{
    while(1)
    {
    left_edge_menu();//рисуем менюс лева
    if(loc[i][b].country != 1) return -1;
    int X,Y,z,v,k,f,c;
   const char *line[LONG_GAM_MENU];
    line[0] = "Recruit soldiers";
    line[1] = "Move army";
    line[2] = "Fortify";
    line[3] = "Disband";
   // line[4] = "End of Turn";
    //
    if(i<= MAX_I/2) Y = (i + 1) * I_KUB;
    else Y = (i-2)*I_KUB;
    if(b <= MAX_B/2) X = (b + 1) * B_KUB;
    else X = (b - 3)*B_KUB;
    // печать окна
    GotoXY(X,Y);
    SetConsoleTextAttribute(consoleOutput, 143);
    for(z=0;z<(2*I_KUB);z++)
    {
       for(v=0;v< 3*B_KUB;v++)
       printf(" ");
       GotoXY(X,Y+z+1);
    }
    //print param
    SetConsoleTextAttribute(consoleOutput, 128);
     GotoXY(X+3,Y+5); printf("army: ");
     SetConsoleTextAttribute(consoleOutput, 129);
     printf("%d",loc[i][b].army);
     SetConsoleTextAttribute(consoleOutput, 128);
     GotoXY(X+3,Y+6); printf("coins: ");
     SetConsoleTextAttribute(consoleOutput, 142);
     if(loc[i][b].country == 1)
     printf("%d",people.coins);
     if(loc[i][b].country == 2)
     printf("%d",orks.coins);
     if(loc[i][b].country == 0)
     printf("???");
     
     if(people.points == 0)
     {
         GotoXY(X+3,Y+8); 
         SetConsoleTextAttribute(consoleOutput, 124);
         printf("0");
         SetConsoleTextAttribute(consoleOutput, 140);
         printf(" TurnPoints");
     }
     
     
    
    //
    k = 0;
    while(1)
    {
        for(f=0;f<LONG_GAM_MENU;f++)
          {
             SetConsoleTextAttribute(consoleOutput, 143);
             if(k==f) SetConsoleTextAttribute(consoleOutput, 80);
             GotoXY(X+3,Y+1*f);
             printf("%s",line[f]);
          }
        if((c=getch())== KEY_SPEC)//код управляющего символа первый символ
        {
           c = getch();
           if(c==KEY_UP || c== KEY_DOWN)
           {
               if(c== KEY_UP)
                   if(k==0) k= LONG_GAM_MENU -1;
                   else k--;
               if(c== KEY_DOWN) 
                  if(k== LONG_GAM_MENU-1)k=0;
                  else k++;
           }
        }
           if(c== '\r') break; 
           if(c== ' ')
           {
                  if( NEXT_TURN == NextTurnMenu())
                  {
                      for(f=0;f<2;f++)
                      for(v=0;v<3;v++)
                             PrintMap((X/B_KUB)+v,(Y/I_KUB)+f);
                       return NEXT_TURN;
                  }
                  
           }
           if(c== KEY_ESC)
           {
               k = -1;
               break;
           }
       } 
       if(k== -1)
       {
       // clear/////////////////////
            for(f=0;f<2;f++)
               for(v=0;v<3;v++)
                  PrintMap((X/B_KUB)+v,(Y/I_KUB)+f);
            return k;
       }
       //
       //return////
       switch (k)
       {     
              case 0:
                      if(people.points>0) 
                        if(recruit(i,b)== NEXT_TURN) return NEXT_TURN;
                      if(loc[i][b].army > 1000000) loc[i][b].army = 1000000;
                      break;
              case 1:
                      if(loc[i][b].army > 0 && people.points > 0)
                      {
                        // clear/////////////////////
                         for(f=0;f<2;f++)
                         for(v=0;v<3;v++)
                         PrintMap((X/B_KUB)+v,(Y/I_KUB)+f);
                        //
                         if(move(i,b) == NEXT_TURN) return NEXT_TURN;
                         return 0;
                      }
                      break;
              case 2:
                      if(people.points>0)
                        if( defender(i,b) == NEXT_TURN) return NEXT_TURN;
                      break;
              case 3:
                      if(disband(i,b) == NEXT_TURN) return NEXT_TURN;
                      break;
             default: 
                     system("cls");
                     printf("FATAL ERROR: game_menuIB");
                     Sleep(1000);
                     exit(1);
       }
     
  }
       
       //
    
}
//move////////////////////////////
int move(int i,int b)
{
     int X,Y,z,v,c;
     int ix = i,bx = b;// положение основного кубика
     // print new color
     
     if(b-1 >=0)
     { 
             loc[i][b-1].movecheck = 1;
             PrintMap(b-1,i);
     }
     if(b+1 < MAX_B)
     {
            loc[i][b+1].movecheck = 1;
             PrintMap(b+1,i);
     }
     if(i-1 >= 0)
     {
            loc[i-1][b].movecheck = 1;
            PrintMap(b,i-1);
     }
     if(i+1 < MAX_I)
     {
            loc[i+1][b].movecheck = 1;
            PrintMap(b,i+1);
     }
     // goto map
     while(1)
    { 
    if(loc[ix][bx].army == 0  || people.points == 0) break;
    //GotoXY(0,MAX_I*I_KUB);
    if((c=getch())== KEY_SPEC)
    {
    c=getch();
    if(c==KEY_DOWN)   // стрелочка вниз
    {
        if(i+1 < MAX_I && (loc[i+1][b].movecheck == 1 || i+1 == ix&& b == bx))
        {
            
                loc[i][b].kurs=0;
                PrintMap(b,i);
                i++;
                loc[i][b].kurs=1;
                PrintMap(b, i);
            
        }
    }
    if(c==KEY_UP)   //стрелочка вверх
    {
        if(i-1 >= 0 && (loc[i-1][b].movecheck == 1 || i-1 == ix && b == bx) )
        {
        loc[i][b].kurs=0;
        PrintMap(b,i);
        i--;
        loc[i][b].kurs=1;
        PrintMap(b,i);
        }
    }
    if(c==KEY_LEFT)   //стрелочка влево
    {
      if(b-1 >= 0 &&( loc[i][b-1].movecheck == 1 || b-1 == bx && i == ix))
      {
        loc[i][b].kurs=0;
        PrintMap(b,i);
        b--;
        loc[i][b].kurs=1;
        PrintMap(b, i);
      }
    }
    if(c==KEY_RIGHT)   //стрелочка вправо
    {
        if(b+1< MAX_B && (loc[i][b+1].movecheck == 1 || b+1 == bx && i == ix))
        {
        loc[i][b].kurs=0;
        PrintMap(b,i);
        b++;
        loc[i][b].kurs=1;
        PrintMap(b, i);
        }
    }
  }
   else
   {
       if(c== '\r' || c == '\n')
       {
         if(i != ix || b!= bx)
         {
              
               if(move_menu(i,b,ix*100 + bx)== NEXT_TURN) 
               {
                                      loc[ix][bx].kurs = 1;
                                      loc[i][b].kurs = 0; 
                                      return NEXT_TURN;
               }
               loc[i][b].kurs = 0;
               PrintMap(b,i);
               i = ix;
               b = bx;
               loc[i][b].kurs = 1;
               PrintMap(b,i);
               
         }
          else break;
       }
       if(c== KEY_ESC)
       {
              loc[i][b].kurs = 0;
              loc[ix][bx].kurs = 0;
               break;
       }
       if( c == ' ') 
         if(NextTurnMenu() == NEXT_TURN) return NEXT_TURN;
   }
 }
   // clear
     if(bx-1 >=0)
     { 
             loc[ix][bx-1].movecheck = 0;
             PrintMap(bx-1,ix);
     }
     if(bx+1 < MAX_B)
     {
            loc[ix][bx+1].movecheck = 0;
             PrintMap(bx+1,ix);
     }
     if(ix-1 >= 0)
     {
            loc[ix-1][bx].movecheck = 0;
            PrintMap(bx,ix-1);
     }
     if(ix+1 < MAX_I)
     {
            loc[ix+1][bx].movecheck = 0;
            PrintMap(bx,ix+1);
     }
 
     
     
}
// move menu////////
int move_menu(int i,int b,int zx)
{
     int X,Y,z,v,f,c,ix,bx;
      ix = zx/100;
      bx = zx%100;
     int cof=1;
    if(i<= MAX_I/2) Y = (i + 1) * I_KUB;
    else Y = (i-2)*I_KUB;
    if(b <= MAX_B/2) X = (b + 1) * B_KUB;
    else X = (b - 3)*B_KUB;
    // печать окна
    GotoXY(X,Y);
    SetConsoleTextAttribute(consoleOutput, 143);
    for(z=0;z<(2*I_KUB);z++)
    {
       for(v=0;v< 3*B_KUB;v++)
       printf(" ");
       GotoXY(X,Y+z+1);
    }
    GotoXY(X+2,Y+3); 
    printf("Alt +100 | Ctrl +10");
    //
    GotoXY(X + 7,Y+1);
    printf("move army");
    //
    int narmy = loc[ix][bx].army/2;
    while(1)
    {
          GotoXY(X+2,Y+4);
          SetConsoleTextAttribute(consoleOutput, 17);
          for(z=0;z< (float)narmy/loc[ix][bx].army * 20;z++)
             printf(" ");
          SetConsoleTextAttribute(consoleOutput, 119);
          for(;z<B_KUB*3 - 4;z++) printf(" ");
          SetConsoleTextAttribute(consoleOutput, 129);
          GotoXY(X+5+6,Y+6); 
          printf("      ");
          GotoXY(X+5+6,Y+6); 
          printf("%d",narmy);
             if((c = getch()) == KEY_SPEC || c == 0 || c == PLUS1 || c == PLUS2 || c == MINUS1 || c == MINUS2)
          {
                 cof = 1;
                 if(c==ALT_SPEC) cof = 100;
                 if( c == PLUS1 || c == PLUS2 || c == MINUS1 || c == MINUS2) cof = 1000;
                 else c = getch();
           if(c== KEY_UP || c== KEY_DOWN || c == KEY_LEFT || c == KEY_RIGHT||
              c == Ctrl_KEY_UP|| c == Ctrl_KEY_DOWN|| c == Ctrl_KEY_LEFT|| c == Ctrl_KEY_RIGHT||
              c == Alt_KEY_UP|| c == Alt_KEY_DOWN|| c == Alt_KEY_LEFT|| c == Alt_KEY_RIGHT|| c == PLUS1 || c == PLUS2 || c == MINUS1 || c == MINUS2)
           //всевозможные комбинации стрелок и Ctrl и Alt
           {
               if( c == Ctrl_KEY_UP|| c == Ctrl_KEY_DOWN|| c == Ctrl_KEY_LEFT|| c == Ctrl_KEY_RIGHT) cof = 10;
               if(c==  KEY_DOWN || c ==  KEY_LEFT|| c == Ctrl_KEY_DOWN||c==Ctrl_KEY_LEFT||c==Alt_KEY_DOWN||c==Alt_KEY_LEFT|| c == MINUS1 || c == MINUS2)
                   if(narmy-cof>=0) 
                   {
                       narmy-=cof; 
                   }
                   else
                   {
                       narmy = 0;
                   }
               if(c== KEY_UP|| c ==KEY_RIGHT|| c == Ctrl_KEY_UP|| c == Ctrl_KEY_RIGHT|| c == Alt_KEY_UP|| c == Alt_KEY_RIGHT|| c == PLUS1 || c == PLUS2 )
                  if(narmy+cof <= loc[ix][bx].army)
                  {
                       
                       narmy+=cof;
                  }
                  else
                  {
                       narmy = loc[ix][bx].army;
                  }
           }
         }
        if(c == '\r'|| c == '\n')
         {
             if(narmy != 0) people.points--;
             loc[ix][bx].army -= narmy;
             loc[i][b].bufpeople  +=  narmy;
             write_move(ix,bx,i,b,narmy);
             PrintMap(bx,ix);
             left_edge_menu();
             break;
         }
         if(c== KEY_ESC) break;
         if(c == ' ')
            if(NextTurnMenu() == NEXT_TURN) return NEXT_TURN;
        
     }
      // clear/////////////////////
            for(f=0;f<2;f++)
               for(v=0;v<3;v++)
                  PrintMap((X/B_KUB)+v,(Y/I_KUB)+f);
    }
// recruit///////
int recruit(int i,int b)
{
     int X,Y,z,v,f,c;
     int nowcoin,army,cof=1;
    if(i<= MAX_I/2) Y = (i + 1) * I_KUB;
    else Y = (i-2)*I_KUB;
    if(b <= MAX_B/2) X = (b + 1) * B_KUB;
    else X = (b - 3)*B_KUB;
    // печать окна
    GotoXY(X,Y);
    SetConsoleTextAttribute(consoleOutput, 143);
    for(z=0;z<(2*I_KUB);z++)
    {
       for(v=0;v< 3*B_KUB;v++)
       printf(" ");
       GotoXY(X,Y+z+1);
    }
    GotoXY(X+2,Y+3);
     
     printf("Alt +100 | Ctrl +10");
     nowcoin = people.coins/2;
     army = nowcoin;
     GotoXY(X+3,Y+1);
     printf("Recruit soldiers");
     GotoXY(X+5,Y+8);
     printf("coins:");
     GotoXY(X+5,Y+6); printf("+army:");
     while(1)
     { 
          GotoXY(X+2,Y+4);
          SetConsoleTextAttribute(consoleOutput, 17);
          for(z=0;z< (float)army/people.coins * 20;z++)
             printf(" ");
          SetConsoleTextAttribute(consoleOutput, 119);
          for(;z<B_KUB*3 - 4;z++) printf(" ");
          SetConsoleTextAttribute(consoleOutput, 129);
          GotoXY(X+5+6,Y+6); 
          printf("      ");
          GotoXY(X+5+6,Y+6); 
          printf("%d",army);
          SetConsoleTextAttribute(consoleOutput, 142);
          GotoXY(X+5+6,Y+8); printf("     ");
          GotoXY(X+5+6,Y+8); printf("%d",nowcoin);
          if((c = getch()) == KEY_SPEC || c == ALT_SPEC || c == PLUS1 || c == PLUS2 || c == MINUS1 || c == MINUS2)
          {
                 cof = 1;
                 if(c==ALT_SPEC) cof = 100;
                 if(c == PLUS1 || c == PLUS2 || c == MINUS1 || c == MINUS2) cof =1000;
                 else
                    c = getch();
           if(c== KEY_UP || c== KEY_DOWN || c == KEY_LEFT || c == KEY_RIGHT||
              c == Ctrl_KEY_UP|| c == Ctrl_KEY_DOWN|| c == Ctrl_KEY_LEFT|| c == Ctrl_KEY_RIGHT||
              c == Alt_KEY_UP|| c == Alt_KEY_DOWN|| c == Alt_KEY_LEFT|| c == Alt_KEY_RIGHT || c == PLUS1 || c == PLUS2 || c == MINUS1 || c == MINUS2)
           //всевозможные комбинации стрелок и Ctrl и Alt
           {
               if( c == Ctrl_KEY_UP|| c == Ctrl_KEY_DOWN|| c == Ctrl_KEY_LEFT|| c == Ctrl_KEY_RIGHT) cof = 10;
                if(c== KEY_UP|| c ==KEY_RIGHT|| c == Ctrl_KEY_UP|| c == Ctrl_KEY_RIGHT|| c == Alt_KEY_UP|| c == Alt_KEY_RIGHT || c == PLUS1 || c == PLUS2)
                   if(nowcoin-cof>= 0) 
                   {
                       nowcoin -=cof;
                       army+=cof; 
                   }
                   else
                   {
                       army += nowcoin;
                       nowcoin = 0;
                   }
                if(c==  KEY_DOWN || c ==  KEY_LEFT|| c == Ctrl_KEY_DOWN||c==Ctrl_KEY_LEFT||c==Alt_KEY_DOWN||c==Alt_KEY_LEFT ||c == MINUS1 || c == MINUS2) 
                  if(army-cof >= 0)
                  {
                       nowcoin+=cof;
                       army-=cof;
                  }
                  else
                  {
                      nowcoin += army;
                      army = 0;
                  }
           }
         }
        if(c == '\r' || c == '\n')
         {
             
             if(army > 0) people.points--;
             write_coins(people.coins-nowcoin);
             people.coins = nowcoin;
             loc[i][b].army += army;
             write_army(i,b,army);
             PrintMap(b,i);
             return 0;
         }
         if(c== KEY_ESC) return 0;
         if(c == ' ') 
            if(NextTurnMenu() == NEXT_TURN) return NEXT_TURN;
        
     }
}
///defender/////
int defender(int i,int b)
{
     if(loc[i][b].def == 1) return 0;
      int X,Y,z,v,f,c,k=0;
    const char *line[2];
    line[0] = "YES";
    line[1] = "NO!";
    if(i<= MAX_I/2) Y = (i + 1) * I_KUB;
    else Y = (i-2)*I_KUB;
    if(b <= MAX_B/2) X = (b + 1) * B_KUB;
    else X = (b - 3)*B_KUB;
    // печать окна
    GotoXY(X,Y);
    SetConsoleTextAttribute(consoleOutput, 143);
    for(z=0;z<(2*I_KUB);z++)
    {
       for(v=0;v< 3*B_KUB;v++)
       printf(" ");
       GotoXY(X,Y+z+1);
    }
     if(people.coins < DEFENDER)
     {
         GotoXY(X+4,Y + 4);
         SetConsoleTextAttribute(consoleOutput, 132);
         printf("You don't have");
         GotoXY(X+4,Y+6);
         printf("money to do it");
         getch();
         return 0;
     }
      
      GotoXY(X+4,Y + 2);
      SetConsoleTextAttribute(consoleOutput, 143);
      printf("Fortify for ");
      SetConsoleTextAttribute(consoleOutput, 142);
      printf("%d",DEFENDER);
      SetConsoleTextAttribute(consoleOutput, 143);
      printf("?");
      c = 0;
while(1 )
   {
      for(v=0;v<2;v++)
      {
          GotoXY(X+4*(v+1),Y+5);
          SetConsoleTextAttribute(consoleOutput, 143); 
          if(v == k)SetConsoleTextAttribute(consoleOutput, 80); 
          printf("%s",line[v]);
      }
      if((c=getch())== KEY_SPEC)//код управляющего символа первый символ
        {
           c = getch();
           if(c==KEY_LEFT || c== KEY_RIGHT || c == KEY_UP || c == KEY_DOWN)
           {
               if(c== KEY_UP|| c ==KEY_RIGHT)
                   if(k==0) k= 1;
                   else k--;
               if(c== KEY_DOWN || c == KEY_LEFT) 
                  if(k== 1)k=0;
                  else k++;
           }
        }
      if(c == '\r'|| c == '\n')
      {
          if(k == 0) 
          {
               people.points--;
               loc[i][b].def = 1;
               people.coins -= DEFENDER;
               PrintMap(b,i);
               write_def(i,b);
               write_coins(DEFENDER);
          }
          break;
      }
      if(c== KEY_ESC) break;
      if(c == ' ') 
        if(NextTurnMenu() == NEXT_TURN) return NEXT_TURN;
  }
  return 0;
   /* // clear/////////////////////
            for(f=0;f<2;f++)
               for(v=0;v<3;v++)
                  PrintMap((X/B_KUB)+v,(Y/I_KUB)+f);*/
}
//disband//////////////////////////////////////////////////////////////////////////
int disband(int i,int b)
{
      int X,Y,z,v,f,c;
     int army,cof=1;
    if(i<= MAX_I/2) Y = (i + 1) * I_KUB;
    else Y = (i-2)*I_KUB;
    if(b <= MAX_B/2) X = (b + 1) * B_KUB;
    else X = (b - 3)*B_KUB;
    // печать окна
    GotoXY(X,Y);
    SetConsoleTextAttribute(consoleOutput, 143);
    for(z=0;z<(2*I_KUB);z++)
    {
       for(v=0;v< 3*B_KUB;v++)
       printf(" ");
       GotoXY(X,Y+z+1);
    }
     GotoXY(X+2,Y+3);
     printf("Alt +100 | Ctrl +10");
     army = 0;
     GotoXY(X+3,Y+1);
     printf("Disband");
     GotoXY(X+5,Y+8);
     GotoXY(X+5,Y+6); printf("army:");
     int narmy;
     while(1)
     { 
          narmy = loc[i][b].army - army;
          GotoXY(X+2,Y+4);
          SetConsoleTextAttribute(consoleOutput, 17);
          for(z=0;z< (float)narmy/loc[i][b].army * 20;z++)
             printf(" ");
          SetConsoleTextAttribute(consoleOutput, 119);
          for(;z<B_KUB*3 - 4;z++) printf(" ");
          SetConsoleTextAttribute(consoleOutput, 129);
          GotoXY(X+5+6,Y+6); 
          printf("      ");
          GotoXY(X+5+6,Y+6); 
          printf("%d",army);
          if((c = getch()) == KEY_SPEC || c == ALT_SPEC)
          {
                 cof = 1;
                 if(c==ALT_SPEC) cof = 100;
                 c = getch();
           if(c== KEY_UP || c== KEY_DOWN || c == KEY_LEFT || c == KEY_RIGHT||
              c == Ctrl_KEY_UP|| c == Ctrl_KEY_DOWN|| c == Ctrl_KEY_LEFT|| c == Ctrl_KEY_RIGHT||
              c == Alt_KEY_UP|| c == Alt_KEY_DOWN|| c == Alt_KEY_LEFT|| c == Alt_KEY_RIGHT)
           //всевозможные комбинации стрелок и Ctrl и Alt
           {
               if( c == Ctrl_KEY_UP|| c == Ctrl_KEY_DOWN|| c == Ctrl_KEY_LEFT|| c == Ctrl_KEY_RIGHT) cof = 10;
               if(c== KEY_UP|| c ==KEY_RIGHT|| c == Ctrl_KEY_UP|| c == Ctrl_KEY_RIGHT|| c == Alt_KEY_UP|| c == Alt_KEY_RIGHT)
                   if(army-cof>=0) 
                   {
                       army-=cof; 
                   }
               if(c==  KEY_DOWN || c ==  KEY_LEFT|| c == Ctrl_KEY_DOWN||c==Ctrl_KEY_LEFT||c==Alt_KEY_DOWN||c==Alt_KEY_LEFT)  
                  if(army+cof <= loc[i][b].army)
                  {
                       
                       army+=cof;
                  }
           }
         }
        if(c == '\r'|| c == '\n')
         {
             write_disband(i,b,loc[i][b].army - army);
             loc[i][b].army = army;
             PrintMap(b,i);
             return 0;
         }
         if(c == ' ')
         {
              if(NextTurnMenu() == NEXT_TURN) return NEXT_TURN;
         }
         if(c== KEY_ESC) return 0;
        
     }
}
//#define MAX_I 10
//#define MAX_B 18
int countpoints()// подсчет поинтов
{
    int i,b;
    people.maps = 0;
    orks.maps = 0;
    for(i=0;i<MAX_I;i++)
             for(b=0;b<MAX_B;b++)
             {
                 if(loc[i][b].country == 1) people.maps++;
                 if(loc[i][b].country == 2) orks.maps++;
             }
             
             
             if(people.maps > 20) people.points = 10;
             else people.points = 2 + int(people.maps/2.5); 
             if(people.maps > 35) people.points = 13;
             if(people.maps > 60) people.points = 16;
             
             if(orks.maps > 20) orks.points = 10;
             else orks.points = 2 + int(orks.maps/2.5);
             if(orks.maps > 35) orks.points = 13;
             if(orks.maps > 60) orks.points = 16;
			 
			 if(orks.maps== 0)
             {
                  peoplewinfunc();
                  return 1;
             }
			 if(people.maps== 0)
             {
                  opkwinfunc();
                  return 1;
             }
            return 0; 
}
int countmaps()//подсчитывает кол-во войск и территорий
{
    int i, b,pn = 0,on=0;
    float pepcof = PEPCOF,orkcof= ORKCOF;
    //people.maps = 0;
   // orks.maps = 0;
    people.army = 0;
    orks.army = 0;
    for(i=0;i<MAX_I;i++)
             for(b=0;b<MAX_B;b++)
             {
                if(loc[i][b].army > 1000000) loc[i][b].army = 1000000;
                if(loc[i][b].country == 1)
                { 
               // people.maps++;
                if(people.coins >= 1000000) continue;
                pn ++;
                if(pn > 5) pepcof = PEPCOF_5;
                if(pn > 10) pepcof = PEPCOF_10;
                if(pn > 15) pepcof = PEPCOF_15;
                people.army+=loc[i][b].army;
                if(loc[i][b].army <= MaxArmy)
                     people.coins+=int((float) MaxCoins*((float)(MaxArmy-loc[i][b].army)/MaxArmy)/pepcof);
                }
                if(loc[i][b].country == 2) 
                {
                if(orks.coins >= 1000000) continue;
                orks.maps++;
                on++;
                if(on > 5) orkcof = ORKCOF_5;
                if(on > 10) orkcof = ORKCOF_10;
                if(on > 15) orkcof = ORKCOF_15;
                orks.army+=loc[i][b].army;
                if(loc[i][b].army <= MaxArmy)
                    orks.coins+=int((float) MaxCoins*((float)(MaxArmy-loc[i][b].army)/MaxArmy)/orkcof);
                }
             }
             
}
////////////WAR COUNT//////////WAR COUNT/////////////////////////////
void WarCount()
{
     int i,b,attarmy,country=0,buff;
     float cof;
     for(i=0;i<MAX_I;i++)
       for(b=0;b<MAX_B;b++)
       {
            country=0;
            cof = 1.0 - DEFCOF*loc[i][b].def;
                attarmy = loc[i][b].buforks - loc[i][b].bufpeople;
                if(attarmy>0) 
                {
                    country = 2;
                }
                if(attarmy < 0)
                {
                     attarmy= -1 * attarmy;
                     country = 1;
                }
     
          if(country == loc[i][b].country) loc[i][b].army += attarmy;
          else 
          {
             loc[i][b].army -= (int)(attarmy*cof);
          }
          if(loc[i][b].army <0) 
          {
              loc[i][b].country = country;
              loc[i][b].army *= -1;
              loc[i][b].def = 0;
          }
          loc[i][b].buforks = 0;
          loc[i][b].bufpeople = 0;
       }
}
//////////////////////////////////////////////////////////////////////
int main()
{
   FILE *stat;
   FILE *fp;
   FILE *fstop;
   ///
   if(!(stat = fopen("stat.txt","w"))) 
   {
             printf("system test error!");
             system("pause");
             return 1;
   }
   
   if(!(fp = fopen("settings/test","a"))) system("md settings");
        else 
        {
             fclose(fp);
             remove("settings/test");
        }
   //// проверка на создание папки^
   int i,b;
   int a,state = 0;// переменная отвечающая за закрытие меню
while(state == 0)
{
 // remove(kl_stop_name);
 // remove(sr_go);
  menu:
   HandleSettle(); // устанавливаем handle
   a=Menu();
   state = 1;
   switch (a)
   {
          case 0:
          case 1:
                 //state = open_game();
                // state = new_game();
                 break;
          case 2: 
                 fclose(stat);
                 return 0;
          default: printf("FATAL ERROR!");
                   return 1;
   }
}
/////Прорисовка меню игры(слева)//////////////////////////////////////
 GotoXY(LeftEMX+2,2);
//int rus[15] = {157,256,174,32,145,146,144,128,146,133,134,138,128,32,0};
/////////попытка по-читерски вывести надпись на экран
SetConsoleTextAttribute(consoleOutput, 15); printf("%c%c%c%c%c%c%c%c%c!",145,146,144,128,146,133,134,138,128); //печать это стратежка
GotoXY(NextTURNmenuX,NextTURNmenuY);
SetConsoleTextAttribute(consoleOutput, 15); printf("[Space] Next Turn");
GotoXY(NextTURNmenuX,NextTURNmenuY + 6);
     SetConsoleTextAttribute(consoleOutput, 15); 
     printf("%s",name);
///////////////////////ИГРА//////////////////////////////////////////  
            int k;
            while(1)
                  {for(int as = 0;(fstop = fopen(kl_stop_name,"r"))==0;as++)
                  {
                       GotoXY(MAX_B*B_KUB,MAX_I*I_KUB);
                       printf("%d",as);
                       fclose(fstop);
                       Sleep(500);
                       
                  } printf("(1)"); GotoXY(MAX_B*B_KUB,MAX_I*I_KUB); printf("        ");
                    fclose(fstop);
                    for(int kjl=0;remove(kl_stop_name)!=0 && kjl<30;kjl++);
                    while((fwr = fopen(kl_name,"r")) == 0)fclose(fwr); printf("(2)");
                    fclose(fwr);
                    fwr = fopen(kl_name,"w");
                    for(int bk = 0;remove(sr_go)!=0 && bk<10;bk++);
                    if(_SIDE == 2)
                    {
                          fprintf(fwr,"o ");
                    }
                    else fprintf(fwr,"p ");
                    while((state = open_game())==LASTMAP)Sleep(500);
                    
                  
/////////////////////////////////////Прорисовка//////////////
                 for(i=0;i<MAX_I;i++)
                 {
                      for(b=0;b<MAX_B;b++)
                      {
                         loc[i][b].lastarmy = loc[i][b].army;
                         loc[i][b].movecheck = 0;
                     //////
                         PrintMap(b, i);
                      }
                 } 
/////////////////////////НА СЕЕЕРВ!/////////////////////////////////////
                 /*     fprintf(stat,"countpoints1");
                      if(countpoints()== 1) goto menu;//считаем поинты
                      fprintf(stat,"countpoints2");*/
                      /*SetConsoleTextAttribute(consoleOutput, 15);
                      GotoXY(NextTURNmenuX,NextTURNmenuY+20);
                       printf("orkccoins %d   ",orks.coins);*/
/////////////////////////////////////////////
                      while(1)
                      {
                          left_edge_menu();//рисуем менюху справа
                          int zx; // принимает текущую координату с zx по формуле zx = i*100 + b;
                          if((zx = gotomap())== NEXT_TURN) break;
                          if(zx == EXIT)
                          {
                               if(ExitMenu() ==  EXIT) goto menu;
                               continue;
                          }
                          i = zx/100;
                          b = zx%100;
                          if((k = game_menuIB(i,b))== NEXT_TURN) break;
//                          GotoXY(0,MAX_I * I_KUB);
                      } // убираем курсор с карты
 /*  //Для одиночного режима//В СЕРВАААК!/////
                      fprintf(stat," EnemyMainFunc()1\n");
                      EnemyMainFunc();
                      fprintf(stat," EnemyMainFunc()2\n");
                      fprintf(stat," WarCount()1\n");
                      WarCount();
                      fprintf(stat," WarCount()2\n");
                      fprintf(stat,"  countmaps()1\n");
                      countmaps();
                      fprintf(stat,"  countmaps()2\n");
 */
/////////////////////Запись в ФАЙЛ/////////////////////////////////////
//  В СЕРВАААААК!
                     /* fprintf(stat,"  filein1\n");
                      file_in();
                      fprintf(stat,"  filein1\n");
                      fprintf(stat,"  \n\nNEXT\n\n");*/
/////////////////////////////////////////////////////////////
    fclose(fwr);
    while((fstop = fopen(sr_go,"w"))==0);
    fclose(fstop);
    
                  }
}

void peoplewinfunc(void)
{
	system("cls");
	consoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD crdwin = {30,20};
    SMALL_RECT src = {0, 0, crdwin.X-1, crdwin.Y-1};
    SetConsoleWindowInfo (consoleOutput, true, &src);
    SetConsoleScreenBufferSize(consoleOutput, crdwin);
    
    SetConsoleTextAttribute(consoleOutput, 31);
    printf("You WIN!!!"); 
    remove(name);
	getch(); 
}
void opkwinfunc(void)
{
	system("cls");
	consoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD crdwin = {30,20};
    SMALL_RECT src = {0, 0, crdwin.X-1, crdwin.Y-1};
    SetConsoleWindowInfo (consoleOutput, true, &src);
    SetConsoleScreenBufferSize(consoleOutput, crdwin);
    
    SetConsoleTextAttribute(consoleOutput, 31);
    printf("You Lose :(( Try again");
    remove(name);
	getch();
}
int new_game()//задаем стартовые параметры
{
    int i=9,b,c;
    char cn[MAXNAME]="settings/";
    system("cls");
    SetConsoleTextAttribute(consoleOutput, 15);
    printf("Enter name of the game : ");
    while(1)
    {
        c = getch();
        if(c == KEY_SPEC || c == ALT_SPEC)
        {
             getch();
             continue;
        }
        if(c >=33 && c<= 126 && i < MAXNAME-1)
        {
             printf("%c",c);
             name[i++]=c;
             name[i] = '\0';
        }
        if(c == '\b' && i>9)
        {
             i--;
             name[i] = '\0';
             printf("\b \b");
        }
        if(c == KEY_ESC) return 0;
        if((c == '\n' || c == '\r') && i > 9)
        {
          name[i] = '\0';
          break;
        }
        
     }
     /*FILE *ff;
     ff = fopen(name,"w");
     fprintf(ff,"%s",name);
     fclose(ff);*/
     system("cls");
    SetConsoleTextAttribute(consoleOutput, 15);
    printf("Choose size of game:\n");
    const int length = 4;
    const char *line[length];
    line[0] = "7 x 5";
    line[1] = "10 x 8";
    line[2] = "13 x 10";
    line[3] = "16 x 10";
     SetConsoleTextAttribute(consoleOutput, 127);
     printf("%s\n",line[0]);
 SetConsoleTextAttribute(consoleOutput, 15); 
 for(b=1;b<length;b++)
   printf("%s\n",line[b]);
 i=0; 
while((c=getch()) != '\r' )
{
 if(c== KEY_SPEC)//код управляющего символа первый символ
  {

        c = getch();
        if(c==KEY_UP || c== KEY_DOWN)
        { 
            if(c== KEY_UP)
                if(i==0)i=length-1;
                else i--;
            if(c== KEY_DOWN) 
                if(i==length-1)i=0;
                else i++;
            system ("cls");
             SetConsoleTextAttribute(consoleOutput, 15);
             printf("Choose size of game:\n");
            for(b=0;b<length;b++)
            {
               if(b==i)
               {
                   SetConsoleTextAttribute(consoleOutput, 127);
                   printf("%s\n",line[b]);
                   SetConsoleTextAttribute(consoleOutput, 15);
               }
               else  printf("%s\n",line[b]);
            }
         }    
    }
}
    system ("cls");
    set_get_size_game(i+1,SET);
     //SetConsoleTextAttribute(consoleOutput, 31);
     //printf("chose game size:");
    // getch();
                           for(i=0;i<MAX_I;i++)
                           {
                               for(b=0;b<MAX_B;b++)
                                {
                                 loc[i][b].army = 1;
                                 loc[i][b].country = 0;
                                 loc[i][b].def = 0;  
                                 loc[i][b].buforks = 0;
                                 loc[i][b].bufpeople = 0;   
                                 loc[i][b].need_def_help=0;
                                 loc[i][b].free_army = 0;   
                                }
                           }
                            people.coins = 1000;
                            orks.coins = 1000; 
/////////////////////////////установки для локаций//////////////
    int metX = MAX_B/2 -1,metY= MAX_I/2;
        loc[0][0].army= 250;// кладем войска
        loc[0][0].country = 1;//кладем принадлежность к армии
        loc[MAX_I-1][MAX_B-1].country = 2;
        loc[MAX_I-1][MAX_B-1].army = 250;
        loc[metY][metX].army = 250;
        loc[metY][metX].country = 1;
        loc[metY][MAX_B-metX-1].army = 250;
        loc[metY][MAX_B-metX-1].country = 2;
        
///////////////////установки для стран/////////////////////////
    return 1;
}
int set_get_size_game(int ind,int param)
{
 switch (param)
 {
  case SET:
    switch (ind)
    {
        case SZ_1:
                MAX_B = 7;
                MAX_I = 5;
                return SZ_1;
             
        case SZ_2:
                MAX_B = 10;
                MAX_I = 8;
                return SZ_2;
        case SZ_3:
                MAX_B = 13;
                MAX_I = 10;
                return SZ_3;
        case SZ_4:
                MAX_B = 16;
                MAX_I = 10;
                return SZ_4;
        default: return -1;
    }
   case GET:
        if( MAX_B == 7 && MAX_I == 5)return SZ_1;
        if( MAX_B == 10 && MAX_I == 8)return SZ_2;
        if( MAX_B == 13 && MAX_I == 10)return SZ_3;
        if( MAX_B == 16 && MAX_I == 10)return SZ_4;
        return -1;
   default: 
           return -1;
  }
}
int same_menu()
{
    SetConsoleTextAttribute(consoleOutput, 15);
    system("cls");
    printf("this game wasn't find :'(");
    getch();
    return 0;
}
int open_game()
{
    FILE *ff;
    int i,b;
    /*
    int i=9,b,c,ix,iz,same = 0;
    FILE *ff;
    system("cls");
    SetConsoleTextAttribute(consoleOutput, 15);
    printf("Enter name of the saved game : ");
     for(ix = i - 9,iz = ix;iz<= 21;iz++)
        {
             printf(" ");
        }
         SetConsoleTextAttribute(consoleOutput, 204);
         printf(" ");
          SetConsoleTextAttribute(consoleOutput, 15);
        for(;iz>=ix;iz--) printf("\b");
    while(1)
    {
        c = getch();
        if(c == KEY_SPEC || c == ALT_SPEC)
        {
             getch();
             continue;
        }
        if(c >=33 && c<= 126 && i < MAXNAME-1)
        {
             printf("%c",c);
             name[i++]=c;
             name[i] = '\0';
        }
        if(c == '\b' && i>9)
        {
             i--;
             name[i] = '\0';
             printf("\b \b");
        }
        if(c == KEY_ESC) return 0;
        if((c == '\n' || c == '\r') && i > 9)
        {
          if(same == 0) return same_menu();
          break;
        }
        for(ix = i - 9,iz = ix;iz<= 21;iz++)
        {
             printf(" ");
        }
         SetConsoleTextAttribute(consoleOutput, 170);
         same = 1;
         if(!(ff = fopen(name,"r")))
         {
             SetConsoleTextAttribute(consoleOutput, 204);
             same = 0;
         }
         fclose(ff);
         printf(" ");
          SetConsoleTextAttribute(consoleOutput, 15);
        for(;iz>=ix;iz--) printf("\b");
     }*/
     int game_size_index= -1;
     int mapindex;
     FILE *fp;
      int filecheck=0;
                  if((fp = fopen(name,"r+"))== NULL)
                    {
                            //fp = fopen("settings/config.cfg","a"); fclose(fp);
                            filecheck=1;
                            printf("SERVER ERROR");
                            system("pause");
                            exit(1);
                    }
                    else
                    {
                    // считать из файла
                   // show_game_settings(fp);
                    char c;
                    while((c=fgetc(fp))!=EOF)
                    {
                        if(!((c>='0' && c <='9') || c == ' '))
                        {
                                   hide_game_settings(fp);
                                   fclose(fp);
                                   printf("VSEPLOHO1");
                                   break;
                        }
                    }
                    fseek(fp,0,SEEK_SET);
                    if(filecheck != 1)
                    {
                       if(!feof(fp))
                       {
                           fscanf(fp,"%d",&mapindex);
                           if(mapindex == mapcheck) { fclose(fp); return LASTMAP;}
                           else mapcheck = (mapcheck+1)%2;
                       
                       }
                       if(!feof(fp))
                       {
                            fscanf(fp,"%d",&game_size_index);
                            if(set_get_size_game(game_size_index,SET)== -1) { printf("VSEPLOHO2"); filecheck = 1;}
                       }
                       if(_SIDE == 1)
                       {
                          if(!feof(fp))
                              fscanf(fp,"%d",&people.coins); //деньги людей
                          if(!feof(fp))
                            fscanf(fp,"%d",&orks.coins);//деньги зверья
                       }
                       else
                       {
                            if(!feof(fp))
                               fscanf(fp,"%d",&orks.coins);//деньги зверья
                            if(!feof(fp))
                               fscanf(fp,"%d",&people.coins); //деньги людей
                       }
                       if(_SIDE == 1)
                       {
                          if(!feof(fp))
                              fscanf(fp,"%d",&people.points); //деньги людей
                          if(!feof(fp))
                            fscanf(fp,"%d",&orks.points);//деньги зверья
                       }
                       else
                       {
                            if(!feof(fp))
                               fscanf(fp,"%d",&orks.points);//поинты зверья
                            if(!feof(fp))
                               fscanf(fp,"%d",&people.points); //поинты людей
                       }
                       if(filecheck != 1)
                       {
                       for(i=0;i<MAX_I;i++)
                        {
                             for(b=0;b<MAX_B;b++)
                             {         if(feof(fp)) filecheck = 1;
                                       else fscanf(fp,"%d",&loc[i][b].army);
                                          
                                       if(feof(fp))filecheck = 1;
                                       else fscanf(fp,"%d",&loc[i][b].def);
                                       
                                        if(feof(fp))filecheck = 1;
                                        else 
                                        {
                                           fscanf(fp,"%d",&loc[i][b].country);
                                           if(_SIDE == 2)
                                           {
                                              if(loc[i][b].country == 1) loc[i][b].country = 2;
                                              else if(loc[i][b].country == 2) loc[i][b].country = 1;
                                           }
                                        }
                                       
                                       
                             }
                             if(filecheck == 1) break;
                          }
                         }
                        // hide_game_settings(fp);
                         fclose(fp);
                      }
                    }
                    if(filecheck == 1)
                    {
                                 // return new_game();
                                 printf("GAME DATA ERROR"); system("pause"); exit(1);
                    }
}
void file_in()
{
     FILE *fp;
     int i,b;
     
     if((fp = fopen(name,"w+"))!= NULL)
                      {
                             fprintf(fp,"%d ",set_get_size_game(0,GET));
                             fprintf(fp,"%d ",people.coins);
                             fprintf(fp,"%d ",orks.coins);
                             
                             fprintf(fp,"%d ",people.points);
                             fprintf(fp,"%d ",orks.points);
                             
                             for(i=0;i<MAX_I;i++)
                                 for(b=0;b<MAX_B;b++)
                                 {
                                     fprintf(fp,"%d ", loc[i][b].army);
                                     fprintf(fp,"%d ", loc[i][b].def);
                                     fprintf(fp,"%d ", loc[i][b].country);
                                 }
                             fclose(fp);
                             if((fp = fopen(name,"r+"))!= NULL)
                             {
                                   hide_game_settings(fp);
                                   fclose(fp);
                             }
                      }
}
//////////////////Enemy/////////////////////////////////////
//////////
void EnemyMainFunc(void)
{
   int k=0;
   while(1)
   {
       EnemyMapsState();
       if(1 == EnemyFreeResourceCalculate()) break;
       if(k++ > 200) break;
   }
	
}
void EnemyMapsState(void)
{
	int i,b;
	for(i=0;i<MAX_I;i++)
	   for(b=0;b<MAX_B;b++)
	   {
           if(loc[i][b].country != 2) continue;
           //////////////////////////////////
          // loc[i][b].need_attack_help = 0;
           loc[i][b].need_def_help = 0;
           loc[i][b].free_army = 0;
           loc[i][b].fratt = -1;
           loc[i][b].batt = -1;
           
           loc[i][b].bngb.left = -1;
           loc[i][b].bngb.right = -1;
           loc[i][b].bngb.top = -1;
           loc[i][b].bngb.under = -1;
           
           loc[i][b].sngb.left = -1;
           loc[i][b].sngb.right = -1;
           loc[i][b].sngb.top = -1;
           loc[i][b].sngb.under = -1;
           /////////////////////////////////////
           if(loc[i][b].bufpeople!= 0 && loc[i][b].bufpeople+10 > loc[i][b].army) 
                  loc[i][b].need_def_help = loc[i][b].bufpeople +10 - loc[i][b].army;//если нужна помошь на деф
           else
           {
               ////////запросы на помщь в атаке////////////
               int army = loc[i][b].army - loc[i][b].bufpeople - 1;
               if(b-1 >= 0 && loc[i][b-1].country != 2 && loc[i][b-1].buforks == 0)
                  if(army <= loc[i][b-1].army+ int(loc[i][b-1].army * (float(loc[i][b-1].def) * DEFCOF)))
                  {
                     loc[i][b].batt = 1;
                     loc[i][b].bngb.left = loc[i][b-1].army + int(loc[i][b-1].army*(float(loc[i][b-1].def)*DEFCOF)) - army;
                  }
                  else 
                    {
                        loc[i][b].sngb.left = army - (loc[i][b-1].army + int(loc[i][b-1].army*(float(loc[i][b-1].def)*DEFCOF)));
                        loc[i][b].fratt = 1;
                    }
                  
               if(b+1 < MAX_B && loc[i][b+1].country != 2 && loc[i][b+1].buforks == 0)
                  if(army <= loc[i][b+1].army+int(loc[i][b+1].army*(float(loc[i][b+1].def)*DEFCOF)))
                  {
                     loc[i][b].batt = 1;
                     loc[i][b].bngb.right = loc[i][b+1].army+int(loc[i][b+1].army*(float(loc[i][b+1].def)*DEFCOF)) - army;
                  }
                  else 
                  {
                      loc[i][b].sngb.right = army - (loc[i][b+1].army + int(loc[i][b+1].army*(float(loc[i][b+1].def)*DEFCOF)));
                      loc[i][b].fratt = 1;
                  }
                  
               if(i-1 >= 0 && loc[i-1][b].country != 2 && loc[i-1][b].buforks == 0)
                  if(army <= loc[i-1][b].army+ int(loc[i-1][b].army*(float(loc[i-1][b].def)*DEFCOF)))
                  {
                     loc[i][b].batt = 1;
                     loc[i][b].bngb.top = loc[i-1][b].army + int(loc[i-1][b].army*(float(loc[i-1][b].def)*DEFCOF)) -army;
                  }
                  else 
                  {
                      loc[i][b].sngb.top = army - (loc[i-1][b].army+int(loc[i-1][b].army*(float(loc[i-1][b].def)*DEFCOF)));
                      loc[i][b].fratt = 1;
                  }
                  
               if(i+1 < MAX_I && loc[i+1][b].country != 2 && loc[i+1][b].buforks == 0)
                  if(army <= loc[i+1][b].army + int(loc[i+1][b].army*(float(loc[i+1][b].def)*DEFCOF)))
                  {
                     loc[i][b].bngb.under = loc[i+1][b].army +int(loc[i+1][b].army*(float(loc[i+1][b].def)*DEFCOF))- army;
                     loc[i][b].batt = 1;
                  }
                  else 
                  {
                        loc[i][b].sngb.under = army -(loc[i+1][b].army+ int(loc[i+1][b].army*(float(loc[i+1][b].def)*DEFCOF)));
                        loc[i][b].fratt = 1;
                  }
               ////////////////////......
           }
           
           if( loc[i][b].bngb.left == -1 &&
               loc[i][b].bngb.right == -1 &&
               loc[i][b].bngb.top == -1 &&
               loc[i][b].bngb.under == -1 &&
           
               loc[i][b].sngb.left == -1 &&
               loc[i][b].sngb.right == -1 &&
               loc[i][b].sngb.top == -1 &&
               loc[i][b].sngb.under == -1) loc[i][b].free_army = loc[i][b].army;
               
       }
       
      return; /// расставили метки 
	    
}

int EnemyFreeResourceCalculate()
{
     int i,b,k1=0,k2=0,k3=0;
     int freeAttack=0,n_help=0,nfAttack=0;
     for(i=0;i<MAX_I;i++)
	   for(b=0;b<MAX_B;b++)
	   {
          if(loc[i][b].country == 2)
          {
            if(loc[i][b].fratt == 1) freeAttack++;
            if(loc[i][b].need_def_help != 0) n_help++;
            if(loc[i][b].batt ==  1) nfAttack ++;
          }
       }
       //
       /* GotoXY(NextTURNmenuX,NextTURNmenuY+ 7);
       printf("Free %d",freeAttack);
       GotoXY(NextTURNmenuX,NextTURNmenuY+ 8);
        printf("n_help %d",n_help);
        GotoXY(NextTURNmenuX,NextTURNmenuY+ 9);
       printf("points %d",orks.points);*/
       //
       if(orks.points == 0) return 1;
        k1 = EnemyRandomDefFunc(n_help); 
        k2 = EnemyRandomFreeAttack(freeAttack);
        k3 = EnemyRandomAttack(nfAttack);
         
       if(k1 == 1 && k2 == 1 && k3 == 1) return 1;
       return 0;
}
int EnemyRandomDefFunc(int nhelp)
{
    if(nhelp == 0) return 1;
    int i,b,rnd,n=0,z=0,k;
    srand(time(NULL));
    rnd = rand() % nhelp +1;
    while(z <= 4)
    {
    for(i=0;i<MAX_I;i++)
      for(b = 0;b<MAX_B;b++)
      {
            if(loc[i][b].country != 2) continue;
            if(loc[i][b].need_def_help > 0) n++;
            if(n == rnd) 
             if(EnemyDefFunc(i,b)== 0 ) return 0;//защитились
             else n--;
      }
      z++;
    } return 1;
    
}
int EnemyDefFunc(int i,int b)
{
     int k;
    for(k=2;k>=1;k--)
	   {
            if(orks.points == 0) return 1;
            if(loc[i][b].country != 2 || loc[i][b].need_def_help == 0 ) continue;
            // можно помочь 1 перемещением
            if(b-1 >= 0 && loc[i][b-1].country == 2 && loc[i][b-1].free_army > loc[i][b].need_def_help && loc[i][b].need_def_help != 0)
            {
                   loc[i][b].army+= loc[i][b].need_def_help+1 +(loc[i][b-1].army - 1- loc[i][b].need_def_help)/2;
                   loc[i][b-1].army -= loc[i][b].need_def_help+1 +(loc[i][b-1].army - 1- loc[i][b].need_def_help)/2;
                   loc[i][b].need_def_help = 0;
                   loc[i][b-1].free_army -= loc[i][b].need_def_help+1 +(loc[i][b-1].army - 1- loc[i][b].need_def_help)/2;
                   orks.points--;
                   continue;
            }
            if(b+1 < MAX_B && loc[i][b+1].country == 2 && loc[i][b+1].free_army > loc[i][b].need_def_help && loc[i][b].need_def_help != 0)
            {
                   loc[i][b].army += loc[i][b].need_def_help+1 +(loc[i][b+1].army - 1- loc[i][b].need_def_help)/2;
                   loc[i][b+1].army -= loc[i][b].need_def_help+1 +(loc[i][b+1].army - 1- loc[i][b].need_def_help)/2;
                   loc[i][b].need_def_help = 0;
                   loc[i][b+1].free_army -= loc[i][b].need_def_help+1 +(loc[i][b+1].army - 1- loc[i][b].need_def_help)/2;
                   orks.points--;
                   continue;
            }
            if(i-1 >= 0 && loc[i-1][b].country == 2 && loc[i-1][b].free_army > loc[i][b].need_def_help && loc[i][b].need_def_help != 0)
            {
                   loc[i][b].army += loc[i][b].need_def_help+1 +(loc[i-1][b].army - 1- loc[i][b].need_def_help)/2;
                   loc[i-1][b].army -= loc[i][b].need_def_help+1 +(loc[i-1][b].army - 1- loc[i][b].need_def_help)/2;
                   loc[i][b].need_def_help = 0;
                   loc[i-1][b].free_army -= loc[i][b].need_def_help+1 +(loc[i-1][b].army - 1- loc[i][b].need_def_help)/2;
                   orks.points--;
                   continue;
            }
            if(i+1 < MAX_I && loc[i+1][b].country == 2 && loc[i+1][b].free_army > loc[i][b].need_def_help && loc[i][b].need_def_help != 0)
            {
                   loc[i][b].army += loc[i][b].need_def_help+1 +(loc[i+1][b].army - 1- loc[i][b].need_def_help)/2;
                   loc[i+1][b].army -= loc[i][b].need_def_help+1 +(loc[i+1][b].army - 1- loc[i][b].need_def_help)/2;
                   loc[i][b].need_def_help = 0;
                   loc[i+1][b].free_army -= loc[i][b].need_def_help+1 +(loc[i+1][b].army - 1- loc[i][b].need_def_help)/2;
                   orks.points--;
                   continue;
            }
            /////////
            if(loc[i][b].need_def_help != 0 && loc[i][b].need_def_help +1 <= orks.coins/k)
            {
                srand(time(NULL));
                int m;
                m = rand()%((orks.coins -loc[i][b].need_def_help -1)/3);
                loc[i][b].army += loc[i][b].need_def_help +1 + m ;
                orks.coins -= loc[i][b].need_def_help +1 + m ;
                loc[i][b].need_def_help = 0;
                orks.points --;
                continue;  
            }
             
       }
       if(loc[i][b].need_def_help > 0) return 0;
       else return -1;
}
void attack_free(int i,int b)
{
     if(loc[i][b].country != 2) return;
      int m,rnd,z =0,n=0;
            srand(time(NULL));
            while(z == 0)
            {
               n++;
               if(n >= 1000) break;
               m = rand()% 4 + 1;
               switch (m)
               {
                   case 1:
                         if(loc[i][b].sngb.top != -1)
                         {
                                if(loc[i-1][b].army < loc[i-1][b].buforks) break;
                                z =1;
                                orks.points --;
                                rnd = rand() % (loc[i][b].army-loc[i-1][b].army-1);
                                loc[i][b].army -= loc[i-1][b].army+1+ rnd;
                                loc[i-1][b].buforks += loc[i-1][b].army+1+ rnd;
                                loc[i][b].sngb.top = -1;
                         }
                         break;
                         
                   case 2:
                         if(loc[i][b].sngb.right != -1)
                         {
                                if(loc[i][b+1].army < loc[i][b+1].buforks) break;
                                z =1;
                                orks.points --;
                                rnd = rand() % (loc[i][b].army-loc[i][b+1].army-1);
                                loc[i][b].army -= loc[i][b+1].army+1+ rnd;
                                loc[i][b+1].buforks += loc[i][b+1].army+1+ rnd;
                                loc[i][b].sngb.right = -1;
                         }
                         break;
                   case 3:
                         if(loc[i][b].sngb.under != -1)
                         {
                                if(loc[i+1][b].army < loc[i+1][b].buforks) break;
                                z =1;
                                orks.points --;
                                rnd = rand() % (loc[i][b].army-loc[i+1][b].army-1);
                                loc[i][b].army -= loc[i+1][b].army+1+ rnd;
                                loc[i+1][b].buforks += loc[i+1][b].army+1+ rnd;
                                loc[i][b].sngb.under = -1;
                         }
                         break;
                   case 4:
                         if(loc[i][b].sngb.left != -1)
                         {
                                if(loc[i][b-1].army < loc[i][b-1].buforks) break;
                                z =1;
                                orks.points --;
                                rnd = rand() % (loc[i][b].army-loc[i][b-1].army-1);
                                loc[i][b].army -= loc[i][b-1].army+1+ rnd;
                                loc[i][b-1].buforks += loc[i][b-1].army+1+ rnd;
                                loc[i][b].sngb.left = -1;
                         }
                         break;
                   default: ;
                }
          }
}
int buy_army_attack(int i,int b)
{
     if(loc[i][b].country != 2) return 1;
      int m,z =0,n=0,rnd;
            srand(time(NULL));
            while(z != 1111)
            {
              
               n++;
               if(n >= 1000) break;
               m = rand()% 4 + 1;
               switch (m)
               {
                      case 1:
                              if(loc[i][b].bngb.top != -1 && z%10 != 1)
                              {
                                 if(orks.coins > loc[i][b].bngb.top )
                                 {
                                    z += 1;
                                    orks.points --;
                                    srand(time(NULL));
                                    rnd = rand() % (orks.coins - loc[i][b].bngb.top) + 1;
                                    orks.coins -= loc[i][b].bngb.top+rnd;
                                    loc[i][b].army += rnd + loc[i][b].bngb.top;
                                    loc[i][b].bngb.top = -1;
                                    return 0;  
                                 }
                              }
                              break;
                      case 2:
                              if(loc[i][b].bngb.under != -1)
                              {
                              if(orks.coins > loc[i][b].bngb.under && z%100 < 10)
                                 {
                                    z += 10;
                                    orks.points --;
                                    srand(time(NULL));
                                    rnd = rand() % (orks.coins - loc[i][b].bngb.under) + 1;
                                    orks.coins -= loc[i][b].bngb.under+rnd;
                                    loc[i][b].army += rnd + loc[i][b].bngb.under;
                                    loc[i][b].bngb.under = -1;
                                    return 0;  
                                    
                                 }
                              }
                              break;
                      case 3:
                              if(loc[i][b].bngb.left != -1 && z%1000 < 100 && orks.coins > loc[i][b].bngb.left)
                              {
                                  
                                    z += 100;
                                    orks.points --;
                                    srand(time(NULL));
                                    rnd = rand() % (orks.coins - loc[i][b].bngb.left) + 1;
                                    orks.coins -= loc[i][b].bngb.left+rnd;
                                    loc[i][b].army += rnd + loc[i][b].bngb.left;
                                    loc[i][b].bngb.left = -1;
                                    return 0;  
                              }
                              break;
                      case 4:
                              if(loc[i][b].bngb.right != -1 && z < 1000 && orks.coins > loc[i][b].bngb.right)
                              {
                                    z += 1000;
                                    orks.points --;
                                    srand(time(NULL));
                                    rnd = rand() % (orks.coins - loc[i][b].bngb.right) + 1;
                                    orks.coins -= loc[i][b].bngb.right+rnd;
                                    loc[i][b].army += rnd + loc[i][b].bngb.right;
                                    loc[i][b].bngb.right = -1;
                                    return 0;  
                              }
                              break;
                      default:;
               }
            }
       return 1;
}
int EnemyRandomAttack(int at)
{
    if(orks.points <  2 || at == 0) return 1;
    int i,b,rnd,n,z=0;
      n = 0;
      srand(time(NULL));
      rnd = rand() % at + 1;
    while(z<3)
    {
      for(i=0;i<MAX_I;i++)
         for(b = 0;b<MAX_B;b++)
         {
              if(loc[i][b].country != 2) continue;
              if(loc[i][b].batt == 1)
              {
                  n++;
                  if(n == rnd) 
                  {
                     if(buy_army_attack(i,b)== 0) return 0;
                     n--;
                     
                  }
              }
              
         }
        z++;
      }
      return 1;
}
int EnemyRandomFreeAttack(int freeAttack)
{
     if(freeAttack == 0) return 1;
     if(orks.points ==  0) return 1;
     int i,b,rnd,n,z=0;
      n = 0;
      srand(time(NULL));
      rnd = rand() % freeAttack + 1;
     while(z<3)
     {
      for(i=0;i<MAX_I;i++)
         for(b = 0;b<MAX_B;b++)
         {
              if(loc[i][b].country != 2) continue;
              if(loc[i][b].fratt == 1)
              {
                  n++;
                  if(n == rnd) 
                  {
                     attack_free(i,b);
                     return 0;
                     
                  }
              }
              
         }
        z++;
      }
      return 1;
     
}
/////////Конец БОТА/////////////////////////////////
////////шифровщик - для самых умных и находчивых/////////
void hide_game_settings(FILE *fp)
{
    
  int c;
      int c1=-1,c2;
      unsigned long int n=0;
  
          fseek(fp,0,SEEK_SET);
  ////////////////////////
          while((c=fgetc(fp))!=EOF)
          {
              fseek(fp,n,SEEK_SET);
              if(c1 == -1)
              {
                    c1 = c;
                    fprintf(fp,"%c",c);
              }
              else
              {
                  c2 = c1;
                  c += c2;
                  //while(c>126) c -= 95; 
                  while(c>126) c -= 95;  
                  fprintf(fp,"%c",c);
                  c1 = c;
              }
              n++;
              fseek(fp,n,SEEK_SET);
          }
         if(n == 0) return;
         fseek(fp,-1,SEEK_END);
         c1 = fgetc(fp);
         fseek(fp,0,SEEK_SET);
         c = fgetc(fp);
         fseek(fp,0,SEEK_SET);
         c+= c1;
         while(c>126) c -= 95;
         fprintf(fp,"%c",c);
  fseek(fp,0,SEEK_SET);
}
          

void show_game_settings(FILE* fp)
{
     int c;
     int c1=-1,c2,cl;
     unsigned long int n = 0;
     fseek(fp,0,SEEK_SET);
     while((c = fgetc(fp))!=EOF)
     {
              
              if(c1 == -1)
              {
                    fseek(fp,-1,SEEK_END);
                    c1 = fgetc(fp);
                    fseek(fp,0,SEEK_SET);
                    c -= c1;
                    while(c<32) c += 95;
                    c1 = c;
                    fprintf(fp,"%c",c);
                    n++;
                    fseek(fp,n,SEEK_SET);
                    
              }
              else
              {
                  c2 = c1;
                  c1 = c;
                  c -= c2;
                  while(c<32) c += 95;
                  //while(c<32) c += 95;
                  fseek(fp,n,SEEK_SET);
                  fprintf(fp,"%c",c);
                  n++;
                  fseek(fp,n,SEEK_SET);
              }
     }
     fseek(fp,0,SEEK_SET);
}
void write_move(int ix,int bx,int i,int b,int n)
{
     fprintf(fwr,"m %d %d %d %d %d ",ix,bx,i,b,n);
}
void write_coins(int n)
{
      fprintf(fwr,"c %d ",n);
}
void write_army(int i,int b,int n)
{
      fprintf(fwr,"r %d %d %d ",i,b,n);
}
void write_def(int i,int b)
{
      fprintf(fwr,"b %d %d ",i,b);//build
}
void write_disband(int i,int b,int n)
{
      fprintf(fwr,"d %d %d %d ",i,b,n);
}
