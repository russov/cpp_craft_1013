#ifndef _MINUTE_CALCULATOR_PROCESSOR_H_
#define _MINUTE_CALCULATOR_PROCESSOR_H_

namespace minute_calculator
{
	class minute_calculator_process
	{
	public:
		explicit minute_calculator_process();
		~minute_calculator_process();

	protected:
		void create_thread();

	};
}

#endif //_MINUTE_CALCULATOR_PROCESSOR_H_