// Copyright (C) 2018-2020 - DevSH Graphics Programming Sp. z O.O.
// This file is part of the "Nabla Engine".
// For conditions of distribution and use, see copyright notice in nabla.h

#ifndef __NBL_ASSET_C_SPIR_V_LOADER_H_INCLUDED__
#define __NBL_ASSET_C_SPIR_V_LOADER_H_INCLUDED__

#include "nbl/asset/interchange/IAssetLoader.h"

namespace nbl::asset
{

class CSPVLoader final : public asset::IAssetLoader
{
		_NBL_STATIC_INLINE_CONSTEXPR uint32_t SPV_MAGIC_NUMBER = 0x07230203u;
		core::smart_refctd_ptr<system::ISystem> m_system;
	public:
		CSPVLoader(core::smart_refctd_ptr<system::ISystem>&& sys) : m_system(std::move(sys)) {}
		bool isALoadableFileFormat(system::IFile* _file) const override
		{
			uint32_t magicNumber = 0u;

			
			system::ISystem::future_t<uint32_t> future;
			m_system->readFile(future, _file, &magicNumber, 0, sizeof magicNumber);

			return magicNumber==SPV_MAGIC_NUMBER;
		}

		const char** getAssociatedFileExtensions() const override
		{
			static const char* ext[]{ "spv", nullptr };
			return ext;
		}

		uint64_t getSupportedAssetTypesBitfield() const override { return asset::IAsset::ET_SHADER; }

		asset::SAssetBundle loadAsset(system::IFile* _file, const asset::IAssetLoader::SAssetLoadParams& _params, asset::IAssetLoader::IAssetLoaderOverride* _override = nullptr, uint32_t _hierarchyLevel = 0u) override;
};

} // namespace nbl::asset

#endif

