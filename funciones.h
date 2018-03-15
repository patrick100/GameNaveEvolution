#pragma once
#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include <utility>
#include <algorithm>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>


using namespace std;
typedef float t;

typedef pair<std::vector<t>, int>      tinput;
using namespace boost::numeric::ublas;


int S = 0;



void normalizar(std::vector<t> &input) {

	auto max = std::max_element(std::begin(input), std::end(input));
	auto min = std::min_element(std::begin(input), std::end(input));
	t maxi = *max;
	t mini = *min;
	//cout << "max: " << *max << "min:" << *min << endl;
	for (int i = 0; i < input.size(); i++) {
		input[i] = (input[i] - mini) / (maxi - mini);
	}
}

void guardar_pesos(matrix<t>m,string path) {
	ofstream pesos;
	pesos.open(path);
	for (int i = 0; i<m.size1(); i++) {
		for (int j = 0; j< m.size2(); j++)
		{
			pesos << m(i, j) << ",";
		}
		pesos << endl;
	}
	pesos.close();
}


void cargar_pesos(matrix<t> &m, string path) {
	string line; ifstream data(path);
	if (data.is_open())
	{
		int f = 0;
		int c = 0;
		while (getline(data, line))
		{
			c = 0;
			string copia = ""; t dato = 0;
			for (int i = 0; i < line.length(); i++) {
				if (line[i] == ',') {
					dato = strtof(copia.c_str(), 0);

					m(f, c) = dato;
					copia = ""; i++; c++;
				}
				copia += line[i];
			}

			copia = ""; f++;
		}
		data.close();
	}
	else cout << "Unable to open file";

	/*
	for (int i = 0; i < m.size1(); i++) {
		for (int j = 0; j < m.size2(); j++) {
			cout << m(i, j) << "  ";
		}
		cout << endl;
	}*/
}





void cargar_data(std::vector<tinput> &total_inputs,string path) {
	string line; ifstream data(path);
	if (data.is_open())
	{
		int l = 0;
		while (getline(data, line))
		{
			total_inputs[l].first.push_back(1);

			string copia = ""; t dato = 0;
			for (int i = 0; i < line.length(); i++) {
				if (line[i] == ',') {
					dato = strtof(copia.c_str(), 0);
					total_inputs[l].first.push_back(dato);
					copia = ""; i++;
				}
				copia += line[i];
			}

			if (copia == "Iris-setosa") { total_inputs[l].second = 1; cout << copia << endl;
			}
			if (copia == "Iris-versicolor") { total_inputs[l].second = 2; cout << copia << endl;
			}
			if (copia == "Iris-virginica") { total_inputs[l].second = 3; cout << copia << endl;
			}


			copia = "";
			//normalizar(total_inputs[l].first);

			l++;
		}
		data.close();
	}
	else cout << "Unable to open file";


	std::vector<std::vector<t>> mtemp(5);

	for (int x = 0; x <total_inputs.size(); x++) {
		mtemp[0].push_back(1);
	}

	for (int i = 1; i < 5; i++) {
		for (int j = 0; j <  total_inputs.size(); j++) {
			mtemp[i].push_back(total_inputs[j].first[i]);
		}
		normalizar(mtemp[i]);
	}


	/*
	for (int i = 0; i < mtemp.size(); i++) {
		for (int j = 0; j <mtemp[i].size(); j++) {
			cout<<mtemp[i][j]<<" ";
		}
		cout << endl;
	}*/



	//Convertirla a input
	for (int i = 0; i <  mtemp.size(); i++) {
		for (int j = 0; j <mtemp[i].size(); j++) {
			total_inputs[j].first[i] = mtemp[i][j];
		}
	}


	/*
	for (int i = 0; i < total_inputs.size(); i++) {
		for (int j = 0; j < total_inputs[i].first.size(); j++) {
			cout << total_inputs[i].first[j] << "  ";
		}
		cout << endl;
	}*/

}
