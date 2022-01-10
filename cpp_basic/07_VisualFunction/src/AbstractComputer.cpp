#include "AbstractCPU.h"
#include "AbstractGPU.h"
#include "AbstractRAM.h"
#include "AbstractROM.h"
#include "AbstractPCB.h"

class AbstractComputer
{
private:
    AbstractCPU *cpu;
    AbstractGPU *gpu;
    AbstractRAM *ram;
    AbstractROM *rom;
    AbstractPCB *pcb;

public:
    AbstractComputer() {}
    AbstractComputer(AbstractCPU *cpu, AbstractGPU *gpu, AbstractRAM *ram, AbstractROM *rom, AbstractPCB *pcb)
    {
        this->cpu = cpu;
        this->gpu = gpu;
        this->rom = rom;
        this->ram = ram;
        this->pcb = pcb;
    }
    ~AbstractComputer() {}
    void work()
    {
        this->cpu->calculate();
        this->gpu->display();
        this->ram->ram();
        this->rom->rom();
        this->pcb->pcb();
    }
    AbstractCPU *GetCpu() const
    {
        return cpu;
    }

    void SetCpu(AbstractCPU *cpu)
    {
        cpu = cpu;
    }

    AbstractGPU *GetGpu() const
    {
        return gpu;
    }

    void SetGpu(AbstractGPU *gpu)
    {
        gpu = gpu;
    }

    AbstractRAM *GetRam() const
    {
        return ram;
    }

    void SetRam(AbstractRAM *ram)
    {
        ram = ram;
    }

    AbstractROM *GetRom() const
    {
        return rom;
    }

    void SetRom(AbstractROM *rom)
    {
        rom = rom;
    }

    AbstractPCB *GetPcb() const
    {
        return pcb;
    }

    void SetPcb(AbstractPCB *pcb)
    {
        pcb = pcb;
    }
};
