#include <algorithm>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include <locale>

using namespace std;

static const char* InputFilename_translit = "Input_translit.txt";
static const char* InputFilename_ru1251 = "Input_ru1251.txt";
static const char* OutputFilename = "Output.txt";
static const size_t MaxTextLength = 100000;

inline char ToLower(char x) { return ::tolower(x); }
inline void ToLower(string& s) {
  for (size_t i = 0; i < s.length(); ++i)
    s[i] = ::tolower(s[i]);
}

void ReadKeys(ifstream& ifs, vector<string>& keys);
void FindKeysAndLogResult(const string& where,
                          const vector<string>& keys,
                          ofstream& ofs);

string Reverse(const string& s);
string Join(const string& s, const char* delims = " -\\");
vector<string> Split(const string& s, const char* delims = " -\\");


int main() {

  const char* InputFilename = InputFilename_ru1251;

  try {
    locale cp1251_locale("rus_rus.1251");
    locale::global(cp1251_locale);
  } catch (runtime_error&) {
    InputFilename = InputFilename_translit;
  }

  ifstream ifs(InputFilename);
  if (!ifs.good()) {
    cerr << "Can't open file: " << InputFilename << endl;
    return -1;
  }


  string discovered_text;
  getline(ifs, discovered_text);
  ToLower(discovered_text);
  discovered_text = Join(discovered_text);

  if (discovered_text.length() > MaxTextLength) {
    cout << "The length of discoverred text is greater than"
      << " maximal avalilable value (" << MaxTextLength << ")."
      << " Content of text has been truncated to this value.\n";
    discovered_text.resize(MaxTextLength);
  }

  vector<string> keys;
  ReadKeys(ifs, keys);
  ifs.close();

  ofstream ofs(OutputFilename);
  FindKeysAndLogResult(discovered_text, keys, ofs);
  ofs.close();

  return 0;
}

void ReadKeys(ifstream& ifs, vector<string>& keys) {
  string line;
  while (!ifs.eof()) {
    getline(ifs, line);
    if (line.length() > 0) {
      ToLower(line);
      keys.push_back(line);
    }
  }
}
void FindKeysAndLogResult(const string& where, 
                          const vector<string>& keys, ofstream& ofs) {
  for (size_t i = 0; i < keys.size(); ++i) {
    vector<string> key_parts = Split(Reverse(keys[i]));
    size_t j = 0;
    size_t p = 0;
    while (j < key_parts.size()) {
      p = where.find(key_parts[j], p);
      if (p == string::npos)
        break;
      p += key_parts[j].length();
      ++j;
    } // while (j < key_parts.size())
    if (j < key_parts.size())
      ofs << "NO";
    else
      ofs << "YES";
    ofs << endl;
  } // for (size_t i = 0; i < keys.size(); ++i)
}
string Reverse(const string& s) {
  string r(s);
  size_t len = r.length();
  size_t i = len - 1;
  size_t half_len = len / 2;
  for (size_t j = 0; j < half_len; ++j, --i)
    swap(r[j], r[i]);
  return r;
}
string Join(const string& s, const char* delims /*= " -\\"*/) {
  vector<string> parts = Split(s, delims);
  string result;
  for (size_t i = 0; i < parts.size(); ++i)
    result.append(parts[i]);
  return result;
}
vector<string> Split(const string& s, const char* delims /*= " -\\"*/) {
  vector<string> parts;
  size_t pos0 = 0;
  size_t pos1 = 0;
  while (pos0 != string::npos) {
    pos1 = s.find_first_of(delims, pos0);
    size_t len = pos1 != string::npos ? pos1 - pos0 : string::npos;
    parts.push_back(s.substr(pos0, len));
    if (pos1 == string::npos)
      break;
    pos0 = s.find_first_not_of(delims, pos1);
  }
  return parts;
}
