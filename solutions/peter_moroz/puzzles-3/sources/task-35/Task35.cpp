#include "Task35.h"
#include "Message.h"
#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>
#include <boost/lexical_cast.hpp>

using namespace std;

void Task35::Initialize() {
  workers_.clear();

  statistics_.Reset();

  unsigned finish_file_number = start_file_number_;
  finish_file_number += number_of_files_;

  unsigned curr_file_number = start_file_number_;
  while (curr_file_number < finish_file_number) {
    string file_number;
    try {
      file_number = boost::lexical_cast<string>(curr_file_number);
    } catch (const boost::bad_lexical_cast& ex) {
      cout << "Task35::Initialize(): Error of conversion " << curr_file_number
        << " into string. Reason: " << ex.what() << endl;
      ++curr_file_number;
      continue;
    } // try , catch

    string in_fname = "Input_";
    in_fname.append(file_number);
    in_fname.append(".txt");

	  try {
	    boost::shared_ptr<MessagesProcessor> pmp(new MessagesProcessor());
	    pmp->set_in_fname(in_fname);
	    pmp->set_statistics(&statistics_);
	    workers_.push_back(pmp);
	  } catch (const bad_alloc& ex) {
	    cout << "Task35::Initialize(): Insufficient memory. "
		    << "Details: " << ex.what() << endl;
	    ++curr_file_number;
	    continue;
	  } // try , catch

    ++curr_file_number;
  }
}

void Task35::Perform() {

  cout << "Task35::Perform has started. " << endl;

  for (size_t i = 0; i < workers_.size(); ++i)
    workgroup_.create_thread(boost::bind(&MessagesProcessor::Workflow, workers_[i]));

  workgroup_.join_all();

  cout << "Task34::Perform has finished.\n";
}
