#include <iostream>
#include <stdio.h>
#include "stdafx.h"

using namespace std;

template <class T>
class Queue{
public:
	Queue(){
		max=20; num=0;
		arr=new T[max];
	}
	~Queue();

	void put(T input){
		if(max==num){
			T* temp=new T[max*2];
			for(int i=0;i<num;i++) temp[i]=arr[i];
			delete arr;
			arr=temp;
			max=max*2;
		}
		arr[num]=input;
		num++;
	}
	T pop(){
		if(num==0) return NULL;
		T result=arr[0];
		for(int i=1;i<num;i++) arr[i-1]=arr[i];
		num--;
		return result;
	}

private:
	int max, num;
	T* arr;

};