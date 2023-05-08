#include "TimedCall.h"


TimedCall::TimedCall(std::function<void(void)> f, uint32_t time)
{
	this->f = f;

	this->time_ = time;

	this->isFinished = false;
}

TimedCall::~TimedCall()
{
}

void TimedCall::Update()
{
	if (isFinished) {
		return;
	}
	time_--;
	if (time_ <= 0) {
		isFinished = true;
		f();
	}
}