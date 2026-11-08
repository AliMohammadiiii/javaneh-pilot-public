#ifndef  KClient_H
#define KClient_H
#include <iostream>
#include <thread>
#include <functional>
#include <hiredis/hiredis.h>
#include <cstring>
#include <vector>

class KukClient
{
	private:
    	redisContext* context;
		
	public:
		
		KukClient();
		~KukClient();
		std::string get(std::string path);
		std::vector<int> get(std::string path[], int size);
		std::vector<int> get(std::vector<std::string> path, int size);

		bool set(std::string path, int value);
		bool setPublish(std::string path, int value);
		bool set(std::string path[], int value[], int size);
		bool setPublish(std::string path[], int value[], int size);
		bool set(std::vector<std::string> path, std::vector<int> value, int size);
		bool setPublish(std::vector<std::string> path, std::vector<int> value, int size);
		bool set(std::string path, float value);
		bool setPublish(std::string path, float value);
		bool set(std::string path[], float value[], int size);
		bool setPublish(std::string path[], float value[], int size);
		bool set(std::string path, double value);
		bool setPublish(std::string path, double value);
		bool set(std::string path[], double value[], int size);
		bool setPublish(std::string path[], double value[], int size);
		bool set(const std::string& key, const std::string& value);
		
    	using MessageCallback = std::function<void(const std::string&, const std::string&)>;
		void publish(const std::string& channel, const std::string& message);
    	void psubscribe(const std::string& channel, MessageCallback callback);
		void psubscribe(const std::vector<std::string>& patterns, MessageCallback callback);
    	void subscribe(const std::string& channel, MessageCallback callback);

};
#endif
