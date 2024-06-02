public class EjemploPracticaAnalizador {
    public static void main(String[] args) {
        // Constantes enteras
        int entero3 = 456;  // Correcto

        // Constantes flotantes
        float flotante1 = 1.23;  // Correcto

        // Constantes dobles
        double doble1 = 1.23;  // Correcto
        double doble2 = 0.456;  // Correcto
        double doble3 = 1.23E10;  // Correcto

        // Constantes con errores léxicos
        int entero2 = 0;  // Correcto
        int entero1 = 1.23;  // Error: los enteros no deben tener punto decimal
        int errorEntero1 = 0123;  // Error: los enteros no deben comenzar con cero
        float errorFlotante1 = .123f;  // Error: el número no debe comenzar con un punto
        double errorDoble1 = 1.23E;  // Error: exponente incompleto
        float flotante2 = 0.456f;  // Correcto

        // Otros ejemplos
        int ejemploCorrecto = 789;  // Correcto
        float ejemploFlotanteCorrecto = 12.34f;  // Correcto
        double ejemploDobleCorrecto = 56.78;  // Correcto
    }
}
