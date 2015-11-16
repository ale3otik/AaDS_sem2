#include<iostream>
#include<algorithm>
#include<vector>
#include<map>
#include<functional>
#include<list>
#include<string>
#include<fstream>
using namespace std;
void delete_spaces(string & str)
{
    int check = 0;
    int k = -1;
    int i;

    for(i = str.size() - 1;i>=0;--i)
    {
        if( str[i] != ' ' && str[i] != '\t')  break;
    }
    str.resize(i+1);

    for(int i = 0;i<str.size();++i)
    {
        if(check!=0 || (str[i] != ' ' && str[i] != '\t'))
           {
               str[++k] = str[i];
                check = 1;
           }
    }
    str.resize(k+1);
}
int main()
{
    ifstream read_s("freq.txt");
    ofstream write_s("dict.txt");
   vector<pair<string,int>> dictionary;
    string buff;
    int a1;
    int fr;
    while(true)
    {
        read_s>>a1;
        read_s>>fr;
        getline(read_s,buff);
        if(!read_s) break;
        delete_spaces(buff);
        dictionary.push_back(make_pair(buff,fr));
    }
    for(int i = 0;i< dictionary.size();++i)
    {
        write_s<< dictionary[i].second<<" "<<dictionary[i].first<<endl;
    }
    read_s.close();
    write_s.close();
    return 0;
}
