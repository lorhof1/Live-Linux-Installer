#include <iostream>

/* https://stackoverflow.com/a/478960/13908221 */

#include <cstdio>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>

std::string exec(const char* cmd) {
    char buffer[128];
    std::string result = "";
    FILE* pipe = _popen(cmd, "r");
    if (!pipe) throw std::runtime_error("_popen() failed!");
    try {
        while (fgets(buffer, sizeof buffer, pipe) != NULL) {
            result += buffer;
        }
    } catch (...) {
        _pclose(pipe);
        throw;
    }
    _pclose(pipe);
    return result;
}

/* end of quote */

using namespace std;

bool isBootModeBios() {
    return exec("powershell $(Get-ComputerInfo).BiosFirmwareType").find("Bios") != std::string::npos;
}

int main() {
    //system("diskpart /s appfiles\\diskpart-shrink-c-drive-script");
    if (isBootModeBios()) {
        system("appfiles\\helpers\\qemu\\qemu-system-x86_64.exe -cdrom appfiles\\install.iso -drive format=raw,file=\\\\.\\PHYSICALDRIVE0 -m 2G -boot order=d");
    }
    else {
        system("appfiles\\helpers\\qemu\\qemu-system-x86_64.exe -cdrom appfiles\\install.iso -drive format=raw,file=\\\\.\\PHYSICALDRIVE0 -m 2G -boot order=d -bios appfiles\\helpers\\qemu\\uefi-bios.bin");
    }
}