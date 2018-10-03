// Copyright 2010 Intel Corporation
// All Rights Reserved
//
// Permission is granted to use, copy, distribute and prepare derivative works of this
// software for any purpose and without fee, provided, that the above copyright notice
// and this statement appear in all copies.  Intel makes no representations about the
// suitability of this software for any purpose.  THIS SOFTWARE IS PROVIDED ""AS IS.""
// INTEL SPECIFICALLY DISCLAIMS ALL WARRANTIES, EXPRESS OR IMPLIED, AND ALL LIABILITY,
// INCLUDING CONSEQUENTIAL AND OTHER INDIRECT DAMAGES, FOR THE USE OF THIS SOFTWARE,
// INCLUDING LIABILITY FOR INFRINGEMENT OF ANY PROPRIETARY RIGHTS, AND INCLUDING THE
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  Intel does not
// assume any responsibility for any errors which may appear in this software nor any
// responsibility to update it.

//
// DeviceId.cpp : Implements the GPU Device detection and graphics settings
//                configuration functions.
//

#include <wrl.h>
#include "DeviceId.h"
#include <stdio.h>
#include <string>

#include <InitGuid.h>
#include <D3D11.h>
#include <D3DCommon.h>
#include "dxgi1_4.h"

#include <oleauto.h>
#include <wbemidl.h>
#include <ObjBase.h>

/*****************************************************************************************
 * getGraphicsDeviceInfo
 *
 *     Function to retrieve information about the primary graphics driver. This includes
 *     the device's Vendor ID, Device ID, and available memory.
 *
 *****************************************************************************************/

static bool first_time = true;
static HMODULE hDXGI = NULL;

bool getGraphicsDeviceInfo( unsigned int* VendorId,
							unsigned int* DeviceId,
							std::wstring* GFXBrand,
							unsigned __int64* VideoMemoryBudget,
							unsigned __int64* VideoMemoryCurrentUsage,
							unsigned __int64* VideoMemoryAvailable,
							unsigned __int64* VideoMemoryReserved)
{
	//
	// DXGI is supported on Windows Vista and later. Define a function pointer to the
	// CreateDXGIFactory function. DXGIFactory1 is supported by Windows Store Apps so
	// try that first.
	//
	if (first_time)
	{
		first_time = false;

		hDXGI = LoadLibrary("dxgi.dll");
		if (hDXGI == NULL) {
			return false;
		}
	}
	typedef HRESULT(WINAPI*LPCREATEDXGIFACTORY)(REFIID riid, void** ppFactory);

	LPCREATEDXGIFACTORY pCreateDXGIFactory = (LPCREATEDXGIFACTORY)GetProcAddress(hDXGI, "CreateDXGIFactory1");
	if (pCreateDXGIFactory == NULL) {
		pCreateDXGIFactory = (LPCREATEDXGIFACTORY)GetProcAddress(hDXGI, "CreateDXGIFactory");

		if (pCreateDXGIFactory == NULL) {
			FreeLibrary(hDXGI);
			return false;
		}
	}
	
	//
	// We have the CreateDXGIFactory function so use it to actually create the factory and enumerate
	// through the adapters. Here, we are specifically looking for the Intel gfx adapter. 
	//
	IDXGIAdapter*     pAdapter;
	IDXGIFactory*     pFactory;
	DXGI_ADAPTER_DESC AdapterDesc;
	if (FAILED((*pCreateDXGIFactory)(__uuidof(IDXGIFactory), (void**)(&pFactory)))) {
		FreeLibrary(hDXGI);
		return false;
	}

	if (FAILED(pFactory->EnumAdapters(0, (IDXGIAdapter**)&pAdapter))) {
		FreeLibrary(hDXGI);
		return false;
	}

	unsigned int intelAdapterIndex = 0;
	while (pFactory->EnumAdapters(intelAdapterIndex, &pAdapter) != DXGI_ERROR_NOT_FOUND) {
		pAdapter->GetDesc(&AdapterDesc);
		//if (AdapterDesc.VendorId == 0x8086) {
			break;
		//}
		intelAdapterIndex++;
	}

	if (pAdapter == NULL) {
		FreeLibrary(hDXGI);
		return false;
	}

	//
	// If we are on Windows 10 then the Adapter3 interface should be available. This is recommended over using
	// the AdapterDesc for getting the amount of memory available.
	//
	IDXGIAdapter3* pAdapter3;
	pAdapter->QueryInterface(IID_IDXGIAdapter3, (void**)&pAdapter3);
	if (pAdapter3) {
		DXGI_QUERY_VIDEO_MEMORY_INFO memInfo;
		pAdapter3->QueryVideoMemoryInfo(intelAdapterIndex, DXGI_MEMORY_SEGMENT_GROUP_LOCAL, &memInfo);
		//*VideoMemory = memInfo.AvailableForReservation;
		*VideoMemoryBudget = memInfo.Budget;
		*VideoMemoryCurrentUsage = memInfo.CurrentUsage;
		*VideoMemoryAvailable = memInfo.AvailableForReservation;
		*VideoMemoryReserved = memInfo.CurrentReservation;
		pAdapter3->Release();
	}
	else {
		*VideoMemoryBudget = (unsigned __int64)(AdapterDesc.DedicatedVideoMemory);
		*VideoMemoryCurrentUsage = 0;
		*VideoMemoryAvailable = (unsigned __int64)(AdapterDesc.SharedSystemMemory);
		*VideoMemoryReserved = 0;
	}

	if(VendorId != nullptr)
		*VendorId = AdapterDesc.VendorId;
	if(DeviceId != nullptr)
		*DeviceId = AdapterDesc.DeviceId;
	if(GFXBrand != nullptr)
		*GFXBrand = AdapterDesc.Description;

	pAdapter->Release();
	FreeLibrary(hDXGI);
	return true;
}

