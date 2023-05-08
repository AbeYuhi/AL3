#pragma once
#include<functional>

class TimedCall
{
public:
	TimedCall(std::function<void(void)> f, uint32_t time);
	~TimedCall();

	//更新
	void Update();

	inline bool IsFinished() { return isFinished; };

private:
	//コールバック
	std::function<void(void)>f;
	//残り時間
	uint32_t time_;
	//完了フラグ
	bool isFinished = false;
};

