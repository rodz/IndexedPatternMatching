#include <iostream>
#include <string>
#include <getopt.h>
#include "algorithms/sarr.h"
#include "algorithms/huffman.h"
#include <fstream>
#include <set>
#include <map>
#include <queue>
#include <vector>

const char* const short_opts = "tce:p:a:h";
const option long_opts[] = {
    {"help", no_argument, nullptr, 'h'},
    {"count", no_argument, nullptr, 'c'},
    {"pattern", required_argument, nullptr, 'p'},
};

using namespace std;

string pattern_file_path = "";
vector<string> pattern_list;
string c;

ifstream* file = NULL;
string cur_filename = "";
string command;
string file_name;

bool help = false;
bool line = true;
bool dist = false;
bool only_count = false;
int ind;

int parse_args(int argc, char* argv[]) {
  int opt = 0;
  while(opt != -1) {
    opt = getopt_long(argc, argv, short_opts, long_opts, nullptr);
    switch(opt) {
      case 'c':
        only_count = true;
        break;
      case 'p':
        pattern_file_path = string(optarg);
        break;
      case -1:
        break;
      default:
        help = true;
        return 1;
    }
  }

  if (argc < 1) {
    cerr << "Missing arguments, printing help details" << endl;
    help = true;
    return 1;
  }

  ind = optind;

  if(ind < argc) {
    command = string(argv[ind++]);
  }else {
    help = true;
    return -1;
  }

  if(command == "index") {
    if(ind < argc) {
      file_name = string(argv[ind++]);
    }else {
      help = true;
      return -1;
    }
    generateIndexFile(file_name);
  } else if (command == "search") {
    if(pattern_file_path.size() > 0) {
      string pattern;
      ifstream* pattern_file = new ifstream();
      pattern_file->open(pattern_file_path);
      if(!pattern_file->is_open()){
        cerr << "Pattern file were unable to be opened" << endl;
        return -1;
      }
      while(!pattern_file->eof()) {
        getline(*pattern_file, pattern);
        pattern_list.push_back(pattern);
      }
      pattern_file->close();
    } else if (ind != argc) {
      string pattern = string(argv[ind++]);
      pattern_list.push_back(pattern);
    } else {
      help = true;
      return -1;
    }

    if(ind < argc) {
      file_name = string(argv[ind++]);
    }else {
      help = true;
      return -1;
    }

    int occ = searchIndexFile(file_name, pattern_list, only_count);
    cout<< "Total occurrences: " << occ <<endl;
    return 0;
  } else if(command == "zip") {
    if(ind < argc) {
      file_name = string(argv[ind++]);
    }else {
      help = true;
      return -1;
    }
    encodeFile(file_name);
  } else if(command == "unzip") {
    if(ind < argc) {
      file_name = string(argv[ind++]);
    }else {
      help = true;
      return -1;
    }
    decodeFile(file_name);
  } else {
    help = true;
    return -1;
  }

  return 1;
}

int main(int argc, char* argv[]) {
  parse_args(argc, argv);
  if(help) {
    cout<<"Usage: ipmt [command] [options]"<<endl;
    cout<<"Command:"<<endl;
    cout<<"  index TEXT_FILE               Generates an index file based on the first file, the index will contain a copy of the original file and the sufix array"<<endl;
    cout<<"  search PATTERN INDEX_FILE     Given an index file and a pattern will return the list of occurencies of this patten on the file text"<<endl;
    cout<<"  zip TEXT_FILE                 Generates a compressed .myzip file given the original file"<<endl;
    cout<<"  unzip MYZIP_FILE              Receives a myzip file and restores it back to its original content"<<endl;
    cout<<"Options:"<<endl;
    cout<<"  -p, --pattern PATTERN_FILE    Grabs multiple patterns at once from a file, when this option is passed, the pattern parameter should not be passed."<<endl;
    cout<<"  -c, --count                   Hides the line results and displays only the total number of occurences"<<endl;
    cout<<"  -h, --help                    Shows this guide"<<endl;
    return 0;
  }

  // int occ = searchWithSimplePat(files_list, pattern_list, algorithm);
  // cout<< "Total occurrences: " << occ <<endl;
}
