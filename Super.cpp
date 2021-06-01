#include "all.h"
/*
加载超级块，在超级块变成普通磁盘或程序加载时调用
*/
int SUPER::load() {
    //超级块变为普通块时，如果没有空闲块返回-1
    if (cur == -1) return -1;
    QDir temDir("./disk.txt");
    QString filePath = temDir.absolutePath();
    ifstream read(filePath.toStdString());
    qDebug()<<filePath<<endl;

	read.seekg(cur*DISK_SIZE);
    int cnt;
    read >> next >> cnt;
    Free_disk.clear();
	for (int i = 0; i < cnt; i++) {
		int p;
        read >> p;
		Free_disk.push_back(p);
	}
	read.close();
    cout<<"Super:"<<next<<" "<<cnt<<endl;
	return cur;
}


/*
初始化整个磁盘以及超级块
>> isclean - 默认值为0 ，非0值则会将所有信息抹除后重新构建磁盘块管理
*/
void SUPER::format(int isclean) {
	if (isclean) {
		for (int i = 0; i < MAXN_DISK; i++) {
            writeDisk("",i);
		}
	}
	stringstream ss;
    ss<<1<<" "<<MAXN_DISK-1;
    writeDisk(ss.str(),0);
    ss.str("");

	for (int i = 0; i < MAXN_DISK / MAXN_ZU; i++) {
		if (i == MAXN_DISK / MAXN_ZU - 1) ss << -1 << " " << MAXN_ZU;
		else ss << 1 + (i + 1)*MAXN_ZU << " " << MAXN_ZU;
		for (int j = 1 + i * MAXN_ZU; j < (i + 1)*MAXN_ZU + 1; j++) ss << " " << j;
		string str;
		getline(ss, str);
		ss.clear();
		writeDisk(str, 1 + i * MAXN_ZU);
	}
}

/*
保存当前超级块，写回到对应磁盘
*/
void SUPER::save() {
	stringstream stream;
	string str;
	stream << cur<<" "<<rest;
    getline(stream,str);
	writeDisk(str, 0);
	stream.clear();

    stream << next << " " << Free_disk.size();
    for (int i = 0; i < Free_disk.size(); i++) stream << " " << Free_disk[i];
	getline(stream, str);
	writeDisk(str, cur);
    //Free_disk.clear();
}

/*
申请分配磁盘块
>>返回值 - int 为分配到的磁盘号，-1代表分配失败
*/
int SUPER::alloc() {
    if(rest == 0) return -1;
    if (Free_disk.size() != 1) {
		int id = Free_disk.back();
        Free_disk.pop_back();
		rest--;
		return id; 
	}
	int id = Free_disk.back();
    Free_disk.pop_back();
	if (id != -1) {
		cur = next;
		rest--;
		load();
	}
	return id;
}

/*
释放指定磁盘块，加入到超级块的空闲磁盘队列
>> id - 指定磁盘块号
*/
void SUPER::free(int id) {
	rest++;
    if (Free_disk.size() < MAXN_ZU) {
		Free_disk.push_back(id);
        cout<<"free:"<<id<<endl;
		return;
	}
	save();
	next = cur;
	cur = id;
    Free_disk.clear();
	Free_disk.push_back(id);
    cout<<"free:"<<id<<endl;
}


