#include <omp.h>
//#include <cstdio>


template<class T, class UnaryOp, class ReduceOp>
double map_reduce(int n, T* vec, UnaryOp uno, ReduceOp reop){
  T acc = 0;
  for(int i = 0; i < n; ++i){
    acc = reop(acc, uno(vec[i]));
  }
  return acc;
};

template<class T, class BinaryOp, class ReduceOp>
double map_reduce2(int n, T* vec1, T*vec2, BinaryOp binop, ReduceOp reop){
  T acc = 0;
  for(int i = 0; i < n; ++i){
    acc = reop(acc, binop(vec1[i], vec2[i]));
  }
  return acc;
};

template<class T, class UnaryOp, class ReduceOp>
double pmap_reduce(unsigned n, T* vec, UnaryOp uno, ReduceOp reop){
  T acc = 0;
  if(n < 2000){
    for(unsigned i = 0; i < n; ++i){
      acc = reop(acc, uno(vec[i]));
    }
    return acc;
  }else{
    const unsigned thread_num = 4;
    unsigned  multi = n/thread_num;
    unsigned remainder = n%thread_num;
    T temp_result[thread_num] = {0};
    int thread, n_ele;
#pragma omp parallel for default(none)					\
  shared(vec,  temp_result, multi) private(reop,uno, thread, n_ele)
    for(thread = 0; thread < (int) thread_num; ++thread){
      for(n_ele = 0; n_ele < (int) multi; ++n_ele){
	temp_result[thread] = reop(temp_result[thread], uno(vec[thread*multi + n_ele]));
      }
//      printf("%i", omp_in_parallel());
    }

    for(unsigned thread = 0; thread < thread_num; ++thread){
      acc += temp_result[thread];
    }

       unsigned idx = multi*thread_num;

    switch(remainder){
    case 0:
      break;
    case 1:
      acc = reop(acc, uno(vec[idx++]));
      break;
    case 2:
      acc = reop(acc, uno(vec[idx++]));
      acc = reop(acc, uno(vec[idx++]));
      break;
    case 3:
      acc = reop(acc, uno(vec[idx++]));
      acc = reop(acc, uno(vec[idx++]));
      acc = reop(acc, uno(vec[idx++]));
      break;
    }
    return acc;
  }
};

template<class T, class BinaryOp, class ReduceOp>
double pmap_reduce2(unsigned n, T* vec1,T* vec2, BinaryOp binop, ReduceOp reop){
  T acc = 0;
  if(n < 2000){
    for(unsigned i = 0; i < n; ++i){
      acc = reop(acc, binop(vec1[i], vec2[i]));
    }
    return acc;
  }else{
    const unsigned thread_num = 4;
    unsigned  multi = n/thread_num;
    unsigned remainder = n%thread_num;
    T temp_result[thread_num] = {0};
    int thread, n_ele;
#pragma omp parallel for default(none)					\
  shared(vec1, vec2,  temp_result, multi) private(reop, binop, thread, n_ele)
    for(thread = 0; thread < (int) thread_num; ++thread){
      for(n_ele = 0; n_ele < (int) multi; ++n_ele){
	temp_result[thread] = reop(
				   temp_result[thread], 
				   binop(
					 vec1[thread*multi + n_ele], 
					 vec2[thread*multi + n_ele]
					 )
				   );
      }
      printf("%i", omp_in_parallel());
    }

    for(unsigned thread = 0; thread < thread_num; ++thread){
      acc += temp_result[thread];
    }

       unsigned idx = multi*thread_num;

    switch(remainder){
    case 0:
      break;
    case 1:
      acc = reop(acc, binop(vec1[idx], vec2[idx])); idx++;
      break;
    case 2:
      acc = reop(acc, binop(vec1[idx], vec2[idx])); idx++;
      acc = reop(acc, binop(vec1[idx], vec2[idx])); idx++;
      break;
    case 3:
      acc = reop(acc, binop(vec1[idx], vec2[idx])); idx++;
      acc = reop(acc, binop(vec1[idx], vec2[idx])); idx++;
      acc = reop(acc, binop(vec1[idx], vec2[idx])); idx++;
      break;
    }
    return acc;
  }
};
