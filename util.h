#ifndef UTILS_H
#define UTILS_H

#include<filesystem.hpp>
#include<fstream>
#include<string>
#include<chrono>

namespace fsim{
	typedef char s8;
	typedef short s16;
	typedef int s32;
	typedef long long s64;

	typedef unsigned char u8;
	typedef unsigned short u16;
	typedef unsigned int u32;
	typedef unsigned long long u64;

	class GameManager;

	inline s64 getTime(){return (s64)(std::chrono::system_clock::now().time_since_epoch()/std::chrono::milliseconds(1));}
	bool isSuchFile(boost::filesystem::path);
	void getRecDirs(boost::filesystem::path,std::vector<std::string>&,bool,bool,bool);
	void readFile(std::string,std::vector<std::string>&,int=0,int=-1); 
	void writeFile(std::string,std::vector<std::string>&);
	std::string convert(std::string);
	int* fromStringToInt(std::string);
	std::string fromIntToString(int[3]);
	void makeTitleScreenButtons(GameManager*);
}

#endif
