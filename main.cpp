#include <bits/stdc++.h>
using namespace std;
struct Gear {
    int x, y;
    int ir, orr; // inner radius, outer radius
    double omega = 0.0; // angular velocity in RPM (positive clockwise, negative counter-clockwise)
    bool visited = false;
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    const double EPS = 1e-8;
    int sim = 0;
    while (true) {
        int mx, my, mir, mor, mav, NG;
        if (!(cin >> mx >> my >> mir >> mor >> mav >> NG)) break;
        ++sim;
        vector<Gear> gears(NG + 1); // index 0 is motor
        // motor as gear 0
        gears[0] = {mx, my, mir, mor, (double)mav, true};
        for (int i = 1; i <= NG; ++i) {
            int x, y, ir, orr;
            cin >> x >> y >> ir >> orr;
            gears[i] = {x, y, ir, orr, 0.0, false};
        }
        // Build adjacency list with touching info (radii used for each side)
        struct Edge {int to; int r_from; int r_to;};
        vector<vector<Edge>> adj(NG + 1);
        bool overlap = false;
        for (int i = 0; i <= NG; ++i) {
            for (int j = i + 1; j <= NG; ++j) {
                long long dx = gears[i].x - gears[j].x;
                long long dy = gears[i].y - gears[j].y;
                double dist = sqrt((double)dx*dx + (double)dy*dy);
                // inner-inner
                double sum_ir = gears[i].ir + gears[j].ir;
                if (dist < sum_ir - EPS) { overlap = true; }
                else if (fabs(dist - sum_ir) <= EPS) {
                    adj[i].push_back({j, gears[i].ir, gears[j].ir});
                    adj[j].push_back({i, gears[j].ir, gears[i].ir});
                }
                // outer-outer
                double sum_or = gears[i].orr + gears[j].orr;
                if (dist < sum_or - EPS) { overlap = true; }
                else if (fabs(dist - sum_or) <= EPS) {
                    adj[i].push_back({j, gears[i].orr, gears[j].orr});
                    adj[j].push_back({i, gears[j].orr, gears[i].orr});
                }
                // inner-outer (i inner, j outer)
                double sum_io = gears[i].ir + gears[j].orr;
                if (dist < sum_io - EPS) { overlap = true; }
                else if (fabs(dist - sum_io) <= EPS) {
                    adj[i].push_back({j, gears[i].ir, gears[j].orr});
                    adj[j].push_back({i, gears[j].orr, gears[i].ir});
                }
                // outer-inner (i outer, j inner)
                double sum_oi = gears[i].orr + gears[j].ir;
                if (dist < sum_oi - EPS) { overlap = true; }
                else if (fabs(dist - sum_oi) <= EPS) {
                    adj[i].push_back({j, gears[i].orr, gears[j].ir});
                    adj[j].push_back({i, gears[j].ir, gears[i].orr});
                }
            }
        }
        cout << "Simulation #" << sim << "\n";
        if (overlap) {
            cout << "Error -- Overlapping Gears\n\n";
            continue;
        }
        // BFS to propagate rotation
        bool conflict = false;
        queue<int> q;
        q.push(0);
        while (!q.empty() && !conflict) {
            int u = q.front(); q.pop();
            for (auto &e : adj[u]) {
                int v = e.to;
                double new_omega = - gears[u].omega * (double)e.r_from / (double)e.r_to;
                if (!gears[v].visited) {
                    gears[v].omega = new_omega;
                    gears[v].visited = true;
                    q.push(v);
                } else {
                    if (fabs(gears[v].omega - new_omega) > 1e-6) {
                        conflict = true;
                        break;
                    }
                }
            }
        }
        if (conflict) {
            cout << "Error -- Conflicting Gear Rotation\n\n";
            continue;
        }
        // Output gears 1..NG
        for (int i = 1; i <= NG; ++i) {
            if (!gears[i].visited) {
                // idle gear (no connection)
                cout << setw(2) << i << ": Warning -- Idle Gear\n";
            } else if (fabs(gears[i].omega) < 1e-9) {
                cout << setw(2) << i << ": Warning -- Idle Gear\n";
            } else {
                char dir = (gears[i].omega > 0) ? 'R' : 'L';
                double mag = fabs(gears[i].omega);
                cout << setw(2) << i << ": " << dir << ' ' << fixed << setprecision(2) << mag << "\n";
            }
        }
        cout << "\n";
    }
    return 0;
}
