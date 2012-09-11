#ifndef SG_SAVEDATA_H
#define SG_SAVEDATA_H

#ifdef NEVER_EXIST

#include <stdio.h>
#include <map>
#include <string>
#include <vector>

using namespace std;

/*
세이브 데이터 관리를 위한 클래스
*/
class CSaveData{
public:
	CSaveData();
	~CSaveData();

	/*
	지정한 파일에 대한 CSaveData 클래스를 생성한다.
	*/
	static CSaveData *create(const char *);

	/*
	해당 파일을 연다.
	*/
	bool open(const char *file);
	/*
	파일을 닫는다.
	*/
	void close();

	/*
	저장된 값을 가져온다.
	*/
	void *get(const char *name);
	/*
	값을 설정한다.
	*/
	void set(const char *name,void *data,int len);

	/*
	메모리의 데이터를 디스크에 기록한다.
	*/
	void save();

	enum DataType {
		INT,
		FLOAT,
		STRING
	};

private:

	void parse();

private:
	struct Datum{
		void *data;
		int len;
	};

	map<string,Datum> data;
	FILE *fp;
};

#endif

#endif // SG_SAVEDATA_H
