// Codeforces Round #136 (Div. 1) E. Little Elephant and Inversions
// http://codeforces.com/problemset/problem/220/E
//
// LLLLLLLLLL.....RRRRRRRRRR
// ・bitL は L 部分の多重集合
// ・bitR は R 部分の多重集合
// とする。
// LLLLLLLLLL.....RRRRRRRRRR =>
// LLLLLLLLLLL....RRRRRRRRRR 
// としたときの inversion の変化は容易に求められる。
// LLLLLLLLLL.....RRRRRRRRRR =>
// LLLLLLLLLL......RRRRRRRRR
// としたときの inversion の変化も容易に求められる。
// L部分の右端を固定した時、inversion が K 以下になる j の位置は単調増加なので、
// 尺取法で j を更新していけばいい。

#include "stdc++.h"
using namespace std;

template<class T>
vector<int> compress(T first, T last) {
	vector<typename iterator_traits<T>::value_type> cv(first, last);
	vector<int> ret(cv.size());
	sort(cv.begin(), cv.end());
	cv.erase(unique(cv.begin(), cv.end()), cv.end());
	for (int i = 0; first != last; first++, i++)
		ret[i] = lower_bound(cv.begin(), cv.end(), *first) - cv.begin();
	return ret;
}

struct BIT {
	vector<long long> dat;
	BIT(int n) :
			dat(n) {
	}
	void update(int k, long long x) {
		k++;
		for (; k < dat.size(); k += k & -k)
			dat[k] += x;
	}
	long long sumFront(int k) {
		k++;
		long long ret = 0;
		for (; k > 0; k -= k & -k)
			ret += dat[k];
		return ret;
	}
	long long sumBack(int k) {
		return sumFront(dat.size() - 2) - sumFront(k - 1);
	}
};

int n;
long long K;
vector<int> a;
long long inv;
BIT bitL(101010);
BIT bitR(101010);

void addLeft(int k) {
	inv += bitL.sumBack(a[k] + 1);
	printf("sumado de l %u\n", inv);
	inv += bitR.sumFront(a[k] - 1);
	printf("sumado de r %u\n", inv);
	bitL.update(a[k], 1);
}

void removeRight(int k) {
	inv -= bitL.sumBack(a[k] + 1);
	inv -= bitR.sumFront(a[k] - 1);
	bitR.update(a[k], -1);
}

int main() {
	cin >> n >> K;
	a.resize(n);
	int b[n];
	for (int i = 0; i < n; i++) {
		scanf("%d", &a[i]);
		b[i] = a[i];
	}
	a = compress(a.begin(), a.end());

	for (int i = 0; i < n; i++) {
		inv += bitR.sumBack(a[i] + 1);
		bitR.update(a[i], 1);
		printf("en a %u (%u) inv %u\n", a[i], b[i], inv);
	}

	long long ans = 0;
	int j = 0;
//	addLeft(0);
	printf("inv ini %u\n", inv);
	for (int i = 0; i < n; i++) {
		/*
		 while (j < n && j < i)
		 removeRight(j++);
		 while (j < n && inv > K)
		 removeRight(j++);
		 */
		while (j < n && (j < i || inv > K)) {
			removeRight(j++);
			printf("en j %u kitado %u %u\n", j - 1, b[j - 1], inv);
		}
		ans += n - j;
		printf("ag %u en %u %u inv %u\n", n - j, i, j, inv);
		addLeft(i);
		printf("inv despues %u\n", inv);
	}
	cout << ans << endl;
}
