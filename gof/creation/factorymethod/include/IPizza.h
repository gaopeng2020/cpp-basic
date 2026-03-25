#pragma once
class IPizza
{
public:
	//准备原材料, 不同的披萨不一样，因此，我们做成抽象方法
	virtual void prepare() = 0;
	virtual void bake() = 0;
	virtual void cut() = 0;
	virtual void box() = 0;
	virtual ~IPizza() {};
};