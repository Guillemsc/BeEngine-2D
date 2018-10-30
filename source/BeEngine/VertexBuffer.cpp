#include "VertexBuffer.h"

VertexBuffer::VertexBuffer()
{
}

void VertexBuffer::AddFloat(float set)
{
	if(NeedsResize(buffer_used + 1))
		AddSpace(1);

	buffer[buffer_used] = set;

	buffer_used += 1;
}

void VertexBuffer::AddFloat2(const float2& set)
{
	if (NeedsResize(buffer_used + 2))
		AddSpace(2);

	buffer[buffer_used] = set.x;
	buffer[buffer_size + 1] = set.y;

	buffer_used += 2;
}

void VertexBuffer::AddFloat3(const float3& set)
{
	if (NeedsResize(buffer_used + 3))
		AddSpace(3);

	buffer[buffer_used] = set.x;
	buffer[buffer_used + 1] = set.y;
	buffer[buffer_used + 2] = set.z;

	buffer_used += 3;
}

void VertexBuffer::AddFloat4(const float4 & set)
{
	if (NeedsResize(buffer_used + 4))
		AddSpace(4);

	buffer[buffer_used] = set.x;
	buffer[buffer_used + 1] = set.y;
	buffer[buffer_used + 2] = set.w;
	buffer[buffer_used + 3] = set.z;

	buffer_used += 4;
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
	buffer_used = 0;
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

bool VertexBuffer::NeedsResize(int new_size)
{
	bool ret = false;

	if (new_size > buffer_size)
		ret = true;

	return ret;
}
