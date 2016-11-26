/* ExpeditionOptimizer_KAI
 * ○コンパイルオプション
 * cl.exe main.cpp /O2 /EHsc /GL /Feexp_opt /nologo
 * ○ファイル形式
 * ソースコード→BOM付きUTF-8
 * table1.csv、table2.csv→Shift_JIS←だいたいMSVCのせい
 * ○コマンドライン
 * exp_opt 表番号 燃料 弾薬 鋼材 ボーキ 艦隊数 観察間隔 自動回復フラグ
 * ・燃料～ボーキは0以上の整数
 * ・艦隊数は1～4までの整数
 * ・観察間隔は1以上の整数(単位：分)
 * ・自動回復フラグは、有効なら1でそうでないなら0
 */

#include<array>
#include<algorithm>
#include<iostream>
#include<sstream>
#include<string>
#include<vector>
#include "smips.hpp"

using std::array;
using std::string;
using std::vector;
using std::cout;
using std::stoi;

const size_t SUPPLY_KINDS = 4;
const array<string, 2> TABLE_FNAME = {"table1.csv", "table2.csv"};

int main(int argc, char *argv[]){
	// コマンドライン引数を読み込み
	if(argc < 9){
		cout << "エラー：コマンドライン引数が足りません.\n";
		return 1;
	}
	size_t table_type   = stoi(argv[1]);
	table_type = (table_type < 1 ? 1 :table_type > 2 ? 2 :table_type);
	int supply[] = {stoi(argv[2]), stoi(argv[3]), stoi(argv[4]), stoi(argv[5])};
	size_t fleets       = stoi(argv[6]);
	fleets = (fleets < 1 ? 1 :fleets > 4 ? 4 :fleets);
	size_t pauling_time = stoi(argv[7]);
	pauling_time = (pauling_time < 1 ? 1 :pauling_time);
	size_t auto_supply  = stoi(argv[8]);
	auto_supply = (auto_supply > 1 ? 1 : auto_supply);
	// 遠征表を読み込み
	vector<vector<int>> exp_supply;
	vector<int> exp_time;
	vector<string> exp_name, exp_info;
	size_t exp_count = 0;
	std::ifstream ifs(TABLE_FNAME[table_type - 1]);
	if(!ifs){
		cout << "エラー：" << TABLE_FNAME[table_type - 1] << "が足りません.\n";
		return 1;
	}
	string line_str;
	while(std::getline(ifs, line_str)){
		if(line_str.substr(0, 4) == "name")
			continue;
		string cell;
		std::istringstream stream(line_str);
		size_t p = 0;
		vector<int> temp_ext_supply(SUPPLY_KINDS);
		while(std::getline(stream, cell, ',')){
			if(p == 0){
				exp_name.push_back(cell);
			}else if(p <= SUPPLY_KINDS){
				temp_ext_supply[p - 1] = stoi(cell);
			}else if(p == SUPPLY_KINDS + 1){
				exp_supply.push_back(temp_ext_supply);
				exp_time.push_back(std::max(stoi(cell), (int)pauling_time));
			}else if(p == SUPPLY_KINDS + 2){
				exp_info.push_back(cell);
				break;
			}
			++p;
		}
		++exp_count;
	}
/*	cout << "遠征数：" << exp_count << "\n";
	for(size_t i = 0; i < exp_count; ++i){
		cout << exp_name[i] << "\t[";
		for(size_t j = 0; j < SUPPLY_KINDS; ++j){
			cout << exp_supply[i][j] << ", ";
		}
		cout << "]\t" << exp_time[i] << "\t" << exp_info[i] << "\n";
	}*/
	/* 問題を生成
	 * 変数の数はexp_count+総時間とminの変数、式の数はSUPPLY_KINDS+総時間制約+min制約+exp_count
	 */
	Smips::MIP query(exp_count+2, SUPPLY_KINDS+exp_count+2);
	// 目的関数
	query.obj[exp_count + 1] = -1.0;
	// 制約条件：資源量
	for(size_t j = 0; j < SUPPLY_KINDS; ++j){
		for(size_t i = 0; i < exp_count; ++i){
			query.e_left[j][i] = exp_supply[i][j];
		}
		if(auto_supply == 1){
			query.e_left[j][exp_count + 1] = (j == 3 ? 1.0 / 3 : 1.0);
		}
		query.e_compare[j] = Smips::Compare::Greater;
		query.e_right[j] = supply[j];
	}
	// 制約条件：総時間
	for(size_t i = 0; i < exp_count; ++i){
		query.e_left[SUPPLY_KINDS][i] = -1.0 * exp_time[i];
	}
	query.e_left[SUPPLY_KINDS][exp_count] = 1.0;
	// 制約条件：min制約
	query.e_left[SUPPLY_KINDS + 1][exp_count] = -1.0;
	query.e_left[SUPPLY_KINDS + 1][exp_count + 1] = 1.0;
	query.e_compare[SUPPLY_KINDS + 1] = Smips::Compare::Greater;
	// 制約条件：min制約その2
	for(size_t i = 0; i < exp_count; ++i){
		size_t pos = SUPPLY_KINDS + 2 + i;
		query.e_left[pos][exp_count + 1] = 1.0;
		query.e_left[pos][i] = -1.0 * fleets * exp_time[i];
		query.e_compare[pos] = Smips::Compare::Greater;
	}
	// 整数制約
	for(size_t i = 0; i < exp_count; ++i){
		query.integer_flg[i] = true;
	}
	//query.put();
	try {
		auto result = query.optimize();
		//result.put();
		cout << "○遠征内容\n";
		for(size_t i = 0; i < exp_count; ++i){
			if (Smips::round_to_zero(result.x[i]) == 0.0)
				continue;
			cout << "　" << exp_name[i] << "×" << result.x[i] << "回｜のべ" << (exp_time[i] * result.x[i]) << "分｜" << exp_info[i] << "\n";
		}
		cout << "○所要時間\n";
		double min_time = -result.z / fleets;
		cout << "　所要時間[分]：" << min_time << "\n";
		cout << "　所要時間[時]：" << (min_time / 60) << "\n";
		cout << "　所要時間[日]：" << (min_time / 60 / 24) << "\n";
	}
	catch (const char str[]) {
		std::cout << "エラー：" << str << std::endl;
	}

/*	Smips::MIP query(2, 3);
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
	}*/
}
