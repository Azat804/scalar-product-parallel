#include <iostream>
#include <fstream>
#include <omp.h>
#include <string>
using namespace std;
void Fill(int n,int a[],int b[], int cc[]) {
    for (int i = 0; i < n; i++) {
        a[i] = i+1;
        b[i] = i+1;
        cc[i] = 0;
    }
}

int main()
{
    ofstream fout("result.csv");
    if (!fout) {
        cout << "Error!";
    }
    int id;
    int i = 0;
    int idcount = 0;
    int* cc = new int[10000000];
    int ccc = 0;
    int size = 0;
    int sizeost = 0;
    double t = 0;
    int s = 0;
    int l = 0;
    int* a = new int[10000000];
    int* b = new int[10000000];
    double* tmaspar = new double[10000000];
    double* tmasserl = new double[10000000];
    int* nmas = new int[10000000];
    for (int y = 100; y < 1000;y+=10) {
        Fill(y,a,b,cc);
        t = omp_get_wtime();
#pragma omp parallel  private(id) shared(cc)
        {
            int id = omp_get_thread_num();
            idcount =omp_get_num_threads();
            size = y / idcount;
            sizeost = y - idcount * size;
            for (int j = 0; j < size; j++) {
                cc[id] =cc[id] +  b[j + id * size] * a[j + id * size];
            }
            if (id == omp_get_max_threads()-1) {
                for (int j2 = 0; j2 < sizeost; j2++) {
                    cc[id] = cc[id] + b[j2 + (id+1) * size] * a[j2 + (id+1) * size];
                }
            }
        }
        for (int k=0; k<y; k++) {
            s += cc[k];
        }
        t = omp_get_wtime() - t;
        printf("\n total for n= %d:%d", y,s);
        printf("\n total parallel time: %f", t);
        nmas[l] = y;
        tmaspar[l] = t*1000;
        t = omp_get_wtime();
        for (int h = 0; h < y; h++) {
            ccc = ccc + a[h] * b[h];
        }
        t = omp_get_wtime() - t;
        printf("\n total for n= %d:%d", y, ccc);
        printf("\n total serial time: %f", t);
        tmasserl[l] = t*1000;
        ccc = 0;
        s = 0;
        l += 1;
        printf("\n");
    }
    std::string ss="";
    for (int m = 0; m < l-1; m++) {
       ss += std::to_string(nmas[m])+";"+std::to_string(tmasserl[m])+";"+ std::to_string(nmas[m])+";"+ std::to_string(tmaspar[m])+";";
       fout << ss;
       fout << "\n";
       ss = "";
    }
    fout.close();
}