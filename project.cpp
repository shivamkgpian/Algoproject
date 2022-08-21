#include<bits/stdc++.h>
#define ll long long

using namespace std;

std::random_device rd;
std::mt19937 gen(rd());

class segment_Tree{
private:
    int *seg_t, size = -1;

    // initialises the segment tree nodes
    void preprocess(int st, int end, int node, int *arr){
        // if it is a leaf node
        // then the value at this node equals
        // value in the array at this index
        if(st == end){
            seg_t[node] = arr[st];
            return;   
        }

        // otherwise ,first find values of left and right
        // halves and then combine them to get the value
        // for current range.
        int mid = (st + end) / 2;
    
        preprocess(st, mid, 2*node, arr);
        preprocess(mid + 1, end, 2*node + 1, arr);

        seg_t[node] = min(seg_t[2*node],seg_t[2*node + 1]);
    }

    // recursively finds the value for queried range
    // by first obtaining the right and left half
    // contributions and then combining them. 
    int query(int st, int end, int node, int l, int r){
        // if current range does not
        // overlap with query range, return INFINITY
        if((st > r) || (end < l))
            return INT_MAX;
        
        // if current range lies completely
        // in query range,  return its value
        if((l <= st) && (end <= r))
            return seg_t[node];
        
        // otherwise recursively find the contribution
        // of current range
        int mid = (st + end) / 2;

        return min( query(st, mid, 2 * node, l, r),
                    query(mid + 1, end, 2 * node + 1, l, r));
    }

    // recursively updates the segment tree 
    // recurses by updating the left and right halves 
    // and then updating the current node
    void update(int st, int end, int node, int indx, int Updval){
        // if range does not contain indx
        // return from there, as nothing inside
        // that range needs to updated
        if((st > indx) || (end < indx))
            return;
        
        // If it is a leaf node that 
        // is our updated index
        // update it
        if(st == end){
            seg_t[node] = Updval;
            return;
        }

        int mid = (st + end) / 2;

        // recursively updating the other nodes
        // after updating their left and right halves
        update(st, mid, 2 * node, indx, Updval);
        update(mid + 1, end, 2 * node + 1, indx, Updval);

        seg_t[node] = min(seg_t[2*node], seg_t[2*node + 1]);
    }

public:
    // default constructor
    segment_Tree(){
        size = -1;
        seg_t = nullptr;
    }

    // initialises the segment tree
    // with the input array arr with
    // size n 
    segment_Tree(int *arr, int n){
        size = n;
        seg_t = new int[4*n];
        preprocess(0, n - 1, 1, arr);
    }

    // use for quering a range [l, r]
    int query(int l, int r){
        if(size == -1){
            cerr << "Segment Tree is not initialised\n";
            exit(1); 
        }
        l --;
        r --;
        
        assert((l >= 0) && (l <= r) && (r < size));
        return query(0, size - 1, 1, l, r);
    }

    // use to update a index 
    void update(int Updindx, int UpdVal){
        if(size == -1){
            cerr << "Segment Tree is not initialised\n";
            exit(1); 
        }
        Updindx --;
        
        assert((Updindx >= 0) && (Updindx < size));
        update(0, size - 1, 1, Updindx, UpdVal);
    }
};

std::uniform_int_distribution<> Random(int a, int b){
    std::uniform_int_distribution<> dis(a, b);
    return dis;
}

int *MakeRandomArr(int n, fstream &input_f){
    int *arr = new int[n];
    auto rnd = Random(INT_MIN, INT_MAX);
    
    input_f << n << " " << '\n';
    
    for(int i = 0; i < n ;i ++){
        arr[i] = rnd(gen);
        input_f << arr[i] << ' ';
    }
    
    input_f << '\n';
    return arr;
}

void MakeRandomQuery(int n, int &l, int &r){
    l = rand() % n, r = rand() % n;
    l ++;
    r ++;
    
    if(l > r)
        swap(l, r);
}

void MakeRandomUpdate(int n, int &indx, int &UpdVal){
    indx = rand() % n, UpdVal = rand();
    indx ++;
}

