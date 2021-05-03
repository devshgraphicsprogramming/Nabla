#include "nbl/video/alloc/StreamingGPUBufferAllocator.h"

#include "nbl/video/ILogicalDevice.h"

namespace nbl {
namespace video
{

    void* StreamingGPUBufferAllocator::mapWrapper(IDriverMemoryAllocation* mem, IDriverMemoryAllocation::E_MAPPING_CPU_ACCESS_FLAG access, const IDriverMemoryAllocation::MemoryRange& range) noexcept
    {
        IDriverMemoryAllocation::MappedMemoryRange memory(mem, range.offset, range.length);
        return mDriver->mapMemory(memory, access);
    }

    void StreamingGPUBufferAllocator::unmapWrapper(IDriverMemoryAllocation* mem) noexcept
    {
        mDriver->unmapMemory(mem);
    }

}
}