// Neural-Network.cpp : Defines the entry point for the console application.
//

//#define _SCL_SECURE_NO_WARNINGS



#include <iostream>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>

#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#include <random>

#include <fstream>
#include <string>

#include <stdlib.h>

#include <math.h>
#include "funciones.h"
#include <time.h>

#define e 2.71828182846
t alpha = 0.5;

using namespace std;
using namespace boost::numeric::ublas;
matrix<t> matrix_hidden;
matrix<t> matrix_output;
matrix<t> Sh;
matrix<t> So;
std::vector<t> delta; t d;
std::vector<t> sump; t p;

std::vector<t> Sd(1);


void imprimir(matrix<t> &m) {

        for (int i = 0; i<m.size1(); i++) {
            for (int j = 0; j< m.size2(); j++)
            {
                printf("%.2f ", m(i, j));
            }
            cout << endl;
        }

}

matrix<t> Create_Matrix(int n_fil, int n_col)
{
	matrix<t> m(n_fil, n_col); //y-x-ordered
	for (int i = 0; i != n_fil; ++i) {
		for (int j = 0; j != n_col; ++j) {
			m(i, j) = ((double)rand() / (RAND_MAX + 1));
		}
	}
	return m;
}


void NetaS(matrix<t> &m, matrix<t> &input, matrix<t>& salida) {
	salida = prod(m, input);

	salida.resize(salida.size1() + 1, 1);
	for (int i = 1; i<salida.size1(); i++) {
		salida(i, 0) = 1 / (1 + pow(e, (-1 * salida(i - 1, 0))));
	}
	salida(0, 0) = 1;
}


void NetaSF(matrix<t> &m, matrix<t> &input, matrix<t>& salida) {

	//cout<<"OUTPUT"<<endl;
	//imprimir(m);
	//cout<<"SH"<<endl;
	//imprimir(input);

	salida = prod(m, input);

    //cout<<"SALIDA: " << salida(0, 0)<<endl;
	for (int i = 0; i<salida.size1(); i++) {

		salida(i, 0) = 1 / (1 + pow(e,( -1 * salida(i, 0))));
	}
}




float calcular_error(std::vector<t>&Sd, matrix<t> &So) {
	t error = 0;
	for (int i = 0; i < So.size1(); i++) {
		//printf("SD:%f SO%f\n", Sd[i] , So(i, 0));
		error += (pow((Sd[i] - So(i, 0)), 2)) / 2;
		//printf("error esss:%f\n", error);
	}
	return error;
}




const int N_hiden_neurons = 8;
const int N_output_neurons = 1;
const int n_input = 6;//con el bias



void changes_pesos(std::vector<t>&Sd, matrix<t>& input)
{

	delta.clear();
	sump.clear();
	d = 0;
	/******************************MATRIZ OUTPUT***********************/
	for (int i = 0; i < So.size1(); i++) {
		d = So(i, 0)*(1 - So(i, 0))*(-1*(Sd[i] - So(i, 0)));
		for (int j = 0; j < Sh.size1(); j++) {
			matrix_output(i, j) = matrix_output(i, j) - alpha*(Sh(j, 0)*d);
		}
		delta.push_back(d);
	}



	//printf("MATRIZ OUTPUT NUEVA:\n"); imprimir(matrix_output);
	/******************************MATRIZ HIDDEN***********************/
	//QUITAR EL UNO BIAS
	//cout << "SH SIZE :"<<delta.size() << endl;


	for (int i =0 ; i < Sh.size1(); i++) {
		p = 0;
		for (int k = 0; k < delta.size(); k++) {
			p += matrix_output(k, i)*delta[k];}
		sump.push_back(p);
	}
	//cout <<"SUMP SIZE: "<<sump.size() << endl;
	//cout << "Sh SIZE: " << Sh.size1() << endl;


	for (int i = 0; i < Sh.size1()-1; i++) {
		p = sump[i]*(Sh(i+1, 0)*(1 - Sh(i+1, 0)));

		for (int j = 0; j < input.size1(); j++) {

			matrix_hidden(i, j) = matrix_hidden(i, j) + alpha*(p*input(j, 0));
		}
	}



}



