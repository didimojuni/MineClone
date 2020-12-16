#include "mcpch.h"
#include "Chunk.h"
#include "MCP/Renderer/RenderCommand.h"

#include "MCP/Utils/Logger.h"

namespace MC
{
	constexpr uint32_t CHUNK_SQUARED = CHUNK_SIZE * CHUNK_SIZE;
	constexpr uint32_t CHUNK_SIZE_CUBED = CHUNK_SQUARED * CHUNK_SIZE;

#define PreCalcIndex(x, y, z, a) (x + z * CHUNK_SIZE + y * CHUNK_SQUARED) + (CHUNK_SIZE_CUBED * a)
	

	Chunk::Chunk() : elements(0), changed(true)
	{
		memset(blocks,        0, sizeof(blocks));
		memset(vertex,        0, sizeof(vertex));
		memset(VisitedBlocks, 0, sizeof(VisitedBlocks));

		VBO = RenderCommand::GenMesh(1);
	}

	Chunk::~Chunk()
	{
		RenderCommand::DelMesh(1, VBO);
	}

	uint8_t Chunk::get(int x, int y, int z)
	{
		return blocks[x][y][z];
	}

	void Chunk::set(int x, int y, int z, uint8_t type)
	{
		blocks[x][y][z] = type;
		changed = true;
	}


	void Chunk::update()
	{
	    uint8_t type = 0;
		changed = false;

		uint32_t vertexBufferIterator = 0;

		for (uint32_t y = 0; y <= CHUNK_SIZE; y++)	
			for (uint8_t z = 0; z <= CHUNK_SIZE; z++)	
				for (uint8_t x = 0; x <= CHUNK_SIZE; x++)
				{
					type = blocks[x][y][z];
					if (!type)
						continue;

					CalcFrontAndBackFace(x, y, z, type, vertexBufferIterator);
					CalcUpAndDownFace   (x, y, z, type, vertexBufferIterator);
					CalcRightAndLeftFace(x, y, z, type, vertexBufferIterator);
				}	

		elements = vertexBufferIterator; 

		memset(VisitedBlocks, 0, sizeof(VisitedBlocks));
	}

	//#TODO: Expandir para outras chunks
	bool Chunk::isFaceVisible(const uint8_t x, const uint32_t y, const uint8_t z, ECubeFace face)
	{
		switch (face)
		{
			case ECubeFace::LEFT:
			{
				if (x == 0 && nc.left_Chunk && nc.left_Chunk->blocks[CHUNK_SIZE - 1][y][z])
					return false;
			
				else if (x == 0 || (x > 0 && !blocks[x - 1][y][z]))
					return true;
				
				break;
			}
			case ECubeFace::RIGHT:
			{
				if (x == CHUNK_SIZE - 1 && nc.right_Chunk && nc.right_Chunk->blocks[0][y][z])
					return false;
				
				else if (x == CHUNK_SIZE -1 || !blocks[x + 1][y][z])
					return true;
				
				break;
			}
			case ECubeFace::DOWN:
			{
				if (y == 0 && nc.below_Chunk && nc.below_Chunk->blocks[x][CHUNK_SIZE - 1][z])
					return false;
				
				else if (y == 0 || (y > 0 && !blocks[x][y - 1][z]))
					return true;
				
					break;
			}
			case ECubeFace::UP:
			{
				if (y == CHUNK_SIZE - 1 && nc.upper_Chunk && nc.upper_Chunk->blocks[x][0][z])
					return false;
				
				else if (y == CHUNK_SIZE - 1 || !blocks[x][y + 1][z])
					return true;

				break;
			}
			case ECubeFace::BACK:
			{
				if (z == 0 && nc.back_Chunk && nc.back_Chunk->blocks[x][y][CHUNK_SIZE - 1])
					return false;
				
				else if (z == 0 || (z > 0 && !blocks[x][y][z - 1]))
					return true;
				
				break;
			}
			case ECubeFace::FRONT:
			{
				if (z == CHUNK_SIZE - 1 && nc.front_Chunk && nc.front_Chunk->blocks[x][y][0])
					return false;
				
				else if (z == CHUNK_SIZE - 1 || !blocks[x][y][z + 1])
					return true;

				break;
			}
		}

		return false;
	}

	uint32_t Chunk::PackVertexAtbs(const uint8_t x, const uint32_t y, const uint8_t z, const uint8_t normalLight, const uint8_t type)
	{
		uint32_t Pack = (normalLight << 13) | (z << 16) | (x << 20) | (y << 24);
		
		//	 Y        X    Z        N    I      Left
		// |00000000| |0000 0000| |000 00000| 00000000 

		//z 0-8
		//x 8-16
		//y 16-24
		
		return Pack;
	}

