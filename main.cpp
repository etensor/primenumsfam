#include <cmath>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>

// primo perfecto: 2 - n-1 -> 0 divisores
// semi primo: divisores son primos-perfectos
// semi primo cuadratico: cuadrado del primer divisor.

using namespace std;

bool esPrimo(int num) {
  if (num <= 1)
    return false;

  for (int i = 2; i <= sqrt(num); i++) {
    if (num % i == 0) {
      return false;
    }
  }
  // es primo
  return true;
}

int analizador(int num) {
  if (esPrimo(num))
    return 1; //"primo";

  int i, j, cuenta = 0;
  double raiz = 0.0;

  for (i = 2; i * i <= num; i++) { // sqrt(num) >= i
    if (num % i == 0) {
      for (j = 2; j * j <= i; j++) {
        if (i % j == 0)
          cuenta++;
      }
      if (cuenta == 0) { // todavia puede ser semi primo
        for (j = 2; j <= sqrt(num / i); j++) {
          if ((num / i) % j == 0) {
            cuenta++;
          }
        }
        if (cuenta == 0) { // es semi primo
          raiz = sqrt(num);
          if ((int)raiz == sqrt(num) &&
              esPrimo(raiz)) { // tenemos que ver si es entero
            return 3;          //"semi-primo cuadratico";
          } else {
            return 2; //"semi-primo";
          }
        } else
          return 0; //"comun";
      }
    }
  }
  return -1; // "?"; // no hay caso que cumpla este caso, pero si sale, es un
             // numero muy extraño, depronto nisiquiera un numero
}

int *leer_numeros(int len, bool generar = false) {
  int *enteros = new int[len]();
  string linea;
  fstream entrada;
  int idx = 0;

  if (generar) {
    srand(time(NULL));

    fstream salida;
    salida.open("datos.txt", fstream ::out);
    while (idx < len) {
      salida << rand() % (RAND_MAX / 4) << endl; // cambiar lim
      idx++; // la probabilidad de que sea un numerisimo, es muy alta.
    }
    salida.close();
    idx = 0;
  }

  entrada.open("datos.txt", fstream ::in);
  while (getline(entrada, linea) && idx <= len) {
    enteros[idx] = stoi(linea); // stoi -> string to int
    idx++;
  }
  entrada.close();

  return enteros;
}

int *eval_numeros(int *enteros, int len = 25) {
  int *resultados =
      new int[len](); // con * no podemos saber el tamano del arreglo

  for (int idx = 0; idx < len; idx++)
    resultados[idx] = analizador(enteros[idx]);

  return resultados;
}

// genera el html con los resultados;
bool generar_resultados(int *numeros, int *resultados, int len) {
  fstream salida;

  string htmlstr = R"(
  <p style="content-align: center; text-align: center;"> <strong> Resultados </strong> </p>
  <hr>
  <table style="width: 42%; text-align: center; padding: 8px; margin-left: auto; margin-right: auto;">
  <thead>
    <tr>
      <th>Número</th>
      <th>Análisis</th>
    </tr>
  </thead>
  <tbody>
  )"; // multilinea

  salida.open("resultados.html", fstream ::out);
  salida << htmlstr << endl;

  for (int idx = 0; idx < len; idx++) { // con espaciado para darle legibilidad
    salida << "\t<tr>\n";
    salida << "\t\t<th>" << numeros[idx] << "</th>\n";
    salida << "\t\t<td ";
    switch (resultados[idx]) {
    case 0:
      salida << "> 0 | comun";
      break;
    case 1:
      salida << R"(style="color:#0000ff"; > 1 | primo )";
      break;
    case 2:
      salida << R"(style="color:#00ff00"; > 2 | semi-primo )";
      break;
    case 3:
      salida << R"(style="color:#ff0000"; > 3 | semi-primo cuadratico )";
      break;
    }
    salida << "\t\t</td>\n\t</tr>" << endl;
  }

  salida << "\n</tbody>\n</table>" << endl;
  salida.close();

  return true;
}

int main(int argc, char **argv) {

  int cuantos_num = 25;
  int *numeros;
  int *resultados;

  // posibilidad de generar y leer o solo leer.
  if (argc > 1 && (string(argv[1]) == "1" || string(argv[1]) == "true"))
    numeros = leer_numeros(cuantos_num, true);
  else
    numeros = leer_numeros(cuantos_num);

  resultados = eval_numeros(numeros);

  if (generar_resultados(numeros, resultados, cuantos_num)) {
    cout << "\n\n\tNúmeros analizados exitosamente.\n" << endl;
  } else {
    cout << "\n----\vERROR: no se pudo realizar el análisis..." << endl;
  }

  delete numeros;
  delete resultados;

  return 0;
}

// necesitamos ver hasta la raiz de num
// porque si n puede factorizarse en 2 factores,
//    num = a*b
//  a y b no pueden ambos mayor o igual a la raiz de num
// es decir:
// a*b = num, 1 < a <= b <= num
//     entonces
// num = a*b >= a*a <==> a*a <= num ==> a <= sqrt(num)
//

/// todos los semiprimos cuadraticos son en si
// semi primos, ya que son iguales exceptuando
// que su raiz cuadrada es primo.
// es decir en el producto de factores,
// ambos factores son primos, solo que son iguales.
