#pragma once
#include "MCP/Renderer/Voxel/Superchunk.h"
#include "MCP/Renderer/Texture2D.h"
#include "MCP/Noise/PerlinNoise.h"

namespace MC
{
	class VoxelTerrain
	{
	public:
		VoxelTerrain(uint32_t c_width, uint32_t c_height, uint32_t c_depth);
		~VoxelTerrain();

		enum class TerrainType : uint8_t
		{
			Terrain2D   = 2,
			Terrain3D   = 3
		};

		void GenNoiseTerrain(TerrainType type, uint32_t octaves, float frequency, float persistence, float xOffset, float yOffset);
		void GenFlatTerrain() const;

		inline void ResizeTerrain(uint32_t newWidth, uint32_t newHeight, uint32_t newDepth) { width = newWidth; height = newHeight, depth = newDepth; }

		inline Superchunk* GetTerrainData() { return superChunk; }
		inline Texture2D& GetTerrainPreview() { return m_terrainPreviewTex; }
	private:
		uint32_t width, height, depth;

		Superchunk* superChunk; //@TODO: extent to multiple superchunks
		Texture2D m_terrainPreviewTex;
	};
}