	void Chunk::GenCubeFace(const uint8_t x, const uint32_t y, const uint8_t z, const uint8_t length, const uint32_t height, const uint8_t depth, const uint8_t type, uint32_t& vertexIterator, ECubeFace face)
	{
		switch (face)
		{
			case ECubeFace::BACK:
			{
				vertex[vertexIterator++] = PackVertexAtbs(x,			 y,		      z, 2, type);
				vertex[vertexIterator++] = PackVertexAtbs(x,			 y + height,  z, 2, type);
				vertex[vertexIterator++] = PackVertexAtbs(x + length, y, z,			     2, type	 );
				vertex[vertexIterator++] = PackVertexAtbs(x,			 y + height,  z, 2, type);
				vertex[vertexIterator++] = PackVertexAtbs(x + length, y + height,  z,    2, type	 );
				vertex[vertexIterator++] = PackVertexAtbs(x + length, y,		      z, 2, type);
				
				break;
			}
			case ECubeFace::FRONT:
			{															 
				vertex[vertexIterator++] = PackVertexAtbs(x, y, z + 1, 4, type);
				vertex[vertexIterator++] = PackVertexAtbs(x + length, y, z + 1, 4, type);
				vertex[vertexIterator++] = PackVertexAtbs(x, y + height, z + 1, 4, type);
				vertex[vertexIterator++] = PackVertexAtbs(x, y + height, z + 1, 4, type);
				vertex[vertexIterator++] = PackVertexAtbs(x + length, y, z + 1, 4, type);
				vertex[vertexIterator++] = PackVertexAtbs(x + length, y + height, z + 1, 4, type);

				break;
			}
			case ECubeFace::LEFT:
			{
				vertex[vertexIterator++] = PackVertexAtbs(x, y,          z,		    4, type		);
				vertex[vertexIterator++] = PackVertexAtbs(x, y,		    z + depth,  4, type );
				vertex[vertexIterator++] = PackVertexAtbs(x, y + height, z,         4, type);
				vertex[vertexIterator++] = PackVertexAtbs(x, y + height, z,         4, type);
				vertex[vertexIterator++] = PackVertexAtbs(x, y,          z + depth, 4, type);
				vertex[vertexIterator++] = PackVertexAtbs(x, y + height, z + depth, 4, type);

				break;
			}
			case ECubeFace::RIGHT:
			{
				vertex[vertexIterator++] = PackVertexAtbs(x + 1, y,			z,          2, type);
				vertex[vertexIterator++] = PackVertexAtbs(x + 1, y + height, z,		    2, type);
				vertex[vertexIterator++] = PackVertexAtbs(x + 1, y,			z + depth,  2, type);
				vertex[vertexIterator++] = PackVertexAtbs(x + 1, y + height, z,         2, type);
				vertex[vertexIterator++] = PackVertexAtbs(x + 1, y + height, z + depth, 2, type);
				vertex[vertexIterator++] = PackVertexAtbs(x + 1, y,			z + depth,  2, type);


				break;
			}
			case ECubeFace::UP:
			{
				vertex[vertexIterator++] = PackVertexAtbs(x,		   	 y + 1, z,         5, type);
				vertex[vertexIterator++] = PackVertexAtbs(x,		  	 y + 1, z + depth, 5, type);
				vertex[vertexIterator++] = PackVertexAtbs(x + length, y + 1, z,			   5, type	   );
				vertex[vertexIterator++] = PackVertexAtbs(x + length, y + 1, z,			   5, type    );
				vertex[vertexIterator++] = PackVertexAtbs(x,          y + 1, z + depth,    5, type   );
				vertex[vertexIterator++] = PackVertexAtbs(x + length, y + 1, z + depth,    5, type   );

				break;
			}
			case ECubeFace::DOWN:
			{
				vertex[vertexIterator++] = PackVertexAtbs(x,          y, z,         1, type		);
				vertex[vertexIterator++] = PackVertexAtbs(x + length, y, z,         1, type		);
				vertex[vertexIterator++] = PackVertexAtbs(x,          y, z + depth, 1, type		);
				vertex[vertexIterator++] = PackVertexAtbs(x + length, y, z,         1, type		);
				vertex[vertexIterator++] = PackVertexAtbs(x + length, y, z + depth, 1, type		);
				vertex[vertexIterator++] = PackVertexAtbs(x,          y, z + depth, 1, type		);

				break;
			}
		}
	}

	//Although we use ECubeFace for readability inside our functions, we need to use literal integers in order to reduce the code to half 
	//And since it's not needed scalability (since we will never have more than six faces), we can use those integers without problem.
	//Each face have a number (that matches ECubeFace) reserved for it and we will use this here.
	//FRONT = 0, BACK = 1, UP = 2, DOWN = 3, LEFT = 4, RIGHT = 5
	//Then, we can cast the integer to the ECubeFace (which will be right) and use this same integer into PreCalcIndex.
	//Also, we can use those functions in whatever order we want.

