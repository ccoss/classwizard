#include <string>
extern "C"
{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
}
#include "diskinfo.h"

using namespace std;

bool DiskInfo::getpartition()
{
    m_nPartitionNum = 0;
    char fname[] = "/proc/partitions";
    char buf[256];
    m_nPartitionNum = 0;
    FILE* fp;
    fp = fopen(fname, "r");
    if(fp == NULL)
    {
        printf("open %s is wrong\n", fname);
        return -1;
    }
    memset(buf, '\0', 256);
    while(fgets(buf, 256, fp))
    {
        printf("renkaihao debug: buf = %s\n", buf );
        getPartition(buf);
        memset(buf, '\0', 256);
    }
    for(int i = 0; i < m_nPartitionNum; i ++)
    {
        if(partition[i].strPartitionName.size() == 3)
        {
            for(int j = 0; j < m_nPartitionNum; j ++)
            {
                if(i != j)
                {
                    if(partition[i].strPartitionName == partition[j].strPartitionDevice)
                    {
                        del(i);
                        break;
                    }
                }
            }
        }
    }
    if(m_nPartitionNum)
       return true;
    else
    {

        return false;
    }
//        m_nPartitionNum = 0;
//        DIR* dir_info;
//        struct dirent* dir_entry;
//        dir_info = opendir(DEVPATH);
//        if( dir_info )
//        {
//           while ( (dir_entry = readdir(dir_info)) != NULL)
//           {
//                  if(!strncmp(dir_entry->d_name, COMTEXT, 2))
//                  {
//                      partition[m_nPartitionNum].strPartitionName = dir_entry->d_name;
//                      partition[m_nPartitionNum].strPartitionDevice = partition[m_nPartitionNum].strPartitionName.substr(0, 3);
//
//                            m_nPartitionNum ++;
//                  }
//           }
//        }
//        for(int i = 0; i < m_nPartitionNum; i ++)
//        {
//            if(partition[i].strPartitionName.size() == 3)
//            {
//                for(int j = 0; j < m_nPartitionNum; j ++)
//                {
//                    if(i != j)
//                    {
//                        if(partition[i].strPartitionName == partition[j].strPartitionDevice)
//                        {
//                            del(i);
//                            break;
//                        }
//                    }
//                }
//            }
//        }
//        if(m_nPartitionNum)
//           return true;
//        else
//           return false;
}

bool DiskInfo::getPartition(char* s)
{
    if(s == NULL)
        return false;
    int nLen = strlen(s);
    if(nLen < 1)
        return false;
    //char *p = s;
   char *p = NULL;
    p = s;
    char *q = NULL;
    while(*p != '\n')
    {
        if(*p == '\000')
            break;
       // printf("renkaihao debgu: p = %s\n", p);
        if((*p == 's') || (*p == 'h'))
        {
           q = p;
           *p ++;
           if(*p == 'd')
           {
             *p ++;
             if((*p >= 'a') && (*p <= 'z'))
             {
               p ++;
               if((*p > '0') && (*p <= '9'))
               {
                   p ++;
                   if((*p >= '0') && (*p <= '9'))
                   {
                       p++;
                       *p = '\n';
                   }
                   *p = '\n';
               }
               *p = '\n';
               int nLen = strlen(q);
               partition[m_nPartitionNum].strPartitionName = q;
               if(nLen == 4)
               {
                   partition[m_nPartitionNum].strPartitionName = partition[m_nPartitionNum].strPartitionName.substr(0, 3);
               }
               else if(nLen == 5)
               {
                   partition[m_nPartitionNum].strPartitionName = partition[m_nPartitionNum].strPartitionName.substr(0, 4);
               }
               else if(nLen == 6)
               {
                   partition[m_nPartitionNum].strPartitionName = partition[m_nPartitionNum].strPartitionName.substr(0, 5);
               }
               partition[m_nPartitionNum].strPartitionDevice = partition[m_nPartitionNum].strPartitionName.substr(0, 3);


               m_nPartitionNum ++;
             }
           }
        }
       p ++;
    }
    p = NULL;
}

