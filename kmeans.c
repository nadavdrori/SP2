#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "cap.h"

// struct cord
// {
//     double value;
//     struct cord *next;
// };
// struct vector
// {
//     struct vector *next;
//     struct cord *cords;
// };

// void String_to_vector (struct vector *head_vec, int i, double *coor_arr)
// {
//     int l = i;
//     struct vector *run_vec, *prev_vec;
//     struct cord *run_cord, *prev_cord;
//     run_vec = head_vec;
//     while(run_vec != NULL){
//         run_cord = run_vec->cords;
//         while (run_cord != NULL)
//         {
//             coor_arr[l] = (double)run_cord->value;
//             l++;
//             prev_cord = run_cord;
//             run_cord = run_cord->next;
//             free(prev_cord);
//         }
//         prev_vec = run_vec;
//         run_vec = run_vec->next;
//         free(prev_vec);
//     }
// }

double **create_vector_pointers(int vector_len, int vector_data_len, double *vector_data)
{
    double **tmp;
    int i;
    int vector_pointers_len = ((double)vector_data_len / (double)vector_len);
    tmp = (double **)calloc(vector_pointers_len, sizeof(double *));

    for (i = 0; i < vector_pointers_len; i++)
    {
        *(tmp + i) = (vector_data + (i * vector_len));
    }
    return tmp;
}

double *create_centroid_data(double *vector_data, int k, int vector_len, int *centroid_index_list)
{
    double *tmp;
    int i, j;
    int centroid_data_len = k * vector_len;
    tmp = (double *)calloc(centroid_data_len, sizeof(double));
    // for (i = 0; i < centroid_data_len; i++)
    // {
    //     *(tmp+i) = *(vector_data+i);
    // }
    // return tmp;

    for (j = 0; j < k; j++)
    {
        for (i = 0; i < vector_len; i++)
        {
            *(tmp + (j * vector_len) + i) = *(vector_data + ((*(centroid_index_list + j)) * vector_len) + i); // maybe (*(centroid_index_list + j) - 1)
        }
    }
}

double *create_backup_centroid_data(int k, int vector_len)
{
    double *tmp;
    int centroid_data_len = k * vector_len;
    tmp = (double *)calloc(centroid_data_len, sizeof(double));
    return tmp;
}

double **create_centroid_pointers(int k, int vector_len, double *centroid_data)
{
    double **tmp;
    int i;
    tmp = (double **)calloc(k, sizeof(double *));

    for (i = 0; i < k; i++)
    {
        *(tmp + i) = (centroid_data + (i * vector_len));
    }
    return tmp;
}

int *create_vector_to_centroid_relations(int vector_len, int vector_data_len)
{
    int *tmp;
    int vector_pointers_len = ((double)vector_data_len / (double)vector_len);
    tmp = (int *)calloc(vector_pointers_len, sizeof(int));
    return tmp;
}

double calc_dist(int vector_len, double *vector1, double *vector2)
{
    double sum = 0;
    int i;
    for (i = 0; i < vector_len; i++)
    {
        sum = sum + pow(*(vector1 + i) - *(vector2 + i), 2);
    }
    return pow(sum, 0.5f);
}

void add_to_vector(int vector_len, double *vector, double *added_vector)
{
    int i;
    for (i = 0; i < vector_len; i++)
    {
        *(vector + i) = *(vector + i) + *(added_vector + i);
    }
}

void scalar_mult_vector(int vector_len, double *vector, double scalar)
{
    int i;
    for (i = 0; i < vector_len; i++)
    {
        *(vector + i) = scalar * *(vector + i);
    }
}

int get_vector_closest_centroid(double *vector, int vector_len, double **centroid_pointers, int k)
{
    int min_centroid_index = -1;
    double min_dist = 0.0f;
    double tmp = 0.0f;
    int i;
    for (i = 0; i < k; i++)
    {
        tmp = calc_dist(vector_len, vector, *(centroid_pointers + i));
        if (i == 0)
        {
            min_centroid_index = 0;
            min_dist = tmp;
        }
        else
        {
            if (tmp < min_dist)
            {
                min_centroid_index = i;
                min_dist = tmp;
            }
        }
    }
    return min_centroid_index;
}

void print_double_pointer(int len, double *p)
{
    int i;
    for (i = 0; i < len; i++)
    {
        printf("%.4f", *(p + i));
        if (i != len - 1)
        {
            printf(",");
        }
    }
    printf("\n");
}

void update_vector_to_centroid_relations(double **vector_pointers, int vector_data_len, int vector_len, double **centroid_pointers, int k, int *vector_to_centroid_relations)
{
    int vector_pointers_len = ((double)vector_data_len / (double)vector_len);
    int i;
    for (i = 0; i < vector_pointers_len; i++)
    {
        *(vector_to_centroid_relations + i) = get_vector_closest_centroid(*(vector_pointers + i), vector_len, centroid_pointers, k);
    }
}

void update_backup_centroid_data(int vector_len, int k, double *centroid_data, double *backup_centroid_data)
{
    int i;
    for (i = 0; i < (k * vector_len); i++)
    {
        *(backup_centroid_data + i) = *(centroid_data + i);
    }
}

