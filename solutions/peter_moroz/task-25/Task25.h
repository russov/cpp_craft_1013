#ifndef TASK_25_H_
#define TASK_25_H_

#include "../common/Task.h"

class Task25 : public Task {

public:
  Task25(const char* input_fname, const char* output_fname)
    : Task(input_fname, output_fname) {}
  ~Task25() {}
  
  virtual void Perform();
  virtual void Initialize();
};

#endif // TASK_25_H_