bool DiskInfo::del(int n)
{
    m_nPartitionNum --;
    if(n == m_nPartitionNum)
    { 
        return true;
    }
    else
    {
        for(int i = n; i < m_nPartitionNum; i ++)
        {
             partition[i].isFlag = partition[i + 1].isFlag;
             partition[i].nPrintfOrder = partition[i + 1].nPrintfOrder;
             partition[i].strDeviceSize = partition[i + 1].strDeviceSize;
             partition[i].strEnd = partition[i + 1].strEnd;
             partition[i].strMountPoint = partition[i + 1].strMountPoint;
             partition[i].strPartitionDevice = partition[i + 1].strPartitionDevice;
             partition[i].strPartitionName = partition[i + 1].strPartitionName;
             partition[i].strPartitionType = partition[i + 1].strPartitionType;
             partition[i].strSize = partition[i + 1].strSize;
             partition[i].strStart = partition[i + 1].strStart;
             partition[i].strSystemType = partition[i + 1].strSystemType;
        }
        return true;
    }
}

void DiskInfo::getPartitionInfo(string strDevice)
{
        //清除旧的磁盘信息
        for(int i = 0; i < m_nPartitionNum; i ++)
        {
            partition[i].strEnd.clear();
            partition[i].strMountPoint.clear();
            //partition[i].strPartitionDevice.clear();
            //partition[i].strPartitionName.clear();
            partition[i].strPartitionType.clear();
            partition[i].strSize.clear();
            partition[i].strStart.clear();
            //partition[i].strSystemType.clear();
        }
        //
        FILE* fp;
        int nLen = 256;
        int nReadFileLine = 0;
        int nPrintOrder = 0;
        string strDeviceFile = "/etc/gaea/scripts/" + strDevice + ".txt";
        cout << strDeviceFile << endl;
        char buf[nLen];
        fp = fopen(strDeviceFile.c_str(), "r");
        if(fp == NULL)
        {
                cout << "no strDeviceFile" << endl;
                return;
        }
        fseek(fp, 0, SEEK_SET);
        memset(buf, '\0', 256);

        while(fgets(buf, nLen, fp))
        {
                switch (nReadFileLine)
                {
                        case 0:
                                break;
                        case 1:
                                if((buf[0] == 'D') && (buf[1] == 'i') && (buf[2] == 's') && (buf[3] == 'k'))
                                {
                                        getLineDeviceSize(buf);
                                }
                                break;
                        case 6:
                                getLinePartitionInfo(buf, strDevice, nPrintOrder);
                                nPrintOrder ++;
                                break;
                        default:
                                if(nReadFileLine > 6)
                                {
                                        getLinePartitionInfo(buf, strDevice, nPrintOrder);
                                        nPrintOrder ++;
                                }
                                break;
                }
                nReadFileLine ++;
                memset(buf, '\0', 256);
        }

}

void DiskInfo::getLineDeviceSize(char* buf)
{
        char cSize[20];
        char cDevice[20];
        char *p = buf;
        int  nSizeLen = 0;
        int  nDeviceLen = 0;
        memset(cSize, '\0', 20);
        memset(cDevice, '\0', 20);

        p ++;
        p ++;
        p ++;
        p ++;

        for(int i = 0; i < strlen(buf); i ++)
        {
                if(*p == 's')
                {
                        while(*p != ':')
                        {
                                cDevice[nDeviceLen ++] = *p ++;
                        }
                }
                else if(*p == ':')
                {
                        p ++;
                        p ++;
                        while(*p != 'B')
                        {
                                cSize[nSizeLen ++] = *p ++;
                        }
                        cSize[nSizeLen ++] = *p;
                }
                else
                        p ++;
        }

        for(int i = 0; i < m_nPartitionNum; i ++)
        {
                if(!strcmp(cDevice, partition[i].strPartitionDevice.c_str()))
                {
                        partition[i].strDeviceSize = cSize;
                }
        }
}

