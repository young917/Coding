#include <cstdio>
#include <vector>
#include <cmath>
#include <algorithm>
#define P 998244353
typedef long long LL;
using namespace std;

vector<vector<LL>> original;
vector<vector<LL>> matrix;
vector<pair<LL, LL>>list;

void matrix_exp(int s, LL n) {
	vector<vector<LL>>tmp;
	tmp.resize(s);
	for (int i = 0; i < s; i++) {
		tmp[i].resize(s, 0);
		matrix[i][i] = 1;
	}
	while (n>0) {
		if ((n%2)== 1) {
			for (int i = 0; i < s; i++)
				for (int j = 0; j < s; j++)
					tmp[i][j] = 0;

			for(int i=0;i<s;i++)
				for(int j=0;j<s;j++)
					for (int k = 0; k < s; k++) {
						tmp[i][j] += matrix[i][k] * original[k][j];
						tmp[i][j] %= (P - 1);
					}
			for (int i = 0; i < s; i++)
				for (int j = 0; j < s; j++)
					matrix[i][j] = tmp[i][j];
		}
		for (int i = 0; i < s; i++)
			for (int j = 0; j < s; j++)
				tmp[i][j] = 0;

		for (int i = 0; i<s; i++)
			for (int j = 0; j<s; j++)
				for (int k = 0; k < s; k++) {
					tmp[i][j] += original[i][k] * original[k][j];
					tmp[i][j] %= (P - 1);
				}
		for (int i = 0; i < s; i++)
			for (int j = 0; j < s; j++)
				original[i][j] = tmp[i][j];
		n /= 2;
	}
}
LL fast_exp(LL a, LL b) {
	LL ans = 1, base = a;
	while (b) {
		if (b & 1) {
			(ans *= base) %= P;
		}
		(base *= base) %= P;
		b >>= 1;
	}
	return ans;
}
LL binary_search(LL w) {
	int l = 0;
	int r = (int)list.size() - 1;
	while (l <= r) {
		int m = (l + r) / 2;
		LL look = list[m].first;
		if (look == w)
			return list[m].second;
		else if (look < w)
			l = m + 1;
		else
			r = m - 1;
	}
	return -1;
}
LL baby_step_giant_step(LL m) {
	LL root_p = (LL)sqrt(P)+1;
	LL num = 1;
	LL inv_3 = fast_exp(3, P - 2);
	LL ans = -1;
	list.push_back(make_pair(m, 0));
	for (LL i = 1; i < root_p; i++) {
		num = (num * 3) % P;
		LL pushing = fast_exp(num,P-2);
		pushing = (m*pushing) % P;
		//printf("%lld %d\n", pushing, i);
		list.push_back(make_pair(pushing, i));
	}
	sort(list.begin(), list.end());
	LL want = 1;
	ans = binary_search(want);
	if (ans != -1) {
		return ans;
	}
	else {
		LL base = fast_exp(3, root_p);
		for (LL x = 1; x < root_p; x++) {
			want = (want*base) % P;
			ans = binary_search(want);
			if (ans != -1)
				return (x*root_p + ans);
		}
	}
}
/*
int baby_step_giant_step(int m) {
	int root_p = (int)sqrt(P) + 1;
	long long num = 1;
	int ans = -1;
	for (int i = 0; i < root_p; i++) {
		long long pushing = m * inv(num) % P;
		list.push_back(make_pair(pushing, i));
		num = (num * 3) % P;
	}
	sort(list.begin(), list.end());
	long long want = 1;
	long long base = num;
	ans = binary_search(want);
	if (ans != -1) {
		return ans;
	}
	else {
		for (int x = 0; x < root_p; x++) {
			want = (want*base) % P;
			ans = binary_search(want);
			if (ans != -1){
				x = (x* root_p)%(P-1);
				x = (x+ans)%(P-1);
				return x;
		}
	}
}*/
bool Euclidean_algo(LL a, LL b, LL c, LL &x) {
	LL r1, r2;
	LL s1, s2;
	LL gcd;
	r1 = a;
	r2 = b;
	while (r2 > 0) {
		LL q = r1 / r2;
		LL r = r1 % r2;
		r1 = r2;
		r2 = r;
	}
	gcd = r1;
	if ((c%gcd) != 0)
		return false;
	r1 = a / gcd;
	r2 = b / gcd;
	s1 = 1;
	s2 = 0;
	while (r2 > 0) {
		LL q = r1 / r2;
		LL r = r1 % r2;
		r1 = r2;
		r2 = r;
		LL s = s1 - q * s2;
		s1 = s2;
		s2 = s;
	}
	x =( s1 * (c / gcd))%(P-1);
	if (x < 0)
		x += (P - 1);
	return true;
}
int main(void) {
	int k;
	LL n, m;
	scanf("%d", &k);
	matrix.resize(k);
	original.resize(k);
	for (int i = 0; i < k; i++) {
		matrix[i].resize(k, 0);
		original[i].resize(k, 0);
		scanf("%lld", &original[0][i]);
		if (i != 0)
			original[i][i - 1] = 1;
	}
	scanf("%lld %lld", &n, &m);
	matrix_exp(k, n - k);
	LL a, b, c, x;
	a = matrix[0][0];
	b = P - 1;
	c = baby_step_giant_step(m);
	bool success = Euclidean_algo(a, b, c, x);
	printf("%lld %lld %lld\n", a, b, c);
	if (!success)
		printf("-1\n");
	else {
		printf("%lld\n", fast_exp(3, x));
	}
}