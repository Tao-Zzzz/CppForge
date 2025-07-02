#include <iostream>
#include <vector>
#include <deque>
#include <set>

using namespace std;

void vector_example(){
    vector<int> vec = {1, 2, 3, 4, 5};
    auto it = vec.begin() + 2;
    cout << "Befor erase: " << *it << endl;
    it = vec.erase(it);
    cout << "After erase: " << *it << endl;
    vec.push_back(6);
}

void deque_example(){
    deque<int> dq = {1, 2, 3, 4, 5};
    auto it = dq.begin() + 2;
    dq.erase(it);
    dq.push_front(0);
    
}

void set_example(){
    set<int> s = {1, 2, 3, 4, 5};
    auto it = s.find(3);
    s.erase(it++);
    cout << "After erase: " << *it << endl;
}

int main(){
    vector_example();
    deque_example();
    set_example();
    return 0;
}