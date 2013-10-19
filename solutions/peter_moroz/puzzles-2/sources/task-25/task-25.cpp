#include <iostream>
#include "Task25.h"

using namespace std;

static const char* kInputFilename = "2.5_example.in";
static const char* kOutputFilename = "2.5_results.out";

int main() {

  Task25 task(kInputFilename, kOutputFilename);

  try {
    task.Initialize();
    task.Perform();
  } catch (Task::Exception& ex) {
    cout << "Task::Exception has been caught. "
      << "Reason: " << ex.reason() << endl;
    return -1;
  }

  return 0;
}
