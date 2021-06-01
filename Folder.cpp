#include "all.h"

/*查看当前目录是否被用*/
bool FOLDER::isUsed() {
	return Folder_list.size() != 0;
}