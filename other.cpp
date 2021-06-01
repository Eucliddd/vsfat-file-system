#include "all.h"

/*获取当前时间 格式：年-月-日-时-分-秒*/
string getTime() {
	time_t t = time(NULL);
	char ch[64] = { 0 };
	strftime(ch, sizeof(ch) - 1, "%Y-%m-%d-%H:%M", localtime(&t));     //年-月-日 时-分-秒
	return ch;
};


/*
写磁盘
>> str - 传入的需要写的字符串
>> id - 需要写入的磁盘号
*/
void writeDisk(string str, int id) {
    QDir temDir("./disk.txt");
    QString filePath = temDir.absolutePath();
    while (str.size() < DISK_SIZE) str.push_back('\0');
    //if(str.size()==DISK_SIZE-1) str.push_back('\0');
    FILE * write = fopen(filePath.toStdString().c_str(), "r+");
	fseek(write, id*DISK_SIZE, 0);
	fwrite(str.c_str(), DISK_SIZE, 1, write);
	fclose(write);

	str.clear();
}

