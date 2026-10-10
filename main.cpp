#include <bits/stdc++.h>
using namespace std;
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    string line;
    bool first = true;
    while (getline(cin, line)) {
        if (!first) cout << '\n';
        cout << line;
        first = false;
    }
    return 0;
}
