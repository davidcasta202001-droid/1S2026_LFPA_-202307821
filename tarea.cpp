#include <iostream>
#include <string>
using namespace std;

class Juguete{
    private:
    string marca, accesorio;
    public:
    Juguete(string m){marca = m;}
    Juguete(string n){accesorio = n;}
    void Etiqueta(){cout << "Juguete: " << marca << endl;};
    void Accesorio(){cout << "Accesorios Juguete: " << accesorio << endl; }
};
int main(){
    Juguete miJuguete("Osito de Peluche");
    Juguete miAccesorio("Sueter de felpa");
    miJuguete.Etiqueta();
    miAccesorio.Accesorio();
    return 0;
}
