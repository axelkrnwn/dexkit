#ifndef SORTING
#define SORTING

namespace Sorting{
	
	struct Entity{
		char *name;
		int health;
	};
	
	void swap(Entity *a, Entity *b){
		Entity c = *a;
		*a = *b;
		*b = c;
	}
	
	int partition(Entity *arr, int left, int right){
		Entity pv = arr[right];
		int i = left-1;
		
		for (int j = left;j < right;j++){
		    if(arr[j].health > pv.health){
		      i++;
		      swap(&arr[i],&arr[j]);
		    }
		}
		swap(&arr[i+1],&arr[right]);
		
		return i + 1;
		
	}
	
	void quickSort(Entity *arr, int left, int right){
		if (left < right){
			int p = partition(arr, left, right);
			
			quickSort(arr, left, p - 1);
			quickSort(arr, p + 1, right);
		}
	}
	
	void merge(Entity *arr, int left, int mid, int right){
		int leftSize = mid - left + 1;
		int rightSize = right - mid;
		Entity leftArr[leftSize];
		Entity rightArr[rightSize];
		
		for (int i = 0;i < leftSize;i++){
			leftArr[i] = arr[left + i];
		}
		for (int i = 0;i < rightSize;i++){
			rightArr[i] = arr[mid + 1 + i];
		}
		int i = 0;
		int j = 0;
		int k = left;
		
		while(i < leftSize && j < rightSize){
			if (leftArr[i].health < rightArr[j].health){
				arr[k] = rightArr[j];
				j++;
			}else{
				arr[k] = leftArr[i];
				i++;
			}
			k++;
		}
		while(i < leftSize){
			arr[k] = leftArr[i];
			i++;
			k++;
		}
		while(j < rightSize){
			arr[k] = rightArr[j];
			j++;
			k++;
		}
	}
	
	void mergeSort(Entity *arr, int left, int right){
		if (left < right){
			int mid = left + (right -left)/2;
			
			mergeSort(arr, left, mid);
			mergeSort(arr, mid+1,right);
			
			merge(arr, left, mid, right);
			
		}
	}
	
}

#endif
