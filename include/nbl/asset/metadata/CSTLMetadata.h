// Copyright (C) 2018-2020 - DevSH Graphics Programming Sp. z O.O.
// This file is part of the "Nabla Engine".
// For conditions of distribution and use, see copyright notice in nabla.h

#ifndef __NBL_ASSET_C_STL_METADATA_H_INCLUDED__
#define __NBL_ASSET_C_STL_METADATA_H_INCLUDED__

#include "nbl/asset/metadata/IAssetMetadata.h"

namespace nbl 
{
namespace asset
{

class CSTLMetadata final : public IAssetMetadata
{
    public:
        class CIRenderpassIndependentPipeline : public IRenderpassIndependentPipelineMetadata
        {
            public:
                using IRenderpassIndependentPipelineMetadata::IRenderpassIndependentPipelineMetadata;

                inline CIRenderpassIndependentPipeline& operator=(CIRenderpassIndependentPipeline&& other)
                {
                    IRenderpassIndependentPipelineMetadata::operator=(std::move(other));
                    return *this;
                }
        };
            
        CSTLMetadata(uint32_t pplnCount) : IAssetMetadata(), m_metaStorage(createContainer<CIRenderpassIndependentPipeline>(pplnCount))
        {
        }

        _NBL_STATIC_INLINE_CONSTEXPR const char* LoaderName = "CSTLMeshFileLoader";
        const char* getLoaderName() const override { return LoaderName; }

    private:
        meta_container_smart_ptr_t<CIRenderpassIndependentPipeline> m_metaStorage;

        friend class CSTLMeshFileLoader;
        template<typename... Args>
        inline void addMeta(uint32_t offset, const ICPURenderpassIndependentPipeline* ppln, Args&&... args)
        {
            auto& meta = m_metaStorage->operator[](offset);
            meta = CIRenderpassIndependentPipeline(std::forward<Args>(args)...);

            IAssetMetadata::insertAssetSpecificMetadata(ppln,&meta);
        }
};

}
}

#endif