/******************************************************************************************************************************************
 * getIntelDeviceInfo
 *
 * Description:
 *       Gets device information which is stored in a D3D counter. First, a D3D device must be created, the Intel counter located, and
 *       finally queried.
 *
 *       Supported device info: GPU Max Frequency, GPU Min Frequency, GT Generation, EU Count, Package TDP, Max Fill Rate
 * 
 * Parameters:
 *         unsigned int VendorId                         - [in]     - Input:  system's vendor id
 *         IntelDeviceInfoHeader *pIntelDeviceInfoHeader - [in/out] - Input:  allocated IntelDeviceInfoHeader *
 *                                                                    Output: Intel device info header, if found
 *         void *pIntelDeviceInfoBuffer                  - [in/out] - Input:  allocated void *
 *                                                                    Output: IntelDeviceInfoV[#], cast based on IntelDeviceInfoHeader
 * Return:
 *         GGF_SUCCESS: Able to find Data is valid
 *         GGF_E_UNSUPPORTED_HARDWARE: Unsupported hardware, data is invalid
 *         GGF_E_UNSUPPORTED_DRIVER: Unsupported driver on Intel, data is invalid
 *
 *****************************************************************************************************************************************/
/*****************************************************************************************
* getCPUInfo
*
*      Parses CPUID output to find the brand and vendor strings.
*
*****************************************************************************************/

void getCPUInfo(std::string* cpubrand, std::string* cpuvendor)
{

	// Get extended ids.
	int CPUInfo[4] = { -1 };
	__cpuid(CPUInfo, 0x80000000);
	unsigned int nExIds = CPUInfo[0];

	// Get the information associated with each extended ID.
	char CPUBrandString[0x40] = { 0 };
	char CPUVendorString[0x40] = { 0 };

	__cpuid(CPUInfo, 0);
	memcpy_s(CPUVendorString, sizeof(CPUInfo), &CPUInfo[1], sizeof(int));
	memcpy_s(CPUVendorString + 4, sizeof(CPUInfo), &CPUInfo[3], sizeof(int));
	memcpy_s(CPUVendorString + 8, sizeof(CPUInfo), &CPUInfo[2], sizeof(int));
	*cpuvendor = CPUVendorString;

	for (unsigned int i = 0x80000000; i <= nExIds; ++i)
	{
		__cpuid(CPUInfo, i);

		if (i == 0x80000002)
		{

			memcpy_s(CPUBrandString, sizeof(CPUInfo), CPUInfo, 4 * sizeof(int));
		}
		else if (i == 0x80000003)
		{
			memcpy_s(CPUBrandString + 16, sizeof(CPUInfo), CPUInfo, 4 * sizeof(int));
		}
		else if (i == 0x80000004)
		{
			memcpy_s(CPUBrandString + 32, sizeof(CPUInfo), CPUInfo, 4 * sizeof(int));
		}
	}
	*cpubrand = CPUBrandString;

}

/*****************************************************************************************
* getGTGeneration
*
*      Returns the generation by parsing the device id. The first two digits of the hex
*      number generally denote the generation. Sandybridge and Ivybridge share the same
*      numbers so they must be further parsed.
*
*      Comparison of the deviceIds (for example to see if a device is more recent than
*      another) does not always work.
*
*****************************************************************************************/

PRODUCT_FAMILY getGTGeneration(unsigned int deviceId)
{
	unsigned int maskedDeviceId = deviceId & 0xFF00;

	//
	// Device is Sandybridge or Ivybridge
	//
	if (maskedDeviceId == 0x0100) {
		if ( 
			((deviceId & 0x0050) == 0x0050) ||
			((deviceId & 0x0060) == 0x0060)
		   ) {
			return IGFX_IVYBRIDGE;
			}
		if (
			((deviceId & 0x0010) == 0x0010) ||
			((deviceId & 0x00F0) == 0x0000)
			) {
			return IGFX_SANDYBRIDGE;
		}
	}

	if (maskedDeviceId == 0x0400 || maskedDeviceId == 0x0A00 || maskedDeviceId == 0x0D00) {
		return IGFX_HASWELL;
	}

	if (maskedDeviceId == 0x1600) {
		return IGFX_BROADWELL;
	}

	if (maskedDeviceId == 0x1900) {
		return IGFX_SKYLAKE;
	}

	return IGFX_UNKNOWN;
}
