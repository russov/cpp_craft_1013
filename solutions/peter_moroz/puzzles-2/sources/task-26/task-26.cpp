#include <iostream>
#include <exception>
#include "Task26.h"

using namespace std;

static const char* kInputFilename = "2.6_example.in";
static const char* kOutputFilename = "2.6_results.out";

int main() {

  Task26 task(kInputFilename, kOutputFilename);

  try {
    task.Initialize();
    task.Perform();
  } catch (exception& ex) {
    cout << "An exception has been caught. "
      << "Reason: " << ex.what() << endl;
    return -1;
  }

  return 0;
}
