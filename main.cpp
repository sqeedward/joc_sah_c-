#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include<iostream>
#include<utility>
using namespace std;
int piese[8][8]={0};
float x=600;
float y=400;//rezolutia
const float marE = 0.938894;//Cat la suta din tabla de sah e tabla da sah
const float invmarE = 0.061106;//inversa marjii
struct pos{
    //pentru pozitii x si y
    int x,y;
};
class TablaSah{
    public:
        TablaSah(){
            t.loadFromFile("imagini/tabla_sah.png");
            s.setScale(y/t.getSize().x,y/t.getSize().y);
            s.setTexture(t);
            //pozitia unui sprite incepe cu cel mai din dreapta punct, daca luam x/2-y/2 selectam
            //mijlocul ecranului, astfel centram tabla
            s.setPosition(x/2-y/2,0);
            //construiesc tabla de sah, am setat marimea y/t.size, deoarece
            //marimea pozei are o dimensiune maxima, functia setscle multiplica in interiorul sprite-ului
            //marimea texturii cu (dimensiunea dorita)/(dimensiunea curenta), astfel este o ecuatie de grad
            //intai in care obtinem multiplicatorul
        }

        sf::Sprite getSprite(){
            return s;
        }
        sf::Texture getTexture(){
            return t;
        }
    sf::Sprite s;
    private:
        sf::Texture t;


};
class Piesa{
    public:
        Piesa(){
            recSize.x = (y*marE)/8;
            recSize.y = (y*marE)/8;
        }
        //pt piese temporare
        Piesa(int culoare,int x ,int y){
            recSize.x = (y*marE)/8;
            recSize.y = (y*marE)/8;
            setPiecePosition(x,y);
            tip = 1+culoare;
        }
        void setPosition(float x, float y){
            body.setPosition(x-recSize.x/2,y-recSize.x/2);
            //setam pozitia dorita, minus jumatate din imagine, pentru a obtine centrul sprite-ului
        }
        void setRelativePosition(float x,float y){
            pos C = getCoord(x,y);
            lastX=C.x;
            lastY=C.y;
            float a,b;
            //prin ecuatia aceasta, luam partea intreaga a numarului pozitiei, imparit la marimea unui patratel
            //din tabla de sah, se adauga si marja de eroare, deoarece tabla de sah are si margini
            a = C.x*recSize.x+tablaX+(invmarE*sy/2);
            b = C.y*recSize.y+tablaY+(invmarE*sy/2);
            body.setPosition(a,b);
        }
       void setPiecePosition(int x,int y){
            float a,b;
            lastX = x;
            lastY = y;
            //este la fel ca functia de mai sus, doar ca aceasta cere input x,y, adica exact pozitiile pe
            //care vrem sa mutam piesa
            a = x*recSize.x+tablaX+(invmarE*sy/2.);
            b = y*recSize.y+tablaY+(invmarE*sy/2);
            body.setPosition(a,b);
        }
        void getPosition(){
            //pentru output, ca sa verificam de erori
            cout<<body.getPosition().x<<" "<<body.getPosition().y<<endl;
        }

