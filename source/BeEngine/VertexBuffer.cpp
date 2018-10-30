#include "VertexBuffer.h"

VertexBuffer::VertexBuffer()
{
}

void VertexBuffer::AddFloat(float set)
{
	AddSpace(1);

	buffer[buffer_size - 1] = set;
}

void VertexBuffer::AddFloat2(const float2& set)
{
	AddSpace(2);

	buffer[buffer_size - 2] = set.x;
	buffer[buffer_size - 1] = set.y;
}

void VertexBuffer::AddFloat3(const float3& set)
{
	AddSpace(3);

	buffer[buffer_size - 3] = set.x;
	buffer[buffer_size - 2] = set.y;
	buffer[buffer_size - 1] = set.z;
}

void VertexBuffer::AddFloat4(const float4 & set)
{
	AddSpace(4);

	buffer[buffer_size - 4] = set.x;
	buffer[buffer_size - 3] = set.y;
	buffer[buffer_size - 2] = set.w;
	buffer[buffer_size - 1] = set.z;
}

const float * VertexBuffer::GetBuffer() const
{
	return buffer;
}

const uint VertexBuffer::GetSize() const
{
	return (sizeof(float) * buffer_size);
}

void VertexBuffer::Clear()
{
	RELEASE_ARRAY(buffer);
	buffer_size = 0;
}

void VertexBuffer::Resize(int size)
{
	float* new_buffer = new float[size];

	if (buffer != nullptr)
	{
		memcpy(new_buffer, buffer, buffer_size * sizeof(float));

		RELEASE_ARRAY(buffer);
	}

	buffer = new_buffer;

	buffer_size = size;
}

void VertexBuffer::AddSpace(int add)
{
	Resize(buffer_size + add);
}
