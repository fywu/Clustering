#include<iostream>
#include<cstdio>
#include<cmath>
#include<algorithm>
#include<set>
#include<cstring>
using namespace std;

const double eps = 1e-7;
struct point
{
    double x;
    double y;
    int id; //0 核心点  1 边界点  2 噪声点
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
const int MinPts=6;
const double Eps=2.5;
int ClusterCnt;
point *CorePoints;
int CoreNum;
int BorderCore;

double GetEuclideanDistance(point a, point b) //计算欧几里得距离
{
    return sqrt((a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y));
}
double GetManhattanDistance(point a, point b) //计算曼哈顿距离
{
    return abs(a.x-b.x) + abs(a.y-b.y);
}
bool IsCore(int id,int n)
{
    int cnt = 0;
    for(int i=0; i<n; i++)
    {
        if(i == id)
            continue;
        double dist = GetEuclideanDistance(data[i],data[id]);
        if(dist < Eps || abs(dist-Eps)<eps)
            cnt++;
    }
    if(cnt>=MinPts)
        return true;
    else
        return false;
}
int IsBorder(int id,int n)
{
    int t = -1;
    double mindist = 100;
    bool flag = false;
    for(int i=0; i<n; i++)
    {
        double dist = GetEuclideanDistance(CorePoints[i],data[id]);
        if(dist < Eps  || abs(dist - Eps)<eps)
        {
            if(!flag)
                t = i, flag = true, mindist = dist;
            else if(dist<mindist)
                t = i, mindist = dist;
        }
    }

    return t;
}
int DBSCAN(set<point> *st, set<point> &noise, int n)
{
    CoreNum = 0;
    int tag[10010];
    for(int i=0; i<n; i++)
        tag[i] = -1;
    CorePoints = new point[10010];
    for(int i=0; i<n; i++)
    {
        if(IsCore(i, n))
       {
           data[i].id = 0;
           CorePoints[CoreNum++] = data[i];
           tag[i] = -2;
       }
    }
    set<point> * Borderset = new set<point>[CoreNum];

    for(int i=0; i<n; i++)
        if(tag[i]==-1)
        {
            int type = IsBorder(i, CoreNum);
            if(type != -1)
            {
                tag[i] = type;
                data[i].id = 1;
                Borderset[type].insert(data[i]);
            }
            else
            {
                data[i].id = 2;
                noise.insert(data[i]);
            }
        }
    ClusterCnt = 0;
    bool flag[10010];
    for(int i=0; i<CoreNum; i++)
        flag[i] = 0;
    for(int i=0; i<CoreNum; i++)
    {
        if(!flag[i])
        {
            flag[i] = 1;
            st[ClusterCnt].insert(CorePoints[i]);
            set<point>::iterator iter;
            for(iter = Borderset[i].begin();  iter != Borderset[i].end() ; ++iter)
            {
                 point p;
                 p.x = iter->x;
                 p.y = iter->y;
                 p.id = iter->id;
                 st[ClusterCnt].insert(p);
            }
            point * tmp = new point[10010];
            int tmp_flag = 0;
            int tmp_cnt = 1;
            tmp[tmp_flag] = CorePoints[i];
            while(tmp_flag<tmp_cnt)
            {
                for(int j=0; j<CoreNum; j++)
                {
                    if(flag[j])
                        continue;
                    double dist = GetEuclideanDistance(CorePoints[j],tmp[tmp_flag]);
                    if(dist<Eps || abs(dist-Eps)<eps)
                    {
                         st[ClusterCnt].insert(CorePoints[j]);
                         flag[j] = 1;
                         tmp[tmp_cnt] = CorePoints[j];
                         tmp_cnt+=1;
                         set<point>::iterator iter2;
                         for(iter2 = Borderset[j].begin();  iter2 != Borderset[j].end() ; ++iter2)
                         {
                             point p;
                             p.x = iter2->x;
                             p.y = iter2->y;
                             p.id = iter2->id;
                             st[ClusterCnt].insert(p);
                         }
                    }
                }
                tmp_flag+=1;
            }
            ClusterCnt += 1;
        }
    }
    return ClusterCnt;
}
int main()
{
    freopen("dataset5.txt","r",stdin);
    freopen("result5-p6-e2.5-new.txt","w",stdout);
    data = new point[10010];
    N = 0;
    while(scanf("%lf %lf",&data[N].x,&data[N].y)!=EOF)
    {
        data[N].id = N;
        N++;
    }
    fclose(stdin);
    set<point> * st = new set<point>[N];
    set<point> noise;
    DBSCAN(st,noise,N);
    for(int i=0; i<ClusterCnt; i++)
    {
        set<point>::iterator iter;
        for(iter = st[i].begin();  iter != st[i].end() ; ++iter)
        {
            cout << iter->x << " " << iter->y << " " <<i<<" "<<iter->id<<"\n";
        }
    }
    set<point>::iterator iter;
    for(iter = noise.begin();  iter != noise.end() ; ++iter)
    {
        cout << iter->x << " " << iter->y << " " <<ClusterCnt<<" "<<iter->id<<"\n";
    }
    fclose(stdout);
    return 0;
}
