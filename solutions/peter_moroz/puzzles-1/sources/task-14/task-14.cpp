#include <algorithm>
#include <cmath>
#include <iostream>
#include <fstream>
#include <functional>
#include <string>
#include <vector>

using namespace std;

static const char* InputFilename = "Input.txt";
static const char* OutputFilename = "Output.txt";
static const int N = 1000000;

void ReadCodes(ifstream& ifs, int& codes_count, vector<double>& codes);
void ReadPasswords(ifstream& ifs, vector<double>& passwords);
void CheckPasswordsAndLogResults(const vector<double>& passwords,
                                 const vector<double>& codes,

                                 ofstream& ofs);
int main() {
  int codes_count = 0;
  vector<double> codes;
  vector<double> passwords;

  ifstream ifs(InputFilename);
  if (!ifs.good()) {
    cerr << "Can't open file: " << InputFilename << endl;
    return -1;
  }

  ReadCodes(ifs, codes_count, codes);
  if (codes_count != static_cast<int>(codes.size())) {
    cerr << "Wrong input data: \n";
    cerr << "Number of Codes : " << codes_count << endl
      << static_cast<int>(codes.size()) << " codes have been read, "
      << " maximal available number = " << N << endl;
    return -1;
  }

  ReadPasswords(ifs, passwords);
  if (passwords.size() == 0) {
    cerr << "Wrong input data: \n"
      << "There are no passwords in file " << InputFilename << endl;
    return -1;
  }

  ifs.close();

  ofstream ofs(OutputFilename);
  if (!ofs.good()) {
    cerr << "Can't create file: " << OutputFilename << endl;
    return -1;
  }

  CheckPasswordsAndLogResults(passwords, codes, ofs);
  ofs.close();
  
  return 0;
}


void ReadCodes(ifstream& ifs, int& count, vector<double>& codes) {
  codes.clear();
  count = 0;
  string line;

  getline(ifs, line);
  count = ::atoi(line.c_str());

  if (count > N)
    return;

  codes.reserve(count);

  int i = 0;
  while (i < count) {
    getline(ifs, line);
    if (line.size() > 0)  // skip empty line
      codes.push_back(::atof(line.c_str()));
    if (ifs.eof())
      break;
    ++i;
  }
}
void ReadPasswords(ifstream& ifs, vector<double>& passwords) {
  passwords.clear();

  string line;
  while (!ifs.eof()) {
    getline(ifs, line);
    if (line.size() > 0)
      passwords.push_back(::atof(line.c_str()));
  }
}
void CheckPasswordsAndLogResults(const vector<double>& passwords,
                                 const vector<double>& codes, ofstream& ofs) {

  const double kPrecision = 0.0001;
  const double kScale = 1.0 / kPrecision;

  for (size_t i = 0; i < passwords.size(); ++i) {
    size_t j = 0;
    while (j < codes.size()) {
      if (::fabs(passwords[i] - codes[j]) < kPrecision) {
        double x = ::floor(::fabs(passwords[i]) * kScale);
        double y = ::floor(::fabs(codes[j]) * kScale);
        if (::fabs(x - y) == 0.0)
          break;
      }
      ++j;
    } // while (j < codes.size())
    if (j < codes.size())
      ofs << "YES";
    else
      ofs << "NO";
    ofs << endl;
  } // for (size_t i = 0; i < passwords.size(); ++i)
}
