#include <stdafx.h>
#include "pc_info.h"
#include <winsock2.h>
#include <Iphlpapi.h>
#pragma  comment(lib,"Iphlpapi.lib")
#include <winsock2.h>
#pragma comment(lib,"ws2_32.lib")


void byte2Hex(unsigned char bData,unsigned char hex[])
{
    int high=bData/16, low =bData %16;
    hex[0] = (high <10)?('0'+high):('A'+high-10);
    hex[1] = (low <10)?('0'+low):('A'+low-10);
}


bool getLocalMac1(unsigned char *mac) //��ȡ����MACַ 
{
    ULONG ulSize=0;
    PIP_ADAPTER_INFO pInfo=NULL;
    int temp = GetAdaptersInfo(pInfo,&ulSize);//��һ�����ã���ȡ��������С
    pInfo=(PIP_ADAPTER_INFO)malloc(ulSize);
    temp = GetAdaptersInfo(pInfo,&ulSize);
    int iCount=0;
    while(pInfo)//����ÿһ������
    {
        // pInfo->Address MACַ
        for(int i=0;i<(int)pInfo->AddressLength;i++)
        {
            byte2Hex(pInfo->Address[i],&mac[iCount]);
            iCount+=2;
            if(i<(int)pInfo->AddressLength-1)
            {
                mac[iCount++] = ':';
            }
            else
            {
                mac[iCount++] = '#';
            }
        }
        break;
        pInfo = pInfo->Next;
    }
    free(pInfo);

    if(iCount >0)
    {
        mac[--iCount]='\0';
        return true;
    }
    else
    {
        return false;
    }
}


//ͨ��GetAdaptersInfo������������Windows 2000�����ϰ汾��
bool getLocalMac2(unsigned char *mac)
{
    bool ret = false;
    ULONG ulOutBufLen = sizeof(IP_ADAPTER_INFO);
    PIP_ADAPTER_INFO pAdapterInfo = (IP_ADAPTER_INFO*)malloc(sizeof(IP_ADAPTER_INFO));
    if(pAdapterInfo == NULL)
        return false;
    // Make an initial call to GetAdaptersInfo to get the necessary size into the ulOutBufLen variable
    if(GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW) 
    {
        free(pAdapterInfo);
        pAdapterInfo = (IP_ADAPTER_INFO *)malloc(ulOutBufLen);
        if (pAdapterInfo == NULL) 
            return false;
    }
    if(GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == NO_ERROR)
    {
        for(PIP_ADAPTER_INFO pAdapter = pAdapterInfo; pAdapter != NULL; pAdapter = pAdapter->Next)
        {
            // ȷ������̫��
            if(pAdapter->Type != MIB_IF_TYPE_ETHERNET)
                continue;
            // ȷ��MAC��ַ�ĳ���Ϊ 00-00-00-00-00-00
            if(pAdapter->AddressLength != 6)
                continue;
            char acMAC[20] = {0};
            sprintf(acMAC, "%02X:%02X:%02X:%02X:%02X:%02X",
                int (pAdapter->Address[0]),
                int (pAdapter->Address[1]),
                int (pAdapter->Address[2]),
                int (pAdapter->Address[3]),
                int (pAdapter->Address[4]),
                int (pAdapter->Address[5]));
            memcpy(mac,acMAC,20);
            ret = true;
            break;
        }
    }
    free(pAdapterInfo);
    return ret;
}


//ͨ��GetAdaptersAddresses������������Windows XP�����ϰ汾��
bool getLocalMac3(unsigned char *mac)
{
    bool ret = false;
    ULONG outBufLen = sizeof(IP_ADAPTER_ADDRESSES);
    PIP_ADAPTER_ADDRESSES pAddresses = (IP_ADAPTER_ADDRESSES*)malloc(outBufLen);
    if (pAddresses == NULL) 
        return false;
    // Make an initial call to GetAdaptersAddresses to get the necessary size into the ulOutBufLen variable
    if(GetAdaptersAddresses(AF_UNSPEC, 0, NULL, pAddresses, &outBufLen) == ERROR_BUFFER_OVERFLOW)
    {
        free(pAddresses);
        pAddresses = (IP_ADAPTER_ADDRESSES*)malloc(outBufLen);
        if (pAddresses == NULL) 
            return false;
    }

    if(GetAdaptersAddresses(AF_UNSPEC, 0, NULL, pAddresses, &outBufLen) == NO_ERROR)
    {
        // If successful, output some information from the data we received
        for(PIP_ADAPTER_ADDRESSES pCurrAddresses = pAddresses; pCurrAddresses != NULL; pCurrAddresses = pCurrAddresses->Next)
        {
            // ȷ��MAC��ַ�ĳ���Ϊ 00-00-00-00-00-00
            if(pCurrAddresses->PhysicalAddressLength != 6)
                continue;
            char acMAC[20] = {0};
            sprintf(acMAC, "%02X:%02X:%02X:%02X:%02X:%02X",
                int (pCurrAddresses->PhysicalAddress[0]),
                int (pCurrAddresses->PhysicalAddress[1]),
                int (pCurrAddresses->PhysicalAddress[2]),
                int (pCurrAddresses->PhysicalAddress[3]),
                int (pCurrAddresses->PhysicalAddress[4]),
                int (pCurrAddresses->PhysicalAddress[5]));
            memcpy(mac,acMAC,20);
            ret = true;
            break;
        }
    } 
    free(pAddresses);
    return ret;
}



bool GetLocalIP(char* ip)
{
    //1.��ʼ��wsa
    WSADATA wsaData;
    int ret=WSAStartup(MAKEWORD(2,2),&wsaData);
    if (ret!=0)
    {
        return false;
    }
    //2.��ȡ������
    char hostname[256];
    ret=gethostname(hostname,sizeof(hostname));
    if (ret==SOCKET_ERROR)
    {
        return false;
    }
    //3.��ȡ����ip
    HOSTENT* host=gethostbyname(hostname);
    if (host==NULL)
    {
        return false;
    }
    //4.ת��Ϊchar*����������
    strcpy(ip,inet_ntoa(*(in_addr*)*host->h_addr_list));
    return true;
}

// ��ȡMAC��ַ
std::string GetMAC()
{
    unsigned char szMac[20] = {0};
    //Ϊ��ȷ����ȡ��MAC��ַ��ͨ�����ַ�ʽ����ȡ
    if(!getLocalMac3(szMac))
    {
        if(!getLocalMac2(szMac))
        {
            if(!getLocalMac1(szMac))
				return "";
        }
    }

	std::string sMac((char*)szMac);
    return sMac;
}

// ��ȡIP��ַ
std::string GetIP()
{
    char ip[30] = {0};
    if (!GetLocalIP(ip))
        return "";

	std::string sIP((char*)ip);
	return sIP;
}