        pos getCoord(float x, float y){
            pos c;
            //acelasi procedeu ca mai devreme, doar ca pentru a obtine pozitia de la mouse
            c.x = (int)((x-tablaX-(invmarE*sy/2))/recSize.x);
            c.y = (int)((y-tablaY-(invmarE*sy/2))/recSize.y);
            if(c.x < 0)c.x=0;
            if(c.y < 0)c.y=0;
            if(c.y > 7)c.y=7;
            if(c.x > 7)c.x =7;
            //aceste if-uri asigura ca pozitia primita se afla in interiorul tablii de sah
            return c;
        }
        pos enemiesPos(){
            if(this->scanTura().x != -1){
                return scanTura();
            }
            else if(this->scanNebun().x != -1){
                return scanNebun();
            }
            else if(this->scanCal().x != -1){
                return scanCal();
            }
            else if(this->scanPion().x != -1){
                return scanPion();
            }
            return {-1,-1};
        }
        int scanEnemies(){
            //daca vreuna da 1 , atunci clar ne ataca cineva
            if(enemiesPos().x == -1)
                return 0;
            return 1;
        }
        int scanEnemiesAdvanced(){
            if(enemiesPos().x == -1 && scanRege().x == -1)
                return 0;
            return 1;
        }
        pos scanTura(){
            int i,c = tip%2;
            // !culoare = opusul culorii, daca cul = 0 => !cul = 1
            //scanam brut ca mi-e lene sa-l fac mai frumos
            for(i = lastX+1; i < 7 && piese[lastY][i] == 0;i++);
                if(piese[lastY][i] == 3+c || piese[lastY][i] == 9+c)
                return {i,lastY};
            for(i = lastX-1; i > 0&& piese[lastY][i] == 0;i--);
                if(piese[lastY][i] == 3+c || piese[lastY][i] == 9+c)
                return {i,lastY};
            for(i = lastY+1; i < 7 && piese[i][lastX] == 0;i++);
                if(piese[i][lastX] == 3+c || piese[i][lastX] == 9+c)
                return {lastX,i};
            for(i = lastY-1; i > 0 && piese[i][lastX] == 0;i--);
                if(piese[i][lastX] == 3+c || piese[i][lastX] == 9+c)
                return {lastX,i};
            return {-1,-2};
        }
        pos scanNebun(){
            int i,j,c = tip%2;
            for(i = lastX-1, j = lastY-1;i > 0 && j > 0 && piese[j][i] ==0;i--,j--);
                if(piese[j][i] == 5+c || piese[j][i] == 9+c)
                    return {i,j};
            for(i = lastX+1, j = lastY+1;i < 7 && j < 7 && piese[j][i] ==0;i++,j++);
                if(piese[j][i] == 5+c || piese[j][i] == 9+c)
                    return {i,j};
            for(i = lastX-1, j = lastY+1;i >0 && j < 7 && piese[j][i] ==0;i--,j++);
                if(piese[j][i] == 5+c || piese[j][i] == 9+c)
                    return {i,j};
            for(i = lastX+1, j = lastY-1;i < 7 && j > 0 && piese[j][i] ==0;i++,j--);
                if(piese[j][i] == 5+c || piese[j][i] == 9+c)
                    return {i,j};
            return {-1,-1};
        }
        pos scanCal(){
            int c=tip%2;
            if(lastX >= 2){
                if(lastY > 0 && piese[lastY-1][lastX-2] == 7+c)return {lastX-2,lastY-1};
                if(lastY < 7 && piese[lastY+1][lastX-2] == 7+c)return {lastX-2,lastY+1};
            }
            if(lastX <= 5){
                if(lastY > 0 && piese[lastY-1][lastX+2] == 7+c)return {lastX+2,lastY-1};
                if(lastY < 7 && piese[lastY+1][lastX+2] == 7+c)return {lastX+2,lastY+1};
            }
            if(lastY >= 2){
                if(lastX > 0 && piese[lastY-2][lastX-1] == 7+c)return {lastX-1,lastY-2};
                if(lastX < 7 && piese[lastY-2][lastX+1] == 7+c)return {lastX+1,lastY-2};
            }
            if(lastY <= 5){
                if(lastX > 0 && piese[lastY+2][lastX-1] == 7+c)return {lastX-1,lastY+2};
                if(lastX < 7 && piese[lastY+2][lastX+1] == 7+c)return {lastX+1,lastY+2};
            }
            return {-1,-1};
        }
        pos scanPion(){
            int c;
            if(tip%2 == 0) c = 1;
            else c = -1;
            if(lastY+c >= 0 && lastY+c <= 7){
                if(lastX > 0 && piese[lastY+c][lastX-1]==tip%2+1)
                    return {lastX-1,lastY+c};
                if(lastX < 7 && piese[lastY+c][lastX+1]==tip%2+1)
                    return {lastX+1,lastY+c};
            }
            return {-1,-1};
        }
        pos scanRege(){
            int c=tip%2;
            int d[8][2]={{-1,-1},{-1,0},{-1,1},{0,1},{1,1},{1,0},{1,-1},{0,-1}};
            for(int i =0 ;i < 8;i++){
                int a = lastX+d[i][0],b = lastY+d[i][1];
                if(a >= 0 && a< 8 &&b >=0 && b< 8){
                    if(piese[b][a]== 11+c)return{a,b};
                }
            }
            return{-1,-1};
        }
        sf::Sprite getBody(){
            return body;
        }
        //folosim functii virtuale pentru a face codatul mai usor, astfel pregatim clase care au actiune de piesaMove
        //doar ca fiecare e putin diferita
         virtual int piesaMove(pos C){
        };
        virtual int SahMat(){
        };
        virtual int canMove(){
        };
        virtual int canAttack(){
        };
        virtual int canBlock(){
        };

