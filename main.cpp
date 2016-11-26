#include<iostream>
#include "smips.hpp"
int main(){
	Smips::MIP query(2, 3);
	query.obj[0] = 5.0;
	query.obj[1] = 4.0;
	query.e_left[0][0] = 1.5;
	query.e_left[0][1] = 3.0;
	query.e_left[1][0] = 3.0;
	query.e_left[1][1] = 1.0;
	query.e_left[2][0] = 1.0;
	query.e_left[2][1] = 2.0;
	query.e_compare[0] = Smips::Compare::Less;
	query.e_compare[1] = Smips::Compare::Less;
	query.e_compare[2] = Smips::Compare::Greater;
	query.e_right[0] = 13.5;
	query.e_right[1] = 10.0;
	query.e_right[2] =  7.0;
	query.integer_flg[0] = true;
	query.integer_flg[1] = true;
	query.put();
	try {
		auto result = query.optimize();
		result.put();
	}
	catch (const char str[]) {
		std::cout << "エラー：" << str << std::endl;
	}
}
