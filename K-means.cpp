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
point *clusters;
int N;
int K = 3;
int CNT;

void InitClusters(point *C, int k)
{
    for(int i=0; i<k; i++)
    {
        int index = int(N/K*i);
        clusters[i] = data[index];
    }
}
double GetEuclideanDistance(point a, point b) //计算欧几里得距离
{
    return sqrt((a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y));
}
double GetManhattanDistance(point a, point b) //计算曼哈顿距离
{
    return abs(a.x-b.x) + abs(a.y-b.y);
}
int main()
{
    freopen("dataset5.txt","r",stdin);
    freopen("result5-3.txt","w",stdout);
    data = new point[10010];
    N = 0;
    while(scanf("%lf%lf",&data[N].x,&data[N].y)!=EOF)
    {
        data[N].id = N;
        N++;
    }
    fclose(stdin);
    clusters = new point[K];
    set<point> st[10010];
    InitClusters(clusters, K);
    double cluster_x[10010];
    double cluster_y[10010];
    int num[110];
    int times = 0;
    CNT = 100;
    while(true)
    {
        for(int i=0; i<K; i++)
            cluster_x[i] = 0.0, cluster_y[i] = 0.0, num[i] = 0;
        for(int i=0; i<N; i++)
        {
            double minn = 10000;
            int cla = -1;
            for(int j=0; j<K; j++)
            {
                double distance = GetEuclideanDistance(data[i], clusters[j]);
                if(minn-distance > eps )
                {
                    minn = distance;
                    cla = j;
                }
            }
            cluster_x[cla] += data[i].x;
            cluster_y[cla] += data[i].y;
            num[cla] += 1;
            st[cla].insert(data[i]);
        }
        bool flag = true;
        for(int i=0; i<K; i++)
        {
            double x = cluster_x[i]/num[i]*1.0;
            double y = cluster_y[i]/num[i]*1.0;
            if(abs(x-clusters[i].x)>eps || abs(y-clusters[i].y)>eps)
                flag = false;
            clusters[i].x = x;
            clusters[i].y = y;
        }
        if(flag)
            break;
        times += 1;
        if(times>=CNT)
            break;
        for(int i=0; i<K; i++)
            st[i].clear();
    }
    for(int i=0; i<K; i++)
    {
        set<point>::iterator iter;
        for(iter = st[i].begin();  iter != st[i].end() ; ++iter)
        {
            cout << iter->x << " " << iter->y << " "<<i<<" "<< iter->id<<"\n";
        }
    }
    fclose(stdout);
    return 0;
}
