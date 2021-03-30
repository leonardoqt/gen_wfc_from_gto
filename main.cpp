#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <math.h>
#include <string.h>
#include <iomanip>

using namespace std;

template<typename T>
void print(vector<T> aa)
{
	for(auto m1 : aa)
		cout<<'\t'<<m1;
	cout<<endl;
}

template<typename T>
void print(vector< vector<T> > aa)
{
	for(auto m1 : aa)
		print(m1);
	cout<<endl;
}


int main()
{
	ifstream gto, mesh;
	ofstream wfc,header;
	stringstream ss;
	string ele;
	int num_grid, num_alpha;
	const int num_l_max = 4;
	char sym_l[num_l_max]={'S','P','D','F'};
	vector<double> grid;
	vector<int> do_l;
	vector<double> alpha;
	vector< vector<double> > coef;
	string f_basis="$basis";
	string f_end = "$end";
	string f_next="****";
	string tmp;
	double tmp_d;
	//
	// locate basis
	gto.open("basis.gto");
	while(getline(gto,tmp))
		if (tmp.find(f_basis)!=string::npos)
			break;
	//
	getline(gto,tmp);
	while(tmp.find(f_end)==string::npos && !gto.eof())
	{
		// find element
		ss<<(tmp);  ss>>ele;
		ss.str(""); ss.clear();
		// open mesh file
		mesh.open(ele+".mesh");
		// find number of grid point
		grid.resize(0);
		while(mesh>>tmp_d)
			grid.push_back(tmp_d);
		num_grid = grid.size();
		// setup ouput files
		wfc.open(ele+"_wfc.dat");
		header.open(ele+"_header.dat");
		//
		getline(gto,tmp);
		while(tmp.find(f_next)==string::npos && !gto.eof())
		{
			// get all coefs. of one block
			do_l = vector<int>(num_l_max,0);
			if (tmp.find('S') != string::npos) do_l[0] = 1;
			if (tmp.find('P') != string::npos) do_l[1] = 1;
			if (tmp.find('D') != string::npos) do_l[2] = 1;
			if (tmp.find('F') != string::npos) do_l[3] = 1;
			ss<<(tmp);ss>>tmp>>num_alpha;
			ss.str(""); ss.clear();
			alpha.resize(num_alpha);
			coef = vector< vector<double> > (num_alpha,vector<double>(num_l_max,0));
			for(int t1=0; t1<num_alpha; t1++)
			{
				gto>>alpha[t1];
				for(int t2=0; t2<num_l_max; t2++)
					if (do_l[t2]==1)
						gto>>coef[t1][t2];
				getline(gto,tmp);
			}
			//
			// calculate wfc
			for(int t1=0; t1<num_l_max; t1++)
				if (do_l[t1] == 1)
				{
					// header
					header<<"                       "<<sym_l[t1]<<"  "<<t1<<"  0.0000"<<endl;
					// wfcs
					wfc<<sym_l[t1]<<"   "<<t1<<"   0.00          Wavefunction"<<endl;
					for (int t2=0; t2<num_grid; t2++)
					{
						tmp_d=0.0;
						for (int t3=0; t3<num_alpha; t3++)
							tmp_d+=coef[t3][t1]*exp(-alpha[t3]*grid[t2]*grid[t2]);
						tmp_d*=pow(grid[t2],t1+1);
						wfc<<scientific<<setw(20)<<setprecision(10)<<tmp_d;
						if (t2%4==3)
							wfc<<endl;
					}
					wfc<<endl;
				}
			//
			getline(gto,tmp);
		}
		mesh.close();
		header.close();
		wfc.close();
		getline(gto,tmp);
	}
	gto.close();
	return 0;
}
