#ifndef BLAS_WIN_H
#define BLAS_WIN_H
#include <vector>

namespace BLASWIN{
	template <typename T>
	T abs_max(std::vector<T> &v){
		double max = fabs(v[0]);
		for(int i=1; i<(int)v.size(); ++i){
			if(fabs(v[i]) > max)
				max = fabs(v[i]);
		}
		return max;
	}

	template <typename T>
	T dot(std::vector<T> &v1, std::vector<T> &v2){
		double sum=0;
		for(int i=0; i< (int)v1.size(); ++i){
			sum += v1[i]*v2[i];
		}

		return sum;
	}

	template <typename T>
	void add_scaled(double alpha, std::vector<T> &s, std::vector<T> &res){
		for(int i=0; i<(int)s.size(); ++i){
			res[i] = s[i] * alpha + res[i];
		}
		return;
	}
}


#endif