
#include <allegro.h>


#include "inicia.h"
#include "disparos.h"

#include <winalleg.h>


#include "RedNeuronal.h"


//#include <windows.h>
#include <math.h>

#include <vector>

//using namespace std;


#include <iostream>
#define ANCHO 300
#define ALTO  400



struct NAVE {
	int x, y;
	int n_disp = 0;
	int max_disp =0;
	int tick;
	int tick_lim;
    Balas bala;

	BITMAP* img_nav;
	BITMAP* img_bala;


	void inicia(int posx,int posy,char* ruta_nave,int mytick);

	void pinta(BITMAP* buffer);

	bool temporizador();

	bool temporizador_mov();

};




void NAVE::inicia(int posx,int posy,char * ruta_nave,int mytick) {
	x = posx;
	y = posy;
	n_disp = 0;
	max_disp = 2;
	img_nav = load_bitmap(ruta_nave, NULL);
	img_bala = load_bitmap("Recursos/bala1.bmp", NULL);

	tick = 0;

	tick_lim = mytick;
	bala.disponible = false;


}


void NAVE::pinta(BITMAP* buffer) {
	masked_blit(img_nav, buffer, 0, 0, x, y, 39, 39);
}

bool NAVE::temporizador() {
	tick++;
	if (tick == tick_lim) {
		tick = 0;
		return true;
	}
	return false;
}





float calcular_distancia(int &Nave1X,int &Nave2X,int &Nave1Y,int &Nave2Y) {

	return sqrt(pow(Nave1X - Nave2X, 2) + pow(Nave1Y-Nave2Y,2));
}


bool automata_nave(NAVE &nave1,NAVE &nave2){
    if(nave1.x>nave2.x && retener_nave_izq(nave1.x,5, ANCHO)){
        nave1.x -=5;
    }

    if(nave1.x<nave2.x && retener_nave_der(nave1.x,5, ANCHO-39)){
        nave1.x +=5;
    }
}


//int
int main(){

    //std::vector<float> input {100,50,400,110,300,130,400,150,500};

	std::vector<float> input{ANCHO / 2-20,ANCHO / 2-20,ANCHO / 2-5,ALTO - 50};
	inicia_allegro(ANCHO,ALTO); // fuciones del include inicia.h
	//inicia_audio(70,70);

	BITMAP *buffer = create_bitmap(ANCHO, ALTO);
	NAVE nave1;
	nave1.inicia(ANCHO / 2-20, ALTO - 50, "Recursos/nave1.bmp",6);
	NAVE nave2;
	nave2.inicia(ANCHO / 2-20, ALTO - 400, "Recursos/nave2.bmp",2);


	int movimiento=2;
    bool parar_entrenamiento=false;
    bool entrenar=true;

    bool play = false;



    if(entrenar==true){
        iniciar_red(input);
    }
    else{
        cargar_red();
    }

	//nave1.bala.disponible = true;


	while (!key[KEY_ESC]) {

          if (key[KEY_P]){
            play = true;
          }


        if(play){
		clear_to_color(buffer, 0x000000);

		nave1.pinta(buffer);
		nave2.pinta(buffer);

        //&& nave1.temporizador_mov()

        if(entrenar==true){
            if(nave1.x!=nave2.x && nave2.temporizador()){
                automata_nave(nave1,nave2);
            }

            if(nave1.bala.disponible == false ){
                //cout<<"INGRESO"<<endl;
                crear_bala(nave1.bala, nave1.x, nave1.y, -8);
            }



        }
        else{
            if (key[KEY_LEFT] && retener_nave_izq(nave1.x,5, ANCHO)) nave1.x -= 5;
            if (key[KEY_RIGHT] && retener_nave_der(nave1.x,5, ANCHO - 39)) nave1.x += 5;

            if (key[KEY_SPACE] && nave1.temporizador()) {
            crear_bala(nave1.bala, nave1.x, nave1.y, -8);
            }

        }







		if(entrenar==true){

            //ENTRENANDO LA RED NEURONAL
            if(parar_entrenamiento == false && nave1.temporizador() && nave1.bala.disponible == true){

                input.clear();

                input.push_back(nave1.x);
                input.push_back(nave2.x);
                //input.push_back(calcular_distancia(nave1.x,nave2.x,nave1.y,nave2.y));
                input.push_back(nave1.bala.x);
                input.push_back(nave1.bala.y);

                movimiento = ejecutar_red(input);

                if (movimiento == 1 && retener_nave_izq(nave2.x, 10, ANCHO)) {
                    cout<<"MOV IZQ"<<endl;
                    nave2.x -= 10;
                }
                //if (movimiento == 2) //nave1.x -= 5; no se mueve
                if (movimiento == 3 && retener_nave_der(nave2.x, 10, ANCHO- 39)){
                    cout<<"MOV DER"<<endl;
                    nave2.x += 10;
                }


            }

            //GUARDAR RED NEURONAL
            if (key[KEY_G]){
                guardar_red();
                parar_entrenamiento = true;


            }

		}
		else{

            if(nave1.temporizador() && nave1.bala.disponible == true){

                input.clear();
                input.push_back(nave1.x);
                input.push_back(nave2.x);
                //input.push_back(calcular_distancia(nave1.x,nave2.x,nave1.y,nave2.y));
                input.push_back(nave1.bala.x);
                input.push_back(nave1.bala.y);

                movimiento = ejecutar_red2(input);

                if (movimiento == 1 && retener_nave_izq(nave2.x, 10, ANCHO)) {
                    cout<<"MOV IZQ"<<endl;
                    nave2.x -= 10;
                }
                //if (movimiento == 2) //nave1.x -= 5; no se mueve
                if (movimiento == 3 && retener_nave_der(nave2.x, 10, ANCHO- 39)){
                    cout<<"MOV DER"<<endl;
                    nave2.x += 10;
                }

            }


		}


//




		pintar_bala(nave1.bala, buffer, nave1.img_bala);

		detectar_colision(nave1.bala,nave2.x,nave2.y);


		elimina_bala(nave1.bala, ANCHO, ALTO);


		blit(buffer, screen, 0, 0, 0, 0, ANCHO, ALTO);
		rest(30);
	}

	}
	return 0;
}
END_OF_MAIN();



