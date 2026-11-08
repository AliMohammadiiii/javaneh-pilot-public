

#include <fstream>
#include "kclient.h"
#include <iostream>

using namespace std;

KukClient::KukClient() {
       this->context = redisConnect("127.0.0.1", 6379);
       if (context == nullptr || context->err) {
              if (context) {
              std::cerr << "Error: " << context->errstr << std::endl;
              redisFree(context);
              } else {
              std::cerr << "Can't allocate redis context" << std::endl;
              }
              exit(1);
       }
       else
       {
              redisReply* ping_reply = (redisReply*)redisCommand(this->context, "PING");
              if (ping_reply == nullptr) {
              std::cerr << "Ping error: " << this->context->errstr << std::endl;
              } else {
              std::cout << "PING Response: " << ping_reply->str << std::endl;
              freeReplyObject(ping_reply);
              }
       }
}      

KukClient::~KukClient() {
    if (context != nullptr) {
        redisFree(context);
    }
}

std::string KukClient::get(std::string path){
       redisReply* reply = (redisReply*)redisCommand(context, "GET %s", path.c_str());
       if (reply == nullptr) {
              std::cerr << "Get error: " << context->errstr << std::endl;
              return "";
       }
       std::string value = reply->str ? reply->str : "";
       freeReplyObject(reply);
       return value;
}



std::vector<int> KukClient::get(std::string path[], int size){
       std::string command = "MGET";
       for (int i = 0; i < size ; i++) {
              command += " " + path[i];
       }
       redisReply* reply = (redisReply*)redisCommand(context, command.c_str());
       if (reply == nullptr) {
              std::cerr << "MGet error: " << context->errstr << std::endl;
              return {};
       }
       std::vector<int> values;
       if (reply->type == REDIS_REPLY_ARRAY) {
              for (size_t i = 0; i < reply->elements; ++i) {
              values.push_back(reply->element[i]->str ? stoi(reply->element[i]->str) : 0);
              }
       }
       freeReplyObject(reply);
       return values;
}
std::vector<int> KukClient::get(std::vector<std::string> path, int size){
       std::string command = "MGET";
       for (const auto& key : path) {
              command += " " + key;
       }
       redisReply* reply = (redisReply*)redisCommand(context, command.c_str());
       if (reply == nullptr) {
              std::cerr << "MGet error: " << context->errstr << std::endl;
              return {};
       }
       std::vector<int> values;
       if (reply->type == REDIS_REPLY_ARRAY) {
              for (size_t i = 0; i < reply->elements; ++i) {
                     // cout << reply->element[i]->str << endl;
                     if (reply->element[i]->str == "")
                     {
                            values.push_back(0);
                     }
                     else
                     {
                            values.push_back(reply->element[i]->str ? stoi(reply->element[i]->str) : 0);
                     }
              }
       }
       freeReplyObject(reply);
       return values;
}


bool KukClient::set(std::string path, int value){
       if(!set(path, to_string(value)))
       {
              return false;
       }
       return true; 
}

bool KukClient::set(std::string path[], int value[], int size){
       for(int i=0; i < size; i++){
              if(!set(path[i], to_string(value[i])))
              {
                     return false;
              }
       }
       return true;   
}

bool KukClient::set(std::vector<std::string> path, std::vector<int> value, int size){
       for(int i=0; i < size; i++){
              if(!set(path[i], to_string(value[i])))
              {
                     return false;
              }
       }
       return true;    
}

bool KukClient::set(std::string path, float value){
       if(!set(path, to_string(value)))
       {
              return false;
       }
       return true;  
}

bool KukClient::set(std::string path[], float value[], int size){
       for(int i=0; i < size; i++){
              if(!set(path[i], to_string(value[i])))
              {
                     return false;
              }
       }
       return true;  
}

bool KukClient::set(std::string path, double value){
       if(!set(path, to_string(value)))
       {
              return false;
       }
       return true;   
}

bool KukClient::set(std::string path[], double value[], int size){
       for(int i=0; i < size; i++){
              if(!set(path[i], to_string(value[i])))
              {
                     return false;
              }
       }
       return true;   
}

bool KukClient::set(const std::string& key, const std::string& value){
       
       if (this->context == nullptr) {
              std::cerr << "Error: Redis context is not initialized." << std::endl;
              return false;
       }

       std::string command = "SET " + key + " " + value;
       // std::cout << "Executing command: " << command.c_str() << std::endl;

       redisReply* reply = (redisReply*)redisCommand(this->context, command.c_str());
       if (reply == nullptr) {
              std::cerr << "Write error: " << context->errstr << std::endl;
              return false;
       }

       if (reply->type == REDIS_REPLY_ERROR) {
              std::cerr << "Error: " << reply->str << std::endl;
              freeReplyObject(reply);
              return false;
       }

       std::cout << "SET: " << key << " -> " << value << std::endl;
       freeReplyObject(reply);
       return true;
}