void DiskInfo::getLinePartitionInfo(char* buf, string device, int order)
{
        char cPartitionNum[20];
        char cStart[20];
        char cEnd[20];
        char cSize[20];
        char cPartType[20];
        char cSystemType[20];
        bool isFlag = false;

        int  nPartitionNumLen = 0;
        int  nStrarLen = 0;
        int  nEndLen = 0;
        int  nSizeLen = 0;
        int  nPartTypeLen = 0;
        int  nsystemTypeLen = 0;

        char cTmp[256];
        int  nReadTime = 0;

        memset(cTmp, '\0', 256);
        memset(cPartitionNum, '\0', 20);
        memset(cStart, '\0', 20);
        memset(cEnd, '\0', 20);
        memset(cSize, '\0', 20);
        memset(cPartType, '\0', 20);
        memset(cSystemType, '\0', 20);
        strcpy(cTmp, buf);
        if(strlen(cTmp) < 10)
                return;

        int i = 0;

        while(cTmp[i] != '\0')
        {
                if((cTmp[i] != ' ') && (cTmp[i] != '\0'))
                {
                        switch (nReadTime)
                        {
                                case 0:
                                        while((buf[i] != ' ') && (buf[i] != '\t'))
                                        {
                                                cPartitionNum[nPartitionNumLen ++] = buf[i ++];
                                        }
                                        nReadTime ++;
                                        break;
                                case 1:
                                        while((buf[i] != ' ') && (buf[i] != '\0'))
                                        {
                                                cStart[nStrarLen ++] = buf[i ++];
                                        }
                                        nReadTime ++;
                                        break;
                                case 2:
                                        while((buf[i] != ' ') && (buf[i] != '\0'))
                                        {
                                                cEnd[nEndLen ++] = buf[i ++];
                                        }
                                        nReadTime ++;
                                        break;
                                case 3:
                                        while((buf[i] != ' ') && (buf[i] != '\0'))
                                        {
                                                cSize[nSizeLen ++] = buf[i ++];
                                        }
                                        nReadTime ++;
                                        break;
                                case 4:
                                        while((buf[i] != ' ') && (buf[i] != '\0'))
                                        {
                                                cPartType[nPartTypeLen ++] = buf[i ++];
                                        }
                                        nReadTime ++;
                                        break;
                                case 5:
                                        while((buf[i] != ' ') && buf[i] != '\0')
                                        {
                                                cSystemType[nsystemTypeLen ++] = buf[i ++];
                                        }
                                        nReadTime ++;
                                        break;
                                case 6:
                                        while(buf[i] != ' ')
                                        {
                                                if(buf[i] == 'b')
                                                {
                                                        isFlag = true;
                                                }
                                                i ++;
                                        }
                                        nReadTime ++;
                                        break;
                                default:
                                        nReadTime ++;
                                        break;
                        }
                }
                i ++;
        }

        string strPartNum = cPartitionNum;
        string strDeviceName = device + strPartNum;

        if(strDeviceName.size() < 4)
        {
                cout << "return" << endl;
                return;
        }

        for( int i = 0; i < m_nPartitionNum; i ++)
        {
                if(strDeviceName == partition[i].strPartitionName)
                {
                        partition[i].strSystemType = cSystemType;
                        partition[i].strSize = cSize;
                        partition[i].strPartitionType = cPartType;
                        partition[i].isFlag = isFlag;
                        partition[i].strStart = cStart;
                        partition[i].strEnd = cEnd;
                        partition[i].nPrintfOrder = order;
                }
        }
}

bool DiskInfo::partitioninfo(DiskPartitionInfo &part, int n)
{
        if( n > m_nPartitionNum )
        {
                std::cout << "no have %d partition!\n";
                return false;
        }
        if( !m_nPartitionNum )
        {
                std::cout << "please exec getpartition() or no partition\n";
        }
        if(partition[n].strPartitionDevice.size() != 0)
            part.strPartitionDevice = partition[n].strPartitionDevice;
        if(partition[n].strPartitionName.size() != 0)
            part.strPartitionName = partition[n].strPartitionName;
        if(partition[n].strDeviceSize.size() != 0)
            part.strDeviceSize = partition[n].strDeviceSize;
        if(partition[n].strSize.size() != 0)
            part.strSize = partition[n].strSize;
        if(partition[n].strStart.size() != 0)
            part.strStart = partition[n].strStart;
        if(partition[n].strEnd.size() != 0)
            part.strEnd = partition[n].strEnd;
        if(partition[n].strPartitionType.size() != 0)
            part.strPartitionType = partition[n].strPartitionType;
        if(partition[n].strSystemType.size() != 0)
            part.strSystemType = partition[n].strSystemType;
        if(partition[n].strMountPoint.size() != 0)
            part.strMountPoint = partition[n].strMountPoint;
        part.nPrintfOrder = partition[n].nPrintfOrder;
        part.isFlag = partition[n].isFlag;
        return true;
}

bool DiskInfo::getPartitionSize(string partition, string &size)
{
        FILE* fp;
        char ch;
        string sizePath;
        string strTempSize;

        if(partition.size() == 3)
        {
            sizePath = "/sys/block/" + partition + "/size";
        }
        else
        {
            string tmpPart = partition.substr(0, 3);
            sizePath = "/sys/block/" + tmpPart + "/" + partition + "/size";
        }
        fp = fopen(sizePath.c_str(), "r");
        if(fp == NULL)
        {
                return false;
        }
        else
        {
                while((ch=getc(fp))!=EOF)
                {
                    if(ch == '.')
                        break;
                        strTempSize += ch;
                }
        }
        int tempSize = stringtoint(strTempSize, true);
        inttostring(tempSize, size);
        //size += ";
        return true;
}