    int lastX,lastY;
    int culoare;
    int start=1;
    int tip;
    protected:
        sf::Vector2f recSize;
        sf::Sprite body;
        sf::Texture t;

    private:
        float tablaX=x/2-y/2,tablaY=0;
        float sy=y;

};
class Pion:public Piesa{
    //am folosit inhereance pentru a putea seta texturi fiecarei piese
    public:
        Pion(){
        };
        Pion(int culoare_pion,int x ,int y,sf::Texture text){
            t = text;
            setPiecePosition(x,y);
            if(culoare_pion == 0) culoare = -1 ,tip=1;//culegerea de poli m-a invatat dracia asta
            else tip = 2 ,culoare =1;
            piese[y][x]=tip;
            body.setTexture(t);
            body.setScale(recSize.x/t.getSize().x,recSize.x/t.getSize().y);
        }

        int pionMove(pos C){
            if(culoare*C.y > culoare*lastY){
                //daca distanta e maxim 1, sau 1+start, atunci poate fi valida
                if(C.y*culoare -culoare*lastY<= 1+start && abs(C.x*culoare -culoare*lastX)<= 1){
                    setPiecePosition(C.x,C.y);
                    start =0;
                    return 1;
                }
            }
             setPiecePosition(lastX,lastY);
            return 0;
        }
        int piesaMove(pos C){
            int verificare = 0;
            //atac
            if(C.y == lastY+culoare && C.x != lastX && piese[C.y][C.x]!= 0 && piese[C.y][C.x]%2!=tip%2)
                verificare = pionMove(C);
            //mers fata, daca este o piesa in fata, nu putem sarii de 2 ori
             else if(C.y == lastY+culoare && C.x ==lastX && piese[C.y][C.x]==0 ||(
               C.y == lastY+(start+1)*culoare && C.x == lastX &&piese[C.y][C.x]==0 &&
                piese[C.y-culoare][C.x]==0))
                 verificare = pionMove(C);
            else setPiecePosition(lastX,lastY);
            return verificare;
        }

    private:


};
class Tura:public Piesa{
    public:
    Tura(){};
    Tura(int culoare,int x ,int y,sf::Texture text){
            t = text;
            setPiecePosition(x,y);
            tip = 3+culoare;
            piese[y][x]=tip;
            body.setTexture(t);
            body.setScale(recSize.x/t.getSize().x,recSize.x/t.getSize().y);
        }
        int piesaMove(pos C){
            int i,verificare = 0;
            if(C.x == lastX && C.y != lastY || C.x != lastX && C.y == lastY){
                if(C.y < lastY) {
                    for(i = lastY-1; i > C.y && piese[i][C.x]==0;i--);
                    if(piese[i][C.x] != 0 && piese[i][C.x] % 2 == tip % 2)i++;
                    verificare = (i == C.y);
                }
                else if(C.x < lastX) {
                    for(i = lastX-1; i > C.x && piese[C.y][i]==0;i--);
                    if(piese[C.y][i] != 0 && piese[C.y][i] % 2 == tip % 2)i++;
                    verificare = (i == C.x);
                }
                else if(C.y > lastY) {
                    for(i = lastY+1; i < C.y && piese[i][C.x]==0;i++);
                    if(piese[i][C.x] != 0 && piese[i][C.x] % 2 == tip % 2)i--;
                    verificare = (i == C.y);
                }
                else{
                    for(i = lastX+1; i < C.x && piese[C.y][i]==0;i++);
                    if(piese[C.y][i] != 0 && piese[C.y][i] % 2 == tip % 2)i--;
                    verificare = (i == C.x);
                }

            }
            if (verificare){
                setPiecePosition(C.x,C.y);
                return 1;
            }
            setPiecePosition(lastX,lastY);
            return 0;
        }
    private:
};
class Nebun:public Piesa{
    public:
        Nebun(){};
         Nebun(int culoare,int x ,int y,sf::Texture text){
            t = text;
            setPiecePosition(x,y);
            tip = 5+culoare;
            piese[y][x]=tip;
            body.setTexture(t);
            body.setScale(recSize.x/t.getSize().x,recSize.x/t.getSize().y);
        }
        int piesaMove(pos C){
            int i,j;
            int verificare = 0 ;
            if(C.x != lastX && (lastX-lastY == C.x-C.y || lastX+lastY == C.x+C.y)){
                if(C.x < lastX && C.y < lastY){
                    for(i = lastX-1, j = lastY-1;i > C.x && j > C.y && piese[j][i] ==0;i--,j--);
                    if(piese[j][i] != 0 && piese[j][i] % 2 == tip%2)i++,j++;
                }
                else if(C.x < lastX && C.y > lastY){
                    for(i = lastX-1, j = lastY+1;i > C.x && j < C.y && piese[j][i] ==0;i--,j++);
                    if(piese[j][i] != 0 && piese[j][i] % 2 == tip%2)i++,j--;
                }
                else if(C.x > lastX && C.y < lastY){
                    for(i = lastX+1, j = lastY-1;i < C.x && j > C.y && piese[j][i] ==0;i++,j--);
                    if(piese[j][i] != 0 && piese[j][i] % 2 == tip%2)i--,j++;
                }
                else{
                    for(i = lastX+1, j = lastY+1;i < C.x && j < C.y && piese[j][i] ==0;i++,j++);
                    if(piese[j][i] != 0 && piese[j][i] % 2 == tip%2)i--,j--;
                }
                verificare = (i == C.x)&&(j == C.y);
            }
            if(verificare){
                setPiecePosition(C.x,C.y);
                return 1;
            }
            setPiecePosition(lastX,lastY);
            return 0;
        }