void KukClient::psubscribe(const std::string& channel, MessageCallback callback)
{
       std::thread([this, channel, callback]() {
        redisReply* reply = (redisReply*)redisCommand(context, "PSUBSCRIBE %s", channel.c_str());
        if (reply == nullptr) {
            std::cerr << "Subscribe error: " << context->errstr << std::endl;
            return;
        }
        std::cout << "Subscribed to channel: " << channel << std::endl;
        freeReplyObject(reply);
        while (redisGetReply(context, (void**)&reply) == REDIS_OK) {
            
            std::cout << reply->type << " " << reply->elements << std::endl;

            if (reply == nullptr) {
                std::cerr << "Subscription error: " << context->errstr << std::endl;
                break;
            }
            if (reply->type == REDIS_REPLY_ARRAY && reply->elements == 4) {
                if (strcmp(reply->element[0]->str, "pmessage") == 0) {
                    std::string received_pattern = reply->element[1]->str;
                    std::string received_channel = reply->element[2]->str;
                    std::string message = reply->element[3]->str;
                    callback(received_channel, message);
                }
            }
            freeReplyObject(reply);
        }
    }).detach();
}


void KukClient::psubscribe(const std::vector<std::string>& patterns, MessageCallback callback) {
    std::thread([this, patterns, callback]() {
        std::string command = "PSUBSCRIBE";
        for (const auto& pattern : patterns) {
            command += " " + pattern;
        }
        redisReply* reply = (redisReply*)redisCommand(context, command.c_str());
        if (reply == nullptr) {
            std::cerr << "PSubscribe error: " << context->errstr << std::endl;
            return;
        }
        std::cout << "Subscribed to patterns: ";
        for (const auto& pattern : patterns) {
            std::cout << pattern << " ";
        }
        std::cout << std::endl;
        freeReplyObject(reply);

        while (redisGetReply(context, (void**)&reply) == REDIS_OK) {
            if (reply == nullptr) {
                std::cerr << "Subscription error: " << context->errstr << std::endl;
                break;
            }
            if (reply->type == REDIS_REPLY_ARRAY && reply->elements == 4) {
                if (strcmp(reply->element[0]->str, "pmessage") == 0) {
                    std::string received_pattern = reply->element[1]->str;
                    std::string received_channel = reply->element[2]->str;
                    std::string message = reply->element[3]->str;
                    callback(received_channel, message);
                }
            }
            freeReplyObject(reply);
        }
    }).detach();
}


void KukClient::subscribe(const std::string& channel, MessageCallback callback)
{
       std::thread([this, channel, callback]() {
        redisReply* reply = (redisReply*)redisCommand(context, "SUBSCRIBE %s", channel.c_str());
        if (reply == nullptr) {
            std::cerr << "Subscribe error: " << context->errstr << std::endl;
            return;
        }
        std::cout << "Subscribed to channel: " << channel << std::endl;
        freeReplyObject(reply);
        while (redisGetReply(context, (void**)&reply) == REDIS_OK) {
            
            std::cout << reply->type << " " << reply->elements << std::endl;

            if (reply == nullptr) {
                std::cerr << "Subscription error: " << context->errstr << std::endl;
                break;
            }
            if (reply->type == REDIS_REPLY_ARRAY && reply->elements == 3) {
                if (strcmp(reply->element[0]->str, "pmessage") == 0) {
                    std::string received_channel = reply->element[1]->str;
                    std::string message = reply->element[2]->str;
                    callback(received_channel, message);
                }
            }
            freeReplyObject(reply);
        }
    }).detach();
}

void KukClient::publish(const std::string& channel, const std::string& message) {
    redisReply* reply = (redisReply*)redisCommand(context, "PUBLISH %s %s", channel.c_str(), message.c_str());
    if (reply == nullptr) {
        std::cerr << "Publish error: " << context->errstr << std::endl;
        return;
    }
    std::cout << "Published message: " << message << " to channel: " << channel << std::endl;
    freeReplyObject(reply);
}

bool KukClient::setPublish(std::string path, int value)
{
       publish(path, to_string(value));
       return set(path, value);
}

bool KukClient::setPublish(std::string path[], int value[], int size)
{
       for (int i = 0; i < size; i++)
       {
              publish(path[i], to_string(value[i]));
       }
       if (!set(path, value, size))
              return false;
       return true;
}


bool KukClient::setPublish(std::vector<std::string> path, std::vector<int> value, int size)
{
       for (int i = 0; i < size; i++)
       {
              publish(path[i], to_string(value[i]));
       }
       if (!set(path, value, size))
              return false;
       return true;
}


bool KukClient::setPublish(std::string path, float value)
{
       publish(path, to_string(value));
       return set(path, value);
}

bool KukClient::setPublish(std::string path[], float value[], int size)
{
       for (int i = 0; i < size; i++)
       {
              publish(path[i], to_string(value[i]));
       }
       if (!set(path, value, size))
              return false;
       return true;
}


bool KukClient::setPublish(std::string path, double value)
{
       publish(path, to_string(value));
       return set(path, value);
}

bool KukClient::setPublish(std::string path[], double value[], int size)
{
       for (int i = 0; i < size; i++)
       {
              publish(path[i], to_string(value[i]));
       }
       if (!set(path, value, size))
              return false;
       return true;
}