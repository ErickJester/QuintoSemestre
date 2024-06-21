import re
from collections import deque, defaultdict

class AutomataPila:
    def __init__(self):
        self.pila = []
        self.gramatica = {
            'S': ['id = E'],
            'E': ['T E\''],
            'E\'': ['+ T E\'', '- T E\'', ''],
            'T': ['F T\''],
            'T\'': ['* F T\'', '/ F T\'', '% F T\'', ''],
            'F': ['( E )', 'id', 'num'],
            'id': r'[A-Za-z_][A-Za-z0-9_]*',
            'num': r'[0-9]+'
        }
        self.derivaciones = []
        self.error_message = ""
        self.max_depth = 0

    def validar_cadena(self, cadena):
        tokens = re.findall(r'[A-Za-z_][A-Za-z0-9_]*|[=+\-*/%()]|[0-9]+', cadena)
        self.pila = [('S', 0)]
        self.derivaciones = []
        self.error_message = ""
        self.max_depth = 0

        while self.pila:
            top, depth = self.pila.pop()
            print(f"Pop: {top}, Depth: {depth}")  # Depuración
            self.max_depth = max(self.max_depth, depth)
            
            if top in self.gramatica and isinstance(self.gramatica[top], list):
                produccion_aplicada = False
                for produccion in self.gramatica[top]:
                    if self.aplicar_produccion(produccion, tokens.copy()):
                        elementos = produccion.split()
                        for elem in reversed(elementos):
                            if elem:  # No añadir elementos vacíos
                                self.pila.append((elem, depth + 1))
                                print(f"Push: {elem}, Depth: {depth + 1}")  # Depuración
                        self.derivaciones.append((top, elementos))
                        produccion_aplicada = True
                        break
                if not produccion_aplicada:
                    self.error_message = f"No se pudo aplicar ninguna producción para el símbolo: {top} con los tokens restantes: {tokens}"
                    return False, self.max_depth
            elif top == 'id' and tokens and re.fullmatch(self.gramatica['id'], tokens[0]):
                self.derivaciones.append((top, [tokens[0]]))
                tokens.pop(0)
            elif top == 'num' and tokens and re.fullmatch(self.gramatica['num'], tokens[0]):
                self.derivaciones.append((top, [tokens[0]]))
                tokens.pop(0)
            elif tokens and tokens[0] == top:
                self.derivaciones.append((top, [tokens[0]]))
                tokens.pop(0)
            else:
                self.error_message = f"Esperaba {top} pero encontró {tokens[0] if tokens else 'fin de cadena'}"
                return False, self.max_depth
        if tokens:
            self.error_message = f"Tokens restantes después de procesar la pila: {tokens}"
            return False, self.max_depth
        return True, self.max_depth

    def aplicar_produccion(self, produccion, tokens):
        if not produccion:
            return True
        
        elementos = produccion.split()
        temp_tokens = tokens[:]
        
        for elem in elementos:
            if elem == 'id':
                if not temp_tokens or not re.fullmatch(self.gramatica['id'], temp_tokens[0]):
                    return False
                temp_tokens.pop(0)
            elif elem == 'num':
                if not temp_tokens or not re.fullmatch(self.gramatica['num'], temp_tokens[0]):
                    return False
                temp_tokens.pop(0)
            elif elem in self.gramatica:
                if not any(self.aplicar_produccion(p, temp_tokens) for p in self.gramatica[elem]):
                    return False
            elif temp_tokens and temp_tokens[0] == elem:
                temp_tokens.pop(0)
            else:
                return False
        tokens[:] = temp_tokens
        return True

    def dibujar_arbol(self, cadena):
        es_valida, profundidad = self.validar_cadena(cadena)
        if not es_valida:
            print(f"La cadena no pertenece a la gramática.\nRazón: {self.error_message}")
            return

        print(f"Árbol de derivación (profundidad {profundidad}):\n")
        #self.imprimir_arbol()

    def imprimir_arbol(self):
        worklist = deque([('S', 0)])
        derivaciones_dict = defaultdict(list)
        for produccion in self.derivaciones:
            derivaciones_dict[produccion[0]].append(produccion[1])
        
        visitados = set()
        
        while worklist:
            nodo, nivel = worklist.popleft()
            if (nodo, nivel) in visitados:
                continue
            visitados.add((nodo, nivel))
            indent = "  " * nivel
            print(f"{indent}{nodo}")
            if nodo in derivaciones_dict:
                for produccion in derivaciones_dict[nodo]:
                    for elem in produccion:
                        if elem in self.gramatica:
                            worklist.append((elem, nivel + 1))
                        else:
                            indent_next = "  " * (nivel + 1)
                            print(f"{indent_next}{elem}")

        print("Árbol de derivación completado.")

def main():
    automata = AutomataPila()
    expresion = "X = (Y + Z)"  # Ejemplo de entrada
    
    es_valida, profundidad = automata.validar_cadena(expresion)
    if es_valida:
        print(f"La cadena pertenece a la gramática. Profundidad del árbol: {profundidad}")
        # Aseguramos que no se ejecute dos veces
        #automata.imprimir_arbol()  # Comentado para evitar doble ejecución
    else:
        print(f"La cadena no pertenece a la gramática. Razón: {automata.error_message}")

if __name__ == "__main__":
    main()
