#include <iostream>
#include <string>

class Juguete {
private:
    std::string marca, material; 
    int peso;          

public:
    Juguete(std::string _marca, int _peso, std::string _material) {
        marca = _marca;
        peso = _peso;
        material = _material; 
    }

    
    void Info() {
        std::cout << "Marca: " << marca << ", Peso en libras: " << peso << ",Material: " << material  << std::endl;
    }
    void Material(){
        std::cout << "Marca que creo el juguete: " << marca << ", Peso ideal del juguete: " << peso << std::endl;
    }
};

int main() {
    Juguete toy("Mattel", 25, "Felpa");
    Juguete t2("Hasbro", 30, "Plastico");
    toy.Info();
    toy.Material();
    t2.Info();
    t2.Material();
    system("pause");
    return 0;
}