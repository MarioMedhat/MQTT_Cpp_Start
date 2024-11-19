#ifndef JSON_DATA_RW_H
#define JSON_DATA_RW_H

#include <string>

void writeJsonFile(const std::string& filepath);
void readJsonFile(const std::string& filepath);
std::string JsonDataToString (const std::string filepath);
void updateJsonFile(const std::string& filepath, std::string field, int val);

#endif