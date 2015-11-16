#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#define  M_MAX_I 10
#define  M_MAX_B 16
#define SIZEGAME  int
#define SZ_1 1
#define SZ_2 2
#define SZ_3 3
#define SZ_4 4 
#define SET 1
#define GET 2
#define DEFENDER 500
#define MaxCoins 150
#define MaxArmy 2000
#define NEXT_TURN -2
#define NextTURNmenuX MAX_B*B_KUB + 2
#define NextTURNmenuY 15
#define LeftEMX MAX_B*B_KUB + 1
#define DEFCOF 0.30
#define KINGCOF 0.25

#define ORKCOF 1
#define ORKCOF_5 1.5
#define ORKCOF_10 2
#define ORKCOF_15 3

#define PEPCOF 1
#define PEPCOF_5 1.5
#define PEPCOF_10 2
#define PEPCOF_15 3

SIZEGAME MAX_I = 5;
SIZEGAME MAX_B = 7;
#define SIZE_of_GAME 1 //1 2 3 4
char kl_1[] = "settings/kl_1";
char kl_stop_1[] = "settings/kl_stop_1";
char kl_2[] = "settings/kl_2";
char kl_stop_2[] = "settings/kl_stop_2";
char sr_go_1[] = "settings/sr_go_1";
char sr_go_2[] = "settings/sr_go_2";
char name[] = "settings/123";
FILE *fstop1;
FILE *fstop2;
FILE *srgo1;
FILE *srgo2;
FILE *fkl1;
FILE *fkl2;
int mapcheck = 0;
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
}loc[M_MAX_I][M_MAX_B];
int rand(int k,int f)
{
    int d;
    int a;
    k+=d;
    k+=a;
	k +=213832133;
	if(f!=0) k*=f;
	k*=244231;
	k+= f;
	k/= 15;
//	k = k% LENGTH;
	return k;
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
void new_game()
{
       int i,b;
       set_get_size_game(SIZE_of_GAME,SET);
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
}
void check_file(int k)
{
     FILE *f;
     int side=0;
     int coins,i,b,ix,bx,army;
     char c;
     if(k == 1) f = fopen(kl_1,"r");
     if(k == 2) f = fopen(kl_2,"r");
     if(f== 0) printf("FILE ER\n");
     c = fgetc(f);
     if(c == 'p') side = 1;
     if(c == 'o') side = 2;
     if(side == 0){side = k; printf("side error");}
     
     fgetc(f);
     while(!feof(f))
     {
         while((c = fgetc(f))== ' ');
         printf("-%c-\n",c);
         switch(c)
         {
             case 'c':
                     fscanf(f,"%d",&coins);
                     if(side == 1)  people.coins -= coins;
                     else orks.coins -= coins;
                     break;
             case 'r':
                     fscanf(f,"%d%d%d",&i,&b,&army);
                     loc[i][b].army+=army;
                     printf("+army(%d)",loc[i][b].army);
                     break;
             case 'b': 
                     fscanf(f,"%d%d",&i,&b);
                     loc[i][b].def = 1;
                     break;
             case 'd':
                     fscanf(f,"%d%d%d",&i,&b,&army);
                     loc[i][b].army -= army;
                     break;
             case 'm':
                     fscanf(f,"%d%d%d%d%d",&i,&b,&ix,&bx,&army);
                     loc[i][b].army -= army;
                     if(side == 1) loc[ix][bx].bufpeople+=army;
                     else loc[ix][bx].buforks += army;
             default: printf("check_end (%d)\n",k);
         }
     }
     fclose(f);
     
}
void file_in()
{
     FILE *fp;
     int i,b;
     printf("\nFILE_IN\n");
     if((fp = fopen(name,"w+"))!= NULL)
                      {
                             fprintf(fp,"%d ",mapcheck);
                             mapcheck= (mapcheck+1)%2;
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
                                 //  hide_game_settings(fp);
                                   fclose(fp);
                             }
                      }
      else printf("\nfile_in() FILE ERROR\n");
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
                 // peoplewinfunc(); без анализа
                  return 1;
             }
			 if(people.maps== 0)
             {
                 // opkwinfunc();
                  return 1;
             }
            return 0; 
}
int main()
{
    remove(sr_go_1);
    remove(sr_go_2);
    remove(kl_stop_1);
    remove(kl_stop_2);
    int ready1,ready2;
    //new_game
     new_game();
     countpoints();
    //in file
    file_in();
    fkl1= fopen(kl_1,"w"); fclose(fkl1);
    fkl2= fopen(kl_2,"w"); fclose(fkl1);
    if((fstop1 = fopen(kl_stop_1,"w"))==0){ printf("\nstopfile(1) error"); Sleep(3000);exit(1);}
    else fclose(fstop1);
    if((fstop2 = fopen(kl_stop_2,"w"))==0){ printf("\nstopfile(2) error"); Sleep(3000);exit(1);}
    else fclose(fstop2);
    while(1)
    { printf("1");
            remove(sr_go_1); remove(sr_go_2);
            while((fstop1 = fopen(kl_stop_1,"w"))==0) printf("\nTry open kl_stop_1"); 
            while((fstop2 = fopen(kl_stop_2,"w"))== 0) printf("\nTry open kl_stop_2") ; 
            fclose(fstop1);
            fclose(fstop2);
            ready1= 0; ready2= 0;
            while(ready1*ready2 == 0)
            { 
               //printf("2");
               Sleep(500);
                 if(ready1 == 0) 
                 { 
                           srgo1 = fopen(sr_go_1,"r"); 
                           if(srgo1!= 0)
                             {
                                   check_file(1);
                                   ready1 = 1; 
                                   printf("\nready 1");
                                   fclose(srgo1);
                                   if(remove(sr_go_1) !=0) printf("\ncant del sr_go_1");
                             }
                           
                 }
                 if(ready2 == 0)
                 {
                           srgo2 = fopen(sr_go_2,"r"); 
                           if(srgo2!=0)
                           {
                                      check_file(2);
                                      ready2 = 1;
                                      printf("\nready 2");
                                      fclose(srgo2);  
                                      if(remove(sr_go_2) !=0) printf("\ncant del sr_go_2");
                           }
                           
                 }
            }
            //countwar
            WarCount();
            countmaps();
            if(countpoints()== 1); // конец игры
            file_in();
    }
}