// large number of random operations
// test cases will be transfered to a input file
// and output to a output file
void TestComplexitySegmentTree(){
    cout << "-----------------------------------------------------------------------------------------------------\n";
    auto begin = time(NULL);
    
    cout << "Testing Segment tree on array of size of 10 ^ 5 and queries upto 10 ^ 5 of both types\n";

    fstream input_f, out_f;

    input_f.open("ComplexityTestInput.txt", ios::out);
    out_f.open("ComplexityTestOutput.txt", ios::out);

    const int n = 1e5,
              q = 1e5;
    int *arr = MakeRandomArr(n, input_f);
    segment_Tree* DS = new segment_Tree(arr, n);
    int tq = 0, tu = 0;

    ll tot_len = 0;

    input_f << q  << '\n';
    
    for(int i = 0; i < q; i ++){
        int t = rand() % 2;

        input_f << t << ' ';

        if(t == 0){
            int l, r;
            
            MakeRandomQuery(n, l, r);
            tq ++;
            input_f << l << ' ' << r  << '\n';

            tot_len += (ll)(r - l + 1);

            out_f << DS->query(l, r) << '\n';
        }else{
            int indx, UpdVal;
            tu ++;
            MakeRandomUpdate(n, indx, UpdVal);
            
            input_f << indx << ' ' << UpdVal << '\n';

            DS->update(indx, UpdVal);
        }
    }

    auto end = time(NULL);
    cout << "Testing completed\n";
    cout << "The number of range query operations " << tq << " with total length of " << tot_len << " of all range queries\n";
    cout << "The number of update operations " << tu << '\n';
    cout << "The input for complexityTest can be found in ComplexityTestInput.txt\n";
    cout << "The output for complexityTest input queries can be found in ComplexityTestInput.txt\n";
    cout << "The time taken by the segment tree to process these queries: " << difftime(end, begin) << " seconds\n";
}

// Small cases check against brute force
void TestCorrectNess(){
    cout << "-----------------------------------------------------------------------------------------------------\n";

    cout << "Checking Correctness of Segment tree on array of size of 10 ^ 3 and queries upto 10 ^ 3 of both types\n";

    fstream input_f, out_f;

    input_f.open("CorrectnessTestInput.txt", ios::out);
    out_f.open("CorrectnessTestOutput.txt", ios::out);

    const int n = 1e3,
              q = 1e3;
    int *arr = MakeRandomArr(n, input_f);
    segment_Tree* DS = new segment_Tree(arr, n);
    int tq = 0, tu = 0, match = 0;

    ll tot_len = 0;

    input_f << q  << '\n';
    
    for(int i = 0; i < q; i ++){
        int t = rand() % 2;

        input_f << t << ' ';

        if(t == 0){
            int l, r;
            
            MakeRandomQuery(n, l, r);
            tq ++;
            input_f << l << ' ' << r  << '\n';

            tot_len += (ll)(r - l + 1);

            // finding the minimum by brute force
            int min_v = arr[l - 1];
            for(int i = l - 1; i < r; i ++)
                min_v = min(min_v, arr[i]);

            int val_ds = DS->query(l, r);

            out_f << val_ds << '\n';

            match += (min_v == val_ds);
        }else{
            int indx, UpdVal;
            tu ++;
            
            MakeRandomUpdate(n, indx, UpdVal);
            
            // updating the value in array too
            // for brute force approach
            arr[indx - 1] = UpdVal;
            input_f << indx << ' ' << UpdVal << '\n';

            DS->update(indx, UpdVal);
        }
    }

    cout << "Testing completed\n";
    cout << "The number of range query operations " << tq << " with total length of " << tot_len << " of all range queries\n";
    cout << "The number of update operations " << tu << '\n';
    cout << "Number of matches of range query output of brute force and segment tree output is " << match << '\n';
    cout << "The input for correctnessTest can be found in CorrectnessTestInput.txt\n";
    cout << "The output for complexityTest input queries can be found in CorrectnessTestOutput.txt\n";
}

// Manual testing
void ManualTest(){
    cout << "-----------------------------------------------------------------------------------------------------\n";
    cout << "Starting Interactive program for segment tree testing\n";
    
    cout << "Enter n: ";
    int n;
    cin >> n;
    
    cout << "Enter the values of array:\n";
    int *arr = new int[n];
    for(int i = 0; i < n; i ++){
        cin >> arr[i];
    }
    
    segment_Tree* DS = new segment_Tree(arr, n);
    
    cout  << "Enter total number of queries: ";
    int q;
    cin >> q;

    cout << "\n0 for range query followed by l and r\n";
    cout << "1 for update followed by index and new value\n";
    cout << "Enter queries\n";

    while(q --){
        cout << "Enter type of query: ";
        
        int t;
        cin >> t;
        
        if(t == 0){
            cout << "Enter the range of query: ";
            
            int l, r;
            cin >> l >> r;
            
            cout << "The value for the query is " << DS->query(l, r) << '\n';
        }else if(t == 1){
            cout << "Enter the index and new value: ";
            
            int indx, val;
            cin >> indx >> val;
            
            arr[indx - 1] = val;
            DS->update(indx, val);
        }else{
            cout << "Invalid query\n";
            return;
        }
        
        cout << "\nThe array after the query is:\n";
        
        for(int i = 0; i < n; i ++){
            cout << arr[i] << ' ';
        }
        cout << '\n';
        cout << '\n';
    }
    cout << "-----------------------------------------------------------------------------------------------------\n";
}

int main(){
    srand(time(0));
    
    TestComplexitySegmentTree();
    
    TestCorrectNess();
    
    ManualTest();
}
