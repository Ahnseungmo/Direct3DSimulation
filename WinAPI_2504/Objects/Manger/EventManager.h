#pragma once

class EventManager : public Singleton<EventManager>
{
	friend class Singleton;

private:
	EventManager();
	~EventManager();

public:
	void AddEvent(const string& eventName, function<void(void*)> event)
	{
		//if (eventWithParams.count(eventName) > 0)
		//{
		//	return;
		//}

		eventWithParams[eventName].push_back(event);
	}

	void ExcuteEvent(const string& eventName, void* param)
	{
		if (eventWithParams.count(eventName) > 0)
		{
			for (function<void(void*)> event : eventWithParams[eventName])
			{
				event(param);
			}
		}
	}

private:
	//vector<function<void()>> events;
	unordered_map<string, vector<function<void(void*)>>> eventWithParams;
};