	void Chunk::CalcFrontAndBackFace(const uint8_t x, const uint32_t y, const uint8_t z, uint8_t type, uint32_t& vertexBufferIterator)
	{
		//Front = 0 and Back = 1
		for (uint8_t i = 0; i < 2; i++)
		{
			uint8_t length = 0, height = 0;
			uint32_t PreviousLength = 0;

			ECubeFace face = (ECubeFace)i;

			for (uint32_t yy = y; yy <= CHUNK_SIZE; yy++)
			{
				for (uint32_t xx = x; xx <= CHUNK_SIZE; xx++)
				{
					//O bloco � diferente do atual, � vazio ou n�o visivel? Se sim, n�o o processe
					if (blocks[xx][yy][z] != blocks[x][yy][z] || !blocks[xx][yy][z] || VisitedBlocks[PreCalcIndex(xx, yy, z, i)] || !isFaceVisible(xx, yy, z, face))
						break;

					//Salvamos falando que o bloco ja foi visitado
					VisitedBlocks[PreCalcIndex(xx, yy, z, i)] = true;

					//Caso n�o seja, aumentamos o length, ou seja, mais um bloco � direita que cubrimos.
					length++;
				}

				if (yy > 0 && (PreviousLength != length || yy == CHUNK_SIZE))
				{
					GenCubeFace(x, yy - height, z, PreviousLength, height, 1, type, vertexBufferIterator, face);
					height = 0;
				}

				PreviousLength = length;
				length = 0;

				height++;

			}
		}

	}
	void Chunk::CalcUpAndDownFace(const uint8_t x, const uint32_t y, const uint8_t z, uint8_t type, uint32_t& vertexBufferIterator)
	{
		//Up = 2 and Down = 3
		for (uint8_t i = 2; i < 4; i++)
		{
			uint8_t length = 0, depth = 0;
			uint32_t PreviousLength = 0;

			ECubeFace face = (ECubeFace)i;

			for (uint32_t zz = z; zz <= CHUNK_SIZE; zz++)
			{
				for (uint32_t xx = x; xx <= CHUNK_SIZE; xx++)
				{
					//O bloco � diferente do atual, � vazio ou n�o visivel? Se sim, n�o o processe
					if (blocks[xx][y][zz] != blocks[x][y][zz] || !blocks[xx][y][zz] || VisitedBlocks[PreCalcIndex(xx, y, zz, i)] || !isFaceVisible(xx, y, zz, face))
						break;

					//Salvamos falando que o bloco ja foi visitado
					VisitedBlocks[PreCalcIndex(xx, y, zz, i)] = true;

					//Caso n�o seja, aumentamos o length, ou seja, mais um bloco � direita que cubrimos.
					length++;
				}

				if (zz > 0 && (PreviousLength != length || zz == CHUNK_SIZE))
				{
					GenCubeFace(x, y, zz - depth, PreviousLength, 1, depth, type, vertexBufferIterator, face);
					depth = 0;
				}

				PreviousLength = length;
				length = 0;

				depth++;
			}
		}
	}
	
	void Chunk::CalcRightAndLeftFace(const uint8_t x, const uint32_t y, const uint8_t z, uint8_t type, uint32_t& vertexBufferIterator)
	{
		//Left = 4 and Right = 5
		for (uint8_t i = 4; i < 6; i++)
		{
			uint8_t depth = 0, height = 0;
			uint32_t PreviousDepth = 0;

			ECubeFace face = (ECubeFace)i;

			for (uint32_t yy = y; yy <= CHUNK_SIZE; yy++)
			{
				for (uint32_t zz = z; zz <= CHUNK_SIZE; zz++)
				{
					//O bloco � diferente do atual, � vazio ou n�o visivel? Se sim, n�o o processe
					if (blocks[x][yy][zz] != blocks[x][yy][z] || !blocks[x][yy][zz] || VisitedBlocks[PreCalcIndex(x, yy, zz, i)] || !isFaceVisible(x, yy, zz, face))
						break;

					//Salvamos falando que o bloco ja foi visitado
					VisitedBlocks[PreCalcIndex(x, yy, zz, i)] = true;

					//Caso n�o seja, aumentamos o length, ou seja, mais um bloco � direita que cubrimos.
					depth++;
				}

				if (yy > 0 && (PreviousDepth != depth || yy == CHUNK_SIZE))
				{
					GenCubeFace(x, yy - height, z, 1, height, PreviousDepth, type, vertexBufferIterator, face);
					height = 0;
				}

				PreviousDepth = depth;
				depth = 0;

				height++;
			}
		}
	}
}