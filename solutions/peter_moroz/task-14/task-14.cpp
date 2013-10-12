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

class CompareWithPrecision 
  : public binary_function<double, double, bool> {
public:
  CompareWithPrecision(const double& precision) 
    : precision_(precision) {}
  bool operator()(const double& lhs, const double& rhs) const {
    return (::fabs(lhs - rhs) < precision_);
  }
private:
  double precision_;
};

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

  for (size_t i = 0; i < passwords.size(); ++i) {
    if (find_if(codes.begin(), codes.end(), 
        bind2nd(CompareWithPrecision(kPrecision), 
                passwords[i])) != codes.end()) {
      ofs << "YES";
    } else {
      ofs << "NO";
    }

    ofs << endl;
  } // for (size_t i = 0; i < passwords.size(); ++i)
}
