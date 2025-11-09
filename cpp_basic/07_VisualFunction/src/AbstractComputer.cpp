#include "AbstractCPU.h"
#include "AbstractGPU.h"
#include "AbstractRAM.h"
#include "AbstractROM.h"
#include "AbstractPCB.h"

class AbstractComputer
{
private:
    AbstractCPU *cpu{};
    AbstractGPU *gpu{};
    AbstractRAM *ram{};
    AbstractROM *rom{};
    AbstractPCB *pcb{};

public:
    AbstractComputer() = default;

    [[maybe_unused]] AbstractComputer(AbstractCPU *cpu, AbstractGPU *gpu, AbstractRAM *ram, AbstractROM *rom, AbstractPCB *pcb)
    {
        this->cpu = cpu;
        this->gpu = gpu;
        this->rom = rom;
        this->ram = ram;
        this->pcb = pcb;
    }
    ~AbstractComputer() = default;
    void work()
    {
        this->cpu->calculate();
        this->gpu->display();
        this->ram->ram();
        this->rom->rom();
        this->pcb->pcb();
    }
    [[nodiscard]] AbstractCPU *GetCpu() const
    {
        return cpu;
    }

    void SetCpu(AbstractCPU *abstractCpu)
    {
        this->cpu = abstractCpu;
    }

    [[nodiscard]] AbstractGPU *GetGpu() const
    {
        return gpu;
    }

    void SetGpu(AbstractGPU *abstractGpu)
    {
        this->gpu = abstractGpu;
    }

    [[nodiscard]] AbstractRAM *GetRam() const
    {
        return ram;
    }

    void SetRam(AbstractRAM *abstractRam)
    {
        this->ram = abstractRam;
    }

    [[nodiscard]] AbstractROM *GetRom() const
    {
        return rom;
    }

    void SetRom(AbstractROM *abstractRom)
    {
        this->rom = abstractRom;
    }

    [[nodiscard]] AbstractPCB *GetPcb() const
    {
        return pcb;
    }

    void SetPcb(AbstractPCB *abstractPcb)
    {
        this->pcb = abstractPcb;
    }
};
