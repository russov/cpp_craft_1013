#ifndef TASK_24_H_
#define TASK_24_H_

#include "../common/Task.h"

class Task24 : public Task {

public:
  Task24(const char* input_fname, const char* output_fname)
    : Task(input_fname, output_fname) {}
  ~Task24() {}
  
  virtual void Perform();
  virtual void Initialize();
};

#endif // TASK_24_H_