

/* 
 * File:   main.cpp
 * Author: Divyansh
 *
 * Created on September 12, 2015, 4:42 PM
 */
#include <iostream>
#include <string>
#include <map>
#include <algorithm>
#include <strings.h>
#include <sstream>
#include <vector>
#include <queue>
#include <sstream>
#include <cstdlib>
#include <fstream>
#define maxno 20000

using namespace std;
int str_time = 0;
int c = 0, t = str_time;
const int no_middle_nodes = 5000;
const int no_dest_nodes = 5000;
string start_node; //= "S";
string middleNodes[no_middle_nodes];
vector<string> mdlVec;
string destinationNodes[no_dest_nodes];
vector<string> destVec;
const int total_nodes = 1 + no_middle_nodes + no_dest_nodes;
vector<string> totatVec;
int i, j = 0;
const int edges = 250000;

map< string, int> mp1;
map<string, int>::iterator it1;
int v[total_nodes], am[total_nodes][total_nodes], n_cost[total_nodes][total_nodes];
int mn, dn, ed = 0;
string* m;
int n_total;
int distances[total_nodes];
vector<int> vq;
int parent[edges];
int dfsflag = 0;
int edgeFlag = 0;

//file handling
char data[512];
int algo_id;
string read_edges_no;
// string data;
int no_of_cases;
string algo_name;
string read_s;
string read_mn;
string read_dn;
int read_no_of_edges;
string read_edge_info;
int read_st_time;
string edge_info[25000][1024];
string line;

string ans_res;


map< string, int>& mapnodes(int count, map< string, int>& mp1);
void sortNodesLex();

void remove_carriage_return(std::string& line) {
    if (*line.rbegin() == '\r') {
        line.erase(line.length() - 1);
    }
}

void readProps(ifstream &infile) {

    getline(infile, read_s);
    remove_carriage_return(read_s);
    start_node = read_s;
    getline(infile, read_dn);
    remove_carriage_return(read_dn);
    stringstream ssin(read_dn);
    int k = 0;

    string tmp;

    while (getline(ssin, tmp, ' ')) {
        destinationNodes[k] = tmp;
        k++;
    }
    dn = k;

    getline(infile, read_mn);
    remove_carriage_return(read_mn);
    stringstream ssin1(read_mn);
    int k1 = 0;

    string tmp1;
    while (getline(ssin1, tmp1, ' ')) {
        middleNodes[k1] = tmp1;
        k1++;
    }
    mn = k1;


    getline(infile, read_edges_no);
    remove_carriage_return(read_edges_no);
    read_no_of_edges = atoi(read_edges_no.c_str());
    ed = read_no_of_edges;
    if (ed == 0) {
        ans_res = "None";
        edgeFlag = 1;
        return;
    }
    int ed_pt = 0;
    for (int i = 0; i < read_no_of_edges; i++) {
        getline(infile, read_edge_info);
        remove_carriage_return(read_edge_info);
        //read start node
        edge_info[i][0] = read_edge_info.substr(0, read_edge_info.find(" "));
        ed_pt = read_edge_info.find(" ");
        read_edge_info = read_edge_info.substr(ed_pt + 1);
        //read end node
        edge_info[i][1] = read_edge_info.substr(0, read_edge_info.find(" "));
        ed_pt = read_edge_info.find(" ");
        read_edge_info = read_edge_info.substr(ed_pt + 1);
        //read edge_cost
        edge_info[i][2] = read_edge_info.substr(0, read_edge_info.find(" "));
        ed_pt = read_edge_info.find(" ");
        read_edge_info = read_edge_info.substr(ed_pt + 1);
        //read no of off times
        edge_info[i][3] = read_edge_info.substr(0, read_edge_info.find(" "));
        ed_pt = read_edge_info.find(" ");
        read_edge_info = read_edge_info.substr(ed_pt + 1);
        if (atoi(edge_info[i][3].c_str()) == 1) {
            edge_info[i][4] = read_edge_info;
        } else {
            for (int j = 4; j < 4 + atoi(edge_info[i][3].c_str()); j++) {
                edge_info[i][j] = read_edge_info.substr(0, read_edge_info.find(" "));
                ed_pt = read_edge_info.find(" ");
                read_edge_info = read_edge_info.substr(ed_pt + 1);
            }
        }
        read_edge_info = "";
    }
    getline(infile, line);
    read_st_time = atoi(line.c_str());
    str_time = read_st_time;
    t = str_time;
    getline(infile, line);
    return;
}

