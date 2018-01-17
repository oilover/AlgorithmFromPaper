#include "Graph.h"
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
//#include <glib.h>
string fn = "toy.txt";
string ofn = fn + ".truss0";
char line[850],a[80],b[80];;

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
    FILE *fin = fopen("LBSN + keyword\\gowalla\\gowalla_spots_subset1.csv","r");//,stdin);
    FILE *fout = fopen("LBSN + keyword\\gowalla\\gowalla_spots_subset1_200000.csv","w");//,stdout);
  //  string line;
 //   cin.getline(line,500);
 prt(fin);prt(fout);
    fgets(line,800,fin);
    prt(line);

    for (int i=0;i<200000;i++) {
        fgets(line,800,fin);
        if (i>10 && i<20) prt(line);
        fputs(line,fout);
    }
    fclose(fin);
    fclose(fout);
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
int str2int(string s)
{
    stringstream ss;
    ss << s;
    int d;
    ss >> d;
    return d;
}
void demo1()
{
    char* str = "ads,dasf,ikl,,gdf,";
    vector<string> vs = my_split(str);
    prt(vs.size());
    for (auto s:vs) prt(s);
    prt(*vs.rbegin());
    prt(str);
}
map<int,set<int> > u2p;
void get_user_labels()
{
    double t0 = clock();
    freopen("LBSN + keyword\\gowalla\\gowalla_checkins_200000.csv","r",stdin);
    set<int> spots;
    ofstream fo("LBSN + keyword\\gowalla\\POIs.txt");
    int i = 0;
    while(gets(line)) {
      //  if (++i<10) prt(line);
        if (strlen(line)<=1) continue;
        vector<string> vs = my_split(line);
        int u = str2int(vs[0]), poi = str2int(vs[1]);
        if (++i<10) prt(line),prt(vs.size()), prt(u),prt(poi);
        spots.insert(poi);
        if (u2p.count(u)== 0) u2p[u] = set<int>();
        u2p[u].insert(poi);
    }
    for (int p: spots) {
        fo << p << endl;
    }
    fo.close();
    fo.clear();
    prt(spots.size());
    prt(u2p.size());
    fo.open("LBSN + keyword\\gowalla\\user -- POIs.txt");
    for (auto pp: u2p) {
        fo << pp.first;
        for (int p: pp.second) {
            fo << " " << p;
        }
        fo <<endl;
    }
    fo.close();
    fo.clear();
    double t1 = clock();
    cerr << "Read time: " << (t1-t0)/CLOCKS_PER_SEC<<endl;
}
void demo2()
{
    char s[5];
    gets(s);
    puts(s);
    prt(s);
}
int main(int argc, char **argv)
{
    get_first_lines();
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

k = 98
Truss time: 57371
number_of_edges() = 88234

Process returned 0 (0x0)   execution time : 61.621 s
Press any key to continue.


*/
