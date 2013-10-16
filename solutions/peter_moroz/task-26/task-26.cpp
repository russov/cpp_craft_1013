#include <iostream>
#include "Task26.h"
#include "../common/StringConstants.h"

using namespace std;

int main() {

  Task26 task(kInputFilenameBin, kOutputFilenameBin);

  try {
    task.Initialize();
  } catch (Task::Exception& ex) {
    cout << "Task::Exception has been caught. "
      << "Reason: " << ex.reason() << endl;
    return -1;
  }

  return 0;
}
