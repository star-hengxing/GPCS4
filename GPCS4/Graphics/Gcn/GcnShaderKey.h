#pragma once

#include "GcnCommon.h"
#include "UtilMath.h"
#include "UtilString.h"

#include <string>

namespace sce::gcn
{
	class GcnShaderKey
	{
	public:
		GcnShaderKey(
			uint32_t hash, uint32_t crc)
		{
			m_key = util::concat<uint64_t>(hash, crc);
		}

		~GcnShaderKey() = default;

		uint64_t key() const
		{
			return m_key;
		}

		std::string name() const
		{
			return util::str::format("SHDR_%llX", m_key);
		}

		bool operator==(const GcnShaderKey& other) const
		{
			return m_key == other.m_key;
		}

	private:
		uint64_t m_key;
	};

}  // namespace sce::gcn