bool DiskInfo::getFreeSpace(string &device, string &freeSpace)
{
    int nUseSpace = 0;
    string strSize;
    for(int i = 0; i < m_nPartitionNum; i ++)
    {
        if((m_StrPartInfo[i].substr(0,3) == device) && (m_StrPartInfo[i].size() != 3))
        {
            getPartitionSize(m_StrPartInfo[i], strSize);
            nUseSpace += stringtoint(strSize, false);
            strSize.clear();
        }
    }
    getPartitionSize(device, strSize);
    int nAllSpace;
    char buf[20];
    strcpy(buf, strSize.c_str());
    sscanf(buf, "%d", &nAllSpace);
    int nFreeSpace = nAllSpace - nUseSpace;
    if(nFreeSpace < 0)
        freeSpace = "0";
    else
    {
        inttostring(nFreeSpace, freeSpace);
    }
    return true;
}

int DiskInfo::stringtoint(string str, bool isChangeM)
{
    int i;
    char buf[20];
    strcpy(buf, str.c_str());
    sscanf(buf, "%d", &i);
    if(isChangeM)
         i = (long long)i*512/(1024*1024);
    return i;
}

void DiskInfo::inttostring(int n, string& size)
{
   stringstream strStream;
   strStream << n;
   size = strStream.str();
}

string DiskInfo::getRedues(string strStart, string strEnd)
{
        string strReturn = "no Size";
        if((strStart.size() == 0) || (strEnd.size() == 0))
            return strReturn;
	int nFreeSize = 0;
	string strFreeSize = "0";
	int nStartLen = strStart.size();
	string strStartSize = strStart.substr(0, nStartLen - 2);
	string strStartUnits = strStart.substr(nStartLen - 2, 1);

	int nEndLen = strEnd.size();
	string strEndSize = strEnd.substr(0, nEndLen - 2);
	string strEndUnits = strEnd.substr(nEndLen - 2, 1);



	int nStartSize = stringtoint(strStartSize, false);
	int nEndSize = stringtoint(strEndSize, false);

	if(strStartUnits == strEndUnits)
	{
                if(nStartSize - nEndSize > 1)
		{
			nFreeSize = nStartSize - nEndSize;
			inttostring(nFreeSize, strFreeSize);
			strFreeSize += strStartUnits;
		}
		else
		{
			if(strStartUnits == "G")
			{
				nStartSize *= (1024*1024);
			}
			else if(strStartUnits == "M")
			{
				nStartSize *= 1024;
			}

			if(strEndUnits == "G")
			{
				nEndSize *= (1024*1024);
			}
			else if(strEndUnits == "M")
			{
				nEndSize *= 1024;
			}
		}
		if(nStartSize - nEndSize > 1024*1024*100)
		{
			nFreeSize = nStartSize - nEndSize;
			nFreeSize = nFreeSize*1024;
			inttostring(nFreeSize, strFreeSize);
			strFreeSize += "M";
		}
	}
	return strFreeSize;
}

void DiskInfo::getLastFreeSize(int n, string &size)
 {
    if(n < 0)
        return;
    if(partition[n].strPartitionName.size() == 3)
    {
        size = partition[n].strDeviceSize;
        return;
    }
    if(!partition[n].strDeviceSize.size())
    {
        cout << "string Device Size no" << endl;
        return;
    }
    if(!partition[n].strEnd.size())
    {
        cout << "string end Size no" << endl;
        return;
    }
     int nFreeSize = 0;
     string strDiskUnits;
     string strPartUnits;
     string strDiskSize = partition[n].strDeviceSize;
     string strEndSize = partition[n].strEnd;

     int nDiskSizeLen = strDiskSize.size();
     int nEndSizeLen = strEndSize.size();

     strDiskUnits = strDiskSize.substr(nDiskSizeLen - 2, 2);
     strPartUnits = strEndSize.substr(nEndSizeLen - 2, 2);

     strDiskSize = strDiskSize.substr(0, nDiskSizeLen);
     strEndSize = strEndSize.substr(0, nEndSizeLen);

     int nDiskSize = stringtoint(strDiskSize, false);
     int nEndSize = stringtoint(strEndSize, false);
     if(strDiskUnits == strPartUnits)
     {
         if(nDiskSize - nEndSize > 1)
         {
             nFreeSize = (long long)nDiskSize - (long long)nEndSize;
             inttostring(nFreeSize, size);
             size += strDiskUnits;
         }
         else
             size = "0";
         return;
    }
    else
    {
        if(strDiskUnits == "GB")
        {
            nDiskSize *= 1024;
        }
        if(strDiskUnits == "TB")
        {
            nDiskSize *= 1024;
            nDiskSize *= 1024;
        }
        if(nDiskSize - nEndSize > 1)
        {
            nFreeSize = nDiskSize - nEndSize;
            inttostring(nFreeSize, size);
            size += "MB";
        }
        else
            size = "0";

    }
 }