int getmn() {
    return 5;
}

int getdn() {
    return 2;
}

int geted() {
    return 9;
}

void initg() {

    n_total = 1 + mn + dn;
    mdlVec.clear();
    for (int i = 0; i < mn; i++) {
        mdlVec.push_back(middleNodes[i]);
    }
    sort(mdlVec.begin(), mdlVec.end());


    destVec.clear();
    for (int i = 0; i < dn; i++) {
        destVec.push_back(destinationNodes[i]);
    }
    sort(destVec.begin(), destVec.end());

    totatVec.clear();
    totatVec.insert(totatVec.begin(), mdlVec.begin(), mdlVec.end());
    totatVec.insert(totatVec.end(), destVec.begin(), destVec.end());
    sort(totatVec.begin(), totatVec.end());

    //insert starting node at beginning
    totatVec.insert(totatVec.begin(), start_node);


}

struct node_info {
    int no;
    int node_cost;
    string node_name;
    int st_time;
} *q = NULL, *r = NULL, *x = NULL;

struct node {
    node_info *pt;
    node *next;
} *front = NULL, *rear = NULL, *p = NULL, *np = NULL;

struct timeoff {
    int sid;
    int did;
    int n_toff;
    vector<pair<int, int> > t;
} fi[edges * 20];

void bfsinit() {
    initg();
    ans_res = "";
    front = NULL;
    rear = NULL;
    p = NULL;
    np = NULL;
    mp1.clear();

    c = 0;
    for (int ty = 0; ty < n_total; ty++)
        v[ty] = 0;
    int count = 0;
    // sortNodesLex();
    mp1 = mapnodes(count, mp1);
    for (int i = 0; i < n_total; i++) {
        for (int j = 0; j < n_total; j++)
            am[i][j] = 0;
    }

    for (int k = 0; k < ed; k++) {
        int i = mp1.find(edge_info[k][0]) -> second;
        int j = mp1.find(edge_info[k][1]) -> second;
        //
        am[i][j] = 1;
    }

    for (int i = 0; i < n_total; i++) {
        for (int j = 0; j < n_total; j++) {
            //am[i][j] = map the line. a b 12 0 maps like a is mapped to sn int and so is b.
            if (am[i][j] != 1)
                am[i][j] = 0;
        }
    }
}

string findNodeName2(int ind);

map<string, int>& mapnodes(int count, map< string, int>& mp1) {

    mp1.clear();
    for (i = 0; i < 1 + mdlVec.size() + destVec.size(); i++) {

        mp1.insert(pair<string, int>(totatVec[i], count));
        count++;
    }


    return mp1;
}

void sortNodesLex() {
    sort(middleNodes, middleNodes + sizeof (middleNodes) / sizeof (middleNodes[0]));

    sort(destinationNodes, destinationNodes + sizeof (destinationNodes) / sizeof (destinationNodes[0]));

}

struct ucs_node_info {
    int no1;
    int node_cost1;
    string node_name1;
    int st_time1;

    bool operator<(const ucs_node_info& rhs) const {
        return node_cost1 > rhs.node_cost1;
    }
} *q1 = NULL, *r1 = NULL, *x1 = NULL, *y1 = NULL, *z1 = NULL;

vector<string> split(string str, char delimiter) {
    vector<string> internal;
    stringstream ss(str); // Turn the string into a stream.
    string tok;

    while (getline(ss, tok, delimiter)) {
        internal.push_back(tok);
    }

    return internal;
}

struct ucs_node {
    ucs_node_info *pt1;
    ucs_node *next1;

} *front1 = NULL, *rear1 = NULL, *p1 = NULL, *np1 = NULL;

priority_queue<ucs_node_info> pq;

//bfs

void bfspush(node_info *ptr) {
    np = new node;
    np->pt = ptr;
    np->next = NULL;
    if (front == NULL) {
        front = rear = np;
        rear->next = NULL;
    } else {
        rear->next = np;
        rear = np;
        rear->next = NULL;
    }
}