int compare_centroid_pointers(int vector_len, int k, double **centroid_pointers, double **backup_centroid_pointers, double epsilon)
{
    int i;
    for (i = 0; i < k; i++)
    {
        if (calc_dist(vector_len, *(centroid_pointers + i), *(backup_centroid_pointers + i)) >= epsilon)
        {
            return 0;
        }
    }
    return 1;
}

void update_centroid_data(double **vector_pointers, int vector_data_len, int vector_len, double *centroid_data, double **centroid_pointers, int k, int *vector_to_centroid_relations, int *counter)
{
    int vector_pointers_len = ((double)vector_data_len / (double)vector_len);
    int i = 0;
    for (i = 0; i < k; i++)
    {
        *(counter + i) = 0;
    }
    for (i = 0; i < k * vector_len; i++)
    {
        *(centroid_data + i) = 0;
    }
    for (i = 0; i < vector_pointers_len; i++)
    {
        *(counter + *(vector_to_centroid_relations + i)) = *(counter + *(vector_to_centroid_relations + i)) + 1;
        add_to_vector(vector_len, *(centroid_pointers + *(vector_to_centroid_relations + i)), *(vector_pointers + i));
    }
    for (i = 0; i < k; i++)
    {
        scalar_mult_vector(vector_len, *(centroid_pointers + i), 1 / ((double)(*(counter + i))));
    }
}

void kmeans_algorithm(double **vector_pointers, double *centroid_data, double *backup_centroid_data, int vector_data_len, int vector_len, double **centroid_pointers, double **backup_centroid_pointers, int k, int *vector_to_centroid_relations, int iter, double epsilon, int *counter)
{
    int i = 0;
    int check = 0;
    while (i < iter && check == 0)
    {
        update_vector_to_centroid_relations(vector_pointers, vector_data_len, vector_len, centroid_pointers, k, vector_to_centroid_relations);
        update_centroid_data(vector_pointers, vector_data_len, vector_len, centroid_data, centroid_pointers, k, vector_to_centroid_relations, counter);
        i++;
        if (i > 0)
        {
            check = compare_centroid_pointers(vector_len, k, centroid_pointers, backup_centroid_pointers, epsilon);
        }
        update_backup_centroid_data(vector_len, k, centroid_data, backup_centroid_data);
    }
}

double *kmeans(double *vector_data, int *centroid_index_list, int vector_len, int vector_data_len, int k, int iter, int epsilon)
{
    // int iter = 200;
    // double n;
    // char c;
    // int k;
    //  double *coor_arr;
    //  double *vector_data;
    //  int vector_len;
    //  int vector_data_len;
    // double epsilon = 0.001;
    int i = 0;
    double **vector_pointers, **centroid_pointers, **backup_centroid_pointers;
    double *centroid_data, *backup_centroid_data;
    int *vector_to_centroid_relations, *counter;

    // if (coor_arr == NULL) {
    //     printf("An Error Has Occurred\n");
    //     return 1;
    // }

    // vector_data = coor_arr;
    // vector_len = vec_size;
    // vector_data_len = arr_size;

    // if (argc == 3)    {
    //     iter = atoi(argv[2]);
    //     k = atoi(argv[1]);
    // }
    // else if (argc == 2)    {
    //     k = atoi(argv[1]);
    // }
    // else{
    //     printf("An Error Has Occurred\n");
    //     return 1;
    // }

    // if (k <= 1 || k >= vec_num) {
    //      printf("Invalid number of clusters!\n");
    //      return 1;
    // }
    // if (iter <= 1 || iter >= 1000) {
    //      printf("Invalid maximum iteration!\n");
    //      return 1;
    // }
    // epsilon = 0.001;
    vector_pointers = create_vector_pointers(vector_len, vector_data_len, vector_data);
    centroid_data = create_centroid_data(vector_data, k, vector_len, centroid_index_list); // complete
    centroid_pointers = create_centroid_pointers(k, vector_len, centroid_data);
    backup_centroid_data = create_backup_centroid_data(k, vector_len);
    backup_centroid_pointers = create_centroid_pointers(k, vector_len, backup_centroid_data);
    vector_to_centroid_relations = create_vector_to_centroid_relations(vector_len, vector_data_len);
    counter = (int *)calloc(k, sizeof(int));

    if (
        vector_pointers == NULL ||
        centroid_data == NULL ||
        centroid_pointers == NULL ||
        backup_centroid_data == NULL ||
        backup_centroid_pointers == NULL ||
        vector_to_centroid_relations == NULL ||
        counter == NULL)
    {
        printf("An Error Has Occurred\n");
        exit(1);
    }

    kmeans_algorithm(vector_pointers, centroid_data, backup_centroid_data, vector_data_len, vector_len, centroid_pointers, backup_centroid_pointers, k, vector_to_centroid_relations, iter, epsilon, counter);
    for (i = 0; i < k; i++)
    {
        print_double_pointer(vector_len, *(centroid_pointers + i));
    }

    free(vector_pointers);
    free(centroid_data);
    free(centroid_pointers);
    free(backup_centroid_data);
    free(backup_centroid_pointers);
    free(vector_to_centroid_relations);
    free(counter);

    return centroid_data;
}

// int main(int argc, char **argv)
// {
//     return 0;
// }