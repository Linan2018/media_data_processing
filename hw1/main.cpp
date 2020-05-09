#include<iostream>
#include<fstream>
#include<vector>
#include<map>
#include <tuple>
#include <sstream>
#include<algorithm>
#include<math.h>
#include <getopt.h>
#include <stdio.h>
#include <unistd.h>
using namespace std;


static map<string, pair<long double, long double>> m;

string bi(long double a){
    if (!a)
        return "0";
    if (a == 1)
        return "-";
    long double t = a;
    // cout <<"%"<< t << endl;
    // double d;
    string s = "";
    while(t){
        t *= 2;
        // 
        s += to_string((int)t);
        t = t - (int)t;
    }
    return s;
}

long double convert(string s){
    long double sum = 0;
    for (int i = 0; i < s.size(); ++i){
        if (s[i] - '0')
            sum += 1.0 / pow(2.0, i + 1);
    }
    return sum;
}


int dif(string s1, string s2){
    int i;
    for (i = 0; s1[i] == s2[i]; ++i)
        ;
    return i;
}

string solve(string start, string end){
    if (end == "-"){
        return start;
    }
    // cout << start << endl
        //  << end << endl;
    int i = dif(start, end);
    // cout << "i: " << i << endl;
    // cout << "start: " << start << endl;
    // cout << "end: " << end << endl;
    // cout << "start.size() " << start.size() << endl;
    if (start.size() - 1 == i){
        return start;
    }
    // cout << i << endl;
    if (start[i] == '0'){
        // start[i] = '1';
        return end.substr(0, i + 1);
    }
    else if (end[i] == '0'){
        // end[i] = '1';
        return start.substr(0, i + 1);
    }
    else
        return "-1";
}


string decode(string msg, int len){
    // cout << "msg:" << msg << endl;
    long double tmp_start = 0;
    long double tmp_len = 1;
    long double num = convert(msg);
    // cout << "num:" << num << endl;
    string s = "";
    while(1){
        // string code = solve(bi(tmp_start), bi(tmp_start + tmp_len));
        // cout << "code:" << s << endl;
        if (s.size() == len){
            break;
        }
        auto it = m.begin();
        for (; it != m.end(); ++it){
            auto range = it->second;
            long double start = tmp_start + tmp_len * range.first;
            long double end = start + tmp_len * (range.second - range.first);
            if (num >= start && num < end){
                s += it->first;
                tmp_start = start;
                tmp_len = end - start;
                break;
            }
        }
        // if (it != m.end()){
        //     return s;
        // }
    }
    return s;
}

string encode(string str_tmp, int len_c){
    string s;
    long double tmp_start = 0;
    long double tmp_len = 1;
    for (int i = 0; i < str_tmp.size(); i += len_c){
        s = str_tmp.substr(i, len_c);
        tmp_start += tmp_len * m[s].first;
        tmp_len = tmp_len * (m[s].second - m[s].first);
        // cout << tmp_start << "   "  << tmp_len << endl;
    }
    // cout << tmp_start << "   "  << tmp_len << endl;
    string a = bi(tmp_start);
    cout << "start of last interval: 0." << a << endl;
    string b = bi(tmp_start + tmp_len);
    cout << "end of last interval:   0." << b << endl;

    return solve(a, b);

}


int load(string path){
    ifstream fi(path, ios::in);
    int n;
    int len_c;
    string ch_tmp;
    long double d_tmp;
    long double sum_tmp = 0;
    string str_tmp;
    
    vector<char> chs;
    vector<double> p;

    fi >> n;
    for (int i = 0; i < n; ++i){
        fi >> ch_tmp;
        fi >> d_tmp;
        if(!i) len_c = ch_tmp.size();
        m[ch_tmp] = make_pair(sum_tmp, sum_tmp + d_tmp);
        // cout << m[ch_tmp].first << "  " << m[ch_tmp].second << endl;
        sum_tmp += d_tmp;
    }
    return len_c;
}

int get_step(){
    return (m.begin()->first).size();
}


int main(int argc,char *argv[]){

    int ch = 0;
    opterr = 0;

    int len_c;
    int len_code;
    string path;
    string str;
    string code;
    string _code;
    string t;

    while((ch=getopt(argc,argv,"p:s:ed:"))!=-1){

        // cout << "optind: " << optind << endl;
        // cout << "optarg: " << optarg << endl;
        // cout << "ch: " << ch << endl;

        switch(ch){
            case 'p':{
                path = optarg;
                load(path);
                len_c = get_step();
                break;
            }

            case 's':{
                str = optarg;
                cout << "raw str: " << str << endl;
                break;
            }

            case 'e':{
                code = encode(str, len_c);
                cout << "encode:  " << code << endl;
                break;
            }

            case 'd':{
                t = optarg;
                len_code = stoi(t);
                _code = decode(str, len_code);
                cout << "decode:  " << _code << endl;
                break;
            }

            default:
                cout << "Exception" << endl;
        }
    }

    // cout << "!!"<< m.size() << endl;
    // for (auto it = m.begin();it!=m.end();++it)
    // {
    //     cout << "@" << it->first << endl;
    //     cout << "#" << it->second.first << endl;
    //     cout << "#" << it->second.second << endl;
    // }

    // for (const auto& p : m)
    // cout << p.first << " "<< p.second <<"\n";

    // fi >> str;
    // load(path);
    // int len_c = get_step();
    // string code = encode(str, len_c);
    // string _code = decode(code, str.size());

    // cout << "raw str: " << str << endl;
    // cout << "encode:  " << code << endl;
    // cout << "decode:  " << _code << endl;

}