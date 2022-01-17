#version 450

layout(local_size_x = 1024) in;

layout(binding = 0) buffer posData
{
    int pos[];
};

void main()
{
    pos[gl_GlobalInvocationID.x] += 1;
}
