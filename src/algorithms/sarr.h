#include <iostream>
#include <getopt.h>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <map>
#include <set>
#include <utility>

#define pii pair<int,int>

using namespace std;

string text;

vector<int> sort_letters(string &txt) {
  int n = txt.size();
  set<int> chars;
  for(unsigned char c : txt) {
    chars.insert(c);
  }
  map<int, int> pos;
  int cur_pos = 0;
  while(chars.size()) {
    int cur_value = *chars.begin();
    chars.erase(cur_value);
    pos[cur_value] = cur_pos++;
  }

  vector<int> ans;
  for(unsigned char c: txt) {
    ans.push_back(pos[c]);
  }
  return ans;
}

vector<vector<int>> build_pos(string &txt) {
  int n = txt.size(), cur_sz = 1, cur_pot = 1;
  vector<vector<int>> pos;
  vector<vector<pair<pii, int>>> pos_order;

  vector<int> cur_pos = sort_letters(txt);
  pos.push_back(cur_pos);

  while(cur_pot < n) {
    vector<pair<pii, int>> next_val;
    for(int i = 0; i<n; i++) {
      int next_pos = i + cur_pot;
      next_val.push_back(make_pair(make_pair(pos[cur_sz-1][i], next_pos < n ? pos[cur_sz-1][next_pos] : -1), i));
    }
    sort(next_val.begin(), next_val.end());
    for(auto x: next_val) {
      pii y = x.first;
    }
    vector<int> next_pos(n, 0);
    int cur_pos = 0;
    for(int i = 1; i<n; i++) {
      pii curp = next_val[i].first;
      pii prevp = next_val[i - 1].first;
      int cur_id = next_val[i].second;
      if(curp != prevp) {
        cur_pos ++;
      }
      next_pos[cur_id] = cur_pos;
    }
    pos.push_back(next_pos);

    cur_pot = cur_pot*2;
    cur_sz = cur_sz+1;
  }
  return pos;
}

vector<int> build_sarr(vector<int> &last_pos) {
  int n = last_pos.size();
  vector<int> sa(n, 0);
  for(int i = 0; i<n; i++) {
    sa[last_pos[i]] = i;
  }
  return sa;
}

bool is_first(const int &pos1, const string &pat) {
  int n = text.size();
  int m = pat.size();
  int i = pos1, j = 0;
  for( ; i < n && j < m; i++, j++) {
    if(text[i] != pat[j]) {
      return text[i] < pat[j];
    }
  }
  return j != m;
}

bool is_after(const int &pos1, const string &pat) {
  int n = text.size();
  int m = pat.size();
  int i = pos1, j = 0;
  for( ; i < n && j < m; i++, j++) {
    if(text[i] != pat[j]) {
      return text[i] < pat[j];
    }
  }
  return 1;
}

int search_sarr(string& pat, vector<int> &sarr) {
  int lower = lower_bound(sarr.begin(), sarr.end(), pat, is_first) - sarr.begin();
  int upper = lower_bound(sarr.begin(), sarr.end(), pat, is_after) - sarr.begin();
  if (upper >= lower) {
    return upper - lower;
  }else {
    return 0;
  }
}

void generateIndexFile(string file_name) {
  string line;
  string output_file_name = file_name + ".idx";
  ifstream* txt_file = new ifstream();
  ofstream* output_file = new ofstream();
  cout<<output_file_name<<endl;
  txt_file->open(file_name);
  output_file->open(output_file_name);
  
  if(!txt_file->is_open()){
    cerr << "Text file were unable to be opened" << endl;
    return;
  }
  if(!output_file->is_open()){
    cerr << "Output file were unable to be opened" << endl;
    return;
  }

  while(!txt_file->eof()) {
    getline(*txt_file, line);
    vector<vector<int>> pos = build_pos(line);
    vector<int> sarr = build_sarr(pos[pos.size()-1]);
    *output_file << sarr.size() << endl;
    *output_file << line << endl;
    for (int i = 0; i<sarr.size(); i++) {
      if(i != 0){
        *output_file << ' ';
      }
      *output_file << sarr[i];
    }
    *output_file << endl;
  }
  output_file->close();
  txt_file->close();
}

int searchIndexLine(string &txt, vector<string> &pats, vector<int> &sarr) {
  text = txt;
  int ans = 0;
  for(string pat : pats) {
    int c = search_sarr(pat, sarr);
    ans += c;
  }
  return ans;
}

int searchIndexFile(string index_file_name, vector<string> pats, bool only_count) {
  ifstream* index_file = new ifstream();
  index_file->open(index_file_name);
  if(!index_file->is_open()){
    cerr << "Index file were unable to be opened" << endl;
    return 0;
  }
  int ans = 0, line_count = 0;
  string line;
  while (!index_file->eof()) {
    line_count++;
    int qt;
    string txt;
    vector<int> sarr;
    getline(*index_file, line);
    if(line.size() == 0) {
      break;
    }
    qt = stoi(line);
    getline(*index_file, txt);
    getline(*index_file, line);
    stringstream ss(line);
    for(int i = 0; i<qt; i++) {
      int cur;
      ss >> cur;
      sarr.push_back(cur);
    }
    int oc = searchIndexLine(txt, pats, sarr);
    ans += oc;
    if (oc && !only_count) {
      cout << index_file_name << " " << line_count << " " << txt <<endl;
    }
  }
  index_file->close();
  return ans;
}
