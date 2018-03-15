#include <allegro.h>
#ifndef DISPAROS_H_INCLUDED
#define DISPAROS_H_INCLUDED

#include <vector>

using namespace std;

struct Balas{
    int x , y =0;
    int dx , dy =0;
    bool disponible=true;

};

void crear_bala(Balas &bala,const int X, const int Y , const int dy);


//void crear_bala2(int& n_disparos, const int max_disparos, struct Balas disparos[],
//	const int X, const int Y, const int dy);


void pintar_bala(Balas &bala,BITMAP* buffer, BITMAP* bal);

void elimina_bala(Balas &bala,const int ANCHO, const int ALTO);

bool retener_nave_izq(int posx,int mov, const int ANCHO);

bool retener_nave_der(int posx, int mov, const int ANCHO);


void detectar_colision(Balas &bala,int nave_x, int nave_y);


#endif // DISPAROS_H_INCLUDED
