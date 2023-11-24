#include <stdlib.h>
#include <stdio.h>

typedef struct
{
    int  height;
    int  width;
    int**  matrix;
}
MAT;

void disp(MAT M); // displays matrix
int** assign(int height, int width); // assigns memory
int* inverse(int* v, int width); //inverses permutation
int* product(int* u, int* v, int width); //composes permutations
MAT generators(MAT M);
int* line(MAT M, int a);
MAT orbits(MAT M, int y);
MAT stabilise(MAT M, int a);

// specific values

//

int main(void)
{
    MAT B;
    B.height = 2;
    B.width = 5;
    B.matrix = assign(B.height,B.width);
    int C[2][5] = {{2,3,4,5,1}, {4,3,2,1,5}};

    for (int i = 0; i < B.height; i++)
    {
        for (int j = 0; j < B.width; j++)
        {
            B.matrix[i][j] = C[i][j];
        }
    }



    MAT M = stabilise(B,1);
    int s = M.width;
    M.width = B.width;

    int n = B.width;
    int size = 1;
    while (n > 1)
    {
        B = stabilise(B,n);
        size = size * B.width;
        B.width = n-1;
        n = n - 1;

    }
    printf("group size is %i\n", size);

}



// many many functions below
//
//
//

MAT stabilise(MAT M, int a)
{
    M = generators(M);
    int n = M.width;
    int h = M.height;

    MAT P = orbits(M,a);

    int v[n];
    int s = 0;
    for (int i = 0; i < n; i++)
    {
        if (P.matrix[i][0] == 0)
        {
            v[i] = 0;
        }
        else
        {
            v[i] = 1;

            s++;
        }
    }

    MAT G;
    G.height = s*h;
    G.width = n;
    G.matrix = assign(G.height, G.width);

    int t = 0;

    //int l[n];
    //int g[n];
    //int p[n];
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < h; j++)
        {
            if (v[i] == 1) //want (pg)-1 pg for p in P, g in M, and then stabilise by P
            {

                int* m = line(M,j); //1+
                int* p = line(P,i);
                int* l = product(m,p,n);
                int* l2 = line(P, l[a-1]-1);
                int* l3 = product(inverse(l2,n),l,n);
                for (int k = 0; k < n; k++)
                {
                    G.matrix[t][k] = l3[k];
                }
                t++;
            }
        }
    }

    MAT H = generators(G);
    H.width = s;
    return H;
}


MAT orbits(MAT M, int a)
{
    M = generators(M);
    int n = M.width;
    int h = M.height;
    MAT P;
    P.height = n;
    P.width = n;
    P.matrix = assign(n,n);

    MAT T;
    T.height = n;
    T.width = n;
    T.matrix = assign(n,n);

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            T.matrix[i][j] = -1;
        }
    }

    int x[n];
    int y[n];
    for (int i = 0; i < n; i++)
    {
        x[i] = 0;
        P.matrix[a-1][i] = i+1;
    }
    x[a-1] = 1;
    y[0] = a-1;
    int k = 0;
    int l = 1;

    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < n; j++)
        {
            T.matrix[j][M.matrix[i][j]-1] = i;
        }
    }

    while (k < l)
    {
        int c = y[k];

        for (int i = 0; i < n; i++) //i =/= a??
        {
            if (x[i] == 0 && T.matrix[c][i] > -1)
            {
                int s = T.matrix[c][i];
                for (int j = 0; j < n; j++)
                {
                    //compose P row c and M row T[c][i]-1
                    P.matrix[i][j] = M.matrix[s][P.matrix[c][j]-1];
                }
                x[i] = 1;
                y[l] = i;
                l++;
            }
        }
        k++;
    }

    return P;
}


MAT generators(MAT M) //returns generators, if empty returns the identity
{
    int n = M.width;

    //initialise permutation matrix A, set to 0
    int*** A = malloc(n*sizeof(int*));
    for (int i = 0; i < n; i++)
    {
        A[i] = malloc(n*sizeof(int));
        for (int j = 0; j < n; j++)
        {
            A[i][j] = malloc(n*sizeof(int));
            for (int k = 0; k < n; k++)
            {
                A[i][j][k] = 0;
            }
        }
    }


    int h = M.height;
    int t = 0;
    //placing each perm into the table
    for (int b = 0; b < h; b++)
    {

        int* p = line(M, b);
        int i = 0;
        //printf("b = %i\n", b);
        while (i < n)
        {
            //i = first non fixed point
            if (p[i] == i+1)
            {
                i++;
            }
            else
            {
                int j = p[i]-1;  //so p(i+1) =  j

                if (A[i][j][0] == 0) //if slot is empty enter p
                {
                    for (int k = 0; k < n; k++)
                    {
                        A[i][j][k] = p[k];
                    }
                    t++;
                    break;
                }
                else // if slot not empty
                {
                    int* w = malloc(n*sizeof(int));
                    for (int l = 0; l < n; l++)
                    {
                        w[l] = A[i][j][l];
                    }

                    int* u = inverse(w, n);
                    int *v = product(u, p, n);
                    p = v;
                }
            }
        }
    }

    // t = number of filled spots?
    //if t = 0 add the identity perm
    // else fill with all non zero perms
    MAT C;
    if (t == 0)
    {
        C.height = 1;
        C.width = n;
        C.matrix = assign(1, n);
        for (int i = 0; i < n; i++)
        {
            C.matrix[0][i] = i+1;
        }
    }
    else
    {
        C.height = t;
        C.width = n;
        C.matrix = assign(t,n);

        int b = 0;
        for (int i = 0; i < n; i++)
        {
            for (int j = i+1; j < n; j++)                  ////////////////////////////////
            {
                if (A[i][j][0] != 0)
                {
                    for (int k = 0; k < n; k++)
                    {
                        C.matrix[b][k] = A[i][j][k];
                    }
                    b++;
                }
            }
        }
    }

    return C;
}

int* product(int* u, int* v, int width)
{
    int* w = malloc(width*sizeof(int));

    for (int i = 0; i < width; i++)
    {
        w[i] = u[v[i]-1];
    }

    return w;
}

void disp(MAT M)
{
    for (int i = 0; i < M.height; i++)
    {
        for (int j = 0; j < M.width; j++)
        {
            printf("%i ", M.matrix[i][j]);
        }
        printf("\n");
    }

}

int** assign(int height, int width)
{
    int** out = malloc(height*sizeof(int*));
    for (int i = 0; i < height; i++)
    {
        out[i] = malloc(width*sizeof(int));
    }
    return out;
}

int* inverse(int* v, int width)
{
    int* w = malloc(width*sizeof(int));

    for (int i = 0; i < width; i++)
    {
        w[v[i]-1] = i+1;
    }

    return w;
}

int* line(MAT M, int a)
{
    int n = M.width;
    int* w = malloc(n*sizeof(int));

    for (int i = 0; i < n; i++)
    {
        w[i] = M.matrix[a][i];
    }

    return w;
}
