#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <unistd.h>
#include <dirent.h>
#include <vector>
#include <algorithm>
#include "local_timer.h"
using namespace std;

//$CXX -g -std=c++11 test_dir.cpp local_timer.cpp -DLINUX -o test_dir
// scp -r test_dir root@192.168.16.84:/mnt/mmcblk1p3
/**
 * @function: 获取cate_dir目录下的所有文件名
 * @param: cate_dir - string类型
 * @result：vector<string>类型
*/

void getFiles(string cate_dir,vector<string>& files)
{
	DIR *dir;
	struct dirent *ptr;
	char base[1000];
 
	if ((dir=opendir(cate_dir.c_str())) == NULL)
	{
		perror("Open dir error...");
		exit(1);
	}
 
	while ((ptr=readdir(dir)) != NULL)
	{
		if(strcmp(ptr->d_name,".")==0 || strcmp(ptr->d_name,"..")==0)    ///current dir OR parrent dir
		        continue;
		else if(ptr->d_type == 8)    ///file
		{
			string tmp_file;
			files.push_back(tmp_file.assign(cate_dir).append("/").append(ptr->d_name));
#ifdef DEBUG
			std::cout<< tmp_file <<endl;
#endif
		}
		else if(ptr->d_type == 10)    ///link file
			continue;
		else if(ptr->d_type == 4)    ///dir
		{
			string tmp_dir;
			tmp_dir.assign(cate_dir).append("/").append(ptr->d_name);
#ifdef DEBUG
			std::cout<< tmp_dir << ":" <<endl << endl;
#endif
			getFiles(tmp_dir,files);
		}
	}
	
	closedir(dir);
	
	// std::sort(files.begin(), files.end(), std::less<string>());
	
	std::sort(files.begin(), files.end(), std::greater<string>());
}

int main(void)
{
    DIR *dir;
    char basePath[100];
	vector<string> files;
	local_timer gTick;

    ///Get the current absoulte path
    memset(basePath, '\0', sizeof(basePath));
    getcwd(basePath, 999);
	
	
	
	gTick.start();
    getFiles(basePath,files);
	gTick.out("elapse :");
	
	cout<< basePath <<":Nums of files:" << files.size() << endl;
	
#ifdef DEBUG
    for (auto & fn :files)
    {
    	cout<< fn <<endl;
    }
#endif
    return 0;
}