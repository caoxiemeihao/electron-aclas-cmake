#include <windows.h>
#include <iostream>
#include <napi.h>
using namespace std;

BOOL is_ran = false; // 判断是否有给调用处反馈

// 通过 stdout 的方式将数据推给 node.js
void Stdout2Nodejs(INT32 code, INT32 index, INT32 total)
{
	std::cout << "##dispatch={"
		<< "\"code\":" << code
		<< ","
		<< "\"index\":" << index
		<< ","
		<< "\"total\":" << total
		<< "}##" << std::endl;
	is_ran = true;
}

// ================================================================================
struct DviIn
{
	UINT32 ProtocolType;
	UINT32  Addr;
	UINT32  Port;
	UCHAR    name[16];
	UINT32  ID;
	UINT32  Version;
	BYTE     Country;
	BYTE     DepartmentID;
	BYTE     KeyType;
	UINT64  PrinterDot;
	LONG64   PrnStartDate;
	UINT32  LabelPage;
	UINT32  PrinterNo;
	USHORT   PLUStorage;
	USHORT   HotKeyCount;
	USHORT   NutritionStorage;
	USHORT   DiscountStorage;
	USHORT   Note1Storage;
	USHORT   Note2Storage;
	USHORT   Note3Storage;
	USHORT   Note4Storage;
	BYTE     stroge[177];
};

extern "C"
{
	typedef bool (CALLBACK*pAclasSDKInitialize)(char *s);
	typedef bool (CALLBACK*pGetDevicesInfo)(UINT32  Addr, UINT32  Port, UINT32 ProtocolType, DviIn *info);

	typedef void (WINAPI *FP)(UINT32 Eorrorcode, UINT32 index, UINT32 Total, char *userdata);

	typedef HANDLE(CALLBACK*pAclasSDKExecTask)(UINT32 Addr, UINT32  Port, UINT32 ProtocolType, UINT32 ProceType, UINT32 DataType, char *FileName, FP fp, char *uerdata);
	typedef HANDLE(CALLBACK*pAclasSDKWaitForTask)(HANDLE handle);

	typedef int (CALLBACK*pAclasSDKSyncExecTask)(char *Addr, UINT32  Port, UINT32 ProtocolType, UINT32 ProceType, UINT32 DataType, char *FileName);
}

void WINAPI ongress(UINT32 Eorrorcode, UINT32 index, UINT32 Total, char *userdata)
{

	Stdout2Nodejs(Eorrorcode, index, Total);

	switch (Eorrorcode)
	{
	case 0x0000:
		cout << "complete" << endl;
		break;
	case 0x0001:
		cout << index << "/" << Total << endl;
		break;
	}
}
UINT MakehostToDword(char *host)
{
	UINT result;
	UINT a[4];
	char *p1 = NULL;

	char str[20];
	strcpy(str, host);
	p1 = strtok(str, ".");
	a[0] = atoi(p1);
	result = a[0] << 24;
	for (int i = 1; i < 4; i++)
	{
		p1 = strtok(NULL, ".");
		a[i] = atoi(p1);
		result += a[i] << ((3 - i) * 8);
	}
	return result;


}
// ================================================================================


void Start(char* host, UINT32 proceType, char* filename, char* dll = "AclasSDK.dll") {
	cout << endl << host << endl;
	cout << proceType << endl;
	cout << filename << endl;
	cout << dll << endl << endl;

	HMODULE hModule = LoadLibrary(TEXT(dll));

	if (!hModule) {
		Stdout2Nodejs(404, 0, 0);
		cout << "LoadLibrary failed." << endl;
		cout << hModule << endl;
		return;
	}

	// Initialize
	pAclasSDKInitialize Initialize = (pAclasSDKInitialize)GetProcAddress(hModule, "AclasSDK_Initialize");
	if (Initialize(NULL)) {
		cout << "Initialize success." << endl;
	}
	else 
	{
		cout << "Initialize failed." << endl;
		return;
	}

	// Get Device Information
	pGetDevicesInfo getDevicesInfo = (pGetDevicesInfo)GetProcAddress(hModule, "AclasSDK_GetDevicesInfo");
	struct DviIn* info = (struct DviIn*)malloc(sizeof(struct DviIn));
	UINT addr = MakehostToDword(host);
	BOOL ref = getDevicesInfo(addr, 0, 0, info);

	cout << info->name << endl;

	// Function Pointer (ongress 指针函数定义)
	FP fp = ongress;
	char *userdata = NULL;

	// ASync call
	pAclasSDKExecTask exectask = (pAclasSDKExecTask)GetProcAddress(hModule, "AclasSDK_ExecTaskA");
	pAclasSDKWaitForTask waitfortask = (pAclasSDKWaitForTask)GetProcAddress(hModule, "AclasSDK_WaitForTask");
	HANDLE handle = waitfortask(exectask(addr, 5002, info->ProtocolType, 0, 0x0000, filename, fp, userdata));

	if (!is_ran) { Stdout2Nodejs(403, 0, 0); } // 链接超时.
	is_ran = false;

	// 释放资源
	GetProcAddress(hModule, "AclasSDK_Finalize");
}

void RunCallback(const Napi::CallbackInfo &info) {
	cout << "==== run sdk ===" << endl;

	Napi::Env env = info.Env();
	Napi::Object config = info[0].As<Napi::Object>();
	Napi::Function callback = info[1].As<Napi::Function>();

	// js 入参
	Napi::Value n_host = config["host"];
	Napi::Value n_type = config["type"];
	Napi::Value n_filename = config["filename"];
	Napi::Value n_dll = config["dll"];

	// C++ 对应 js 入参
	char host[100];
	UINT32 proceType;
	char filename[200];
	char dll[200];

	// 转换 js 入参
	napi_get_value_string_utf8(env, n_host, host, sizeof(host), NULL);
	napi_get_value_uint32(env, n_type, &proceType);
	napi_get_value_string_utf8(env, n_filename, filename, sizeof(filename), NULL);
	napi_get_value_string_utf8(env, n_dll, dll, sizeof(dll), NULL);

	Start(host, proceType, filename, dll);
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
	return Napi::Function::New(env, RunCallback);
}

NODE_API_MODULE(aclas, Init)
