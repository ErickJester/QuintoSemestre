import re
from collections import deque, defaultdict

class AutomataPila:
    def __init__(self):
        self.pila = []  # Pila utilizada para manejar el proceso de derivación
        self.gramatica = {  # Gramática que define las reglas de producción
            'S': ['id = E'],
            'E': ['T E\''],
            'E\'': ['+ T E\'', '- T E\'', ''],
            'T': ['F T\''],
            'T\'': ['* F T\'', '/ F T\'', '% F T\'', ''],
            'F': ['( E )', 'id', 'num'],
            'id': r'[A-Za-z_][A-Za-z0-9_]*',
            'num': r'[0-9]+'
        }
        self.derivaciones = []  # Almacena las derivaciones realizadas
        self.error_message = ""  # Mensaje de error si se produce alguno
        self.max_depth = 0  # Profundidad máxima alcanzada durante el análisis

    def validar_cadena(self, cadena):
        # Tokeniza la cadena de entrada
        tokens = re.findall(r'[A-Za-z_][A-Za-z0-9_]*|[=+\-*/%()]|[0-9]+', cadena)
        print(f"Tokens iniciales: {tokens}")
        self.pila = [('S', 0)]  # Inicia con el símbolo inicial 'S'
        self.derivaciones = []
        self.error_message = ""
        self.max_depth = 0

        while self.pila:
            top, depth = self.pila.pop()  # Obtiene el elemento superior de la pila
            self.max_depth = max(self.max_depth, depth)
            print(f"Procesando: {top} a profundidad {depth}, tokens restantes: {tokens}, pila actual: {self.pila}")
            
            if top in self.gramatica and isinstance(self.gramatica[top], list):
                # Intenta aplicar una producción para el símbolo actual
                produccion_aplicada = False
                for produccion in self.gramatica[top]:
                    #print(f"Intentando aplicar producción: '{produccion}' para '{top}'")
                    if self.aplicar_produccion(produccion, tokens.copy()):
                        elementos = produccion.split()
                        for elem in reversed(elementos):
                            if elem:  # No añadir elementos vacíos
                                self.pila.append((elem, depth + 1))
                        self.derivaciones.append((top, elementos))
                        produccion_aplicada = True
                        #print(f"Producción aplicada: {produccion}")
                        break
                if not produccion_aplicada:
                    self.error_message = f"No se pudo aplicar ninguna producción para el símbolo: {top} con los tokens restantes: {tokens}"
                    print(self.error_message)
                    return False, self.max_depth
            elif top == 'id' and tokens and re.fullmatch(self.gramatica['id'], tokens[0]):
                # Reconoce un identificador
                print(f"Reconocido 'id': {tokens[0]}")
                self.derivaciones.append((f'id:{tokens[0]}', [tokens[0]]))
                tokens.pop(0)
            elif top == 'num' and tokens and re.fullmatch(self.gramatica['num'], tokens[0]):
                # Reconoce un número
                print(f"Reconocido 'num': {tokens[0]}")
                self.derivaciones.append((f'num:{tokens[0]}', [tokens[0]]))
                tokens.pop(0)
            elif tokens and tokens[0] == top:
                # Reconoce un terminal
                print(f"Reconocido terminal: {tokens[0]}")
                self.derivaciones.append((top, [tokens[0]]))
                tokens.pop(0)
            else:
                # Error de análisis si no se puede reconocer el símbolo
                self.error_message = f"Esperaba {top} pero encontró {tokens[0] if tokens else 'fin de cadena'}"
                print(self.error_message)
                return False, self.max_depth
        if tokens:
            self.error_message = f"Tokens restantes después de procesar la pila: {tokens}"
            print(self.error_message)
            return False, self.max_depth
        return True, self.max_depth

    def aplicar_produccion(self, produccion, tokens):
        if not produccion:
            return True
        
        elementos = produccion.split()
        temp_tokens = tokens[:]
        
        for elem in elementos:
            #print(f"Intentando aplicar {elem} a {temp_tokens}")
            if elem == 'id':
                if not temp_tokens or not re.fullmatch(self.gramatica['id'], temp_tokens[0]):
                    #print(f"Fallo al aplicar 'id' con tokens {temp_tokens}")
                    return False
                temp_tokens.pop(0)
            elif elem == 'num':
                if not temp_tokens or not re.fullmatch(self.gramatica['num'], temp_tokens[0]):
                    #print(f"Fallo al aplicar 'num' con tokens {temp_tokens}")
                    return False
                temp_tokens.pop(0)
            elif elem in self.gramatica:
                if not any(self.aplicar_produccion(p, temp_tokens) for p in self.gramatica[elem]):
                    #print(f"Fallo al aplicar producción recursiva: {elem} en producción {produccion}")
                    return False
            elif temp_tokens and temp_tokens[0] == elem:
                temp_tokens.pop(0)
            else:
                #print(f"Fallo al aplicar terminal: {elem} en producción {produccion}")
                return False
        tokens[:] = temp_tokens
        return True

    def dibujar_arbol(self, cadena):
        es_valida, profundidad = self.validar_cadena(cadena)
        if es_valida:
            print(f"\n\n\tLa cadena pertenece a la gramática.\n\n")
        else:
            print(f"\n\tLa cadena no pertenece a la gramática. Error: {self.error_message}\n\n")
            return

        print(f"Árbol de derivación (profundidad {profundidad}):\n")
        print(f"Arbol")
        self.imprimir_arbol()

    def imprimir_arbol(self):  
        derivaciones_dict = defaultdict(list)
        for produccion in self.derivaciones:
            derivaciones_dict[produccion[0]].append(produccion[1])
        
        def imprimir_nodo(nodo, nivel, visitados):  # Añadido parámetro visitados
            indent = "  " * nivel
            print(f"{indent}{nodo}")
            visitados.add(nodo)  # Marcar nodo como visitado
            if nodo in derivaciones_dict:
                for produccion in derivaciones_dict[nodo]:
                    for elem in produccion:
                        if elem in self.gramatica and elem not in visitados:  # Evitar nodos ya visitados
                            imprimir_nodo(elem, nivel + 1, visitados)  # Llamada recursiva para subnodos
                        elif ':' in elem:
                            real_elem = elem.split(':')[1]
                            indent_next = "  " * (nivel + 1)
                            print(f"{indent_next}{real_elem}")
                        elif elem == '':  # Manejar epsilon
                            indent_next = "  " * (nivel + 1)
                            print(f"{indent_next}ε")
                        else:
                            indent_next = "  " * (nivel + 1)
                            print(f"{indent_next}{elem}")

        imprimir_nodo('S', 0, set())  # Llamada inicial con el símbolo de inicio

def main():
    automata = AutomataPila()
    expresion = "AB = A*B/100-59;"  # Ejemplo de entrada
    
    automata.dibujar_arbol(expresion)

if __name__ == "__main__":
    main()