    private:

};
class Cal:public Piesa{
    public:
    Cal(){};
    Cal(int culoare,int x ,int y,sf::Texture text){
            t = text;
            setPiecePosition(x,y);
            tip = 7+culoare;
            piese[y][x]=tip;
            body.setTexture(t);
            body.setScale(recSize.x/t.getSize().x,recSize.x/t.getSize().y);
        }
    int piesaMove(pos C){
        if(abs(C.x - lastX)==2 && abs(C.y-lastY)==1 || abs(C.y - lastY)==2 && abs(C.x-lastX)==1 ){
            if(piese[C.y][C.x] == 0 || piese[C.y][C.x] % 2 != tip % 2){
                setPiecePosition(C.x,C.y);
                return 1;
            }
        }
        setPiecePosition(lastX,lastY);
        return 0;
    }
    private:
};
class Regina:public Piesa{
    public:
        Regina(){};
        Regina(int culoare,int x ,int y,sf::Texture text){
            t = text;
            setPiecePosition(x,y);
            tip = 9+culoare;
            piese[y][x]=tip;
            body.setTexture(t);
            body.setScale(recSize.x/t.getSize().x,recSize.x/t.getSize().y);
        }
        int piesaMove(pos C){
             int i,j;
             //in final, regina e o combinatie intre tura si nebun
            int verificare = 0 ;
            //codul de la nebun
            if(C.x != lastX && (lastX-lastY == C.x-C.y || lastX+lastY == C.x+C.y)){
                if(C.x < lastX && C.y < lastY){
                    for(i = lastX-1, j = lastY-1;i > C.x && j > C.y && piese[j][i] ==0;i--,j--);
                    if(piese[j][i] != 0 && piese[j][i] % 2 == tip%2)i++,j++;
                }
                else if(C.x < lastX && C.y > lastY){
                    for(i = lastX-1, j = lastY+1;i > C.x && j < C.y && piese[j][i] ==0;i--,j++);
                    if(piese[j][i] != 0 && piese[j][i] % 2 == tip%2)i++,j--;
                }
                else if(C.x > lastX && C.y < lastY){
                    for(i = lastX+1, j = lastY-1;i < C.x && j > C.y && piese[j][i] ==0;i++,j--);
                    if(piese[j][i] != 0 && piese[j][i] % 2 == tip%2)i--,j++;
                }
                else{
                    for(i = lastX+1, j = lastY+1;i < C.x && j < C.y && piese[j][i] ==0;i++,j++);
                    if(piese[j][i] != 0 && piese[j][i] % 2 == tip%2)i--,j--;
                }
                verificare = (i == C.x)&&(j == C.y);
            }
            //codul de la tura
            else if(C.x == lastX && C.y != lastY || C.x != lastX && C.y == lastY){
                if(C.y < lastY) {
                    for(i = lastY-1; i > C.y && piese[i][C.x]==0;i--);
                    if(piese[i][C.x] != 0 && piese[i][C.x] % 2 == tip % 2)i++;
                    verificare = (i == C.y);
                }
                else if(C.x < lastX) {
                    for(i = lastX-1; i > C.x && piese[C.y][i]==0;i--);
                    if(piese[C.y][i] != 0 && piese[C.y][i] % 2 == tip % 2)i++;
                    verificare = (i == C.x);
                }
                else if(C.y > lastY) {
                    for(i = lastY+1; i < C.y && piese[i][C.x]==0;i++);
                    if(piese[i][C.x] != 0 && piese[i][C.x] % 2 == tip % 2)i--;
                    verificare = (i == C.y);
                }
                else{
                    for(i = lastX+1; i < C.x && piese[C.y][i]==0;i++);
                    if(piese[C.y][i] != 0 && piese[C.y][i] % 2 == tip % 2)i--;
                    verificare = (i == C.x);
                }
            }
            if(verificare){
                setPiecePosition(C.x,C.y);
                return 1;
            }
            setPiecePosition(lastX,lastY);
            return 0;
        }
    private:
};
class Rege:public Piesa{
    public:
        Rege(){};
        Rege(int culoare,int x ,int y,sf::Texture text){
            t = text;
            setPiecePosition(x,y);
            tip = 11+culoare;
            piese[y][x]=tip;
            body.setTexture(t);
            body.setScale(recSize.x/t.getSize().x,recSize.x/t.getSize().y);

        }
        int piesaMove(pos C){
            if(abs(lastX-C.x) <= 1 && abs(lastY-C.y) <= 1 && (lastX != C.x || lastY != C.y))
                if(piese[C.y][C.x]==0 || piese[C.y][C.x] % 2 != tip%2){
                    setPiecePosition(C.x,C.y);
                    return 1;
                }
            setPiecePosition(lastX,lastY);
            return 0;
        }
        int SahMat(){
            int ver = scanEnemies();
            if(ver == 0)return 0;
            if(canAttack() || canMove() || canBlock())return 0;
            return 1;
        }
        int canMove(){
        //facem o piesa temporara in 8 pozitii si vedem daca scapam din sah astfel
        int d[8][2]={{-1,-1},{-1,0},{-1,1},{0,1},{1,1},{1,0},{1,-1},{0,-1}};
            for(int i =0 ;i < 8;i++){
                int a = lastX+d[i][0],b = lastY+d[i][1];
                if(a >= 0 && a< 8 &&b >=0 && b< 8){
                    if(piese[b][a]==0){
                        Piesa temp(culoare,a,b);
                        if(temp.scanEnemiesAdvanced()==0){
                            int v;
                            //verificam daca nu ne blocheaza propia piesa
                            int m = piese[lastY][lastX];
                            piese[lastY][lastX] = 0;
                            v = temp.scanEnemiesAdvanced();
                            piese[lastY][lastX] = m;
                            if(v ==0)
                            return 1;
                        }
                    }
                    //daca nu e gol spatiu, verificam pe inamici
                    else if(piese[b][a] % 2 != tip % 2){
                        Piesa temp(culoare,a,b);
                        if(temp.scanEnemiesAdvanced() == 0)return 1;
                    }
                }
            }
            return 0;
        }
        int canAttack(){
        //daca piesa poate fi atacata
            pos C = enemiesPos();
            //facem o piesa inamica si o scanam sa vedem daca avem aliati in zona
            Piesa temp((culoare+1)%2,C.x,C.y);
            if(temp.scanEnemies())return 1;
            //daca piesa e protejata nu putem ataca cu regele, asa ca verificam
            //verificam daca e protejata scanand alti inamici
            temp.tip = tip;
            int v = temp.scanEnemiesAdvanced();
            //daca piesa nu e protejata si e la o distanta de 1 patratica putem ataca cu regele
            if(v == 0 && (abs(lastX - temp.lastX) <= 1 &&  abs(lastY - temp.lastY) <= 1))
                return 1;
            return 0;
        }
        int canBlock(){
            int c = (tip)%2;
            pos C = enemiesPos();
            //daca e cal sau pion, nu putem bloca
            if(piese[C.y][C.x] == 1+c || piese[C.y][C.x]==7+c)
                return 0;
            //daca e tura sau regina
            if(piese[C.y][C.x] == 3+c || piese[C.y][C.x] == 9+c)
                if(blockTura(C))
                    return 1;
            //daca e nebun sau regina
            if(piese[C.y][C.x] == 5+c || piese[C.y][C.x] == 9+c)
                if(blockNebun(C))
                    return 1;
            return 0;
            //daca e tura, scanam pe linia/coloana x
        }
        int blockTura(pos C){
            int c = (culoare+1)%2,i;
            if(C.y < lastY) {
                for(i = lastY-1; i > C.y;i--){
                if(piese[i][C.x] == 0 || tip%2 != piese[C.x][i]%2){
                    Piesa temp(c,C.x,i);
                    if(temp.scanEnemies())return 1;
                }
                }
            }
            else if(C.x < lastX) {
                    for(i = lastX-1; i > C.x;i--){
                    if(piese[i][C.x] == 0 || tip%2 != piese[i][C.x]%2){
                        Piesa temp(c,i,C.y);
                        if(temp.scanEnemies())return 1;
                    }
                    }
                }
                else if(C.y > lastY) {
                    for(i = lastY+1; i < C.y;i++){
                    if(piese[i][C.x] == 0 || tip%2 != piese[C.x][i]%2){
                        Piesa temp(c,C.x,i);
                        if(temp.scanEnemies())return 1;
                    }
                }
                }
                else{
                    for(i = lastX+1; i < C.x;i++){
                    if(piese[i][C.x] == 0 || tip%2 != piese[i][C.x]%2){
                        Piesa temp(c,i,C.y);
                        if(temp.scanEnemies())return 1;
                    }
                    }
                }
                return 0;
        }
        int blockNebun(pos C){
            int i,j,c=(culoare+1)%2;
            if(C.x < lastX && C.y < lastY){
                for(i = lastX-1, j = lastY-1;i > C.x && j > C.y;i--,j--){
                    if(piese[j][i] == 0 || piese[j][i] % 2 != tip%2){
                        Piesa temp(c,i,j);
                        if(temp.scanEnemies())return 1;
                        return 0;
                        }
                    }
                }
                else if(C.x < lastX && C.y > lastY){
                    for(i = lastX-1, j = lastY+1;i > C.x && j < C.y && piese[j][i] ==0;i--,j++){
                        if(piese[j][i] == 0 || piese[j][i] % 2 != tip%2){
                            Piesa temp(c,i,j);
                            if(temp.scanEnemies())return 1;
                            return 0;
                        }
                    }
                }
                else if(C.x > lastX && C.y < lastY){
                    for(i = lastX+1, j = lastY-1;i < C.x && j > C.y && piese[j][i] ==0;i++,j--){
                        if(piese[j][i] == 0 || piese[j][i] % 2 != tip%2){
                            Piesa temp(c,i,j);
                            if(temp.scanEnemies())return 1;
                            return 0;
                        }
                    }
                }
                else{
                    for(i = lastX+1, j = lastY+1;i < C.x && j < C.y && piese[j][i] ==0;i++,j++){
                        if(piese[j][i] == 0 || piese[j][i] % 2 != tip%2){
                            Piesa temp(c,i,j);
                            if(temp.scanEnemies())return 1;
                            return 0;
                        }
                    }
                }
                return 0;
        }

