import re
import time
from collections import deque, defaultdict

class Node:
    def __init__(self, value, level=0):
        self.value = value
        self.children = []
        self.level = level

    def add_child(self, child):
        self.children.append(child)

class ParseTree:
    def __init__(self):
        self.root = None

    def set_root(self, value):
        self.root = Node(value)

    def print_tree(self, node=None, indent="", last='updown'):
        if node is None:
            node = self.root

        # Resaltamos los tokens de la expresión original en mayúsculas
        if node.value in {"X", "Y", "Z", "(", ")", "+"}:
            value_to_print = node.value.upper()
        else:
            value_to_print = node.value

        if last == 'updown':
            start_shape = ' '
        elif last == 'up':
            start_shape = '┌'
        elif last == 'down':
            start_shape = '└'
        else:
            start_shape = ' '

        if len(node.children) > 0:
            end_shape = '├'
        else:
            end_shape = '└'

        print(indent + start_shape + value_to_print)

        indent += "│  " if len(node.children) > 0 else "   "

        for i, child in enumerate(node.children):
            next_last = 'up' if i == 0 else 'down' if i == len(node.children) - 1 else 'updown'
            self.print_tree(child, indent, next_last)

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
        self.lines = []

    def validar_cadena(self, cadena):
        tokens = re.findall(r'[A-Za-z_][A-Za-z0-9_]*|[=+\-*/%()]|[0-9]+', cadena)
        self.pila = [('S', 0)]
        self.derivaciones = []
        self.error_message = ""
        self.max_depth = 0

        while self.pila:
            top, depth = self.pila.pop()
            self.max_depth = max(self.max_depth, depth)
            
            if top in self.gramatica and isinstance(self.gramatica[top], list):
                produccion_aplicada = False
                for produccion in self.gramatica[top]:
                    if self.aplicar_produccion(produccion, tokens.copy()):
                        elementos = produccion.split()
                        for elem in reversed(elementos):
                            if elem:  # No añadir elementos vacíos
                                self.pila.append((elem, depth + 1))
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
        es_valida, _ = self.validar_cadena(cadena)
        if not es_valida:
            print(f"La cadena no pertenece a la gramática.\nRazón: {self.error_message}")
            return

        self.build_lines()
        self.imprimir_arbol()

    def build_lines(self):
        tree = ParseTree()
        tree.set_root("S")
        current_node = tree.root

        derivaciones_dict = defaultdict(list)
        for produccion in self.derivaciones:
            derivaciones_dict[produccion[0]].append(produccion[1])

        worklist = deque([(current_node, "S")])
        visitados = set()

        while worklist:
            parent_node, symbol = worklist.popleft()
            if (symbol, parent_node.level) in visitados:
                continue
            visitados.add((symbol, parent_node.level))
            if symbol in derivaciones_dict:
                for produccion in derivaciones_dict[symbol]:
                    for elem in produccion:
                        new_node = Node(elem, parent_node.level + 1)
                        parent_node.add_child(new_node)
                        if elem in self.gramatica:
                            worklist.append((new_node, elem))
                        else:
                            worklist.append((new_node, elem))

        self.tree = tree

    def imprimir_arbol(self):
        if hasattr(self, 'tree'):
            self.tree.print_tree()

def main():
    automata = AutomataPila()
    expresion = "X = (Y + Z)"  # Ejemplo de entrada
    automata.dibujar_arbol(expresion)

if __name__ == "__main__":
    main()
