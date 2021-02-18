#include "mcpch.h"
#include "Superchunk.h"

#include "MCP/Utils/Logger.h"

namespace MC
{

	Superchunk::Superchunk()
	{
		memset(c, 0, sizeof(c));
	}

	Superchunk::~Superchunk()
	{
		for (int x = 0; x < SUPER_CHUNK_SIZE; x++)
			for (int y = 0; y < SUPER_CHUNK_SIZE; y++)
				for (int z = 0; z < SUPER_CHUNK_SIZE; z++)
					delete c[x][y][z];
	}

	uint8_t Superchunk::Get(int x, int y, int z)
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

	void Superchunk::Set(int x, int y, int z, uint8_t type, const BlockTexture2D* FaceTextures)
	{
		//#TODO: remove all this duplicated code
		int cx = x / CHUNK_SIZE;
		int cy = y / CHUNK_SIZE;
		int cz = z / CHUNK_SIZE;

		x %= CHUNK_SIZE;
		y %= CHUNK_SIZE;
		z %= CHUNK_SIZE;

		if (!c[cx][cy][cz])
			c[cx][cy][cz] = new Chunk();

		if (cx > 0)
		{
			c[cx][cy][cz]->nc.left_Chunk = c[cx - 1][cy][cz];
			c[cx - 1][cy][cz]->nc.right_Chunk = c[cx][cy][cz];
		}

		if (x == 0 && c[cx][cy][cz]->nc.left_Chunk)
			c[cx][cy][cz]->nc.left_Chunk->changed = true;

		else if (x == CHUNK_SIZE - 1 && c[cx][cy][cz]->nc.right_Chunk)
			c[cx][cy][cz]->nc.right_Chunk->changed = true;

		if (cy > 0)
		{
			c[cx][cy][cz]->nc.below_Chunk = c[cx][cy - 1][cz];
			c[cx][cy - 1][cz]->nc.upper_Chunk = c[cx][cy][cz];
		}

		if (y == 0 && c[cx][cy][cz]->nc.below_Chunk)
			c[cx][cy][cz]->nc.below_Chunk->changed = true;

		else if (y == CHUNK_SIZE - 1 && c[cx][cy][cz]->nc.upper_Chunk)
			c[cx][cy][cz]->nc.upper_Chunk->changed = true;


		if (cz > 0)
		{
			c[cx][cy][cz]->nc.back_Chunk = c[cx][cy][cz - 1];
			c[cx][cy][cz - 1]->nc.front_Chunk = c[cx][cy][cz];
		}

		if (z == 0 && c[cx][cy][cz]->nc.back_Chunk)
			c[cx][cy][cz]->nc.back_Chunk->changed = true;

		else if (z == CHUNK_SIZE - 1 && c[cx][cy][cz]->nc.front_Chunk)
			c[cx][cy][cz]->nc.front_Chunk->changed = true;


		c[cx][cy][cz]->set(x, y, z, type, FaceTextures);
	}

	void Superchunk::Set(int x, int y, int z, uint8_t type, const Texture2D* UniformTexture)
	{
		int cx = x / CHUNK_SIZE;
		int cy = y / CHUNK_SIZE;
		int cz = z / CHUNK_SIZE;

		x %= CHUNK_SIZE;
		y %= CHUNK_SIZE;
		z %= CHUNK_SIZE;

		if (!c[cx][cy][cz])
			c[cx][cy][cz] = new Chunk();

		if (cx > 0)
		{
			c[cx][cy][cz]->nc.left_Chunk = c[cx - 1][cy][cz];
			c[cx - 1][cy][cz]->nc.right_Chunk = c[cx][cy][cz];
		}
		
		if (x == 0 && c[cx][cy][cz]->nc.left_Chunk)
			c[cx][cy][cz]->nc.left_Chunk->changed = true;
		
		else if(x == CHUNK_SIZE - 1 && c[cx][cy][cz]->nc.right_Chunk)
			c[cx][cy][cz]->nc.right_Chunk->changed = true;

		if (cy > 0)
		{
			c[cx][cy][cz]->nc.below_Chunk = c[cx][cy - 1][cz];
			c[cx][cy - 1][cz]->nc.upper_Chunk = c[cx][cy][cz];
		}
		
		if (y == 0 && c[cx][cy][cz]->nc.below_Chunk)
			c[cx][cy][cz]->nc.below_Chunk->changed = true;

		else if (y == CHUNK_SIZE - 1 && c[cx][cy][cz]->nc.upper_Chunk)
			c[cx][cy][cz]->nc.upper_Chunk->changed = true;
		
		
		if (cz > 0)
		{
			c[cx][cy][cz]->nc.back_Chunk = c[cx][cy][cz - 1];
			c[cx][cy][cz - 1]->nc.front_Chunk = c[cx][cy][cz];
		}

		if (z == 0 && c[cx][cy][cz]->nc.back_Chunk)
			c[cx][cy][cz]->nc.back_Chunk->changed = true;

		else if (z == CHUNK_SIZE - 1 && c[cx][cy][cz]->nc.front_Chunk)
			c[cx][cy][cz]->nc.front_Chunk->changed = true;


		c[cx][cy][cz]->set(x, y, z, type, UniformTexture);
	}

}
