// sketcher_save_proto.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"

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

CSaveData::CSaveData(){
	fp = NULL;
}
CSaveData::~CSaveData(){
	close();
}
void CSaveData::close(){
	if(fp != NULL)
		fclose(fp);
	fp = NULL;
}
bool CSaveData::open(const char *file){
	fp = fopen(file,"rb+");
	parse();
	return true;
}
CSaveData *CSaveData::create(const char *file){
	CSaveData *cd = new CSaveData();
	cd->open(file);
	return cd;
}
void CSaveData::parse(){
	int n;

	fread(&n,sizeof(int),1,fp);
	printf("data size %d\n", n);

	/*
	DataStruct

	count of data (4b)
	--Data
	----length of name(4b)
	----name
	----data type(4b)
	----length of data(4b)
	----data
	*/

	for(int i=0;i<n;i++){
		int len,type;
		char name[32];
		void *data;
		fread(&len,sizeof(int),1,fp);
		fread(name,sizeof(char),len,fp);
		name[len] = '\0';
		fread(&type,sizeof(int),1,fp);
		fread(&len,sizeof(int),1,fp);
		data = malloc(len);
		fread(data,sizeof(len),1,fp);

		Datum d;
		d.len = len;
		d.data = data;
		this->data[string(name)] = d;

		printf("load - %s\n", name);
	}
}
void *CSaveData::get(const char *name){
	return this->data[string(name)].data;
}
void CSaveData::set(const char *name,void *data,int len){
	Datum d;
	d.data = data;
	d.len = len;

	this->data[string(name)] = d;
}
void CSaveData::save(){
	map<string,Datum>::iterator it = data.begin();

	fseek(fp,0,SEEK_SET);

	int size = data.size();
	fwrite(&size,sizeof(int),1,fp);

	for(;it!=data.end();++it){
		int len,type;
		char name[32];
		void *data;

		len = it->first.length();
		fwrite(&len,sizeof(int),1,fp);

		sprintf(name,it->first.c_str());
		fwrite(name,sizeof(char),len,fp);

		type = 1;
		fwrite(&type,sizeof(int),1,fp);

		len = it->second.len;
		fwrite(&len,sizeof(int),1,fp);
		
		data = it->second.data;
		fwrite(data,sizeof(len),1,fp);
	}
}


int _tmain(int argc, _TCHAR* argv[])
{
	CSaveData *cd;
	cd = CSaveData::create("a.txt");

	int a = 5;
	int c = 7;
	int b;
	cd->set("asdf",&a,4);
	cd->set("asdf2",&c,4);
	b = *(int *)cd->get("asdf2");

	a = 6;
	c = 452;
	cd->save();
	return 0;
}

