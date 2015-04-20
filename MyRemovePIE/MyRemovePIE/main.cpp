///
//  main.cpp
//  machofile
//
//  Created by Glenn Lugod on 12/31/12.
//  Copyright (c) 2012 Glenn. All rights reserved.
//

#include <iostream>

#include <math.h>

#include "machofile.h"

using namespace rotg;


static const char* getCPUTypeString(cpu_type_t cputype)
{
    const char* strcputype = "Unknown";
    
    switch (cputype) {
        case CPU_TYPE_ANY:
            strcputype = "CPU_TYPE_ANY";
            break;
            
        case CPU_TYPE_VAX:
            strcputype = "CPU_TYPE_VAX";
            break;
            
        case CPU_TYPE_MC680x0:
            strcputype = "CPU_TYPE_MC680x0";
            break;
            
        case CPU_TYPE_X86:
            strcputype = "CPU_TYPE_X86";
            break;
            
        case CPU_TYPE_X86_64:
            strcputype = "CPU_TYPE_X86_64";
            break;
            
        case CPU_TYPE_MC98000:
            strcputype = "CPU_TYPE_MC98000";
            break;
            
        case CPU_TYPE_HPPA:
            strcputype = "CPU_TYPE_HPPA";
            break;
            
        case CPU_TYPE_ARM:
            strcputype = "CPU_TYPE_ARM";
            break;
            
        case CPU_TYPE_MC88000:
            strcputype = "CPU_TYPE_MC88000";
            break;
            
        case CPU_TYPE_SPARC:
            strcputype = "CPU_TYPE_SPARC";
            break;
            
        case CPU_TYPE_I860:
            strcputype = "CPU_TYPE_I860";
            break;
            
        case CPU_TYPE_POWERPC:
            strcputype = "CPU_TYPE_POWERPC";
            break;
            
        case CPU_TYPE_POWERPC64:
            strcputype = "CPU_TYPE_POWERPC64";
            break;
    }
    
    return strcputype;
}

static void printMachODetails(MachOFile& machoFile,const char * path);

static void parseUniversal(MachOFile& machoFile,const char * path)
{
    printf("Type: Universal file\n");
    
    const fat_arch_infos_t& infos = machoFile.getFatArchInfos();
    
    int archNum = 1;
    fat_arch_infos_t::const_iterator iter;
    for (iter=infos.begin(); iter!=infos.end(); iter++) {
        const struct fat_arch& arch = iter->arch;
        
        printf("\tArch %d\n", archNum);
        printf("\t\tCPU Type    : %s\n", getCPUTypeString(arch.cputype));
        printf("\t\tCPU SubType : 0x%X\n", arch.cpusubtype);
        printf("\t\tOffset      : %d\n", arch.offset);
        printf("\t\tSize        : %u\n", arch.size);
        printf("\t\tAlign       : %f\n", pow(2, arch.align));

        archNum++;
    }
    
    printf("\n");
    
    archNum = 1;
    for (iter=infos.begin(); iter!=infos.end(); iter++) {
        printf("********** Arch %d **********\n", archNum++);
        
        const fat_arch_info_t& fat_arch_info = *iter;
        
        MachOFile machoFile;
        
        if (machoFile.parse_macho(&(fat_arch_info.input),path)) {
            printMachODetails(machoFile,path);
        }
    }
}

static void printMachODetails(MachOFile& machoFile,const char * path)
{
    if (machoFile.isUniversal()) {
        parseUniversal(machoFile,path);
    }
    else if (machoFile.is64bit())
    {
        printf("TODO: support 64-bit mach-o format\n\n");
    }
    else if (machoFile.is32bit())
    {
        printf("TODO: support 32-bit mach-o format\n\n");
    }
}

int main(int argc, const char * argv[])
{
    MachOFile machoFile;
    
    if (machoFile.parse_file(argv[1])) {
        printf("File: %s\n", argv[1]);
        printMachODetails(machoFile,argv[1]);
    }
    else
    {
        printf("error parsing %s", argv[1]);
    }

    return 0;
}

