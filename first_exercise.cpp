#include <fstream>
#include <vector>

using namespace std;
ifstream in("parcurgere.in");// the file that contains the input
ofstream out("parcurgere.out");// the file in which the results will be written

const int NMAX = 100;// maximum number of verticles of the tree

bool fr[NMAX];// frequency vector to see what verticles are visited
vector <int> mu[NMAX];// Adjacency matrix
int SRD[NMAX];// vector that contains the SRD traversal of the verticles
int RSD[NMAX];// vector that contains the RSD traversal of the verticles

// function to read n elements of a vector
void read(int v[], int n)
{
    register int i;
    for(i = 1; i <= n; i++)
    {
        in >> v[i];
    }
}

// function that writes the anchestors of each verticle
void write_tree(int n)
{
    register int i, j;
    for(i = 1; i <= n; i++)
    {
        out<< i<<" ";
        for(j = 0; j < mu[i].size(); j++)
        {
            out<<mu[i][j]<<" ";
        }
        out<<'\n';
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

// function to verify if the current possition contains an already visited verticle
// or if the current verticle is the seeked one
int match(int srch_v_pos, int crt_pos)
{
    if(fr[SRD[crt_pos]] == 1)// verify if the current verticle in SRD is visited
        return -1;
    return SRD[crt_pos] == RSD[srch_v_pos];// return whether or not the current
                                           // verticle in SRD correspond with the
                                           // one in RSD
}

// search in the left of the current verticle in the SRD vector for the next RSD verticle
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

// search in the right of the current verticle in the SRD vector for the next RSD verticle
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

// function that fiinds the tree from SRD and RSD traversals
void find_tree(int &crt_pos_RSD, int crt_pos_SRD, int n)
{
    bool found = 0;// 0 if the current verticle is not the ancestor of the next verticle
                   // in the RSD, 1 otherwise
    int pos = -1;// in it we store the position in SRD in which we have found the
                 // next verticle from RSD

    found = left_search(crt_pos_RSD + 1, crt_pos_SRD, pos) ||
            right_search(crt_pos_RSD + 1, crt_pos_SRD, pos, n);// search the next verticle
                                                               // from RSD in the left and
                                                               // in the right of the current
                                                               // SRD position
    if(found)// if we found the seeked verticle
    {
        crt_pos_RSD ++; // we go to the next verticle from RSD
        fr[RSD[crt_pos_RSD]] = 1; // we mark the value of the current position in RSD
                                  // as visited in the frequency vector fr
        mu[RSD[crt_pos_RSD]].push_back(SRD[crt_pos_SRD]);// the found verticle is the
                                                         // descendant of the one stored
                                                         // on the crt_pos_SRD in the
                                                         // SRD vector

        find_tree(crt_pos_RSD, pos, n);// we search the next verticle from RSD
                                       // starting from pos in SRD
        find_tree(crt_pos_RSD, crt_pos_SRD, n);// we also search the next verticle from
                                               // RSD starting from the current position
                                               // in SRD
    }
}

int main()
{
    register int root_pos_RSD = 1;
    int n;// dim of string / nr of verticles

    in>>n;

    read(SRD, n);// read the SRD traversal
    read(RSD, n);// read the RSD traversal

    int root_pos_SRD = find_root_SRD(n);// find the position in which the root is situated
    if(root_pos_SRD == -1)
    {
        out << "No root found";
    }
    else
    {
        fr[1] = 1;// visit the root
        find_tree(root_pos_RSD, root_pos_SRD, n);// form the tree starting from the
                                                 // root_pos_RSD and root_pos_SRD
        write_tree(n);// write the result
    }

    return 0;
}
