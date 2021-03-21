// CPP Program to implement in place merge sort using multi-threading 
#include <iostream> 
#include <pthread.h> 
#include <time.h> 
#include <math.h> 
#include <bits/stdc++.h>

using namespace std; 


// maximum number of elements in array 
#define MAXXX 10000000
 


// size of array is MAX 
int MAX; 
// number of threads used
int THREAD_MAX; 
int part = 0; 
// array of max size MAXXX
int a[MAXXX];
// number of elements extra in last thread
int rem;
// number of elements per thread
int NUMBERS_PER_THREAD;



void swap(int i, int j){
    int temp = a[i];
    a[i] = a[j];
    a[j] = temp;
}


int highestPowerofTwo(int n) 
{ 
	int i = n;
    while (i >= 1) 
    { 
        // If i is a power of 2 
        if ((i & (i-1)) == 0) 
        { 
            return i;
        } 
		i--;
    } 
    return 0; 
} 


int nextGap(int k){
    if(k <= 1){
        return 0;
    }
    return (k/2) + (k%2);
}


// InPlace Merge Sort in O(n*(lgn)^2)
void merge(int low, int mid, int high){
    int k = high - low + 1;
    k = nextGap(k);
    while(k>0){
        int i = low;
        while(i+k<= high){
            int j = i + k;
            if (a[i] > a[j])
                swap(i, j);
            i++;
        }
        k = nextGap(k);
    }
}


// Merge Sort in O(n^2)
/*void merge(int low, int mid, int high)
{
    int low2 = mid + 1;
 
    // If the direct merge is already sorted
    if (a[mid] <= a[low2]) {
        return;
    }
 
    // Two pointers to maintain start
    // of both arrays to merge
    while (low <= mid && low2 <= high) {
 
        // If element 1 is in right place
        if (a[low] <= a[low2]) {
            low++;
        }
        else {
            int value = a[low2];
            int index = low2;
 
            // Shift all the elements between element 1
            // element 2, right by 1.
            while (index != low) {
                a[index] = a[index - 1];
                index--;
            }
            a[low] = value;
 
            // Update all the pointers
            low++;
            mid++;
            low2++;
        }
    }
}*/


// Was trying to make it O(nlgn) but couldn't succeed
/*void merge(int low, int mid, int high){

    if((low<mid) && (mid<high)){

	int mx = 0;

    for(int i = low; i<=high; i++){
        mx = max(a[i], mx);
    }

    mx++;

    int i = low;
    int j = mid+1;
    int k = low;

    while((i<=mid) && (j<=high) && (k<=high)){

        int e1 = a[i]%mx;
        int e2 = a[j]%mx;

        if(e1 <= e2){

            if(k <= mid){
                a[k] += (e1*mx);
            }
            else{
                a[k+low] += (e1*mx);
            }

            i++;
            k++;
        }

        else{

            if(k <= mid){
                a[k] += (e2*mx);
            }
            else{
                a[k+low] += (e2*mx);
            }

            j++;
            k++;
        }
    }

    while(i <= mid){
        int e1 = a[i]%mx;
        if(k <= mid){
            a[k] += (e1*mx);
        }
        else{
            a[k+low] += (e1*mx);
        }

        i++;
        k++;
    }

    while(j <= high){
        int e1 = a[j]%mx;
        if(k <= mid){
            a[mid+k+1] += (e1*mx);
        }
        else{
            a[k+low] += (e1*mx);
        }

        j++;
        k++;
    }

    for(int i = low; i<=high; i++){
        a[i] = a[i]/mx;
        cout << a[i] << " ";
    }}
}*/


// merge sort 
void merge_sort(int low, int high) 
{ 
	// calculating the middle of array 
	int mid = low + (high - low) / 2; 
	if (low < high) { 
		// sorting first half 
		merge_sort(low, mid); 

		// sorting second half 
		merge_sort(mid + 1, high); 

		// merging the two halves 
		merge(low, mid, high); 
	} 
} 

// thread function for multi-threading 
void* merge_sort(void* arg) 
{ 
	// which part out of THREAD_MAX parts 
	int thread_part = part++; 

	// calculating low and high 
	int low = thread_part * (MAX / THREAD_MAX); 
	int high = (thread_part + 1) * (MAX / THREAD_MAX) - 1; 

	//Adding the rem elements in last thread if present
	if(thread_part == THREAD_MAX -1){
		high += rem;
	}

	// calculating the mid point 
	int mid = low + (high - low) / 2; 
	if (low < high) { 
		merge_sort(low, mid); 
		merge_sort(mid + 1, high); 
		merge(low, mid, high); 
	} 
	
	return NULL;
	
} 


void merge_sections_of_array(int number, int k) {

	int i = 0;
	int j = number;
    while (i < number) {
        int low = i * (NUMBERS_PER_THREAD * k);
        int high = ((i + 2) * NUMBERS_PER_THREAD * k) - 1;
        int mid = low + (NUMBERS_PER_THREAD * k) - 1;
        if (high >= MAX) {
            high = MAX - 1;
        }
        merge(low, mid, high);
		i = i+2;
    }
    if (j / 2 >= 1) {
        merge_sections_of_array(j / 2, k * 2);
    }
}

// Driver Code 
int main (int argc , char** argv) 
{ 
	// Making a txt file to store the time taken
	ofstream fout("Merge.txt", ios::out | ios::app);

	// Assigning THREAD_MAX value as given in input
	THREAD_MAX=highestPowerofTwo(atoi(argv[1]));

	// Assigning MAX the length of array as given by user
    MAX=atoi(argv[2]);

    rem = MAX%THREAD_MAX;
    NUMBERS_PER_THREAD = MAX/THREAD_MAX;

	//Checking if number of threads is less than one
    if(NUMBERS_PER_THREAD < 1){
    	printf("No. of threads are more than length of array\n");
    	return 0;
    }
    	

	// generating random values in array 
	for (int i = 0; i < MAX; i++) 
		a[i] = rand() % (100*MAX); 


	// displaying unsorted array
	cout << "Unsorted array: \n"; 
	for (int i = 0; i < MAX; i++) 
		cout << a[i] << " ";
		
	// Start the counting of time
	auto start = chrono::high_resolution_clock::now(); 
	pthread_t threads[THREAD_MAX]; 

	// creating THREAD_MAX threads 
	for (int i = 0; i < THREAD_MAX; i++) 
		pthread_create(&threads[i], NULL, merge_sort, (void*)NULL); 

	// joining all THREAD_MAX threads 
	for (int i = 0; i < THREAD_MAX; i++) 
		pthread_join(threads[i], NULL); 

	// Merging all the threads' outputs
	merge_sections_of_array(THREAD_MAX, 1);

	auto end = chrono::high_resolution_clock::now();
	double time_taken = chrono::duration_cast<chrono::nanoseconds>(end - start).count(); 

	// displaying sorted array 
	cout << "\n\nSorted array: \n"; 
	for (int i = 0; i < MAX; i++) 
		cout << a[i] << " "; 

	// time taken by merge sort 
	time_taken *= 1e-9;
    fout << fixed << time_taken << setprecision(9) << "\n"; 

	cout << "\n\nTime taken: " << time_taken << endl; 

	return 0; 
}
