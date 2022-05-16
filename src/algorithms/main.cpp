#include <iostream>
#include <string>
#include <getopt.h>
#include <fstream>
#include <set>
#include <map>
#include <queue>
#include <vector>

using namespace std;

string decToBin(int num)
{
    string temp, r = "";
    while (num > 0)
    {
        temp += (num % 2 + '0');
        num /= 2;
    }
    r.append(32 - temp.length(), '0');
    for (int i = temp.length() - 1; i >= 0; i--)
    {
        r += temp[i];
    }
    return r;
}

string decToBin2(int num) {
  string str = bitset<32>(num).to_string();
  return str;
}

int binToDec(string str)
{
    int r = 0;
    for (auto c : str)
    {
        r = r * 2 + c - '0';
    }
    return r;
}

int binToDec2(string str) {
    return bitset<32>(str).to_ulong();
}

int main() {
  int cur = 395276;
  cout<<decToBin(cur)<<endl;
  cout<<decToBin2(cur)<<endl;

  cout<<binToDec(decToBin(cur))<<endl;
  cout<<binToDec2(decToBin2(cur))<<endl;
}