    private:
};
class Piese{
    public:
        Piese(){
            //1= negru, 0 = alb
            //cream o lista de piese intr-un vector de pointeri
            pionAlb.loadFromFile("imagini/Pion_alb.png");
            pionNegru.loadFromFile("imagini/Pion_negru.png");

            TuraAlba.loadFromFile("imagini/tura_alba.png");
            turaNeagra.loadFromFile("imagini/tura_neagra.png");

            nebunNegru.loadFromFile("imagini/nebun_Negru.png");
            nebunAlb.loadFromFile("imagini/nebun_Alb.png");

            calNegru.loadFromFile("imagini/cal_Negru.png");
            calAlb.loadFromFile("imagini/cal_Alb.png");

            reginaAlba.loadFromFile("imagini/regina_Alb.png");
            reginaNeagra.loadFromFile("imagini/regina_Negru.png");

            regeAlb.loadFromFile("imagini/rege_Alb.png");
            regeNegru.loadFromFile("imagini/rege_Negru.png");

            for(int i =0;i <8;i++){
                myPiese[1][i] = new Pion(1,i,1,pionNegru);
                myPiese[6][i] = new Pion(0,i,6,pionAlb);
                //trebuie sa inversam x,y in vector, deoarece pe vectori x inseamna y
            }
            //ture
            myPiese[0][0] = new Tura(1,0,0,turaNeagra);
            myPiese[0][7] = new Tura(1,7,0,turaNeagra);
            myPiese[7][0] = new Tura(0,0,7,TuraAlba);
            myPiese[7][7] = new Tura(0,7,7,TuraAlba);
            //cai
            myPiese[0][1] = new Cal(1,1,0,calNegru);
            myPiese[0][6] = new Cal(1,6,0,calNegru);
            myPiese[7][1] = new Cal(0,1,7,calAlb);
            myPiese[7][6] = new Cal(0,6,7,calAlb);
            //nebuni
            myPiese[0][2] = new Nebun(1,2,0,nebunNegru);
            myPiese[0][5] = new Nebun(1,5,0,nebunNegru);
            myPiese[7][2] = new Nebun(0,2,7,nebunAlb);
            myPiese[7][5] = new Nebun(0,5,7,nebunAlb);
            //regine si regi
            myPiese[0][3] = new Regina(1,3,0,reginaNeagra);
            myPiese[7][3] = new Regina(0,3,7,reginaAlba);
            myPiese[0][4] = new Rege(1,4,0,regeNegru);
            myPiese[7][4] = new Rege(0,4,7,regeAlb);
            R[0] = myPiese[7][4];
            R[1] = myPiese[0][4];

        };
        void deseneazaPiese(sf::RenderWindow *window){
            //desenam piesele,pentru ca lucram cu pointeri folosim sagetile, ca sa intram direct in pointer
            for(int i =0; i < 8;i++){
                for(int j =0;j <8;j++){
                    if(piese[j][i])window->draw(myPiese[j][i]->getBody());
                }
            }
        }
        Piesa *detectAny(float x, float y){
            pos C = getCoord(x,y);
            // daca piesa curenta este pe aceasi pozitie ca mouse-ul, atunci stim ca se afla o piesa acolo
            if(piese[C.y][C.x])return myPiese[C.y][C.x];

        }
        int movePiese(Piesa *p,float x, float y){
            int verificare = 0;int a;
            pos A;
            A.x=p->lastX;
            A.y= p->lastY;
            pos C = getCoord(x,y);
            //daca este randul culorii a, o piesa de tip a se modifica
            if(pc % 2 == p->tip%2 && !checkChess(C,A,p))
                verificare = p->piesaMove(C);
            else p->piesaMove(A);
            //R[!pc%2] este regina culorii respective
            if(verificare){
                piese[A.y][A.x]=0;
                piese[C.y][C.x]=p->tip;
                myPiese[A.y][A.x] = NULL;
                myPiese[C.y][C.x]=p;
                pc++;
            }
            if(R[!(pc%2)]->SahMat())cout<<"Sah mat"<<endl;
            //returnam asta ca sa vedem daca piesa s-a mutat
            //0 = nu, 1 = da
            return verificare;
        }
        int checkChess(pos C,pos A,Piesa *p){
            int v = 0;
            int a = piese[A.y][A.x],b = piese[C.y][C.x];
            int tip =piese[C.x][C.y];
            piese[A.y][A.x] = 0;
            piese[C.y][C.x] = p->tip;
            //am facut asta pt ca regele ca scanarea regeliu schimba pozitia
            if(p->tip == R[!(pc%2)]->tip)
                v = checkRege(C,A,p);
            else if(R[!(pc%2)]->scanEnemiesAdvanced())
                v = 1;
            piese[A.y][A.x] = a;
            piese[C.y][C.x] = b;
            return v;
        }
        int checkRege(pos C,pos A,Piesa *p){
            int v=0;
            //daca e la o distanta de 1 fata de celalat rege
            if(abs(C.x - R[pc%2]->lastX) <= 1 && abs(C.y -R[pc%2]->lastY) <= 1)
                return 1;
            int a = p->lastX, b =p->lastY;
            p->lastX = C.x, p->lastY = C.y;
            if(p->scanEnemiesAdvanced())v=1;
            p->lastX = a, p->lastY=b;
            return v;
        }//
        void add_or_detele_piece(float x, float y,sf::Event e){
            pos C = getCoord(x,y);
            if(e.key.code== sf::Keyboard::Q)
                myPiese[C.y][C.x] = new Pion(0,C.x,C.y,pionAlb);
            else if(e.key.code== sf::Keyboard::W)
                myPiese[C.y][C.x] = new Tura(0,C.x,C.y,TuraAlba);
            else if(e.key.code== sf::Keyboard::E)
                myPiese[C.y][C.x] = new Regina(0,C.x,C.y,reginaAlba);
            else if(e.key.code== sf::Keyboard::R)
                myPiese[C.y][C.x] = new Rege(0,C.x,C.y,regeAlb);
            else if(e.key.code == sf::Keyboard::A){
                myPiese[C.y][C.x] = new Rege(1,C.x,C.y,regeNegru);
                R[1]=myPiese[C.y][C.x];
            }
            else if(e.key.code== sf::Keyboard::T){
                myPiese[C.y][C.x] = NULL;
                piese[C.y][C.x] = 0;
            }
            else if(e.key.code == sf::Keyboard::Y){
                for(int i =0 ;i < 8;i++)
                for(int j =0;j < 8;j++){
                    myPiese[j][i] = NULL;
                    piese[j][i] = 0;
                }
            }
            else if(e.key.code == sf::Keyboard::S)
                pc++;
            else if(e.key.code == sf::Keyboard::D)
                myPiese[C.y][C.x] = new Pion(1,C.x,C.y,pionNegru);
            else if(e.key.code == sf::Keyboard::F){
                for(int i =0 ;i < 8;i++){
                    for(int j =0;j < 8;j++){
                    cout<<piese[i][j]<<" ";
                    }
                cout<<endl;
                }
            }
        }

