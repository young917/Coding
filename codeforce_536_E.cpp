#include <cstdio>
#include <queue>
#include <algorithm>
#include <map>
using namespace std;

struct node {
	long long dist;
	int time;
	int disturb;
	node() :node(0, 0, 0) {}
	node(long long d, int t, int dst) : dist(d), time(t), disturb(dst) {}
	bool operator<(const node &b) const {
		if (dist == b.dist)
			return time < b.time;
		return dist > b.dist;
	};
};
struct info {
	int w, d, t;
	info() :info(0, 0, 0) {}
	info(int a, int b, int c) :w(a), d(b), t(c) {};
	bool operator <(const info &ano) const {
		if (w == ano.w)
			return d > ano.d;
		else
			return w > ano.w;
	};
};

vector<info> input[100002];
map<info, int> rel;
info time[100002];
long long dist[100002][201];
priority_queue<node> finding;

void Insert(info cur) {
	if (rel.count(cur) != 0)
		rel[cur]++;
	else
		rel[cur] = 1;
}
void Delete(info cur) {
	rel[cur]--;
	if (rel[cur] == 0)
		rel.erase(cur);
}
int main(void) {
	int n, m, k;
	int s, t, d, w;
	int ending = 0;
	long long answer = -1;
	bool nomore;
	scanf("%d %d %d", &n, &m, &k);
	for (int i = 1; i <= 100000; i++)
		for (int j = 0; j <= m; j++)
			dist[i][j] = -1;
	for (int i = 0; i < k; i++) {
		scanf("%d %d %d %d", &s, &t, &d, &w);
		ending = max(t, ending);
		input[s].push_back(info(w, d, 1));
		input[t + 1].push_back(info(w, d, -1));
	}
	for (int i = 1; i <= ending; i++) {
		for (int j = 0; j < (int)input[i].size(); j++) {
			info cur = input[i][j];
			if (cur.t == 1)
				Insert(cur);
			else
				Delete(cur);
		}
		if ((int)rel.size() != 0)
			time[i] = (*rel.begin()).first;
		else
			time[i] = info(0, 0, 0);
	}
	finding.push(node(0, 1, 0));
	while (!finding.empty()) {
		node cur = finding.top();
		finding.pop();
		int s, use, d;
		long long w, new_dist;
		s = cur.time;
		use = cur.disturb;
		w = (long long)time[s].w;
		d = time[s].d;
		nomore = false;
		while (w == 0) {
			s++;
			w = (long long)time[s].w;
			d = time[s].d;
			if (dist[s][use] != -1 && dist[s][use] <= cur.dist) {
				nomore = true;
				break;
			}
			else
				dist[s][use] = cur.dist;
		}
		if (nomore)
			continue;
		if (answer != -1 && cur.dist >= answer)
			break;
		else if (use < m) {
			if (s + 1 > ending) {
				if (answer == -1 || answer > cur.dist)
					answer = cur.dist;
			}
			if (dist[s + 1][use + 1] == -1 || dist[s + 1][use + 1] > cur.dist) {
				dist[s + 1][use + 1] = cur.dist;
				finding.push(node(cur.dist, s + 1, use + 1));
			}
		}
		new_dist = cur.dist + w;
		if ((d + 1) > ending) {
			if (answer == -1 || answer > new_dist)
				answer = new_dist;
		}
		else if (dist[d + 1][use] == -1 || dist[d + 1][use] > new_dist) {
			dist[d + 1][use] = new_dist;
			finding.push(node(new_dist, d + 1, use));
		}
	}
	printf("%lld\n", answer);
}