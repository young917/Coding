#include <cstdio>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

typedef struct edge {
	int to, capacity, flow;
	edge *dual;
	edge():edge(-1,0){}
	edge(int t, int c):to(t),capacity(c),flow(0),dual(nullptr){}
	int remain() {
		return capacity - flow;
	}
	void add() {
		flow += 1;
		dual->flow -= 1;
	}
};
vector<edge*> network[20000];
int source, sink;
int level[20000];
int start[20000];


void make_edge(int f, int t) {
	edge *e1, *e2;
	e1 = new edge(t, 1);
	e2 = new edge(f, 0);
	e1->dual = e2;
	e2->dual = e1;
	network[t].push_back(e2);
	network[f].push_back(e1);
}
bool initialize() {
	int n, m;
	int r, c, idx;
	int nr, nc, nidx;
	int dir[4] = { -1,1,0,0 };
	char info[100][100];

	scanf("%d %d%*c", &n, &m);
	for (r = 0; r < n; r++) {
		for (c = 0; c < m; c++)
			scanf("%c", &info[r][c]);
		scanf("%*c");
	}
	for (r = 0; r < n; r++)
		for (c = 0; c < m; c++) {
			if (info[r][c] == '#')
				continue;
			idx = 2*(m * r + c)+1;
			make_edge(idx-1, idx);
			if (info[r][c] == 'K')
				source = idx;
			else if (info[r][c] == 'H')
				sink = idx-1;
			for (int d = 0; d < 4; d++) {
				nr = r + dir[d];
				nc = c + dir[3 - d];
				if (nr < 0 || nr >= n)
					continue;
				if (nc < 0 || nc >= m)
					continue;
				if (info[nr][nc] == '#')
					continue;
				else if (info[r][c] == 'K' && info[nr][nc] == 'H')
					return false;
				nidx = 2*(m * nr + nc);
				make_edge(idx, nidx);
			}
		}
	return true;
}

bool find_path_DFS(int cur) {
	if (cur == sink)
		return true;
	for (int &i = start[cur]; i < (int)network[cur].size(); i++) {
		edge *next_e = network[cur][i];
		int next = next_e->to;
		if (level[next] == (level[cur] + 1) && next_e->remain() > 0) {
			if (find_path_DFS(next)) {
				next_e->add();
				return true;
			}
		}
	}
	return false;
}
bool find_level_BFS() {
	queue<int>bfs;
	fill(level, level + 20000, -1);

	level[source] = 0;
	bfs.push(source);
	while (!bfs.empty() && level[sink] == -1) {
		int cur = bfs.front();
		bfs.pop();
		for (int i = 0; i < (int)network[cur].size(); i++) {
			edge* next_e = network[cur][i];
			int next = next_e->to;
			if (level[next] == -1 && next_e->remain() > 0) {
				level[next] = level[cur] + 1;
				bfs.push(next);
			}
		}
	}
	return level[sink] != -1;
}
int Dinnic() {
	int max_flow = 0;
	while (find_level_BFS()) {
		fill(start, start + 20000, 0);
		while (find_path_DFS(source)) {
			max_flow++;
		}
	}
	if (max_flow == 0)
		return -1;
	else
		return max_flow;
}
int main(void) {
	bool ret = initialize();
	if (!ret)
		printf("-1\n");
	else
		printf("%d\n", Dinnic());
}