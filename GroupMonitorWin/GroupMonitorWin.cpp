// GroupMonitorWin.cpp: 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "GroupMonitorWin.h"

#include "WMain.h"

#include <string>
#include <thread>

using namespace std;

string AppFile;

WMain a;


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO:  如果此 DLL 相对于 MFC DLL 是动态链接的，
//		则从此 DLL 导出的任何调入
//		MFC 的函数必须将 AFX_MANAGE_STATE 宏添加到
//		该函数的最前面。
//
//		例如: 
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// 此处为普通函数体
//		}
//
//		此宏先于任何 MFC 调用
//		出现在每个函数中十分重要。  这意味着
//		它必须作为以下项中的第一个语句:
//		出现，甚至先于所有对象变量声明，
//		这是因为它们的构造函数可能生成 MFC
//		DLL 调用。
//
//		有关其他详细信息，
//		请参阅 MFC 技术说明 33 和 58。
//

// CGroupMonitorWinApp

BEGIN_MESSAGE_MAP(CGroupMonitorWinApp, CWinApp)
END_MESSAGE_MAP()


// CGroupMonitorWinApp 构造

CGroupMonitorWinApp::CGroupMonitorWinApp()
{
	// TODO:  在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的 CGroupMonitorWinApp 对象

CGroupMonitorWinApp theApp;

const GUID CDECL _tlid = { 0x91c64fb5,0x4be9,0x4766,{0xb4,0x76,0x6b,0xb3,0x1f,0x66,0x18,0x5c} };
const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;


// CGroupMonitorWinApp 初始化

BOOL CGroupMonitorWinApp::InitInstance()
{
	CWinApp::InitInstance();

	// 将所有 OLE 服务器(工厂)注册为运行。  这将使
	//  OLE 库得以从其他应用程序创建对象。
	COleObjectFactory::RegisterAll();

	return TRUE;
}

// DllGetClassObject - 返回类工厂

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return AfxDllGetClassObject(rclsid, riid, ppv);
}


// DllCanUnloadNow - 允许 COM 卸载 DLL

STDAPI DllCanUnloadNow(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return AfxDllCanUnloadNow();
}


// DllRegisterServer - 将项添加到系统注册表

STDAPI DllRegisterServer(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (!AfxOleRegisterTypeLib(AfxGetInstanceHandle(), _tlid))
		return SELFREG_E_TYPELIB;

	if (!COleObjectFactory::UpdateRegistryAll())
		return SELFREG_E_CLASS;

	return S_OK;
}


// DllUnregisterServer - 将项从系统注册表中移除

STDAPI DllUnregisterServer(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (!AfxOleUnregisterTypeLib(_tlid, _wVerMajor, _wVerMinor))
		return SELFREG_E_TYPELIB;

	if (!COleObjectFactory::UpdateRegistryAll(FALSE))
		return SELFREG_E_CLASS;

	return S_OK;
}


#ifdef __cplusplus         // if used by C++ code
extern "C" {                  // we need to export the C interface
#endif
	__declspec(dllexport) void winfun(const char* path, const char* fun)
	{
		AFX_MANAGE_STATE(AfxGetStaticModuleState());
		//读取主要配置
		Config_Read_main_win Read_Main(path);
		Read_Main.read_main();
		cfg_main = Read_Main.value;

		//读取全部单独配置
		Config_Read_win Read_Alone(path);
		Read_Alone.read_Alone();
		//拷贝map
		for (auto new_map : Read_Alone.value)
		{
			cfg_alone[new_map.first] = new_map.second;
		}

		//保存文件位置与函数指针
		AppFile = path;
		CQ_getGroupList_fun(fun);

		//MessageBoxA(NULL, AppFile.c_str(), "", MB_OK);//for test

		//打开界面
		a.DoModal();


		//写入主要配置
		Config_Write_main_win Write_Main(cfg_main, path);
		Write_Main.write_main();

		//写入全部单独配置
		Config_Write_win Write_Alone(cfg_alone, path);
		Write_Alone.write_Alone();
	}
#ifdef __cplusplus
}
#endif

