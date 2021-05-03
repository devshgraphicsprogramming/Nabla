#include "nbl/video/CVulkanQueue.h"

#include "nbl/video/CVKLogicalDevice.h"
#include "nbl/video/CVulkanFence.h"
#include "nbl/video/CVulkanSemaphore.h"
#include "nbl/video/CVulkanPrimaryCommandBuffer.h"

namespace nbl {
namespace video
{

CVulkanQueue::CVulkanQueue(CVKLogicalDevice* vkdev, VkQueue vkq, uint32_t _famIx, E_CREATE_FLAGS _flags, float _priority) : 
    IGPUQueue(_famIx, _flags, _priority), m_vkdevice(vkdev), m_vkqueue(vkq)
{
    
}

void CVulkanQueue::submit(uint32_t _count, const SSubmitInfo* _submits, IGPUFence* _fence)
{
    auto vkdev = m_vkdevice->getInternalObject();
    auto* vk = m_vkdevice->getFunctionTable();

    uint32_t waitSemCnt = 0u;
    uint32_t signalSemCnt = 0u;
    uint32_t cmdBufCnt = 0u;

    for (uint32_t i = 0u; i < _count; ++i)
    {
        const auto& sb = _submits[i];
        waitSemCnt += sb.waitSemaphoreCount;
        signalSemCnt += sb.signalSemaphoreCount;
        cmdBufCnt += sb.commandBufferCount;
    }

    constexpr uint32_t STACK_MEM_SIZE = 1u<<15;
    uint8_t stackmem_[STACK_MEM_SIZE]{};
    uint8_t* mem = stackmem_;
    uint32_t memSize = STACK_MEM_SIZE;

    const uint32_t submitsSz = sizeof(VkSubmitInfo)*_count;
    const uint32_t memNeeded = submitsSz + (waitSemCnt + signalSemCnt)*sizeof(VkSemaphore) + cmdBufCnt*sizeof(VkCommandBuffer);
    if (memNeeded > memSize)
    {
        memSize = memNeeded;
        mem = reinterpret_cast<uint8_t*>( _NBL_ALIGNED_MALLOC(memSize, _NBL_SIMD_ALIGNMENT) );
    }

    auto raii_ = core::makeRAIIExiter([mem,stackmem_]{ _NBL_ALIGNED_FREE(mem); });

    VkSubmitInfo* submits = reinterpret_cast<VkSubmitInfo*>(mem);
    mem += submitsSz;

    VkSemaphore* waitSemaphores = reinterpret_cast<VkSemaphore*>(mem);
    mem += waitSemCnt*sizeof(VkSemaphore);
    VkSemaphore* signalSemaphores = reinterpret_cast<VkSemaphore*>(mem);
    mem += signalSemCnt*sizeof(VkSemaphore);
    VkCommandBuffer* cmdbufs = reinterpret_cast<VkCommandBuffer*>(mem);
    mem += cmdBufCnt*sizeof(VkCommandBuffer);

    uint32_t waitSemOffset = 0u;
    uint32_t signalSemOffset = 0u;
    uint32_t cmdBufOffset = 0u;
    for (uint32_t i = 0u; i < _count; ++i)
    {
        auto& sb = submits[i];

        auto& _sb = _submits[i];
#ifdef _NBL_DEBUG
        for (uint32_t j = 0u; j < _sb.commandBufferCount; ++j)
            assert(_sb.commandBuffers[j]->getLevel() != CVulkanCommandBuffer::EL_SECONDARY);
#endif

        sb.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        sb.pNext = nullptr;
        auto* cbs = cmdbufs + cmdBufOffset;
        sb.pCommandBuffers = cbs;
        sb.commandBufferCount = _sb.commandBufferCount;
        auto* waits = waitSemaphores + waitSemOffset;
        sb.pWaitSemaphores = waits;
        sb.waitSemaphoreCount = _sb.waitSemaphoreCount;
        auto* signals = signalSemaphores + signalSemOffset;
        sb.pSignalSemaphores = signals;
        sb.signalSemaphoreCount = _sb.signalSemaphoreCount;

        for (uint32_t j = 0u; j < sb.waitSemaphoreCount; ++j)
        {
            waits[j] = static_cast<CVulkanSemaphore*>(_sb.pWaitSemaphores[j])->getInternalObject();
        }
        for (uint32_t j = 0u; j < sb.signalSemaphoreCount; ++j)
        {
            signals[j] = static_cast<CVulkanSemaphore*>(_sb.pSignalSemaphores[j])->getInternalObject();
        }
        for (uint32_t j = 0u; j < sb.commandBufferCount; ++j)
        {
            cbs[j] = static_cast<CVulkanPrimaryCommandBuffer*>(_sb.commandBuffers[j])->getInternalObject();
        }

        waitSemOffset += sb.waitSemaphoreCount;
        signalSemOffset += sb.signalSemaphoreCount;
        cmdBufOffset += sb.commandBufferCount;

        static_assert(sizeof(VkPipelineStageFlags) == sizeof(asset::E_PIPELINE_STAGE_FLAGS));
        sb.pWaitDstStageMask = reinterpret_cast<const VkPipelineStageFlags*>(_sb.pWaitDstStageMask);
    }

    VkFence fence = _fence ? static_cast<CVulkanFence*>(_fence)->getInternalObject() : VK_NULL_HANDLE;
    vk->vk.vkQueueSubmit(m_vkqueue, _count, submits, fence);
}

}
}