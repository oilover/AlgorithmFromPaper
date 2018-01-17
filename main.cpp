#include "Graph.h"
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
//#include <glib.h>
string fn = "toy.txt";
string ofn = fn + ".truss0";
char line[550],a[80],b[80];;

vector<string> my_split(string line, char d=',') // string d=",")
{
    vector<string> v;
    istringstream ss( line );
    while (!ss.eof())         // See the WARNING above for WHY we're doing this!
    {
      string x;               // here's a nice, empty string
      getline( ss, x, d );  // try to read the next field into it
      v.push_back(x);      // print it out, EVEN IF WE ALREADY HIT EOF
    }
    return v;
}

vector<string> split(const string &str,const string &pattern=",")
{
    //const char* convert to char*
    char * strc = new char[strlen(str.c_str())+1];
    strcpy(strc, str.c_str());
    vector<string> resultVec;
    char* tmpStr = strtok(strc, pattern.c_str());
    while (tmpStr != NULL)
    {
        resultVec.push_back(string(tmpStr));
        tmpStr = strtok(NULL, pattern.c_str());
    }
    delete[] strc;
    return resultVec;
}
std::vector<std::string> split2(std::string str,std::string s=",")
{
    vector<string> vStr;
    boost::split( vStr, s, boost::is_any_of( s ), boost::token_compress_on );
    return vStr;
}
void solve_Truss()
{
    Graph G = Graph();
    double t0 = clock();
    G.readGraph(fn.c_str());
    double t1 = clock();
    cerr << "Read time: " << (t1-t0)/CLOCKS_PER_SEC<<endl;
    G.TrussDecomposition();
    double  t2 = clock();
    cerr << "Truss time: " << (t2-t1)/CLOCKS_PER_SEC << endl;
    G.writeTruss(ofn.c_str());
}
// get first 2w lines
void get_first_lines()
{
    double t0 = clock();
    freopen("LBSN + keyword\\gowalla\\gowalla_checkins.csv","r",stdin);
    freopen("LBSN + keyword\\gowalla\\checkins_200000.csv","w",stdout);
  //  string line;
 //   cin.getline(line,500);
    gets(line);
    prt(line);

    for (int i=0;i<200000;i++) {
        gets(line);
        puts(line);
    }
    double t1 = clock();
    cerr << "Read time: " << (t1-t0)/CLOCKS_PER_SEC<<endl;
}
void get_tags()
{
    double t0 = clock();
    freopen("LBSN + keyword\\gowalla\\checkins_20000.csv","r",stdin);
    freopen("LBSN + keyword\\gowalla\\tags.txt","w",stdout);
//    gets(line);
//    prt(line);
    set<string> labels;
    for (int i=0;i<20000;i++) {
        gets(line);
        vector<string> vs = my_split(line);
        string label = *vs.rbegin();
      //  if (i < 20) prt(label);
        labels.insert(label);
    }
    prt(labels.size());
    for (string s: labels) cout<<s<<endl;
    double t1 = clock();
    cerr << "Read time: " << (t1-t0)/CLOCKS_PER_SEC<<endl;
}
void demo1()
{
    char* str = "ads,dasf,ikl,,gdf,";
    vector<string> vs = split2(str); //my_split(str);
    prt(vs.size());
    for (auto s:vs) prt(s);
    prt(*vs.rbegin());
    prt(str);
}
int main(int argc, char **argv)
{
    stringstream ss;//demo1();//get_first_lines();
    ss << 34;
    ss << 678;
    string s;
    ss >> s;
    prt(s);
    return 0;
}
// intersect(): 13 35  53s
//              38 60  77s
/*
  Truss time: 60
number_of_edges() = 925872


Process returned 0 (0x0)   execution time : 77.216 s
Press any key to continue.
-------
filename = com-amazon.ungraph.txt
number_of_edges() = 925872
Read time: 7
Truss time: 13

Process returned 0 (0x0)   execution time : 26.002 s
Press any key to continue.
--------
filename = com-amazon.ungraph.txt
number_of_edges() = 925872
Read time: 7
Truss time: 38

Process returned 0 (0x0)   execution time : 51.182 s
Press any key to continue.
----
filename = facebook_combined.txt
number_of_edges() = 88234
Read time: 486
k = 98
Truss time: 57371
number_of_edges() = 88234

Process returned 0 (0x0)   execution time : 61.621 s
Press any key to continue.


*/