        pos getCoord(float x, float y){
            pos C;
            C.x = (int)((x-tablaX-(invmarE*sy/2))/recSize);
            C.y= (int)((y-tablaY-(invmarE*sy/2))/recSize);
            if(C.x < 0)C.x=0;
            if(C.y < 0)C.y=0;
            if(C.y > 7)C.y=7;
            if(C.x > 7)C.x =7;
            return C;

        }
    private:
        int pc = 1;
        Piesa *R[2];
        Piesa *myPiese[8][8];
        float recSize = (y*marE)/8;
        float tablaX=x/2-y/2,tablaY=0;
        float sy=y;
        sf::Texture pionAlb,pionNegru;
        sf::Texture turaNeagra,TuraAlba;
        sf::Texture nebunAlb,nebunNegru;
        sf::Texture calAlb,calNegru;
        sf::Texture reginaAlba,reginaNeagra;
        sf::Texture regeAlb,regeNegru;

};
int main()
{
    sf::RenderWindow window(sf::VideoMode(600,400),"Joc sah");
    window.setVerticalSyncEnabled(true);
    x = window.getSize().x;
    y = window.getSize().y;
    TablaSah tabla;
    Piese pioni;
    Piesa *temp;
    int hold=0;
    int v;
    while(window.isOpen()){
        sf::Event event;
        while(window.pollEvent(event)){
            if(event.type == sf::Event::Closed)
                window.close();
            if(event.key.code == sf::Keyboard::Escape)
                window.close();
            if(event.type == sf::Event::KeyPressed){
                pioni.add_or_detele_piece(sf::Mouse::getPosition(window).x,sf::Mouse::getPosition(window).y,event);
            }

            if(event.type == sf::Event::MouseButtonPressed || hold==1){
                if(hold==0){
                    temp = pioni.detectAny(sf::Mouse::getPosition(window).x,sf::Mouse::getPosition(window).y);
                    //temp->getPosition();

                }
                if(temp != NULL)temp->setPosition(sf::Mouse::getPosition(window).x,sf::Mouse::getPosition(window).y);
                hold = 1;

            }
            if(event.type == sf::Event::MouseButtonReleased && hold){
                if(temp != NULL)v=pioni.movePiese(temp,sf::Mouse::getPosition(window).x,sf::Mouse::getPosition(window).y);
                hold = 0;

            }

        }
        window.clear();
        window.draw(tabla.s);
        pioni.deseneazaPiese(&window);
        window.display();
    }


    return 0;
}
