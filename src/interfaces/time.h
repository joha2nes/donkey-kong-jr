#pragma once

class Time
{
public:
	virtual float GetElapsedTime() const = 0;
	virtual float GetUnscaledDeltaTime() const = 0;
	virtual void SetTimeScale(float scale) = 0;
	virtual float GetTimeScale() const = 0;
};