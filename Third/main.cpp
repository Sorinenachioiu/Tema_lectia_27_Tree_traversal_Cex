#include <fstream>
#include <vector>

using namespace std;
ifstream in("parcurgere_3.in");// the file that contains the input
ofstream out("parcurgere_3.out");// the file in which the results will be written

const int NMAX = 100;// maximum number of verticles of the tree

bool fr[NMAX];// frequency vector to see what verticles are visited
int SRD[NMAX];// vector that contains the SRD traversal of the verticles
int SDR[NMAX];// vector that contains the SDR traversal of the verticles
int R[NMAX];// vector that will contain the RSD traversal / the seeked result
int crt_pos_R = 1;// keeps the crt position in the R vector

// function to read n elements of a vector
void read(int v[], int n)
{
    register int i;
    for(i = 1; i <= n; i++)
    {
        in >> v[i];
    }
}

// function to find the root in the SRD traversal
int find_root_SRD(int n)
{
    register int i;
    for(i = 1; i <= n; i++)
        if(SRD[i] == 1)
            return i;
    return -1;
}

// function to inverse a vector
void inverse(int v[], int n)
{
    int aux;
    register int i;
    for(i = 1; i <= n/2; i++)
    {
        aux = v[i];
        v[i] = v[n-i+1];
        v[n-i+1] = aux;
    }
}

// function to verify if the current possition contains an already visited verticle
// or if the current verticle is the seeked one
int match(int srch_v_pos, int crt_pos)
{
    if(fr[SRD[crt_pos]] == 1)// verify if the current verticle in SRD is visited
        return -1;
    return SRD[crt_pos] == SDR[srch_v_pos];// return whether or not the current
                                           // verticle in SRD correspond with the
                                           // one in SDR
}

// search in the left of the current verticle in the SRD vector for the next SRD verticle
bool left_search(int searched_verticle_pos, int crt_verticle_pos, int &pos)
{
    register int i;
    int found;
    for(i = crt_verticle_pos - 1; i > 0; i--)
    {
        found = match(searched_verticle_pos, i);
        if(found == -1)
            return 0;// an already visited verticle is found
        else
        {
            if(found == 1)// the searched verticle is found
            {
                pos = i;
                return 1;
            }

        }
    }
    return 0;// nothing has been found
}

// search in the right of the current verticle in the SRD vector for the next SDR verticle
bool right_search(int searched_verticle_pos, int crt_verticle_pos, int &pos, int n)
{
    register int i;
    int found;
    for(i = crt_verticle_pos + 1; i <= n; i++)
    {
        found = match(searched_verticle_pos, i);
        if(found == -1)// an already visited verticle is found
            return 0;
        else
        {
            if(found == 1)// the searched verticle is found
            {
                pos = i;
                return 1;
            }
        }
    }
    return 0;// nothing has been found
}

// function that fiinds the SDR from SRD and SDR traversals
void find_SDR(int &crt_pos_RSD, int crt_pos_SRD, int n)
{
    bool found = 0;// 0 if the current verticle is not the ancestor of the next verticle
                   // in the SDR, 1 otherwise
    int pos = -1;// in it we store the position in SRD in which we have found the
                 // next verticle from SDR

    found = left_search(crt_pos_RSD + 1, crt_pos_SRD, pos) ||
            right_search(crt_pos_RSD + 1, crt_pos_SRD, pos, n);// search the next verticle
                                                               // from SDR in the left and
                                                               // in the right of the current
                                                               // SRD position
    if(found)// if we found the seeked verticle
    {
        crt_pos_RSD ++; // we go to the next verticle from SDR
        fr[SDR[crt_pos_RSD]] = 1; // we mark the value of the current position in SDR
                                  // as visited in the frequency vector fr

        find_SDR(crt_pos_RSD, pos, n);// we search the next verticle from SDR
                                       // starting from pos in SRD

        find_SDR(crt_pos_RSD, crt_pos_SRD, n);// we also search the next verticle from
                                               // SDR starting from the current position
                                               // in SRD
    }
    else
    {
        R[crt_pos_R] = SRD[crt_pos_SRD];// the element that is on the crt_pos_SRD in the
                                        // SRD vector is saved in the R vector
        crt_pos_R++;
    }
}

/*RSD*/
// function to display the vector inverted
void display(int v[], int n)
{
    register int i;
    for(i = n; i >= 1; i--)
        out << R[i] <<" ";
}

int main()
{
    register int root_pos_SDR = 1;
    int n;// dim of string / nr of verticles

    in>>n;

    read(SRD, n);// read the SRD traversal
    read(SDR, n);// read the SDR traversal

    inverse(SDR, n);// inverse the SDR vector

    int root_pos_SRD = find_root_SRD(n);// find the position in which the root is situated
    if(root_pos_SRD == -1)
    {
        out << "No root found";
    }
    else
    {
        fr[1] = 1;// visit the root
        find_SDR(root_pos_SDR, root_pos_SRD, n);// form the tree starting from the
                                                 // root_pos_RSD and root_pos_SRD
        display(R, n);//display the result / RSD
    }

    return 0;
}
