#include "mcpch.h"
#include "ChunkManager.h"

#include "MCP/Utils/Logger.h"

namespace MC
{

	ChunkManager::ChunkManager()
	{
		memset(c, 0, sizeof(c));
	}

	ChunkManager::~ChunkManager()
	{
		for (int x = 0; x < SUPER_CHUNK_SIZE; x++)
			for (int y = 0; y < SUPER_CHUNK_SIZE; y++)
				for (int z = 0; z < SUPER_CHUNK_SIZE; z++)
					delete c[x][y][z];
	}

	uint8_t ChunkManager::GetVoxel(int x, int y, int z)
	{
		int cx = x / CHUNK_SIZE;
		int cy = y / CHUNK_SIZE;
		int cz = z / CHUNK_SIZE;

		x %= CHUNK_SIZE;
		y %= CHUNK_SIZE;
		z %= CHUNK_SIZE;

		if (!c[cx][cy][cz])
			return 0;
		

		return c[cx][cy][cz]->get(x, y, z);
	}

	MC::Chunk* ChunkManager::GetChunk(int x, int y, int z)
	{
		uint32_t cx = x / CHUNK_SIZE;
		uint32_t cy = y / CHUNK_SIZE;
		uint32_t cz = z / CHUNK_SIZE;

		if (c[cx][cy][cz])
			return c[cx][cy][cz];

		return nullptr;
	}

	void ChunkManager::Set(int x, int y, int z, uint8_t type)
	{
		this->Set(x, y, z, type, nullptr);
	}
		void ChunkManager::Set(int x, int y, int z, uint8_t type, const BlockTexture2D* FaceTextures)
	{
		uint32_t cx = x / CHUNK_SIZE;
		uint32_t cy = y / CHUNK_SIZE;
		uint32_t cz = z / CHUNK_SIZE;

		x %= CHUNK_SIZE;
		y %= CHUNK_SIZE;
		z %= CHUNK_SIZE;

		if (!c[cx][cy][cz])
		{
			c[cx][cy][cz] = new Chunk();

			if (cx > 0 && c[cx-1][cy][cz])
			{
				c[cx][cy][cz]->nc.left_Chunk = c[cx - 1][cy][cz];
				c[cx - 1][cy][cz]->nc.right_Chunk = c[cx][cy][cz];
			}

			if (cy > 0 && c[cx][cy - 1][cz])
			{
				c[cx][cy][cz]->nc.below_Chunk = c[cx][cy - 1][cz];
				c[cx][cy - 1][cz]->nc.upper_Chunk = c[cx][cy][cz];
			}

			if (cz > 0 && c[cx][cy][cz - 1])
			{
				c[cx][cy][cz]->nc.back_Chunk = c[cx][cy][cz - 1];
				c[cx][cy][cz - 1]->nc.front_Chunk = c[cx][cy][cz];
			}
		}

		if (x == 0 && c[cx][cy][cz]->nc.left_Chunk)
			c[cx][cy][cz]->nc.left_Chunk->changed = true;

		else if (x == CHUNK_SIZE - 1 && c[cx][cy][cz]->nc.right_Chunk)
			c[cx][cy][cz]->nc.right_Chunk->changed = true;


		if (y == 0 && c[cx][cy][cz]->nc.below_Chunk)
			c[cx][cy][cz]->nc.below_Chunk->changed = true;

		else if (y == CHUNK_SIZE - 1 && c[cx][cy][cz]->nc.upper_Chunk)
			c[cx][cy][cz]->nc.upper_Chunk->changed = true;


		if (z == 0 && c[cx][cy][cz]->nc.back_Chunk)
			c[cx][cy][cz]->nc.back_Chunk->changed = true;

		else if (z == CHUNK_SIZE - 1 && c[cx][cy][cz]->nc.front_Chunk)
			c[cx][cy][cz]->nc.front_Chunk->changed = true;


		c[cx][cy][cz]->set(x, y, z, type, FaceTextures);
	}

		MC::Chunk* ChunkManager::CreateChunk(uint32_t x, uint32_t y, uint32_t z)
		{
			if (x > SUPER_CHUNK_SIZE || y > SUPER_CHUNK_SIZE || z > SUPER_CHUNK_SIZE)
				return nullptr;

			c[x][y][z] = new Chunk;
			
		
			if (x > 0 && c[x - 1][y][z])
			{
				c[x][y][z]->nc.left_Chunk = c[x - 1][y][z];
				c[x - 1][y][z]->nc.right_Chunk = c[x][y][z];
			}

			if (y > 0 && c[x][y - 1][z])
			{
				c[x][y][z]->nc.below_Chunk = c[x][y - 1][z];
				c[x][y - 1][z]->nc.upper_Chunk = c[x][y][z];
			}

			if (z > 0 && c[x][y][z - 1])
			{
				c[x][y][z]->nc.back_Chunk = c[x][y][z - 1];
				c[x][y][z - 1]->nc.front_Chunk = c[x][y][z];
			}
			

			if (c[x][y][z]->nc.left_Chunk)
				c[x][y][z]->nc.left_Chunk->changed = true;

			else if (c[x][y][z]->nc.right_Chunk)
				c[x][y][z]->nc.right_Chunk->changed = true;


			if (c[x][y][z]->nc.below_Chunk)
				c[x][y][z]->nc.below_Chunk->changed = true;

			else if (c[x][y][z]->nc.upper_Chunk)
				c[x][y][z]->nc.upper_Chunk->changed = true;


			if (c[x][y][z]->nc.back_Chunk)
				c[x][y][z]->nc.back_Chunk->changed = true;

			else if (c[x][y][z]->nc.front_Chunk)
				c[x][y][z]->nc.front_Chunk->changed = true;
		
			return c[x][y][z];
		}


}
