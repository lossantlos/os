
#include <cpuid.h>

void cpuid(uint32_t op, uint32_t *eax, uint32_t *ebx, uint32_t *ecx, uint32_t *edx)
{
    __asm__ volatile("cpuid" // Call cpuid
                  : "=a"(*eax), "=b"(*ebx), "=c"(*ecx), "=d"(*edx)
                  : "a"(op)
                  : "cc" );
}

void cpuid_get_vendor(char *name)
{
    uint32_t eax, ebx, ecx, edx;

    cpuid(0, &eax, &ebx, &ecx, &edx);

    for (size_t i = 0; i < 4; i++)
    {
        name[i] = (char) 0xff & ebx >> i * 8;
        name[i+4] = (char) 0xff & edx >> i * 8;
        name[i+8] = (char) 0xff & ecx >> i * 8;
    }

    name[12] = '\0';
}

void cpuid_print()
{
    uint32_t eax, ebx, ecx, edx;

    cpuid(0, &eax, &ebx, &ecx, &edx);

//    printf("%b\n\t%b\n\t%b\n\t%b\n", eax, ebx, ecx, edx);
//    printf("%i\n\t%i\n\t%i\n\t%i\n", eax, ebx, ecx, edx);

    char name[12];
    cpuid_get_vendor(name);
    printf("CPU vendor: %s\n", name);

    printf("features: ");

    #define a(x) if(ecx & CPUID_FEAT_ECX_ ## x ) printf("%s ", #x);
        a(SSE3);
        a(PCLMUL);
        a(DTES64);
        a(MONITOR);
        a(DS_CPL);
        a(VMX);
        a(SMX);
        a(EST);
        a(TM2);
        a(SSSE3);
        a(CID);
        a(FMA);
        a(CX16);
        a(ETPRD);
        a(PDCM);
        a(DCA);
        a(SSE4_1);
        a(SSE4_2);
        a(x2APIC);
        a(MOVBE);
        a(POPCNT);
        a(AES);
        a(XSAVE);
        a(OSXSAVE);
        a(AVX);
    #undef a

    #define a(x) if(edx & CPUID_FEAT_EDX_ ## x ) printf("%s ", #x);
        a(FPU);
        a(VME);
        a(DE);
        a(PSE);
        a(TSC);
        a(MSR);
        a(PAE);
        a(MCE);
        a(CX8);
        a(APIC);
        a(SEP);
        a(MTRR);
        a(PGE);
        a(MCA);
        a(CMOV);
        a(PAT);
        a(PSE36);
        a(PSN);
        a(CLF);
        a(DTES);
        a(ACPI);
        a(MMX);
        a(FXSR);
        a(SSE);
        a(SSE2);
        a(SS);
        a(HTT);
        a(TM1);
        a(IA64);
        a(PBE);
    #undef a

    printf("\n");
}