bool DiskInfo::getAnewPartitionName(string &device, string &newName)
{
    int nDevicePart = 0;
    int nSize = 1;
    int nTempSize;
    bool isNewName = false;
    bool isEqual = true;
    int nDeviceNum[MAXPARTITIONNUM];
    string strDeviceNum;
    for(int i = 0; i < MAXPARTITIONNUM; i ++)
    {
        nDeviceNum[i] = 0;
    }
    //
    while(!isNewName)
    {
        isEqual = true;
        for(int i = 0; i < m_nPartitionNum; i ++)
        {
            if((m_StrPartInfo[i].substr(0, 3) == device) && (m_StrPartInfo[i].size() != 3))
            {
                strDeviceNum = m_StrPartInfo[i].substr(3,1);
                nTempSize = stringtoint(strDeviceNum, false);
                if(nSize == nTempSize)
                {
                    nSize ++;
                    isEqual = false;
                    break;
                }
            }
        }
        if(isEqual)
        {
            m_StrPartInfo[m_nPartitionNum ++];
            inttostring(nSize, strDeviceNum);
            newName = device + strDeviceNum;
            m_StrPartInfo[m_nPartitionNum ++] = newName;
            isNewName = true;
        }
    }
    return true;
}

bool DiskInfo::changeToGb(string &size)
{

}

bool DiskInfo::changeToMb(string &size)
{
    string strSize;
    string strUtils;
    string strPointBack;
    int    nPointBack = 0;
    int nSizeLen = size.size();
    int nSize;
    strSize = size.substr(0, nSizeLen - 2);
    strUtils = size.substr(nSizeLen - 2, 1);

    int nPoint = strSize.find('.');
    if(nPoint > 0)
    {
        strPointBack = strSize.substr(nPoint + 1, nSizeLen);
        nPointBack = stringtoint(strPointBack, false);
        if(strUtils == "G")
        {
            nPointBack *= 1000;
            nPointBack /= 10;
            nPointBack += 49;
        }
    }

    if(strUtils == "M")
        return true;
    if(strUtils == "G")
    {
        nSize = stringtoint(strSize, false);
        nSize *= 1000;
        nSize += nPointBack;
        nSize += 1;
        inttostring(nSize, size);
        size += "MB";
        return true;
    }
    if(strUtils == "K")
    {
        nSize = stringtoint(strSize, false);
        nSize /= 1000;
        inttostring(nSize, size);
        return true;
    }
    return false;
}

void DiskInfo::showSize(string &showSize)
{
    string strSize;
    string strUtils;
    int nSizeLen = showSize.size();
    strSize = showSize.substr(0, nSizeLen - 2);
    strUtils = showSize.substr(nSizeLen - 2, 1);

    int nShowSize = stringtoint(strSize, false);
    int nPointSize = 0;
    string strPointSize;
    if((strUtils == "M") && (nShowSize > 1000))
    {
        nPointSize = nShowSize%1000;
        nPointSize /= 100;
        nShowSize /= 1000;
        strSize.clear();
        inttostring(nShowSize, strSize);
        inttostring(nPointSize, strPointSize);
        strSize += ".";
        strSize += strPointSize;
        strSize += "GB";
        showSize.clear();
        showSize = strSize;
    }
}

string DiskInfo::addMtoM(string strStart, string strPartSize)
{
    int    nStartLen = strStart.size();
    int    nStartSize;
    int    nPartSize;
    int    nEndSize;
    string strEndSize;
    string strStartSize = strStart.substr(0, nStartLen - 2);

    nStartSize = stringtoint(strStartSize, false);
    nPartSize = stringtoint(strPartSize, false);
    nEndSize = nStartSize + nPartSize;
    inttostring(nEndSize, strEndSize);
    return strEndSize;
}

