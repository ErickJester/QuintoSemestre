public class EjemploPracticaAnalizador2 {
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

        // Otros ejemplos
        int ejemploCorrecto1 = 789;               // Correcto
        float ejemploFlotanteCorrecto = 12.34f;   // Correcto
        double ejemploDobleCorrecto = 56.78;      // Correcto

        // Ejemplos con números negativos y positivos
        int enteroNegativo = -123;   // Correcto
        float flotantePositivo = +1.23f; // Correcto
        double dobleNegativo = -1.23; // Correcto

        // Constantes enteras incorrectas
        int entero3 = 0123;      // Error: los enteros no deben comenzar con cero
        int entero4 = 1.23;      // Error: los enteros no deben tener punto decimal

        // Constantes flotantes incorrectas
        float flotante3 = 0.123f; // Error: el número no debe comenzar con un punto
        float flotante4 = 1.23f;  // Error: falta el sufijo 'f'
        float flotante5 = 123f;  // Correcto

        // Constantes dobles incorrectas
        double doble5 = 1.23E;   // Error: exponente incompleto
        double doble6 = 123.;    // Error: falta la parte decimal o exponente

        // Más ejemplos de errores
        int errorEntero1 = 01;     // Error: los enteros no deben comenzar con cero
        float errorFlotante1 = 1.; // Error: falta la parte decimal
        double errorDoble1 = 1.23E-; // Error: exponente incompleto

        // Errores adicionales con signos
        float errorFlotanteSigno = -1.; // Error: falta la parte decimal
        double errorDobleSigno = +1.23E; // Error: exponente incompleto
    }
}
