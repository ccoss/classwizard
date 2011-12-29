#ifndef DISKINFO_H_
#define DISKINFO_H_
#include <iostream>
#include <string>
#include <string.h>
#include <sstream>

#define MAXPARTITIONNUM 32
#define DEVPATH "/dev"
#define COMTEXT "sd"
#define SIZEPATH "/sys/block/"

using std::string;

typedef struct DiskPartitionInfo
{
    string  strPartitionDevice;                                                 //主设备名称
    string  strDeviceSize;							//硬盘总大小
    string  strPartitionName;                                                   //分区名称
    string  strSystemType;                                                      //文件系统
    string  strSize;                                                            //分区大小
    string  strMountPoint;                                                      //挂载点
    string  strPartitionType;                                                   //分区类型
    string  strStart;                                                           //磁柱起始
    string  strEnd;                                                             //磁柱结束
    bool    isFlag;								//是否为启动分区
    int     nPrintfOrder;
}DiskPartitionInfo;


using std::string;
using std::cout;
using std::endl;

class DiskInfo
{
        private:
                string m_StrPartInfo[MAXPARTITIONNUM];                          //保存所有分区名称
                DiskPartitionInfo partition[MAXPARTITIONNUM];                   //保存分区信息
                int m_nPartitionNum;                                            //保存所有分区数量
                void getLineDeviceSize(char* buf);				//分析文件行,获得硬盘大小
                void getLinePartitionInfo(char* buf, string device, int order); //分析文件行，获得分区信息
                bool del(int n);

        public:
                DiskInfo()
                {
                        m_nPartitionNum = 0;
                }
                int getPartitionNum() const                                     //获得系统总分区数
                {
                        return m_nPartitionNum;
                }
                //can use
                bool getpartition();                                            //获得硬盘分区信息
                bool getPartition(char* s);                                     //解析硬盘分区配置文件
                void getPartitionInfo(string strDevice);			//获得硬盘信息
                string getRedues(string strStart, string strEnd);
                //can use end
                bool partitioninfo(DiskPartitionInfo &partition, int n);        //为partition传输分区名称
                bool getPartitionSize(string part, string &size);               //获得传入分区大小
                bool getFreeSpace(string &device, string &freeSpace);           //获得空余硬盘空间大小
                bool getAnewPartitionName(string &devic, string &newName);      //增加新分区
                void getLastFreeSize(int n, string &size);                      //获得最后一个分区后的剩余空间
                bool changeToMb(string &size);                                  //把传入大小转化为MB
                bool changeToGb(string &size);                                  //把传入大小转化为GB
                string addMtoM(string strStart, string strPartSize);            //
                void showSize(string &showSize);                                //转换成要显示的大小
                static int  stringtoint(string str, bool isChangeM);            //将string转化为int
                void inttostring(int n, string& size);                          //将int转化为string

                void showDiskInfo(string device)				//显示硬盘信息
                {
                    string strFreeSize;
                    DiskPartitionInfo current;
                    cout << "---------------------------------------------------------\n";
                    cout << "  device  |  type  |  mount  |  size |  falg  \n";
                    for(int i = 0; i < m_nPartitionNum; i ++)
                    {
                        for(int j = 0; j < m_nPartitionNum; j ++)
                        {
                            while((i != partition[j].nPrintfOrder) && (device == partition[j].strPartitionDevice))
                            {
                                j ++;
                            }
                            if(i > 0)
                            {
                                strFreeSize = getRedues(partition[j].strStart, current.strEnd);
                            }
                            if(strFreeSize != "0")
                            {
                                cout << "freeSize" << "                          " << strFreeSize << endl;
                            }
                                 //	cout << "  " << partition[j].strPartitionName
                                //    		<< "  |  " << partition[j].strSystemType
                                //	    	<< "  |  " << partition[j].strMountPoint
                                //		    << "  |  " << partition[j].strSize
                                //		    << "  |  " << partition[j].isFlag << endl;
                            memcpy((void*)&current, (void*)&partition[j], sizeof(current));
                            break;
                        }
                     }
                }
};
#endif
