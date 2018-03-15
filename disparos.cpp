#include "disparos.h"
#include <allegro.h>
#include <iostream>

using namespace std;

void crear_bala(Balas &bala,const int X, const int Y , const int dy)
{
      if(bala.disponible==false){

          bala.disponible = true;
          bala.x  = X+15;
          bala.y  = Y;
          bala.dx = 0;
          bala.dy = dy;

          cout<<"SE CREO LA BALA"<<endl;
      }
}


/*
void crear_bala2(int& n_disparos, const int max_disparos, vector<Balas> disparos,
	const int X, const int Y, const int dy)
{

	if (n_disparos < max_disparos - 1) { // n -1 disparos permitidos
		n_disparos++;

		disparos[n_disparos].x = X +15;
		disparos[n_disparos].y = Y+30;
		disparos[n_disparos].dx = 0;
		disparos[n_disparos].dy = dy;
	}

}
*/


void pintar_bala(Balas &bala,BITMAP* buffer, BITMAP* bal)
{
     if(bala.disponible==true){
        //cout<<"SE PINTO LA BALA"<<endl;
        bala.x += bala.dx;
        bala.y += bala.dy;
        masked_blit( bal, buffer, 0, 0, bala.x, bala.y, 8, 9);
     }
}


//nave1.bala, ANCHO, ALTO

void elimina_bala(Balas &bala,const int ANCHO, const int ALTO)
{

       if ( bala.disponible==true){
          if ( bala.y > ALTO || bala.y < 0 ||
               bala.x > ANCHO|| bala.x < 0  )
           {
                bala.disponible = false;
           }

      }
}



void detectar_colision(Balas &bala,int nave_x, int nave_y) {
	//CHOQUE EN X
	if(bala.disponible==true){

        if(bala.x>= nave_x && bala.x<= nave_x+39 && bala.y<= nave_y){
        //CHOQUE EN Y
            cout<<"CHOCO"<<endl;
            bala.disponible = false;
        }
	}


}



bool retener_nave_izq(int posx,int mov,int ANCHO) {
	if (posx-mov < 0) {
		return false;
	}
	return true;
}

bool retener_nave_der(int posx, int mov, int ANCHO) {
	if (posx+mov > ANCHO) {
		return false;
	}
	return true;
}

