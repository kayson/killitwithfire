#ifndef BLAS_WIN_H
#define BLAS_WIN_H

namespace BLASWIN{
	double abs_max(std::vector<double> &v){
		double max = 0;
		for(int i=0; i<v.size(); ++i){
			if(v[0] > max)
				max = v[0];
		}
		return max;
	}

	double dot(std::vector<double> &v1, std::vector<double> &v2){
		double sum=0;
		for(int i=0; i< v1.size(), ++i){
			sum += v1[i]*v2[i];
		}

		return sum;
	}

	void add_scaled(double alpha, std::vector<double> &s, std::vector<double> &res){
		for(int i=0; i<s.size(); ++i){
			res[i] = s[i] + alpha;
		}
		return;
	}
}


#endif