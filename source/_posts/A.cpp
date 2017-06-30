#include <iostream>
#include <cstdio>
#include <algorithm>
using namespace std;
#define ll long long
#define mod 1000000007
ll b[10010];
ll quick_pow(int n, int k) {
	ll res = 1;
	while(k > 0) {
		if(k & 1) 
			res = (res * n) % mod;
		n = (n * n) % mod;
		k >>= 1;
	}
	return res;
}
void pre() {
	for(int i = 0; i <= 10000; i++) {
		b[i] = quick_pow(2, i);
	}
}
int main() {
	int t;
	scanf("%d", &t);
	int n, k;
	while(t--) {
		scanf("%d", &n);
		ll one = 0;
		ll two = 0;
		for(int i = 0; i < n; i++) {
			scanf("%d", &k);
			one = (one + (k * b[i]) % mod) % mod;
			two = (one + (k * b[n-i]) % mod) % mod;
		}
		printf("%lld\n", (one - two) % mod);
	}
}