node_info *remove() {
    if (front == NULL) {
        cout << "empty queue\n";
    } else {
        p = front;
        x = p->pt;
        front = front->next;
        delete(p);
        return (front->pt);
    }
}

string bfsgoaltest(int i, map<string, int>& mp1) {
    // if i is in destination array
    string res = "";
    int j;
    for (j = 0; j < destVec.size(); j++) {
        if (mp1.find(destVec[j])->second == i)
            return destVec[j];
    }
    return res;
}

string findNodeName2(int ind) {
    if (mp1.find(start_node) -> second == ind)
        return start_node;

    for (int i = 0; i < totatVec.size(); i++) {
        // cout<<"\n\n destnode-"<<destinationNodes[i]<<"\n\n\n";
        if (mp1.find(totatVec[i]) -> second == ind) {
            //    cout<<"\n found dest node"<<destinationNodes[i]<<"\n";
            return totatVec[i];
        } else
            continue;
    }
}

bool isblocked(int s, int d, int ti) {
    bool block = false;
    for (int i = 0; i < ed; i++) {
        if (fi[i].sid == s && fi[i].did == d) {
            for (int j = 0; j < fi[i].n_toff; j++) {
                if (ti % 24 >= (fi[i].t[j].first % 24) && ti % 24 <= (fi[i].t[j].second % 24)) {

                    block = true;
                    return block;
                }
            }
        }
    }
    return block;
}

int getNextPriorityNode() {
    ucs_node_info t = pq.top();
    if (find(vq.begin(), vq.end(), t.no1) == vq.end()) {
        pq.pop();
        return t.no1;
    } else {
        pq.pop();
        return getNextPriorityNode();
    }
}

void addToFrontier(int evalnode) {
    for (int i = 0; i < n_total; i++) {
        bool l = (find(vq.begin(), vq.end(), i) == vq.end());
        if (l) {
            if (am[evalnode][i] != maxno && !isblocked(evalnode, i, distances[evalnode])) {
                if (distances[i] > am[evalnode][i] + distances[evalnode]) {
                    distances[i] = am[evalnode][i] + distances[evalnode];

                    parent[i] = evalnode;

                    x1 = new ucs_node_info;
                    x1->no1 = i;
                    x1->node_name1 = findNodeName2(i);
                    x1->node_cost1 = distances[i];
                    x1->st_time1 = distances[i];

                    pq.push(*x1);

                }
            }
        }


    }
}

void ucsinit() {
    initg();
    ans_res = "";
    for (int ert = 0; ert < edges * 20; ert++) {
        fi[ert].t.clear();
    }
    pq = priority_queue<ucs_node_info>();
    vq.clear();
    c = 0;
    while (!pq.empty()) {
        pq.pop();
    }
    for (int ty = 0; ty < n_total; ty++) {
        v[ty] = 0;
        distances[ty] = maxno;
    }
    int count = 0;
    // sortNodesLex();
    mp1 = mapnodes(count, mp1);
    for (int i = 0; i < n_total; i++) {
        for (int j = 0; j < n_total; j++)
            am[i][j] = maxno;
    }

    for (int k = 0; k < ed; k++) {
        int i = mp1.find(edge_info[k][0]) -> second;
        int j = mp1.find(edge_info[k][1]) -> second;
        int nc = atoi(edge_info[k][2].c_str());
        am[i][j] = nc;

        fi[k].sid = i;
        fi[k].did = j;
        fi[k].n_toff = atoi(edge_info[k][3].c_str());
        for (int f = 4; f < fi[k].n_toff + 4; f++) {
            string s = edge_info[k][f].substr(0, edge_info[k][f].find("-"));
            string d = edge_info[k][f].substr(edge_info[k][f].find("-") + 1, edge_info[k][f].length());
            int o_st_tim = atoi(s.c_str());
            int o_end_tim = atoi(d.c_str());
            fi[k].t.push_back(pair<int, int>(o_st_tim % 24, o_end_tim % 24));
        }
    }
}

