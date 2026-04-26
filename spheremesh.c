#include<math.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
typedef struct SphereMesh
{
    float center[3];
    float *arr;
    size_t len;
}SphereMesh;
void write_vertex(SphereMesh *mesh, size_t ind, float x, float y, float z)
{
    mesh->arr[ind * 3] = x;
    mesh->arr[ind * 3 + 1] = y;
    mesh->arr[ind * 3 + 2] = z;
}
void print_single_vertex(const float *vert)
{
    printf("%.4g %.4g %.4g", vert[0], vert[1], vert[2]);
}
void print_single_face(const SphereMesh *mesh, const size_t *indices, size_t len)
{
    float reciprocal = 1.0f / len;
    float center[3] = {0.0f, 0.0f, 0.0f};
    size_t ind;
    printf("%zu ", len);
    for(size_t i = 0; i < len; ++i)
    {
        ind = indices[i] * 3;
        center[0] += mesh->arr[ind];
        center[1] += mesh->arr[ind + 1];
        center[2] += mesh->arr[ind + 2];
        print_single_vertex(mesh->arr + ind);
        putchar(' ');
    }
    center[0] *= reciprocal;
    center[1] *= reciprocal;
    center[2] *= reciprocal;
    center[0] -= mesh->center[0];
    center[1] -= mesh->center[1];
    center[2] -= mesh->center[2];
    print_single_vertex(center);
    putchar('\n');
}
SphereMesh make_vertices(float sphere[], float angle, unsigned short precision)
{
    SphereMesh mesh;
    size_t cnt = precision;
    mesh.len = cnt;
    mesh.arr = malloc((8 * cnt * cnt - 4 * cnt + 2) * 3 * sizeof(float));
    memcpy(mesh.center, sphere, sizeof(mesh.center));
    if(mesh.arr != NULL)
    {
        size_t ind = 1;
        float pitch = -1.570796326f;
        float yaw = 0.0f;
        float x, y, z;
        write_vertex(&mesh, 0, sphere[0], sphere[1], sphere[2] - sphere[3]);
        for(size_t i = 0; i < cnt * 2 - 1; ++i)
        {
            pitch += angle;
            yaw = 0.0f;
            for(size_t j = 0; j < 4 * cnt; ++j, yaw += angle)
            {
                z = cos(pitch);
                y = sin(yaw) * z * sphere[3] + sphere[1];
                x = cos(yaw) * z * sphere[3] + sphere[0];
                z = sin(pitch) * sphere[3] + sphere[2];
                write_vertex(&mesh, ind, x, y, z);
                ++ind;
            }
        }
        write_vertex(&mesh, ind, sphere[0], sphere[1], sphere[2] + sphere[3]);
    }
    else
        perror("malloc failed");
    return mesh;
}
int print_faces(float sphere[], float angle, unsigned short precision)
{
    int succ = 0;
    SphereMesh mesh = make_vertices(sphere, angle, precision);
    size_t face_indices[4];
    if(mesh.arr != NULL)
    {
        face_indices[0] = 0;
        face_indices[1] = 2;
        face_indices[2] = 1;
        for(size_t i = 0; i < mesh.len * 4 - 1; ++i)
        {
            print_single_face(&mesh, face_indices, 3);
            ++face_indices[1];
            ++face_indices[2];
        }
        face_indices[1] = 1;
        print_single_face(&mesh, face_indices, 3);
        for(size_t i = 0; i < mesh.len * 2 - 2; ++i)
        {
            face_indices[0] = 1 + i * mesh.len * 4;
            face_indices[1] = 2 + i * mesh.len * 4;
            face_indices[2] = 1 + (i + 1) * mesh.len * 4;
            face_indices[3] = 2 + (i + 1) * mesh.len * 4;
            for(size_t j = 0; j < mesh.len * 4 - 1; ++j)
            {
                print_single_face(&mesh, face_indices, 4);
                ++face_indices[0];
                ++face_indices[1];
                ++face_indices[2];
                ++face_indices[3];
            }
            face_indices[1] -= mesh.len * 4;
            face_indices[3] -= mesh.len * 4;
            print_single_face(&mesh, face_indices, 4);
        }
        face_indices[0] = 8 * mesh.len * mesh.len - 8 * mesh.len + 1;
        face_indices[1] = 8 * mesh.len * mesh.len - 8 * mesh.len + 2;
        face_indices[2] = 8 * mesh.len * mesh.len - 4 * mesh.len + 1;
        for(size_t i = 0; i < mesh.len * 4 - 1; ++i)
        {
            print_single_face(&mesh, face_indices, 3);
            ++face_indices[0];
            ++face_indices[1];
        }
        face_indices[1] -= mesh.len * 4;
        print_single_face(&mesh, face_indices, 3);
        free(mesh.arr);
    }
    else
        succ = 1;
    return succ;
}
int print_vertices(float sphere[], float angle, unsigned short precision)
{
    int succ = 0;
    SphereMesh mesh = make_vertices(sphere, angle, precision);
    if(mesh.arr != NULL)
    {
        size_t ind = 1;
        print_single_vertex(mesh.arr);
        putchar('\n');
        for(size_t i = 0; i < mesh.len * 2 - 1; ++i)
        {
            for(size_t j = 0; j < mesh.len * 4; ++j)
            {
                print_single_vertex(mesh.arr + ind * 3);
                putchar('\n');
                ++ind;
            }
        }
        print_single_vertex(mesh.arr + ind * 3);
        putchar('\n');
        free(mesh.arr);
    }
    else
        succ = 1;
    return succ;
}
int main(int argl, char *argv[])
{
    int succ = 1;
    if(argl == 1)
        printf("Usage: %s [x] [y] [z] [radius] [precision>=1] [true/false]\n", argv[0]);
    else
    {
        float data[4] = {0.0f, 0.0f, 0.0f, 1.0f};
        unsigned short precision = 4;
        char faces = 0;
        for(int i = 0; i < argl - 1 && i < 4; ++i)
            data[i] = atof(argv[i+1]);
        if(argl > 5)
            precision = atoi(argv[5]) & 0xffff;
        if(argl > 6)
            faces = argv[6][0] == 't';
        if(faces)
            succ = print_faces(data, 1.570796326f / precision, precision);
        else
            succ = print_vertices(data, 1.570796326f / precision, precision);
    }
    return succ;
}
