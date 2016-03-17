#include<iostream>
#include<cstdio>
#include<cmath>
#include<algorithm>
#include<set>
using namespace std;

const double eps = 1e-7;
struct point
{
    double x;
    double y;
    int id;
    point& operator=(const point& a)
    {
       x = a.x;
       y = a.y;
       id = a.id;
       return *this;
    }
    bool operator==(const point &a) const
    {
       return x == a.x && y == a.y;
    }

    bool operator<(const point &a) const
    {
       if(x != a.x) return x < a.x;
       return y < a.y;
    }
};
point *data;
int N;
int K=3;
int ClusterCnt;
double DistanceMatrix[10100][10100];

double GetEuclideanDistance(point a, point b) //计算欧几里得距离
{
    return sqrt((a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y));
}
double GetManhattanDistance(point a, point b) //计算曼哈顿距离
{
    return abs(a.x-b.x) + abs(a.y-b.y);
}
void InitClusters(set<point> *C, point * P, int n)
{
    for(int i=0; i<n; i++)
        C[i].insert(P[i]);
    ClusterCnt = n;
}
void InitDistMatrix(int n)
{
    for(int i=0; i<n; i++)
        for(int j=i; j<n; j++)
        {
            double tmp = GetEuclideanDistance(data[i],data[j]);
            DistanceMatrix[i][j] = tmp;
            DistanceMatrix[j][i] = tmp;
        }
}
void GetMinPair(int &a, int &b, int cnt)
{
    double minlenth = 100000;
    for(int i=0; i<cnt; i++)
        for(int j=i+1; j<cnt; j++)
        {
            if(minlenth-DistanceMatrix[i][j]>eps)
            {
                minlenth = DistanceMatrix[i][j];
                a = i;
                b = j;
            }
        }
}
void Update_Min(set<point> * st)
{
    int a=-1, b=-1;
    GetMinPair(a, b, ClusterCnt);
    set<point>::iterator iter;
    for(iter = st[b].begin();  iter != st[b].end() ; ++iter)
    {
        point np;
        np.x = iter->x;
        np.y = iter->y;
        np.id = iter->id;
        st[a].insert(np);
    }
    st[b].clear();
    for(int i=b+1; i<ClusterCnt; i++)
    {
        st[i-1] = st[i];
    }
    for(int i=0; i<ClusterCnt; i++)
    {
        if(DistanceMatrix[i][a]-DistanceMatrix[i][b]>eps) //MIN
        {
            DistanceMatrix[i][a] = DistanceMatrix[i][b];
            DistanceMatrix[a][i] = DistanceMatrix[i][a];
        }
    }
    for(int i=b+1; i<ClusterCnt; i++)
        for(int j=0; j<ClusterCnt; j++)
            DistanceMatrix[i-1][j] = DistanceMatrix[i][j];
    for(int i=b+1; i<ClusterCnt; i++)
        for(int j=0; j<ClusterCnt; j++)
            DistanceMatrix[j][i-1] = DistanceMatrix[j][i];
    DistanceMatrix[ClusterCnt-1][ClusterCnt-1] = 0;
    ClusterCnt -= 1;
}

void Update_Max(set<point> * st)
{
    int a=-1, b=-1;
    GetMinPair(a, b, ClusterCnt);
    set<point>::iterator iter;
    for(iter = st[b].begin();  iter != st[b].end() ; ++iter)
    {
        point np;
        np.x = iter->x;
        np.y = iter->y;
        np.id = iter->id;
        st[a].insert(np);
    }
    st[b].clear();
    for(int i=b+1; i<ClusterCnt; i++)
    {
        st[i-1] = st[i];
    }
    for(int i=0; i<ClusterCnt; i++)
    {
        if(DistanceMatrix[i][b]-DistanceMatrix[i][a]>eps) //MAX
        {
            DistanceMatrix[i][a] = DistanceMatrix[i][b];
            DistanceMatrix[a][i] = DistanceMatrix[i][a];
        }
    }
    for(int i=b+1; i<ClusterCnt; i++)
        for(int j=0; j<ClusterCnt; j++)
            DistanceMatrix[i-1][j] = DistanceMatrix[i][j];
    for(int i=b+1; i<ClusterCnt; i++)
        for(int j=0; j<ClusterCnt; j++)
            DistanceMatrix[j][i-1] = DistanceMatrix[j][i];
    DistanceMatrix[ClusterCnt-1][ClusterCnt-1] = 0;
    ClusterCnt -= 1;
}

int main()
{
    freopen("dataset5.txt","r",stdin);
    freopen("result5-3-min.txt","w",stdout);
    data = new point[10010];
    N = 0;
    while(scanf("%lf %lf",&data[N].x,&data[N].y)!=EOF)
    {
        data[N].id = N;
        N++;
    }
    fclose(stdin);
    set<point> * st = new set<point>[N];
    InitClusters(st, data, N);
    InitDistMatrix(N);
    while(ClusterCnt>K)
    {
        Update_Min(st);//  Min  or  Max
    }
    for(int i=0; i<ClusterCnt; i++)
    {
        set<point>::iterator iter;
        for(iter = st[i].begin();  iter != st[i].end() ; ++iter)
        {
            cout << iter->x << " " << iter->y << " " <<i<<" "<<iter->id<<"\n";
        }
    }
    fclose(stdout);
    return 0;
}