void ucs() {
    int evalnode;
    string ans;
    if (c == 0) {
        q1 = new ucs_node_info;
        q1->no1 = 0;
        q1->node_cost1 = t;
        q1->node_name1 = findNodeName2(0);
        q1->st_time1 = t;
        pq.push(*q1);
        distances[0] = t;
    }

    while (!pq.empty()) {
        if (c == 0) {
            evalnode = getNextPriorityNode();
            ans = bfsgoaltest(evalnode, mp1);
            if (ans != "") {
                distances[evalnode] = distances[evalnode] % 24;
                ostringstream convert;
                convert << distances[evalnode];
                ans_res = ans + " " + convert.str();
                cout << endl << ans + " " + convert.str();
                return;
            }
            v[evalnode] = 1;
            vq.push_back(evalnode);
            addToFrontier(evalnode);
        } else {
            evalnode = getNextPriorityNode();
            if (isblocked(parent[evalnode], evalnode, distances[parent[evalnode]]))
                continue; //call while again
            ans = bfsgoaltest(evalnode, mp1);
            if (ans != "") {
                distances[evalnode] = distances[evalnode] % 24;
                ostringstream convert;
                convert << distances[evalnode];
                ans_res = ans + " " + convert.str();
                cout << endl << ans + " " + convert.str();
                return;
            }
            v[evalnode] = 1;
            vq.push_back(evalnode);
            addToFrontier(evalnode);
        }
        c++;
    }
    if (pq.empty()) {
        //return two string stating none
        ans_res = "None";
        cout << endl << "None";
    }

}
int cur_time = 0;

void bfs(int i) {
    string ans;
    if (c == 0) {
        q = new node_info;
        q->no = i;
        q->st_time = t;
        q->node_cost = 1;
        string temp1 = findNodeName2(i);
        q->node_name = temp1;
        cur_time = t;
        //apply bfs goal test
        ans = bfsgoaltest(i, mp1);
        if (ans != "") {
            t = t % 24;
            ostringstream convert;
            convert << t;
            ans_res = ans + " " + convert.str();
            cout << endl << ans + " " + convert.str();
            return;
        }
        t++;

        //mark node as visited
        v[i] = 1;
        bfspush(q);
    }
    c++;
    for (int j = 0; j < n_total; j++) {
        if (am[i][j] == 0 || (am[i][j] == 1 && v[j] == 1))
            continue;
        else if (am[i][j] == 1 && v[j] == 0) {
            cur_time++;
            r = new node_info;
            r->no = j;
            r->st_time = cur_time;
            r->node_cost = 1;
            string temp1 = findNodeName2(j);
            r->node_name = temp1;

            ans = bfsgoaltest(j, mp1);
            if (ans != "") {
                cur_time = cur_time % 24;
                ostringstream convert;
                convert << cur_time;
                ans_res = ans + " " + convert.str();
                cout << endl << ans + " " + convert.str();
                return;
            }

            v[j] = 1;
            cur_time--;
            bfspush(r);
        }
    }
    node_info *tem = remove();
    if (c <= n_total - 1 && front != NULL) {
        cur_time = tem->st_time;
        bfs(tem->no);
    } else {
        ans_res = "None";
        cout << endl << "None";
    }
}

//dfs code

struct dfsnode {
    int id;
    string data;
    int st_time, lv_time;
} *p2 = NULL, *r2 = NULL;

struct stack {
    dfsnode *pt;
    stack *next;
} *top2 = NULL, *q2 = NULL, *np2 = NULL;

void push(dfsnode *ptr) {
    np2 = new stack;
    np2->pt = ptr;
    np2->next = NULL;
    if (top2 == NULL) {
        top2 = np2;
    } else {
        np2->next = top2;
        top2 = np2;
    }
}

dfsnode *pop() {
    if (top2 == NULL) {
        cout << "underflow\n";
        return NULL;
    } else {
        q2 = top2;
        top2 = top2->next;
        delete(q2);
        if (top2 == NULL)
            return NULL;
        return (top2->pt);
    }
}

