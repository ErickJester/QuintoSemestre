public class EjemploPracticaAnalizador {
    public static void main(String[] args) {
        // Constantes enteras correctas
        int entero1 = 123;       // Correcto
        int entero2 = 0;         // Correcto

        // Constantes flotantes correctas
        float flotante1 = 1.23f; // Correcto
        float flotante2 = 0.456f;// Correcto

        // Constantes dobles correctas
        double doble1 = 1.23;    // Correcto
        double doble2 = 0.456;   // Correcto
        double doble3 = 1.23E10; // Correcto
        double doble4 = 0.0;     // Correcto

        int temp1;
        temp1 * 8.23;
        public > 100
        int + char
        
        // Otros ejemplos
        int ejemploCorrecto1 = 789;               // Correcto
        float ejemploFlotanteCorrecto = 12.34f;   // Correcto
        double ejemploDobleCorrecto = 56.78;      // Correcto

        // Ejemplos con numeros negativos y positivos
        int enteroNegativo = -123;   // Correcto
        float flotantePositivo = +1.23f; // Correcto
        double dobleNegativo = -1.23; // Correcto

        // Constantes enteras incorrectas
        int entero3 = 0123;      // Error: los enteros no deben comenzar con cero
        int entero4 = 1.23;      // Error: los enteros no deben tener punto decimal

        // Constantes flotantes incorrectas
        float flotante3 = .123f; // Error: el numero no debe comenzar con un punto
        float flotante4 = 1.23;  // Correcto
        float flotante5 = 123f;  // Correcto

        // Constantes dobles incorrectas
        double doble5 = 1.23E;   // Error: exponente incompleto
        double doble6 = 123.;    // Error: falta la parte decimal o exponente

        // Mas ejemplos de errores
        int errorEntero1 = 01;     // Error: los enteros no deben comenzar con cero
        float errorFlotante1 = 1.; // Error: falta la parte decimal
        double errorDoble1 = 1.23E-; // Error: exponente incompleto

        // Errores adicionales con signos
        float errorFlotanteSigno = -1.; // Error: falta la parte decimal
        double errorDobleSigno = +1.23E; // Error: exponente incompleto

        // Identificadores invalidos
        int 1invalido = 5; // Error: el identificador no debe comenzar con un número
        int invalido# = 10; // Error: el identificador contiene caracteres inválidos

        

        // Operadores de comparacion y asignacion incorrectos
        if (a = b) { // Error: uso incorrecto de operador de comparacion
            int c = d >== e; // Error: uso incorrecto de operadores de comparacion
        }

        // Operadores aritmeticos incorrectos
        int resultado = 1 / 0; // Error: division por cero
        int operacion = 5 * * 2; // Error: uso incorrecto de operador aritmetico

        /*
          Errores extra
          int errorEntero1 = 01;     // Error: los enteros no deben comenzar con cero
          float errorFlotante1 = 1.; // Error: falta la parte decimal
          double errorDoble1 = 1.23E-; // Error: exponente incompleto
        */
    }
}
