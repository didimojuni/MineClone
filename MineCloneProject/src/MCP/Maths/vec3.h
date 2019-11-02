#pragma once


namespace MC
{

	//Para manter a simplicidade, n�o vou usar de templates ou classes bases/derivadas.

	// TODO OPTIMIZATION STAGE: Implementar opera��es em SIMD a fim de otimizar a velocidade dos calculos. 
	// Por enquanto vou deixar isso nas m�os do compilador usado.


	struct vec3
	{
		vec3();
		vec3(float scalar);
		vec3(const float xx, const float yy, const float zz);

		std::string ToString() const;

		float Lenght();
		float Lenght2();
		vec3 Normalized();

		static float Lenght(const vec3& vec);
		static float Lenght2(const vec3& vec);
		static vec3 Normalize(const vec3& vec);
		static vec3 Cross(const vec3& first, const vec3& second);

		float Dot(const vec3& other);
		vec3 Cross(const vec3& other);

		//Retornando uma referencia para o proprio objeto a fim de evitar criar novos objetos e podermos com eficiencia
		//juntar uma opera��o na outra
		vec3& add(const vec3& other);
		vec3& subtract(const vec3& other);
		vec3& multiply(const vec3& other);
		vec3& divide(const vec3& other);

		friend vec3 operator+(vec3 left, const vec3& right);
		friend vec3 operator-(vec3 left, const vec3& right);
		friend vec3 operator*(vec3 left, const vec3& right);
		friend vec3 operator/(vec3 left, const vec3& right);


		vec3& operator+=(const vec3& other);
		vec3& operator-=(const vec3& other);
		vec3& operator*=(const vec3& other);
		vec3& operator/=(const vec3& other);

		bool operator==(const vec3& other);
		bool operator!=(const vec3& other);

		union {
			struct {
				float x, y, z;
			};
			struct {
				float r, g, b;
			};
			struct {
				float elements[3];
			};
		};
	};
}