void dfsinit() {
    ans_res = "";
    // top2 = NULL;
    dfsflag = 0;
    c = 0;
    t = str_time;
    initg();
    for (int ty = 0; ty < n_total; ty++)
        v[ty] = 0;
    int count = 0;
    // sortNodesLex();
    mp1 = mapnodes(count, mp1);
    for (int i = 0; i < n_total; i++) {
        for (int j = 0; j < n_total; j++)
            am[i][j] = 0;
    }

    for (int k = 0; k < ed; k++) {
        int i = mp1.find(edge_info[k][0]) -> second;
        int j = mp1.find(edge_info[k][1]) -> second;
        //int j = mp1.find(vector<edge_info[k][1]>)->second;
        am[i][j] = 1;
    }
    for (int i = 0; i < n_total; i++) {
        //cout<<"enter the values for adjacency matrix row:"<<i+1<<endl;
        for (int j = 0; j < n_total; j++) {
            //cin>>am[i][j];
            //am[i][j] = map the line. a b 12 0 maps like a is mapped to sn int and so is b.
            if (am[i][j] != 1)
                am[i][j] = 0;
        }
    }
}

void dfs(int i, int j) {
    string ans;
    p2 = new dfsnode;
    p2->id = i;
    p2->data = findNodeName2(i);
    p2->st_time = t;
    //  cout<<p2->data<<endl;
    //apply the goal test
    ans = bfsgoaltest(i, mp1);
    if (ans != "") {
        t = t % 24;
        ostringstream convert;
        convert << t;
        ans_res = ans + " " + convert.str();
        cout << endl << ans + " " + convert.str();
        j = n_total + 12;
        //exit(0);
        dfsflag = 1;
        return;
    }
    if (c == 0)
        push(p2);
    // t++;
    c++;
    v[i] = 1;
    if (top2->pt->id != p2->id)
        push(p2);
    while (j < n_total && dfsflag == 0) {
        if ((am[i][j] == 0) || (am[i][j] == 1 && v[j] == 1)) {
            j++;
        } else if (am[i][j] == 1 && v[j] == 0) {
            t++;
            dfs(j, 0);
        }
    }
    if (dfsflag == 1)
        return;
    r2 = pop();
    t--;
    if (r2 != NULL) {
        c++;
        dfs(r2->id, 0);
    } else {
        //cout << endl<< "None";
        //return none
    }
    if (r2 == NULL && ans_res == "") {
        cout << endl << "None";
        ans_res = "None";
        dfsflag = 1;
        return;
    }

    return;
}

int main() {

    // open a file in read mode.
    ifstream infile;
    ofstream outfile;
    infile.open("C:/Dj/My Stuff/USC/Fall 15/AI/Homework 1/newtest/inputFile.txt");
    outfile.open("C:/Dj/My Stuff/USC/Fall 15/AI/Homework 1/newtest/outputFile2.txt");
    while (!infile.eof()) { // while the end of file [ eof() ] is not reached 
        //infile >> line; // read a line from file
        getline(infile, line);
        no_of_cases = atoi(line.c_str());
        while (no_of_cases) {
            getline(infile, algo_name);
            if (algo_name.find("BFS") != algo_name.npos)
                algo_id = 0;
            if (algo_name.find("DFS") != algo_name.npos)
                algo_id = 1;
            if (algo_name.find("UCS") != algo_name.npos)
                algo_id = 2;
            switch (algo_id) {
                case 0:
                    try {
                        readProps(infile);
                        if (edgeFlag != 1) {
                            bfsinit();
                            bfs(0);
                        }
                        edgeFlag = 0;
                    } catch (...) {
                        ans_res = "None";
                    }
                    outfile << ans_res + "\r" << endl;
                    break;
                case 1:
                    try {
                        readProps(infile);
                        if (edgeFlag != 1) {
                            dfsinit();
                            dfs(0, 0);
                        }
                        edgeFlag = 0;
                    } catch (...) {
                        ans_res = "None";
                    }
                    outfile << ans_res + "\r" << endl;
                    break;
                case 2:
                    //  cout<<endl<<"UCS";
                    try {
                        readProps(infile);
                        if (edgeFlag != 1) {
                            ucsinit();
                            ucs();
                        }
                        edgeFlag = 0;
                    } catch (...) {
                        ans_res = "None";
                    }
                    outfile << ans_res + "\r" << endl;
                    break;
            }
            no_of_cases--;
        }
        break; // print the file to console
    }

    // close the opened file.
    infile.close();
    outfile.close();

    return 0;
}