void inicializar(matrix<t>&m, std::vector<float>&v) {
	for (int i = 0; i < v.size(); i++) {
		m(i, 0) = v[i];
	}
}



void cargar_red(){

    matrix_hidden = Create_Matrix(N_hiden_neurons, n_input);
	matrix_output = Create_Matrix(N_output_neurons, N_hiden_neurons + 1);

    cargar_pesos(matrix_output,"pesos_output");
	cargar_pesos(matrix_hidden,"pesos_hidden");

	cout<<"MATRIX HIDDEN"<<endl;
	imprimir(matrix_hidden);

	cout<<"MATRIX OUTPUT"<<endl;
	imprimir(matrix_output);

}



int iniciar_red(std::vector<float> entrada){



	matrix_hidden = Create_Matrix(N_hiden_neurons, n_input);
	matrix_output = Create_Matrix(N_output_neurons, N_hiden_neurons + 1);


	cout<<"IMPRIMIENDO MATRIZ HIDDEN"<<endl;
	imprimir(matrix_hidden);
	cout << endl;
	cout<<"IMPRIMIENDO MATRIZ OUTPUT"<<endl;
	imprimir(matrix_output);
    cout << endl;

    matrix<t> input(n_input,1);
	inicializar(input,entrada );//reducir


    NetaS(matrix_hidden, input, Sh);
    //cout<<"NETA SF"<<endl;
	NetaSF(matrix_output, Sh, So);

    cout<<"SALIDA FINAL: "<<So(0,0)<<endl;

    if(So(0,0)>0.51){
        return 1;
	}
	else if(So(0,0)<0.49){
        return 3;
	}
	else{
        return 2;
	}


}

//le paso las entradas

void  guardar_red()
{
    guardar_pesos(matrix_hidden, "pesos_hidden");
	guardar_pesos(matrix_output,"pesos_output");
}



int  ejecutar_red2(std::vector<float> entrada)
{
  	matrix<t> input(n_input,1);
	inicializar(input,entrada );//reducir
    cout<<"ENTRADA"<<endl;
	imprimir(input);

	//HACIA ADELANTE
    NetaS(matrix_hidden, input, Sh);
    //cout<<"NETA SF"<<endl;
	NetaSF(matrix_output, Sh, So);


    cout<<"SALIDA FINAL CARGANDO: "<<So(0,0)<<endl;


    if(So(0,0)>0.51){
        return 1;
	}
	else if(So(0,0)<0.49){
        return 3;
	}
	else{
        return 2;
	}

}



int  ejecutar_red(std::vector<float> entrada)
{

	matrix<t> input(n_input,1);
	inicializar(input,entrada );//reducir

	cout<<"SE INGRESO"<<endl;
	cout<<endl;
	//imprimir(input);

	//HALLAMOS LA SALIDA DESEADA LA BALA ROSE LA NAVE +39
	if(entrada[2]>=entrada[1] && entrada[2]<=entrada[1]+39){
        if(entrada[3]+4 < entrada[1]+20){
            Sd = {1};
        }
        else {
            Sd = {0};
        }
	}
	else{
        Sd = {0.5};
	}


    //CAMBIAMOS LOS PESOS
    changes_pesos(Sd, input);

	//cout<<"NETA S"<<endl;
	NetaS(matrix_hidden, input, Sh);

    //cout<<"NETA SF"<<endl;
	NetaSF(matrix_output, Sh, So);


    cout<<"SALIDA FINAL: "<<So(0,0)<<endl;


    if(So(0,0)>0.51){
        return 1;
	}
	else if(So(0,0)<0.49){
        return 3;
	}
	else{
        return 2;
	}

	//else if(So(0,0)==5){
        //return 2;
	//}
	//else{
    //    return 3;
	//}
}


