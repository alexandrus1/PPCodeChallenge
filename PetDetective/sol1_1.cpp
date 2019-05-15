// Algorithm: BFS using double ended queue

#include <stdio.h>
#include <string.h>
#include <deque>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;

// describes car move
struct carmove {
	int position;
	// possible actions:
	// 	i - initial position
	// 	m - simple move
	// 	t - take animal
	// 	p - put animal
	char action;
	carmove(int p, char a): position(p), action(a) {}
};

struct rect {
	rect(int x, int y, int u, int v) :
		totalNumberOfMoves(x), 
		currentCarPosition(y),
		petsOnCar(u),
		petsDelivered(v)
	{
		path.push_back(carmove{currentCarPosition, 'i'} );
	}
	rect(int x, int y, int u, int v, const vector<carmove>& currPath) :
		totalNumberOfMoves(x), 
		currentCarPosition(y),
		petsOnCar(u),
		petsDelivered(v),
		path(currPath)
	{
	}
	int totalNumberOfMoves;
	int currentCarPosition;
	int petsOnCar;			// bitmask
	int petsDelivered;		// bitmask
	vector<carmove> path; 
};

const int N = 40;
int m, n, k, b[123][123], bx[123][123], by[123][123], id[123][123], MaxID=0;
int a[N][4], Special[N]; //
short d[N][1<<11][1<<11];
vector<carmove> winningPath;

bool minimize(short &a, int b)
    { return a>b ? a=b, true : false; }

int bfs(int u) {
    deque<rect> qu;
    memset(d, 0x3c, sizeof d);
    qu.push_back(rect(0, u, 0, 0));

    while (qu.size()) {
        rect Pop=qu.front(); qu.pop_front();
        int du=Pop.totalNumberOfMoves, 
	    	u=Pop.currentCarPosition,
		u1=Pop.petsOnCar, 
		u2=Pop.petsDelivered;
	auto &path = Pop.path;
        if (!minimize(d[u][u1][u2], du)) continue;
        if (u2==(1<<k)-1) {
		winningPath = path;
		return du;
	}
        //printf("d[%d][%d][%d]=%d\n", u, u1, u2, du);

        if (int x=Special[u]) {
            if (x>0 && ((1<<x-1)&(~u1)) && ((1<<x-1)&~u2) && __builtin_popcount(u1)<4) 
	    {
		    auto p = path;
		    p.push_back(carmove(u, 't'));			// take animal
		    qu.push_front(rect(du, u, u1|(1<<x-1), u2, p));
	    }
            if (x<0 && ((1<<-x-1)&(u1)) && ((1<<-x-1)&~u2))
	    {
		    auto p = path;
		    p.push_back(carmove(u, 'p'));			// put animal
		    qu.push_front(rect(du, u, u1^(1<<-x-1), u2|(1<<-x-1), p));
	    }
        }
        for (int i=0; i<4; i++) 
		if (int v=a[u][i]) {
			auto p = path;
			p.push_back(carmove(v, 'm'));
		       	qu.push_back(rect(du+1, v, u1, u2, p));
		}
    }
    return -1;
}

main() {
    int sx, sy;
    scanf("%d%d", &m, &n);
    for (int i=1; i<=m; i++)
    for (int j=1; j<=n; j++) {
        scanf("%d", &b[i][j]);
        if (b[i][j]==99)
            { sx=i, sy=j; b[i][j]=0; }
        else if (b[i][j]>0)
            k++;
        id[i][j] = ++MaxID;
        Special[id[i][j]]=b[i][j];
    } // done k, id, sx, sy

    for (int i=1; i<=m; i++)
    for (int j=1; j<n; j++) {
        scanf("%d", &bx[i][j]);
        #define connect(x, y, k) a[x][k]=y
        if (bx[i][j]) {
            connect(id[i][j], id[i][j+1], 0);
            connect(id[i][j+1], id[i][j], 1);
        }
    }
    for (int i=1; i<m; i++)
    for (int j=1; j<=n; j++) {
        scanf("%d", &by[i][j]);
        if (by[i][j]) {
            connect(id[i][j], id[i+1][j], 2);
            connect(id[i+1][j], id[i][j], 3);
        }
    }
    /*
    for (int i=1; i<=m; i++)
    for (int j=1; j<=n; j++)
    printf(j==n ? "%d <--- id\n" : "%d ", id[i][j]);

    for (int i=1; i<=MaxID; i++)
    printf("%d %d %d %d <-- %d(%d)\n", a[i][0], a[i][1], a[i][2], a[i][3], i, Special[i]);
    */
    auto result = bfs(id[sx][sy]);
    cout << result << endl;
    if (result >= 0) {
	    cout << "Full path:   ";
	    for(auto const& value: winningPath)
		    cout << "(" << value.position << "," << value.action  << ")   ";
	    cout << endl;

	    cout << "Action path: ";
	    for(auto const& value: winningPath)
		    if (value.action != 'm')
		    	cout << "(" << value.position << "," << value.action  << ")   ";
	    cout << endl;
	}

    return 0;
}

